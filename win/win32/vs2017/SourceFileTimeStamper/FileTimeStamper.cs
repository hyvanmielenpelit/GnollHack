using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

namespace SourceFileTimeStamper
{
    public class FileTimeStamper
    {
        string _modifiedDateString = "/* GnollHack File Has Last Changed: {0} */";
        string _stampDateString = "/* GnollHack File Time Stamped: {0} */";
        string _separatorString = "";

        public FileTimeStamper()
        {

        }

        public void StampFile(FileInfo file)
        {
            var lines = File.ReadAllLines(file.FullName);

            string modifiedDateStringStart = _modifiedDateString.Substring(0, _modifiedDateString.Length - 6);
            string stampDateStringStart = _stampDateString.Substring(0, _stampDateString.Length - 6);

            var fileLastWriteTimeString = file.LastWriteTimeUtc.ToString("u");
            var nowString = DateTime.Now.ToUniversalTime().ToString("u");

            var modifiedTimeStamp = string.Format(_modifiedDateString, fileLastWriteTimeString);
            var stampTimeStamp = string.Format(_stampDateString, nowString);

            if (lines.Length < 3 || !lines[0].StartsWith(modifiedDateStringStart) || !lines[1].StartsWith(stampDateStringStart) || lines[2] != _separatorString)
            {
                //No stamp, insert
                List<string> lines2 = new List<string>(lines);
                lines2.Insert(0, modifiedTimeStamp);
                Console.WriteLine("- Adding Modified Time Stamp: " + fileLastWriteTimeString);
                lines2.Insert(1, stampTimeStamp);
                Console.WriteLine("- Adding Stamp Time Stamp: " + nowString);
                lines2.Insert(2, _separatorString);

                File.WriteAllLines(file.FullName, lines2);
            }
            else if (lines[0].StartsWith(modifiedDateStringStart) && lines[1].StartsWith(stampDateStringStart) && lines[2] == _separatorString)
            {
                //Already stamped
                var stampedDateString = lines[1].Substring(stampDateStringStart.Length, lines[1].Length - stampDateStringStart.Length - 3);
                var stampedDate = DateTime.ParseExact(stampedDateString, "u", CultureInfo.InvariantCulture);
                if(file.LastWriteTimeUtc.AddSeconds(-10) < stampedDate && stampedDate < file.LastWriteTimeUtc.AddSeconds(10))
                {
                    //Already stamped, do nothing
                    Console.WriteLine("- OK: Already stamped and not modified");
                }
                else
                {
                    //Changed!, update stamps
                    lines[0] = modifiedTimeStamp;
                    Console.WriteLine("- Updating Modified Time Stamp: " + fileLastWriteTimeString);
                    lines[1] = stampTimeStamp;
                    Console.WriteLine("- Updating Stamp Time Stamp: " + nowString);
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
