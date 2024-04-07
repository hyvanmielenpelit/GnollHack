using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public static class GHUtils
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

        public static string GetIndentationString(string usedtext, int usedattributes)
        {
            if (usedtext == null || usedtext == "")
                return "";

            string str = usedtext;

            int indentation_bits = (usedattributes & (int)MenuItemAttributes.IndentMask);
            if (indentation_bits != 0)
            {
                string cstr = " ", cstr2 = "";
                switch(indentation_bits)
                {
                    case (int)MenuItemAttributes.IndentBracketOrDoubleSpace:
                        cstr = "]";
                        cstr2 = "  ";
                        break;
                    case (int)MenuItemAttributes.IndentDoubleSpace:
                        cstr = "  ";
                        break;
                    case (int)MenuItemAttributes.IndentPeriod:
                        cstr = ".";
                        break;
                    case (int)MenuItemAttributes.IndentDash:
                        cstr = "-";
                        break;
                    case (int)MenuItemAttributes.IndentColon:
                        cstr = ":";
                        break;
                    case (int)MenuItemAttributes.IndentAstr:
                        cstr = "*";
                        break;
                    case (int)MenuItemAttributes.IndentSpace:
                        cstr = " ";
                        break;
                    case (int)MenuItemAttributes.IndentBracket:
                        cstr = "]";
                        break;
                    default:
                        break;
                }

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

        public static string StrToKey(string str)
        {
            if (string.IsNullOrWhiteSpace(str))
                return "";
            else
                str.Replace("M", "X");

            string key = "";
            int cnt = 0;
            for(int i = 0; i < str.Length && cnt < 32; i += 2, cnt++)
            {
                if (i >= str.Length)
                    break;

                key += str.Substring(i, 1);
            }
            if (key.Length < 32)
            {
                int missingLength = 32 - key.Length;
                for(int i = 0; i < missingLength; i++)
                {
                    key = key + (char)(i + 32);
                }
            }
            return key;
        }

        public static string DecryptString(string key, string cipherText)
        {
            if (key == null || cipherText == null || key == "" || cipherText == "") return "";

            try
            {
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
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("DecryptString: " + ex.Message);
            }
            return "";
        }
    }
}
