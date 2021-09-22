using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;

namespace SourceFileTimeStamper
{
    public enum ModifificationMarkStyles
    {
        C = 0,
        Des,
        YaccLex,
        Shell,
        MaxStyles
    }

    public class FileTimeStamper
    {
        string _baseModifiedDateString = "GnollHack File Change Notice: This file has been changed from the original. Date of last change: {0}";
        string[] _firstLineString = new string[(int)ModifificationMarkStyles.MaxStyles]
        {
            null,
            null,
            "%{",
            "#!/bin/sh",
        };
        string[] _prefixString = new string[(int)ModifificationMarkStyles.MaxStyles]
        {
            "/* ",
            "# ",
            "/* ",
            "# ",
        };
        string[] _suffixString = new string[(int)ModifificationMarkStyles.MaxStyles]
        {
            " */",
            "",
            " */",
            "",
        };

        string _separatorString = "";
        string _gitDateFormatString = @"yyyy\-MM\-dd HH\:mm\:ss zz00";
        string _dayFormatString = @"yyyy\-MM\-dd";

        public FileTimeStamper()
        {

        }

        public void StampFile(FileInfo file)
        {
            ModifificationMarkStyles style = 0;
            string ext = file.Extension;
            switch(ext)
            {
                case ".c":
                case ".cpp":
                case ".h":
                case ".hpp":
                    style = ModifificationMarkStyles.C;
                    break;
                case ".des":
                case ".def":
                case ".txt":
                case "":
                    style = ModifificationMarkStyles.Des;
                    break;
                case ".sh":
                    style = ModifificationMarkStyles.Shell;
                    break;
                case ".y":
                case ".l":
                    style = ModifificationMarkStyles.YaccLex;
                    break;
                default:
                    if(file.Name.StartsWith("Makefile."))
                    {
                        style = ModifificationMarkStyles.Des;
                        break;
                    }
                    else
                        return;
            }

            Encoding used_encoding = Encoding.UTF8;
            if (file.Name == "quest.txt")
                used_encoding = Encoding.Default;

            var lines = File.ReadAllLines(file.FullName, used_encoding);
            string _modifiedDateString = _prefixString[(int)style] + _baseModifiedDateString + _suffixString[(int)style];

            string modifiedDateStringStart = _modifiedDateString.Substring(0, _modifiedDateString.Length - 3 - _suffixString[(int)style].Length);

            Process p = new Process();
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = "git";
            p.StartInfo.Arguments = "log -1 --format=\"%ai\" " + file.FullName;
            p.Start();
            string gitTimeStamp = p.StandardOutput.ReadToEnd().Trim();
            p.WaitForExit();

            var gitTime = DateTime.ParseExact(gitTimeStamp, _gitDateFormatString, CultureInfo.InvariantCulture);
            var gitTimeString = gitTime.ToUniversalTime().ToString(_dayFormatString);
            //var modifiedTimeStamp = string.Format(_modifiedDateString, gitTimeString);

            var nowTimeString = DateTime.Now.ToUniversalTime().ToString(_dayFormatString);
            var nowTimeStamp = string.Format(_modifiedDateString, nowTimeString);

            int modLineNum = 0;
            int sepLineNum = 1;
            int minlinenum = 3;
            bool has_first_line = false;
            if(_firstLineString[(int)style] != null)
            {
                has_first_line = true;
                modLineNum++;
                sepLineNum++;
                minlinenum++;
            }

            if (has_first_line && !lines[0].StartsWith(_firstLineString[(int)style]))
            {
                //Error
                throw new Exception(file.FullName + " does not start with the correct line.");
            }
            else if (lines.Length < minlinenum
                || !lines[modLineNum].StartsWith(modifiedDateStringStart) || lines[sepLineNum] != _separatorString)
            {
                //No stamp, insert
                List<string> lines2 = new List<string>(lines);
                lines2.Insert(modLineNum, nowTimeStamp);
                Console.WriteLine("- Adding Modified Time Stamp: " + nowTimeString);
                lines2.Insert(sepLineNum, _separatorString);

                File.WriteAllLines(file.FullName, lines2, used_encoding);
            }
            else if (lines[modLineNum].StartsWith(modifiedDateStringStart) && lines[sepLineNum] == _separatorString)
            {
                //Already stamped
                var modifiedDateString = lines[modLineNum].Substring(modifiedDateStringStart.Length, lines[modLineNum].Length - modifiedDateStringStart.Length - _suffixString[(int)style].Length);
                //var modifiedDate = DateTime.ParseExact(modifiedDateString, _dayFormatString, CultureInfo.InvariantCulture);
                if (modifiedDateString == gitTimeString)
                {
                    //Already stamped with Git Date
                    Console.WriteLine("- OK: Already stamped with Git Time");
                }
                else if (modifiedDateString == nowTimeString)
                {
                    //Already stamped, do nothing
                    Console.WriteLine("- OK: Already stamped with Today");
                }
                else
                {
                    //Changed!, update stamps
                    lines[modLineNum] = nowTimeStamp;
                    Console.WriteLine("- Updating Modified Time Stamp: " + nowTimeString);
                    File.WriteAllLines(file.FullName, lines);
                }
            }
            else
            {
                //Error
                throw new Exception(file.FullName + " is somehow wrong.");
            }
        }
    }
}
