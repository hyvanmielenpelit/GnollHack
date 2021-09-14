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
                    Extensions = new List<string>() { "*.c", "*.cpp" }
                },
                new DirectoryData()
                {
                    RelativePath = "include",
                    Extensions = new List<string>() { "*.h", "*.hpp" }
                },
                new DirectoryData()
                {
                    RelativePath = @"win",
                    Extensions = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp" },
                    DirectorySearchOption = SearchOption.AllDirectories
                },
                new DirectoryData()
                {
                    RelativePath = @"sys",
                    Extensions = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp" },
                    DirectorySearchOption = SearchOption.AllDirectories
                },
                new DirectoryData()
                {
                    RelativePath = "util",
                    Extensions = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp", "*.l", "*.y" }
                },
                new DirectoryData()
                {
                    RelativePath = "dat",
                    Extensions = new List<string>() { "*.des", "*.def" }
                },
            };

            string baseDirectoryPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), BaseDirectoryRelativePath));

            Console.WriteLine("Base Directory Path: " + baseDirectoryPath);

            foreach (var directoryData in DirectoriesToProcess)
            {
                string directoryPath = Path.Combine(baseDirectoryPath, directoryData.RelativePath);
                Console.WriteLine("Processing Directory: " + directoryData.RelativePath);
                foreach (string searchPattern in directoryData.Extensions)
                {
                    var filePaths = Directory.GetFiles(directoryPath, searchPattern, directoryData.DirectorySearchOption);
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

