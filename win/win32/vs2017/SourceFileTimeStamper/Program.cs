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
                    SearchPatterns = new List<string>() { "*.c", "*.cpp" }
                },
                new DirectoryData()
                {
                    RelativePath = "include",
                    SearchPatterns = new List<string>() { "*.h", "*.hpp" }
                },
                new DirectoryData()
                {
                    RelativePath = @"win",
                    SearchPatterns = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp" },
                    DirectorySearchOption = SearchOption.AllDirectories
                },
                new DirectoryData()
                {
                    RelativePath = @"sys",
                    SearchPatterns = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp", "sysconf", "sysconfdgl" },
                    DirectorySearchOption = SearchOption.AllDirectories
                },
                new DirectoryData()
                {
                    RelativePath = @"sys\unix",
                    SearchPatterns = new List<string>() { "Makefile.dat", "Makefile.src", "Makefile.top", "Makefile.utl" },
                },
                new DirectoryData()
                {
                    RelativePath = "util",
                    SearchPatterns = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp", "*.l", "*.y" }
                },
                new DirectoryData()
                {
                    RelativePath = "dat",
                    SearchPatterns = new List<string>() { "*.des", "*.def", "symbols" },
                },
            };

            string baseDirectoryPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), BaseDirectoryRelativePath));

            Console.WriteLine("Base Directory Path: " + baseDirectoryPath);

            foreach (var directoryData in DirectoriesToProcess)
            {
                string directoryPath = Path.Combine(baseDirectoryPath, directoryData.RelativePath);
                Console.WriteLine("Processing Directory: " + directoryData.RelativePath);
                foreach (string searchPattern in directoryData.SearchPatterns)
                {
                    var filePaths = Directory.GetFiles(directoryPath, searchPattern, directoryData.DirectorySearchOption);

                    foreach (string filePath in filePaths)
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
                    }
                }
            }
            
            Console.WriteLine("Finished");            
        }
    }
}

