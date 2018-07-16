using System;
using System.Globalization;
using System.IO;
using System.Runtime.Serialization;
using System.Security.Cryptography;
using System.Text;
using EasyLibForCSharp.Cryptor.Exceptions;
using EasyLibForCSharp.Cryptor.Enums;
using EasyLibForCSharp.FileMan;

namespace EasyLibForCSharp.Cryptor
{
    public sealed class EncryptionProvider
    {
        public static byte[] CodeBytes(byte[] data, int offset, bool encrypt, int style)
        {
            if (encrypt)
            {
                if (style == 3)
                {
                    return EncryptionProvider.rijndaelAlgorithm.CreateEncryptor().TransformFinalBlock(data, offset, data.Length - offset);
                }
                Aes aes = EncryptionProvider.algorithm;
                return aes.CreateEncryptor(aes.Key, EncryptionProvider.algorithm.IV).TransformFinalBlock(data, offset, data.Length - offset);
            }
            else
            {
                if (style == 3)
                {
                    return EncryptionProvider.rijndaelAlgorithm.CreateDecryptor().TransformFinalBlock(data, offset, data.Length - offset);
                }
                Aes aes2 = EncryptionProvider.algorithm;
                return aes2.CreateDecryptor(aes2.Key, EncryptionProvider.algorithm.IV).TransformFinalBlock(data, offset, data.Length - offset);
            }
        }

        public static string Encrypt(string input)
        {
            byte[] bytes = Encoding.Unicode.GetBytes(input);
            Aes aes = EncryptionProvider.localAlgorithm;

            return Convert.ToBase64String(aes.CreateEncryptor(aes.Key, aes.IV).TransformFinalBlock(bytes, 0, bytes.Length));
        }

        public static string Decrypt(string input)
        {
            byte[] array = Convert.FromBase64String(input);
            Encoding unicode = Encoding.Unicode;
            Aes aes = EncryptionProvider.localAlgorithm;

            return unicode.GetString(aes.CreateDecryptor(aes.Key, aes.IV).TransformFinalBlock(array, 0, array.Length));
        }

        static EncryptionProvider()
        {
            byte[] salt = new byte[] { 0x1d, 0x4a, 0xea, 0x31, 0x87, 0xb8, 0x35, 0xfe };
            string pwd1 = "DetroitDiesel.Security.Cryptography";
            string pwd2 = "aaaaaswwaaadacij";

            EncryptionProvider.algorithm = EncryptionProvider.GetAlgorithm(pwd1, salt);
            EncryptionProvider.rijndaelAlgorithm = EncryptionProvider.GetRijndaelAlgorithm(pwd1, salt);
            EncryptionProvider.localAlgorithm = EncryptionProvider.GetAlgorithm(pwd2, salt);
        }

        private static Aes GetAlgorithm(string password, byte[] salt)
        {
            Rfc2898DeriveBytes rfc2898DeriveBytes = new Rfc2898DeriveBytes(password, salt);
            Aes aes = new AesCryptoServiceProvider();
            aes.Key = rfc2898DeriveBytes.GetBytes(aes.KeySize / 8);
            aes.IV = rfc2898DeriveBytes.GetBytes(aes.BlockSize / 8);

            return aes;
        }

        private static RijndaelManaged GetRijndaelAlgorithm(string password, byte[] salt)
        {
            RijndaelManaged rijndaelManaged = new RijndaelManaged();
            rijndaelManaged.Mode = CipherMode.CBC;
            rijndaelManaged.Padding = PaddingMode.PKCS7;
            rijndaelManaged.KeySize = 128;
            rijndaelManaged.BlockSize = 128;

            byte[] array = new byte[16];
            Array.Copy(Encoding.UTF8.GetBytes(Convert.ToBase64String(new Rfc2898DeriveBytes(password, salt).GetBytes(16))), array, 16);
            rijndaelManaged.Key = array;
            rijndaelManaged.IV = array;

            return rijndaelManaged;
        }

        private static Aes algorithm;

        private static Aes localAlgorithm;

        private static RijndaelManaged rijndaelAlgorithm;
    }

    public class FileEncryptionProvider
    {
        static FileEncryptionProvider()
        {

        }

        private static string CodeChars(string fileName, bool encrypt)
        {
            byte[] bytes = Encoding.UTF8.GetBytes("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_^$~!#%&-{}()@'`+=[];,. ");
            byte[] bytes2 = Encoding.UTF8.GetBytes("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_^$~!#%&-{}()@'`+=[];,. ");
            byte[] bytes3 = Encoding.UTF8.GetBytes(fileName.ToUpper(CultureInfo.InvariantCulture));
            for (int i = 0; i < bytes3.Length; i++)
            {
                if (bytes3[i] != 46)
                {
                    bool flag = false;
                    for (int j = 0; j < bytes.Length; j++)
                    {
                        if (encrypt)
                        {
                            if (bytes[j] == bytes3[i])
                            {
                                bytes3[i] = bytes2[j];
                                flag = true;
                                break;
                            }
                        }
                        else if (bytes2[j] == bytes3[i])
                        {
                            bytes3[i] = bytes[j];
                            flag = true;
                            break;
                        }
                    }
                    if (!flag)
                    {
                        throw new BadFileNameException("Invalid character in filename " + fileName);
                    }
                }
            }
            return Encoding.UTF8.GetString(bytes3);
        }

