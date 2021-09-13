using SourceFileTimeStamper.Data;
using System;
using System.Collections.Generic;
using System.IO;

namespace SourceFileTimeStamper
{
    class Program
    {
        public static FileTimeStamper TimeStamper { get; set; }
        public static string BaseDirectoryRelativePath { get; set; }
        public static List<DirectoryData> DirectoriesToProcess { get; set; }

        static void Main(string[] args)
        {
            TimeStamper = new FileTimeStamper();
            BaseDirectoryRelativePath = @"..\..\..\..\..\..\..";
            DirectoriesToProcess = new List<DirectoryData>()
            {
                new DirectoryData()
                {
                    RelativePath = "src",
                    Extensions = new List<string>() { "*.c" }
                }
            };

            string baseDirectoryPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), BaseDirectoryRelativePath));

            Console.WriteLine("Base Directory Path: " + baseDirectoryPath);

            foreach (var directoryData in DirectoriesToProcess)
            {
                string directoryPath = Path.Combine(baseDirectoryPath, directoryData.RelativePath);
                Console.WriteLine("Processing Directory: " + directoryData.RelativePath);
                foreach (string searchPattern in directoryData.Extensions)
                {
                    var filePaths = Directory.GetFiles(directoryPath, searchPattern, SearchOption.TopDirectoryOnly);
                    foreach(string filePath in filePaths)
                    {
                        Console.WriteLine("Processing File: " + directoryData.RelativePath + @"\" + Path.GetFileName(filePath));
                        FileInfo file = new FileInfo(filePath);
                        try
                        {
                            TimeStamper.StampFile(file);
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine("Exception: " + ex.Message);
                        }
                        break;
                    }
                }
            }
            
            Console.WriteLine("Finished");            
        }
    }
}

