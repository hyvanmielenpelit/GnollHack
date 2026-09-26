using GnollHack.PerformanceAnalyzer.Commands;

namespace GnollHack.PerformanceAnalyzer
{
    /* Offline analysis for the GnollHack performance suites. See DEVEL/performance/README.md. */
    public static class Program
    {
        public static int Main(string[] argv)
        {
            Args a = new Args(argv);
            try
            {
                switch (a.Command)
                {
                case "ingest":
                    return IngestCommand.Run(a);
                case "compare":
                    return CompareCommand.Run(a);
                case "smoothness":
                    return SmoothnessCommand.Run(a);
                case "history":
                    return HistoryCommand.Run(a);
                case "help":
                case null:
                    PrintHelp();
                    return a.Command == null ? 2 : 0;
                default:
                    Console.Error.WriteLine("unknown command: " + a.Command);
                    PrintHelp();
                    return 2;
                }
            }
            catch (ArgumentException ex)
            {
                Console.Error.WriteLine("error: " + ex.Message);
                return 2;
            }
            catch (IOException ex)
            {
                Console.Error.WriteLine("error: " + ex.Message);
                return 1;
            }
            catch (InvalidDataException ex)
            {
                Console.Error.WriteLine("error: " + ex.Message);
                return 1;
            }
        }

        private static void PrintHelp()
        {
            Console.WriteLine("GnollHack.PerformanceAnalyzer");
            Console.WriteLine();
            Console.WriteLine("  ingest  --presentmon <csv> | --gfxinfo <dir|file>  --out <run.json>");
            Console.WriteLine("          [--arm A] [--scenario W1] [--scenario-kind gameplay|replay] [--platform Windows|Android]");
            Console.WriteLine("          [--process <exe name>] [--refresh-hz 60] [--target-fps <fps>] [--warmup <s>] [--window <s>]");
            Console.WriteLine("          [--env-before <json>] [--env-after <json>] [--build-config Release]");
            Console.WriteLine("          [--commit <sha>] [--tag <tag>] [--dirty] [--device-id X] [--device-model X] [--device-os X]");
            Console.WriteLine("          [--run-json <in-app run.json>]");
            Console.WriteLine("          [--version-<name> <v> ...] [--config-<name> <v> ...] [--include-throttled] [--notes text]");
            Console.WriteLine();
            Console.WriteLine("  compare --a <dir|run.json ...> --b <dir|run.json ...> [--label-a X] [--label-b Y]");
            Console.WriteLine("          [--series external|internal|smoothness] [--resamples 2000] [--seed 1]");
            Console.WriteLine("          [--include-throttled] [--include-excluded] [--allow-mixed-cadence] [--out report.md]");
            Console.WriteLine();
            Console.WriteLine("  smoothness <in-app run.json> [--presentmon <csv>] [--process <exe name>]");
            Console.WriteLine("          [--perfetto <dir>] [--out report.md]");
            Console.WriteLine();
            Console.WriteLine("  history --file <history.jsonl> --append <run.json|dir ...> [--allow-debug]");
            Console.WriteLine("  history --file <history.jsonl> --list [--scenario W1] [--platform Windows]");
        }
    }
}
