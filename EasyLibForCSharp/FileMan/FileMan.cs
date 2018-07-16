using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using Microsoft.Win32;

namespace EasyLibForCSharp.FileMan
{
    public static class FileManagement
    {
        public enum ExpirationAge
        {
            Never,
            OneDay,
            OneWeek,
            OneMonth,
            ThreeMonths,
            SixMonths,
            OneYear
        }

        public static int DeleteExpiredFiles(string path, ExpirationAge expirationAge)
        {
            return FileManagement.DeleteExpiredFiles(path, expirationAge, false);
        }

        public static int DeleteExpiredFiles(string path, ExpirationAge expirationAge, bool subfolders)
        {
            return FileManagement.DeleteExpiredFiles(path, expirationAge, subfolders, "*.*");
        }

        public static int DeleteExpiredFiles(string path, ExpirationAge expirationAge, bool subfolders, string searchPattern)
        {
            int num = 0;
            if (Directory.Exists(path) && expirationAge != ExpirationAge.Never)
            {
                string[] files = Directory.GetFiles(path, searchPattern, subfolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly);
                TimeSpan expirationAgeTimeSpan = FileManagement.GetExpirationAgeTimeSpan(expirationAge);
                foreach (string text in files)
                {
                    try
                    {
                        if (FileManagement.FileExpired(text, expirationAgeTimeSpan))
                        {
                            File.Delete(text);
                            num++;
                        }
                    }
                    catch (IOException)
                    {
                    }
                    catch (UnauthorizedAccessException)
                    {
                    }
                }
            }

            return num;
        }

        public static bool FileExpired(string filePath, ExpirationAge expirationAge)
        {
            return FileManagement.FileExpired(filePath, FileManagement.GetExpirationAgeTimeSpan(expirationAge));
        }

        public static bool FileExpired(string filePath, TimeSpan timeSpan)
        {
            bool result = false;
            if (File.Exists(filePath))
            {
                TimeSpan t = DateTime.UtcNow - File.GetLastWriteTimeUtc(filePath);
                result = (t > timeSpan);
            }

            return result;
        }

        public static TimeSpan GetExpirationAgeTimeSpan(ExpirationAge expirationAge)
        {
            TimeSpan maxValue = TimeSpan.MaxValue;
            switch (expirationAge)
            {
                case ExpirationAge.OneDay:
                    maxValue = new TimeSpan(1, 0, 0, 0);
                    break;
                case ExpirationAge.OneWeek:
                    maxValue = new TimeSpan(7, 0, 0, 0);
                    break;
                case ExpirationAge.OneMonth:
                    maxValue = new TimeSpan(30, 0, 0, 0);
                    break;
                case ExpirationAge.ThreeMonths:
                    maxValue = new TimeSpan(90, 0, 0, 0);
                    break;
                case ExpirationAge.SixMonths:
                    maxValue = new TimeSpan(180, 0, 0, 0);
                    break;
                case ExpirationAge.OneYear:
                    maxValue = new TimeSpan(365, 0, 0, 0);
                    break;
            }

            return maxValue;
        }

        public static byte[] ReadFile(string path)
        {
            byte[] array = null;
            try
            {
                FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read);
                array = new byte[fileStream.Length];
                fileStream.Read(array, 0, (int)fileStream.Length);
                fileStream.Close();
            }
            catch (FileNotFoundException)
            {
                throw;
            }

            return array;
        }

        public static void EnsureWritePossible(string path)
        {
            if (File.Exists(path))
            {
                FileAttributes fileAttributes = File.GetAttributes(path);
                if ((fileAttributes & FileAttributes.ReadOnly) != (FileAttributes)0)
                {
                    fileAttributes &= ~FileAttributes.ReadOnly;
                    File.SetAttributes(path, fileAttributes);
                }
            }
        }

        public static string FormatFileSize(long sizeInBytes)
        {
            int num = 0;
            double num2 = (double)sizeInBytes;
            while (num < FileManagement.SizeUnit.Length && num2 >= 1024.0)
            {
                num2 /= 1024.0;
                num++;
            }

            return string.Format(CultureInfo.InvariantCulture, "{0:f0} {1}", new object[]
            {
                Math.Round(num2, 0),
                FileManagement.SizeUnit[num]
            });
        }

        public static void CopyToMemoryStream(Stream stream, MemoryStream memoryStream)
        {
            int num = 0;
            int num2;
            do
            {
                memoryStream.SetLength((long)(num + FileManagement.BlockSize));
                num2 = stream.Read(memoryStream.GetBuffer(), num, FileManagement.BlockSize);
                num += FileManagement.BlockSize;
            }
            while (num2 == FileManagement.BlockSize);
        }

        public static bool AreFilesEqual(string path1, string path2)
        {
            if (string.Equals(path1, path2, StringComparison.OrdinalIgnoreCase))
            {
                return true;
            }

            FileInfo fileInfo = new FileInfo(path1);
            FileInfo fileInfo2 = new FileInfo(path2);
            if (fileInfo.Length != fileInfo2.Length)
            {
                return false;
            }

            FileStream fileStream = fileInfo.OpenRead();
            FileStream fileStream2 = fileInfo2.OpenRead();
            byte[] array = new byte[FileManagement.BlockSize];
            byte[] array2 = new byte[FileManagement.BlockSize];
            bool flag = true;
            int num;
            do
            {
                num = fileStream.Read(array, 0, FileManagement.BlockSize);
                fileStream2.Read(array2, 0, FileManagement.BlockSize);
                for (int i = 0; i < FileManagement.BlockSize; i++)
                {
                    if (array[i] != array2[i])
                    {
                        flag = false;
                        break;
                    }
                }
            }
            while (flag && num == FileManagement.BlockSize);

            return flag;
        }

        public static ICollection<string> ProhibitedFile
        {
            get
            {
                return FileManagement.ProhibitedLibraries;
            }
            set
            {
                FileManagement.ProhibitedLibraries = value;
            }
        }

        public static Assembly LoadAssembly(string path)
        {
            if (FileManagement.ProhibitedLibraries.Contains(Path.GetFileName(path), StringComparer.OrdinalIgnoreCase))
            {
                return null;
            }

            Assembly result = null;
            try
            {
                result = Assembly.LoadFrom(path);
            }
            catch (BadImageFormatException)
            {
            }

            return result;
        }

        static FileManagement()
        {
        }

        private static ICollection<string> ProhibitedLibraries;
        private static readonly string[] SizeUnit = new string[]
        {
            "B",
            "KB",
            "MB",
            "GB",
            "TB",
            "PB",
            "EB"
        };
        private static readonly int BlockSize = 32768;
    }



    
}
