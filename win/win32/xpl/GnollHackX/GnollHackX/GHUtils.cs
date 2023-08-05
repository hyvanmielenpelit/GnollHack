using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;

namespace GnollHackX
{
    public class GHUtils
    {
        public static int Ctrl(int c)
        {
            return (0x1f & (c));
        }
        public static int Meta(int c)
        {
            return (0x80 | (c));
        }
        public static bool isok(int x, int y)
        {
            if (x < 1 || x >= GHConstants.MapCols)
                return false;
            if (y < 0 || y >= GHConstants.MapRows)
                return false;

            return true;
        }
        public static Color NHColor2Color(nhcolor nhcolor)
        {
            Color res = Color.White;
            switch (nhcolor)
            {
                case nhcolor.CLR_BLACK:
                    res = Color.DarkGray;
                    break;
                case nhcolor.CLR_RED:
                    res = Color.Red;
                    break;
                case nhcolor.CLR_GREEN:
                    res = Color.Green;
                    break;
                case nhcolor.CLR_BROWN:
                    res = Color.Brown;
                    break;
                case nhcolor.CLR_BLUE:
                    res = Color.Blue;
                    break;
                case nhcolor.CLR_MAGENTA:
                    res = Color.Magenta;
                    break;
                case nhcolor.CLR_CYAN:
                    res = Color.Cyan;
                    break;
                case nhcolor.CLR_GRAY:
                    res = Color.LightGray;
                    break;
                case nhcolor.NO_COLOR:
                    break;
                case nhcolor.CLR_ORANGE:
                    res = Color.Orange;
                    break;
                case nhcolor.CLR_BRIGHT_GREEN:
                    res = Color.LightGreen;
                    break;
                case nhcolor.CLR_YELLOW:
                    res = Color.Yellow;
                    break;
                case nhcolor.CLR_BRIGHT_BLUE:
                    res = Color.LightBlue;
                    break;
                case nhcolor.CLR_BRIGHT_MAGENTA:
                    res = Color.LightPink;
                    break;
                case nhcolor.CLR_BRIGHT_CYAN:
                    res = Color.LightCyan;
                    break;
                case nhcolor.CLR_WHITE:
                    res = Color.White;
                    break;
                case nhcolor.CLR_MAX:
                    break;
                default:
                    break;
            }

            return res;
        }

        public static string GetIndentationString(string usedtext, int usedattributes)
        {
            if (usedtext == null || usedtext == "")
                return "";

            string str = usedtext;

            if ((usedattributes & (((int)MenuItemAttributes.IndentDash) | (int)MenuItemAttributes.IndentColon | (int)MenuItemAttributes.IndentAstr | (int)MenuItemAttributes.IndentSpace | (int)MenuItemAttributes.IndentBracket)) != 0)
            {
                string cstr = " ", cstr2 = "";
                if ((usedattributes & (int)MenuItemAttributes.IndentBracketOrDoubleSpace) == (int)MenuItemAttributes.IndentBracketOrDoubleSpace)
                {
                    cstr = "]";
                    cstr2 = "  ";
                }
                else if ((usedattributes & (int)MenuItemAttributes.IndentDoubleSpace) == (int)MenuItemAttributes.IndentDoubleSpace)
                    cstr = "  ";
                else if ((usedattributes & (int)MenuItemAttributes.IndentPeriod) == (int)MenuItemAttributes.IndentPeriod)
                    cstr = ".";
                else if ((usedattributes & (int)MenuItemAttributes.IndentDash) == (int)MenuItemAttributes.IndentDash)
                    cstr = "-";
                else if ((usedattributes & (int)MenuItemAttributes.IndentColon) == (int)MenuItemAttributes.IndentColon)
                    cstr = ":";
                else if ((usedattributes & (int)MenuItemAttributes.IndentAstr) == (int)MenuItemAttributes.IndentAstr)
                    cstr = "*";
                else if ((usedattributes & (int)MenuItemAttributes.IndentSpace) == (int)MenuItemAttributes.IndentSpace)
                    cstr = " ";
                else if ((usedattributes & (int)MenuItemAttributes.IndentBracket) == (int)MenuItemAttributes.IndentBracket)
                    cstr = "]";

                int idx = str.IndexOf(cstr);
                if (idx < 0 && cstr2 != "")
                    idx = str.IndexOf(cstr2);
                if (idx >= 0)
                {
                    int spacepos = idx + cstr.Length;
                    if (spacepos < str.Length)
                    {
                        string searchstr = str.Substring(spacepos);
                        int numberOfSpaces = searchstr.TakeWhile(c => c == ' ').Count();
                        if (spacepos + numberOfSpaces < str.Length)
                            return str.Substring(0, spacepos + numberOfSpaces);

                    }
                    return str;
                }
            }

            return "";
        }