        private static string RearrangeChars(string fileName)
        {
            byte[] bytes = Encoding.UTF8.GetBytes(fileName);
            for (int i = 0; i < fileName.Length - 1; i += 3)
            {
                if (bytes[i] != 46 && bytes[i + 1] != 46 && bytes[i] != 42 && bytes[i + 1] != 42)
                {
                    byte b = bytes[i];
                    bytes[i] = bytes[i + 1];
                    bytes[i + 1] = b;
                }
            }
            return Encoding.UTF8.GetString(bytes);
        }

        public static string EncryptFileName(string pathName)
        {
            string fileName = FileEncryptionProvider.RearrangeChars(Path.GetFileName(pathName));
            string path = FileEncryptionProvider.CodeChars(fileName, true);
            return Path.Combine(Path.GetDirectoryName(pathName), path);
        }

        public static string DecryptFileName(string pathName)
        {
            string fileName = FileEncryptionProvider.RearrangeChars(Path.GetFileName(pathName));
            string path = FileEncryptionProvider.CodeChars(fileName, false);
            return Path.Combine(Path.GetDirectoryName(pathName), path);
        }

        private static byte SwitchNibbles(byte source)
        {
            byte b = Convert.ToByte((int)(source & 15));
            b = Convert.ToByte((int)b << 4);
            source = Convert.ToByte(source >> 4);
            source = Convert.ToByte((int)(source | b));
            return source;
        }

        private static byte ApplyKey(byte source)
        {
            return source ^= 63;
        }

        private static byte[] CodeBytes(byte[] source, int offset, bool encrypt)
        {
            byte[] array = new byte[source.Length - offset];
            for (int i = offset; i < source.Length; i++)
            {
                if (encrypt)
                {
                    array[i - offset] = FileEncryptionProvider.ApplyKey(FileEncryptionProvider.SwitchNibbles(source[i]));
                }
                else
                {
                    array[i - offset] = FileEncryptionProvider.SwitchNibbles(FileEncryptionProvider.ApplyKey(source[i]));
                }
            }
            return array;
        }

        private static byte CalculateChecksumB(byte[] data, int offset)
        {
            ushort num = 0;
            for (int i = offset; i < data.Length; i++)
            {
                num += (ushort)data[i];
            }
            num = Convert.ToUInt16((int)((~num & 255) + 1));
            return Convert.ToByte((int)(num & 255));
        }

        public static byte[] ReadEncryptedFile(string path, bool hasChecksum)
        {
            return FileEncryptionProvider.ReadEncryptedFile(path, hasChecksum, EncryptionType.Custom);
        }

        public static byte[] ReadEncryptedFile(string path, EncryptionType encryptionType)
        {
            return FileEncryptionProvider.ReadEncryptedFile(path, true, encryptionType);
        }

        private static byte[] ReadEncryptedFile(string path, bool hasChecksum, EncryptionType encryptionType)
        {
            byte[] array = FileManagement.ReadFile(path);
            byte[] result = null;
            if (array != null)
            {
                EncryptionType encryptionType2 = encryptionType;
                if (hasChecksum)
                {
                    if (array.Length <= 0)
                    {
                        throw new InvalidChecksumException("checksum not present " + path);
                    }
                    byte b = array[0];
                    byte b2 = FileEncryptionProvider.CalculateChecksumB(array, 1);
                    if (b == b2)
                    {
                        if (encryptionType == EncryptionType.Aes || encryptionType == EncryptionType.RijndaelManaged)
                        {
                            throw new WrongEncryptionTypeException("checksum failure " + path);
                        }

                        encryptionType2 = EncryptionType.Custom;
                    }
                    else if (b - 1 == b2)
                    {
                        if (encryptionType == EncryptionType.Custom || encryptionType == EncryptionType.RijndaelManaged)
                        {
                            throw new WrongEncryptionTypeException("checksum failure " + path);
                        }

                        encryptionType2 = EncryptionType.Aes;
                    }
                    else
                    {
                        if (b - 2 != b2)
                        {
                            throw new InvalidChecksumException("checksum failure " + path);
                        }

                        if (encryptionType == EncryptionType.Custom || encryptionType == EncryptionType.Aes)
                        {
                            throw new WrongEncryptionTypeException("checksum failure " + path);
                        }

                        encryptionType2 = EncryptionType.RijndaelManaged;
                    }
                }
                else if (encryptionType == EncryptionType.Any)
                {
                    throw new WrongEncryptionTypeException("Encryption type must be specified when not using a checksum");
                }

                try
                {
                    switch (encryptionType2)
                    {
                        case EncryptionType.Custom:
                            result = FileEncryptionProvider.CodeBytes(array, hasChecksum ? 1 : 0, false);
                            break;
                        case EncryptionType.Aes:
                            result = EncryptionProvider.CodeBytes(array, hasChecksum ? 1 : 0, false, 2);
                            break;
                        case EncryptionType.RijndaelManaged:
                            result = EncryptionProvider.CodeBytes(array, hasChecksum ? 1 : 0, false, 3);
                            break;
                    }
                }
                catch (CryptographicException innerException)
                {
                    throw new InvalidChecksumException("checksum failure " + path, innerException);
                }
            }

            return result;
        }

