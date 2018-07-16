using System;
using System.Globalization;
using System.Text;
using EasyLibForCSharp.Textor;
using System.Management;
using Microsoft.Win32;
using System.Security.Cryptography;
using System.Runtime.InteropServices;

namespace EasyLibForCSharp.Computer
{
    public static class ComputerIdentification
    {
        public static string FormattedComputerId
        {
            get
            {
                byte[] bytes = ComputerIdentification.RawComputerId();
                return HexEncoder.EncodeHex(bytes, true);
            }
        }

        private static bool IdsMatch(byte[] trimmedHash, byte[] storedHash)
        {
            bool result = true;
            if (trimmedHash != null && storedHash != null)
            {
                if (trimmedHash.Length == storedHash.Length)
                {
                    for (int i = 0; i < trimmedHash.Length; i++)
                    {
                        if (trimmedHash[i] != storedHash[i])
                        {
                            result = false;
                            break;
                        }
                    }
                }
                else
                {
                    result = false;
                }
            }
            else
            {
                result = false;
            }
            return result;
        }

        private static string RemoveReservedBits(string cpuSerialNumber)
        {
            ulong num = ulong.Parse(cpuSerialNumber, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            return (num & 17588286127051UL).ToString(CultureInfo.InvariantCulture);
        }

        public static string GetCPUID()
        {
            try
            {
                //需要在解决方案中引用System.Management.DLL文件  
                ManagementClass mc = new ManagementClass("Win32_Processor");
                ManagementObjectCollection moc = mc.GetInstances();
                string strCpuID = null;
                foreach (ManagementObject mo in moc)
                {
                    strCpuID = mo.Properties["ProcessorId"].Value.ToString();
                    mo.Dispose();
                    break;
                }
                return strCpuID;
            }
            catch
            {
                return "";
            }
        }

        public static byte[] RawComputerId()
        {
            string volumeSerialNumber = ComputerIdentification.ReadVolumeSerial("c");
            string cpuSerialNumber = ComputerIdentification.RemoveReservedBits(GetCPUID());
            string madeUpNumber = ComputerIdentification.ReadUniqueRegistryId();
            byte[] storedHash = ComputerIdentification.ReadComputerId();
            byte[] array = ComputerIdentification.GenerateComputerId(volumeSerialNumber, cpuSerialNumber, madeUpNumber);

            if (!ComputerIdentification.IdsMatch(array, storedHash))
            {
                string text = ComputerIdentification.ReadCpuId();
                if (!string.IsNullOrEmpty(text))
                {
                    array = ComputerIdentification.GenerateComputerId(volumeSerialNumber, text, madeUpNumber);
                }
                if (!ComputerIdentification.IdsMatch(array, storedHash))
                {
                    madeUpNumber = ComputerIdentification.CreateUniqueRegistryId();
                    array = ComputerIdentification.GenerateComputerId(volumeSerialNumber, cpuSerialNumber, madeUpNumber);
                    ComputerIdentification.WriteComputerId(array);
                }
            }

            return array;
        }

        private static void WriteComputerId(byte[] computerId)
        {
            string subkey = string.Format(CultureInfo.InvariantCulture, "Software\\Detroit Diesel\\Drumroll\\{0}\\Settings", new object[]
            {
                8
            });
            RegistryKey registryKey = Registry.LocalMachine.CreateSubKey(subkey);
            registryKey.SetValue("Computer ID", computerId);
        }

        private static byte[] ReadComputerId()
        {
            string subkey = string.Format(CultureInfo.InvariantCulture, "Software\\Detroit Diesel\\Drumroll\\{0}\\Settings", new object[]
            {
                8
            });
            RegistryKey registryKey = Registry.LocalMachine.CreateSubKey(subkey);
            return registryKey.GetValue("Computer ID") as byte[];
        }

        private static string CreateUniqueRegistryId()
        {
            string text = string.Empty;
            string subkey = string.Format(CultureInfo.InvariantCulture, "CLSID\\{{341DD6AA-87AF-4994-B911-70A8B7DB4CE8}}\\DR\\{0}\\Settings", new object[]
            {
                8
            });
            RegistryKey registryKey = Registry.ClassesRoot.CreateSubKey(subkey);
            text = Guid.NewGuid().ToString();
            registryKey.SetValue("Value", text);
            return text;
        }

        private static string ReadUniqueRegistryId()
        {
            string result = string.Empty;
            string subkey = string.Format(CultureInfo.InvariantCulture, "CLSID\\{{341DD6AA-87AF-4994-B911-70A8B7DB4CE8}}\\DR\\{0}\\Settings", new object[]
            {
                8
            });
            RegistryKey registryKey = Registry.ClassesRoot.CreateSubKey(subkey);
            object value = registryKey.GetValue("Value");
            if (value != null)
            {
                string text = value as string;
                if (text != null)
                {
                    result = text;
                }
            }
            else
            {
                result = ComputerIdentification.CreateUniqueRegistryId();
            }
            return result;
        }

        private static byte[] GenerateComputerId(string volumeSerialNumber, string cpuSerialNumber, string madeUpNumber)
        {
            SHA1 sha = new SHA1CryptoServiceProvider();
            byte[] array = sha.ComputeHash(Encoding.ASCII.GetBytes(volumeSerialNumber + cpuSerialNumber + madeUpNumber));
            byte[] array2 = new byte[5];
            array2[0] = array[0];
            array2[1] = array[1];
            array2[2] = array[2];
            array2[3] = array[3];
            byte[] array3 = array2;
            int num = 0;
            array3[num] |= 128;
            byte[] array4 = sha.ComputeHash(array2, 0, 4);
            array2[4] = array4[0];
            return array2;
        }

        private static string ReadCpuId()
        {
            if (ComputerIdentification.cpuId == null)
            {
                StringBuilder stringBuilder = new StringBuilder();
                try
                {
                    ManagementClass managementClass = new ManagementClass("Win32_Processor");
                    ManagementObjectCollection instances = managementClass.GetInstances();
                    if (instances != null)
                    {
                        foreach (ManagementBaseObject managementBaseObject in instances)
                        {
                            ManagementObject managementObject = (ManagementObject)managementBaseObject;
                            PropertyData propertyData = managementObject.Properties["ProcessorId"];
                            if (propertyData != null && propertyData.Value != null)
                            {
                                stringBuilder.Append(propertyData.Value.ToString());
                            }
                        }
                    }
                }
                catch (ManagementException)
                {
                }
                ComputerIdentification.cpuId = stringBuilder.ToString();
            }
            return ComputerIdentification.cpuId;
        }

        private static string ReadVolumeSerial(string driveLetter)
        {
            uint value = 0u;
            uint num = 0u;
            StringBuilder stringBuilder = new StringBuilder();
            uint num2 = 0u;
            StringBuilder stringBuilder2 = new StringBuilder();
            driveLetter += ":\\";
            if (!ComputerIdentification.NativeMethods.GetVolumeInformation(driveLetter, stringBuilder, (uint)stringBuilder.Capacity, ref value, ref num, ref num2, stringBuilder2, (uint)stringBuilder2.Capacity))
            {
                value = 0u;
            }
            return Convert.ToString(value, CultureInfo.InvariantCulture);
        }

        private static string cpuId;

        private static class NativeMethods
        {
            [DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
            [return: MarshalAs(UnmanagedType.Bool)]
            public static extern bool GetVolumeInformation(string PathName, StringBuilder VolumeNameBuffer, uint VolumeNameSize, ref uint VolumeSerialNumber, ref uint MaximumComponentLength, ref uint FileSystemFlags, StringBuilder FileSystemNameBuffer, uint FileSystemNameSize);
        }
    }
}
