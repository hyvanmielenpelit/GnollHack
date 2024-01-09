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
                    SearchPatterns = new List<string>() { "*.c", "*.cpp" },
                    ExcludedFiles = new List<string>() { "animation.c", "animdef.c", "encounter.c", "matcomps.c", "npc.c", "rm.c", 
                        "soundset.c", "tiledata.c", "vis_tab.c" }
                },
                new DirectoryData()
                {
                    RelativePath = "include",
                    SearchPatterns = new List<string>() { "*.h", "*.hpp" },
                    ExcludedFiles = new List<string>() { "animation.h", "animoff.h", "animtotals.h", "encounter.h", "general.h", 
                        "layers.h", "action.h", "matcomps.h", "npc.h", "soundset.h", "tiledata.h", "date.h", "pm.h", "onames.h",
                        "vis_tab.h", "dgn_comp.h", "lev_comp.h" }
                },
                new DirectoryData()
                {
                    RelativePath = @"win",
                    SearchPatterns = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp" },
                    DirectorySearchOption = SearchOption.AllDirectories,
                    ExcludedFiles = new List<string>() { "callback.c", "callback.h", "dllhack.c", "dllmain.c", "dllproc.c", "dllhack.h", "dllproc.h",
                        "date_unix.h", "gnollhackdroid.h", "gnollhackios.h", "gnhapi.h", "libproc.h", "pch.h", "gnhmain.c", "gnhunix.c", "gnhapi.c", "gnollhackdroid.c", "gnollhackios.c", "libproc.c",
                        "soundfx.cpp", "soundfx.h", "mhwingdi.cpp", "mhwingdi.h", "resource.h", "nhresource.h" }
                },
                new DirectoryData()
                {
                    RelativePath = @"sys",
                    SearchPatterns = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp", "sysconf", "sysconfdgl" },
                    DirectorySearchOption = SearchOption.AllDirectories,
                    ExcludedFiles = new List<string>() { "dgn_comp.h", "dgn_lex.c", "dgn_yacc.c", "lev_comp.h", "lev_lex.c", "lev_yacc.c" }
                },
                new DirectoryData()
                {
                    RelativePath = @"sys\unix",
                    SearchPatterns = new List<string>() { "Makefile.dat", "Makefile.src", "Makefile.top", "Makefile.utl" },
                },
                new DirectoryData()
                {
                    RelativePath = "util",
                    SearchPatterns = new List<string>() { "*.c", "*.cpp", "*.h", "*.hpp", "*.l", "*.y" },
                    ExcludedFiles = new List<string>() { "dgn_comp.tab.c", "dgn_lex.c", "dgn_yacc.c", "lev_comp.tab.c", "lev_lex.c", "lev_yacc.c", "lex.yy.c",
                    "dgn_comp.tab.h", "lev_comp.tab.h" }
                },
                new DirectoryData()
                {
                    RelativePath = "dat",
                    SearchPatterns = new List<string>() { "*.des", "*.def", "*.txt", "symbols" },
                    ExcludedFiles = new List<string>() { "oracles.txt", "engrave.txt", "epitaph.txt" }
                },
            };

            string baseDirectoryPath = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), BaseDirectoryRelativePath));

            Console.WriteLine("Base Directory Path: " + baseDirectoryPath);
            int count = 0;
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
                        bool doexclude = false;
                        foreach(string exclfile in directoryData.ExcludedFiles)
                        {
                            if(exclfile == file.Name)
                            {
                                doexclude = true;
                                break;
                            }
                        }
                        if (doexclude)
                        {
                            Console.WriteLine("Excluded File: " + directoryData.RelativePath + @"\" + Path.GetFileName(filePath));
                            continue;
                        }

                        try
                        {
                            TimeStamper.StampFile(file);
                            count++;
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine("Exception: " + ex.Message);
                        }
                    }
                }
            }
            
            Console.WriteLine("Finished. Processed " + count + " files.");            
        }
    }
}

