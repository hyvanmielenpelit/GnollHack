using System;

namespace GnollHackMG
{
    public static class Program
    {
        [STAThread]
        static void Main()
        {
            using (var game = new GnollHackGame())
                game.Run();
        }
    }
}