        public static void WriteEncryptedFile(byte[] data, string path, bool withChecksum)
        {
            FileEncryptionProvider.WriteEncryptedFile(data, path, withChecksum, EncryptionType.Custom);
        }

        public static void WriteEncryptedFile(byte[] data, string path, EncryptionType encryptionType)
        {
            FileEncryptionProvider.WriteEncryptedFile(data, path, true, encryptionType);
        }

        private static void WriteEncryptedFile(byte[] data, string path, bool withChecksum, EncryptionType encryptionType)
        {
            FileStream fileStream = File.Create(path);
            byte[] array;

            switch (encryptionType)
            {
                case EncryptionType.Custom:
                    array = FileEncryptionProvider.CodeBytes(data, 0, true);
                    break;
                case EncryptionType.Aes:
                    array = EncryptionProvider.CodeBytes(data, 0, true, 2);
                    break;
                case EncryptionType.RijndaelManaged:
                    array = EncryptionProvider.CodeBytes(data, 0, true, 3);
                    break;
                default:
                    throw new WrongEncryptionTypeException();
            }

            if (withChecksum)
            {
                byte b = FileEncryptionProvider.CalculateChecksumB(array, 0);
                switch (encryptionType)
                {
                    case EncryptionType.Aes:
                        b += 1;
                        break;
                    case EncryptionType.RijndaelManaged:
                        b += 2;
                        break;
                }

                fileStream.WriteByte(b);
            }

            fileStream.Write(array, 0, array.Length);
            fileStream.Close();
        }

        public static string Decrypt(string path, string unencryptedFileName, EncryptionType encryptionType)
        {
            string path2 = Path.Combine(path, FileEncryptionProvider.EncryptFileName(unencryptedFileName));
            string text = Path.Combine(path, unencryptedFileName);
            byte[] array = FileEncryptionProvider.ReadEncryptedFile(path2, true, encryptionType);
            if (array != null)
            {
                FileStream fileStream = File.Create(text);
                fileStream.Write(array, 0, array.Length);
                fileStream.Close();
            }
            else
            {
                text = "";
            }

            return text;
        }

        public static string Encrypt(string path, string unencryptedFileName, EncryptionType encryptionType)
        {
            string path2 = Path.Combine(path, unencryptedFileName);
            string text = Path.Combine(path, FileEncryptionProvider.EncryptFileName(unencryptedFileName));
            byte[] array = FileManagement.ReadFile(path2);
            if (array != null)
            {
                FileEncryptionProvider.WriteEncryptedFile(array, text, true, encryptionType);
            }
            else
            {
                text = "";
            }

            return text;
        }

        private const char FILE_SEPARATOR = '.';

        private const byte KEY_MASK = 63;

        private const byte MASK_CHAR = 42;

        private const byte SWITCH_MASK = 15;

        private const byte HIGH_BYTE = 255;
    }

    namespace Enums
    {
        public enum EncryptionType
        {
            Any,
            Custom,
            Aes,
            RijndaelManaged
        }
    }

    namespace Exceptions
    {
        [Serializable]
        public class BadFileNameException : ArgumentException, ISerializable
        {
            public BadFileNameException()
            {
            }

            protected BadFileNameException(SerializationInfo info, StreamingContext context) : base(info, context)
            {
            }

            public BadFileNameException(string message) : base(message)
            {
            }

            public BadFileNameException(string message, Exception innerException) : base(message, innerException)
            {
            }
        }

        [Serializable]
        public class InvalidChecksumException : IOException
        {
            public InvalidChecksumException()
            {
            }

            public InvalidChecksumException(string message) : base(message)
            {
            }

            public InvalidChecksumException(string message, Exception innerException) : base(message, innerException)
            {
            }

            protected InvalidChecksumException(SerializationInfo info, StreamingContext context) : base(info, context)
            {
            }
        }

        [Serializable]
        public class WrongEncryptionTypeException : IOException
        {
            public WrongEncryptionTypeException()
            {
            }

            public WrongEncryptionTypeException(string message) : base(message)
            {
            }

            public WrongEncryptionTypeException(string message, Exception innerException) : base(message, innerException)
            {
            }

            protected WrongEncryptionTypeException(SerializationInfo info, StreamingContext context) : base(info, context)
            {
            }
        }
    }
}