        public static List<string> GetAllStringsFromZeroTerminatedArray(IntPtr ptr)
        {
            return GetAllStringsFromZeroTerminatedArray(ptr, 0);
        }

        public static List<string> GetAllStringsFromZeroTerminatedArray(IntPtr ptr, int maxsize)
        {
            var list = new List<string>();
            int cnt = 0;
            if(ptr != IntPtr.Zero)
            {
                IntPtr strPtr;
                while((strPtr = (IntPtr)Marshal.PtrToStructure(ptr, typeof(IntPtr))) != IntPtr.Zero)
                {
                    if (maxsize > 0 && cnt >= maxsize)
                        break;
                    list.Add(Marshal.PtrToStringAnsi(strPtr));
                    ptr = new IntPtr(ptr.ToInt64() + IntPtr.Size);
                    cnt++;
                }
            }
            return list;
        }

        public static List<string> GetAllStringsFromFixedArray(IntPtr ptr, int size)
        {
            var list = new List<string>();
            if (ptr != IntPtr.Zero)
            {
                for (int i = 0; i < size; i++)
                {
                    IntPtr strPtr = (IntPtr)Marshal.PtrToStructure(ptr, typeof(IntPtr));
                    if (strPtr != IntPtr.Zero)
                    {
                        list.Add(Marshal.PtrToStringAnsi(strPtr));
                    }
                    ptr = new IntPtr(ptr.ToInt64() + IntPtr.Size);
                }
            }
            return list;
        }

        public static string EncryptString(string key, string plainText)
        {
            if (key == null || plainText == null || key == "" || plainText == "") return "";

            byte[] iv = new byte[16];
            byte[] array;

            using (Aes aes = Aes.Create())
            {
                aes.Key = Encoding.UTF8.GetBytes(key);
                aes.IV = iv;

                ICryptoTransform encryptor = aes.CreateEncryptor(aes.Key, aes.IV);

                using (MemoryStream memoryStream = new MemoryStream())
                {
                    using (CryptoStream cryptoStream = new CryptoStream((Stream)memoryStream, encryptor, CryptoStreamMode.Write))
                    {
                        using (StreamWriter streamWriter = new StreamWriter((Stream)cryptoStream))
                        {
                            streamWriter.Write(plainText);
                        }

                        array = memoryStream.ToArray();
                    }
                }
            }

            return Convert.ToBase64String(array);
        }

        public static string DecryptString(string key, string cipherText)
        {
            if (key == null || cipherText == null || key == "" || cipherText == "") return "";

            byte[] iv = new byte[16];
            byte[] buffer = Convert.FromBase64String(cipherText);

            using (Aes aes = Aes.Create())
            {
                aes.Key = Encoding.UTF8.GetBytes(key);
                aes.IV = iv;
                ICryptoTransform decryptor = aes.CreateDecryptor(aes.Key, aes.IV);

                using (MemoryStream memoryStream = new MemoryStream(buffer))
                {
                    using (CryptoStream cryptoStream = new CryptoStream((Stream)memoryStream, decryptor, CryptoStreamMode.Read))
                    {
                        using (StreamReader streamReader = new StreamReader((Stream)cryptoStream))
                        {
                            return streamReader.ReadToEnd();
                        }
                    }
                }
            }
        }
    }
}
