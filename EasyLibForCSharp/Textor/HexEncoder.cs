using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EasyLibForCSharp.Textor
{
    public sealed class HexEncoder
    {
        private HexEncoder()
        {
        }

        public static string EncodeHex(byte[] bytes, bool dashes)
        {
            StringBuilder stringBuilder = new StringBuilder();
            for (int i = 0; i < bytes.Length; i++)
            {
                stringBuilder.Append(string.Format(CultureInfo.InvariantCulture, "{0:X2}", new object[]
                {
                    bytes[i]
                }));
                if (dashes && i < bytes.Length - 1)
                {
                    stringBuilder.Append("-");
                }
            }
            return stringBuilder.ToString();
        }

        public static string EncodeHex(byte[] bytes)
        {
            return HexEncoder.EncodeHex(bytes, false);
        }

        public static byte[] DecodeHex(string encodedNumber)
        {
            encodedNumber = encodedNumber.Replace("-", "");
            int num = encodedNumber.Length / 2;
            byte[] array = new byte[num];
            for (int i = 0; i < num; i++)
            {
                array[i] = byte.Parse(encodedNumber.Substring(i * 2, 2), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            }
            return array;
        }
    }
}
