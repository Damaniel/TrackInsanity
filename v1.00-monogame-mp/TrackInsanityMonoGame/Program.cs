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
            using (TrackInsanity game = new TrackInsanity())
            {
                game.Run();
            }
        }
    }
#endif
}

