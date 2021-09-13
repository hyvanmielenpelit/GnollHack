using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;

namespace SourceFileTimeStamper
{
    public class FileTimeStamper
    {
        string _modifiedDateString = "/* GnollHack File Has Last Changed: {0} */";
        string _separatorString = "";
        string _gitDateFormatString = @"yyyy\-MM\-dd HH\:mm\:ss zz00";
        string _dayFormatString = @"yyyy\-MM\-dd";

        public FileTimeStamper()
        {

        }

        public void StampFile(FileInfo file)
        {
            var lines = File.ReadAllLines(file.FullName);

            string modifiedDateStringStart = _modifiedDateString.Substring(0, _modifiedDateString.Length - 6);

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

            if (lines.Length < 3 || !lines[0].StartsWith(modifiedDateStringStart) || lines[1] != _separatorString)
            {
                //No stamp, insert
                List<string> lines2 = new List<string>(lines);
                lines2.Insert(0, nowTimeStamp);
                Console.WriteLine("- Adding Modified Time Stamp: " + nowTimeString);
                lines2.Insert(1, _separatorString);

                File.WriteAllLines(file.FullName, lines2);
            }
            else if (lines[0].StartsWith(modifiedDateStringStart) && lines[1] == _separatorString)
            {
                //Already stamped
                var modifiedDateString = lines[0].Substring(modifiedDateStringStart.Length, lines[0].Length - modifiedDateStringStart.Length - 3);
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
                    lines[0] = nowTimeStamp;
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
