using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

/* The Server class.  Maintains all information required to retain state for multiple, in-progress
 * games between networked multiplayer clients.
 */

namespace TrackInsanity.Source
{
    /* ServerState contains a single, contained entity that represents an ongoing game on the server.  
     * This includes player information, tile pool and board information. */
    class ServerState
    {
        private int stateId;        
        private Board b;
        private TilePool tp;
        private int playerCount;
        private int[] playerState;

        public ServerState()
        {
        }
    }

    class Server
    {
        public const int MAX_IN_PROGRESS_GAMES = 32;

        // Important state components
        private ServerState[] state;

        public Server()
        {
            System.Console.WriteLine("Creating support for " + MAX_IN_PROGRESS_GAMES + " game states...");
            state = new ServerState[MAX_IN_PROGRESS_GAMES];

            /* Initialize network components */
            System.Console.WriteLine("Initializing network interface...");
        }

        public int Run()
        {
            System.Console.WriteLine("Starting server main loop...");
            return 0;
        }
    }
}
