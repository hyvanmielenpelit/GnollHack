using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

namespace SourceFileTimeStamper
{
    public class FileTimeStamper
    {
        string _modifiedDateString = "//** GnollHack File Changed: ";
        string _stampDateString = "//** GnollHack File Time Stamped: ";
        string _separatorString = "";

        public FileTimeStamper()
        {

        }

        public void StampFile(FileInfo file)
        {
            var lines = File.ReadAllLines(file.FullName);

            var fileLastWriteTimeString = file.LastWriteTimeUtc.ToString("u");
            var nowString = DateTime.Now.ToUniversalTime().ToString("u");

            var modifiedTimeStamp = _modifiedDateString + fileLastWriteTimeString;
            var stampTimeStamp = _stampDateString + nowString;

            if (lines.Length < 3 || !lines[0].StartsWith(_modifiedDateString) || !lines[1].StartsWith(_stampDateString) || lines[2] != _separatorString)
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
            else if (lines[0].StartsWith(_modifiedDateString) && lines[1].StartsWith(_stampDateString) && lines[2] == _separatorString)
            {
                //Already stamped
                var stampedDateEnd = lines[1].Substring(_stampDateString.Length);
                var stampedDate = DateTime.ParseExact(stampedDateEnd, "u", CultureInfo.InvariantCulture);
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
