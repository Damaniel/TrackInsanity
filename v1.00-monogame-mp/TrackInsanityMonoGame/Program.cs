using System;

namespace TrackInsanity.Source
{
#if WINDOWS || XBOX
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            if(args.Length > 0 && (args[0] == "-S" || args[0] == "--server"))
            {
                System.Console.WriteLine("Invoking as server...");
                Server s = new Server();
                s.Run();
            }
            else 
            {
                System.Console.WriteLine("Invoking as client...");
                using (TrackInsanity game = new TrackInsanity())
                {
                    game.Run();
                }
            }
        }
    }
#endif
}

