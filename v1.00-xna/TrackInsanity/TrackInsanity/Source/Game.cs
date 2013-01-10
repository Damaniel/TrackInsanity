/****************************************************************************
*
* Game.cs - Information about the current game state.
*
* Copyright 2007-2010 Shaun Brandt / Holy Meatgoat Software
*     <damaniel@damaniel.org>
*
* This file is part of TrackInsanity.
*
* TrackInsanity is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* TrackInsanity is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TrackInsanity; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TrackInsanity.Source
{
    public class Game
    {
        public enum GameState
        {
            DEFAULT,
            NEXT_PLAYER_TURN,
            SELECT_ACTION,
            TILE_DRAW,
            TILE_SELECT,
            TILE_PLAY,
            PASS,
            DISCARD,
            END_TURN,
            GAME_FINISHED,
            CONFIRM_EXIT,
            SHOW_BOARD,
            COMPUTER_MOVE,
            PAUSE
        };

        private const int CPU_MIN_THINK_TIME = 1200;
        private const int CPU_MAX_THINK_TIME = 2200;

        public const int OPTION_NO = 0;
        public const int OPTION_YES = 1;
        public const int OPTION_LAST_PLAYER = 2;
        public const int OPTION_ALL_PLAYERS = 3;
        public const int OPTION_AI_EASY = 0;
        public const int OPTION_AI_MEDIUM = 1;
        public const int OPTION_AI_HARD = 2;

        public Board board;
        public TilePool tilepool;
        public Player[] players;
        public int numPlayers { get; set; }
        public int numStationsPerPlayer { get; set; }
        public int gameState { get; set; }
        public int previousGameState { get; set; }
        public int gameStateChanged { get; set; }
        public int curPlayer { get; set; }

        /* Options in the 'options' screen */
        public int highlightTracks { get; set; }
        public int highlightLegalMoves { get; set; }
        public int showLastMove { get; set; }
        public int musicVolume { get; set; }
        public int effectsVolume { get; set; }
        public int defaultAILevel { get; set; }

        /* Temporary set options (the real values will only be changed to these if 'OK' is selected) */
        public int tmpHighlightTracks { get; set; }
        public int tmpHighlightLegalMoves { get; set; }
        public int tmpShowLastMove { get; set; }
        public int tmpMusicVolume { get; set; }
        public int tmpEffectsVolume { get; set; }
        public int tmpDefaultAILevel { get; set; }

        /* Used by various game states to determine the validity of particular options */
        public int playerHasDrawn { get; set; }
        public int playIsValid { get; set; }
        public int drawIsValid { get; set; }
        public int discardIsValid { get; set; }
        public int passIsValid { get; set; }

        public int selectedMoveTileX { get; set; }
        public int selectedMoveTileY { get; set; }
        public int selectedMoveTileId { get; set; }
        public int selectedMoveIsReserveTile { get; set; }

        /****************************************************************************
        * Game
        *
        * Description:
        *   Constructor
        *
        ****************************************************************************/
        public Game()
        {
            tilepool = new TilePool();
            board = new Board(tilepool);

            highlightTracks = OPTION_YES;
            highlightLegalMoves = OPTION_YES;
            showLastMove = OPTION_ALL_PLAYERS;
            defaultAILevel = OPTION_AI_MEDIUM;
            musicVolume = 4;
            effectsVolume = 4;

            tmpHighlightTracks = OPTION_YES;
            tmpHighlightLegalMoves = OPTION_YES;
            tmpShowLastMove = OPTION_ALL_PLAYERS;
            tmpDefaultAILevel = OPTION_AI_MEDIUM;
            tmpMusicVolume = 4;
            tmpEffectsVolume = 4;

            if(reset() == TrackInsanity.ERROR)
            {
                System.Diagnostics.Debug.WriteLine("Game: unable to reset the game structure!");
            }

            players = new Player[TrackInsanity.MAX_PLAYERS];
        }

        /****************************************************************************
        * reset
        *
        * Description:
        *   Sets the game to its initial state.
        *
        ****************************************************************************/
        public int reset()
        {
            tilepool = new TilePool();
            board = new Board(tilepool);

            numPlayers = 0;
            curPlayer = 0;
            gameState = (int)(GameState.DEFAULT);
            previousGameState = (int)(GameState.DEFAULT);
            playerHasDrawn = TrackInsanity.FALSE;
            playIsValid = TrackInsanity.FALSE;
            drawIsValid = TrackInsanity.FALSE;
            discardIsValid = TrackInsanity.FALSE;
            passIsValid = TrackInsanity.FALSE;
            selectedMoveTileX = -1;
            selectedMoveTileY = -1;
            selectedMoveTileId = Tile.NONE;
            selectedMoveIsReserveTile = TrackInsanity.FALSE;

            return TrackInsanity.OK;    
        }

        /****************************************************************************
        * initPlayersFromUi
        *
        * Description:
        *   Uses settings gathered from the player to set up the game.
        *
        ****************************************************************************/
        public int initPlayersFromUi()
        {
            int counter;
            int stationX = -1, stationY = -1, exit = -1;
            int aiLevel;

            if(SharedData.selectedPlayers < TrackInsanity.MIN_PLAYERS || SharedData.selectedPlayers > TrackInsanity.MAX_PLAYERS)
            {
                System.Diagnostics.Debug.WriteLine("initPlayersFromUi: Invalid number of players selected");
                return TrackInsanity.ERROR;
            }

            if(defaultAILevel == OPTION_AI_EASY)
            {
                aiLevel = Player.AI_DEFAULT;
            }
            else if(defaultAILevel == OPTION_AI_MEDIUM)
            {
                aiLevel = Player.AI_SMARTER;
            }
            else
            {
                aiLevel = Player.AI_SMARTEST;
            }

            System.Diagnostics.Debug.WriteLine("initPlayersFromUi: Set AI level to {0}", aiLevel);

            /* Initialize active players */
            for(counter=0;counter<SharedData.selectedPlayers;counter++)
            {
                System.Diagnostics.Debug.WriteLine("initPlayersFromUi: Initializing active player {0}", counter + 1);
                players[counter] = new Player(SharedData.playerState[counter], 0, aiLevel);
            }

            /* Initialize inactive players */
            for(counter=SharedData.selectedPlayers;counter<TrackInsanity.MAX_PLAYERS; counter++)
            {
                players[counter] = new Player(Player.NOBODY, 0, aiLevel);
            }

            /* Set the number of stations that each player receives (pretty much just for the UI) */
            numStationsPerPlayer = (int)(Board.NUM_STATIONS / SharedData.selectedPlayers);

            /* Set the number of players for this game */
            numPlayers = SharedData.selectedPlayers;
            
            /* Set the current player to player 1 */
            curPlayer = 0;

            /* Populate all station tiles with the appropriate owner */
            for(counter=0;counter<Board.NUM_STATIONS; counter++)
            {
                board.getStationInfo(counter, ref stationX, ref stationY, ref exit);
                board.b[stationX, stationY].trainPresent = board.playerStations[numPlayers,counter];
            }

            return TrackInsanity.OK;
        }

        /****************************************************************************
        * changeState
        *
        * Description:
        *   Changes the current game state. 
        *
        ****************************************************************************/
        public int changeState(int state)
        {
            int tileIdToUse;
            int numLegalMoves;
            int counter, humanPlayers;

            switch(state)
            {
                case (int)(GameState.DEFAULT):
                    break;
                case (int)(GameState.NEXT_PLAYER_TURN):
                    System.Diagnostics.Debug.WriteLine("Changing game state to NEXT_PLAYER_TURN");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    /* Check to see if there's only one human player.  If so, skip the 'player n's turn' dialog */
                    humanPlayers = 0;                    
                    for (counter = 0; counter < numPlayers; counter++)
                    {
                        if (players[counter].controlledBy == Player.HUMAN)
                        {
                            humanPlayers++;
                        }
                    }
                    if (humanPlayers == 1 || players[curPlayer].controlledBy == Player.COMPUTER)
                    {
                        changeState((int)GameState.SELECT_ACTION);
                    }
                    break;
                case (int)(GameState.SELECT_ACTION):
                    System.Diagnostics.Debug.WriteLine("Changing game state to SELECT_ACTION");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    /* Move into the computer move state (if applicable) */
                    if(players[curPlayer].controlledBy == Player.COMPUTER)
                    {
                        changeState((int)(GameState.COMPUTER_MOVE));
                    }
                    else
                    {
                        /* If a player holds 2 tiles, make sure 'Draw tile' is disabled */
                        if(players[curPlayer].currentTileId != Tile.NONE &&
                            players[curPlayer].reserveTileId != Tile.NONE)
                        {
                            playerHasDrawn = TrackInsanity.TRUE;
                        }

                        /* If a reserve tile is present, select it by default */
                        if(players[curPlayer].reserveTileId != Tile.NONE)
                        {
                            selectedMoveIsReserveTile = TrackInsanity.TRUE;
                        }
                        else
                        {
                            selectedMoveIsReserveTile = TrackInsanity.FALSE;
                        }
                    }
                    break;
                case (int)(GameState.TILE_DRAW):
                    System.Diagnostics.Debug.WriteLine("Changing game state to TILE_DRAW");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    break;
                case (int)(GameState.TILE_SELECT):
                    System.Diagnostics.Debug.WriteLine("Changing game state to TILE_SELECT");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    if (players[curPlayer].reserveTileId != Tile.NONE)
                    {
                        tileIdToUse = players[curPlayer].reserveTileId;
                        selectedMoveIsReserveTile = TrackInsanity.TRUE;
                    }
                    else
                    {
                        tileIdToUse = players[curPlayer].currentTileId;
                        selectedMoveIsReserveTile = TrackInsanity.FALSE;
                    }
                    selectedMoveTileId = tileIdToUse;
                    numLegalMoves = board.markLegalMoves(tilepool.getTile(tileIdToUse));
                    break;
                case (int)(GameState.TILE_PLAY):
                    System.Diagnostics.Debug.WriteLine("Changing game state to TILE_PLAY");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    board.placeTile(selectedMoveTileX, selectedMoveTileY, selectedMoveTileId);
                    checkForCompletedTracks();
                    selectedMoveTileX = -1;
                    selectedMoveTileY = -1;
                    selectedMoveTileId = Tile.NONE;
                    changeState((int)Game.GameState.END_TURN);
                    break;                        
                case (int)(GameState.DISCARD):
                    System.Diagnostics.Debug.WriteLine("Changing game state to DISCARD");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    break;
                case (int)(GameState.END_TURN):
                    System.Diagnostics.Debug.WriteLine("Changing game state to END_TURN");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    playerHasDrawn = TrackInsanity.FALSE;
                    /* Increment the player number */
                    curPlayer++;
                    if (curPlayer >= numPlayers)
                    {
                        /* If we've reached the end of the player list, wrap around to player 1 */
                        curPlayer = 0;
                    }
                    /* Check for end of game */
                    if (checkForEndOfGame() == TrackInsanity.TRUE)
                    {
                        changeState((int)(GameState.GAME_FINISHED));
                    }
                    else
                    {
                        /* Set the initial game state for the next player */
                        changeState((int)(GameState.NEXT_PLAYER_TURN));
                    }
                    break;
                case (int)(GameState.CONFIRM_EXIT):
                    System.Diagnostics.Debug.WriteLine("Changing game state to CONFIRM_EXIT");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    break;
                case (int)(GameState.GAME_FINISHED):
                    System.Diagnostics.Debug.WriteLine("Changing game state to GAME_FINISHED");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    break;
                case (int)(GameState.SHOW_BOARD):
                    System.Diagnostics.Debug.WriteLine("Changing game state to SHOW_BOARD");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    break;
                case (int)(GameState.COMPUTER_MOVE):
                    System.Diagnostics.Debug.WriteLine("Changing game state to COMPUTER_MOVE");
                    previousGameState = gameState;
                    gameState = state;
                    gameStateChanged = TrackInsanity.TRUE;
                    SharedData.cpuMoveAllowed = false;
                    SharedData.cpuDelayTicks = SharedData.curTicks + RNG.GetRandomInt(Game.CPU_MIN_THINK_TIME, Game.CPU_MAX_THINK_TIME);
                    System.Diagnostics.Debug.WriteLine("setting delay to {0}...", SharedData.cpuDelayTicks);
                    break;
                default:
                    return TrackInsanity.ERROR;
            }
            return TrackInsanity.OK;
        }

        /****************************************************************************
        * checkForEndOfGame
        *
        * Description:
        *   Check the board and tilepool to see if the end-of-game conditions have
        *   been met.
        *
        ****************************************************************************/
        public int checkForEndOfGame()
        {
            int gameEnded = TrackInsanity.TRUE;
            int counter;

            /* If there are tiles in the tile pool, the game isn't over yet.  */
            if (tilepool.numUnplayedTiles > 0)
            {
                gameEnded = TrackInsanity.FALSE;
            }

            /* If any player still has tiles left, the game isn't over */
            for (counter = 0; counter < numPlayers; counter++)
            {
                if (players[counter].currentTileId != Tile.NONE ||
                   players[counter].reserveTileId != Tile.NONE)
                {
                    gameEnded = TrackInsanity.FALSE;
                }
            }

            return gameEnded;
        }

        /****************************************************************************
        * discardTile
        *
        * Description:
        *   Process a player's discarded tile. 
        *
        ****************************************************************************/
        public int discardTile()
        {
            int result;

            result = tilepool.returnTileToPool(selectedMoveTileId);
            if (result != TrackInsanity.OK)
            {
                return TrackInsanity.ERROR;
            }

            if (selectedMoveIsReserveTile == TrackInsanity.TRUE)
            {
                players[curPlayer].reserveTileId = Tile.NONE;
            }
            else
            {
                if (players[curPlayer].reserveTileId != Tile.NONE)
                {
                    players[curPlayer].currentTileId = players[curPlayer].reserveTileId;
                    players[curPlayer].reserveTileId = Tile.NONE;
                }
                else
                {
                    players[curPlayer].currentTileId = Tile.NONE;
                }
            }

            return TrackInsanity.OK;
        }

        /****************************************************************************
        * checkForCompletedTracks
        *
        * Description:
        *   Checks each station to see if a track extends from it to another station.
        *
        ****************************************************************************/
        public int checkForCompletedTracks()
        {
            int counter;
            int stationX = -1, stationY = -1, stationExit = -1;
            int oldX, oldY, oldExit;
            int newX = -1, newY = -1, newExit = -1;
            int newType;
            int loopCatcher, loopLimit = 255;

            for (counter = 0; counter < Board.NUM_STATIONS; counter++)
            {
                board.getStationInfo(counter, ref stationX, ref stationY, ref stationExit);
                if (board.b[stationX, stationY].type != BoardSquare.STATION)
                {
                    System.Diagnostics.Debug.WriteLine("checkForCompletedTracks: starting point isn't station");
                    return TrackInsanity.ERROR;
                }

                newType = board.findNextTrackSection(stationX, stationY, stationExit, ref newX, ref newY, ref newExit);

                loopCatcher = 0;
                while (newType == BoardSquare.PLAYED_TILE && loopCatcher < loopLimit)
                {
                    loopCatcher++;
                    oldX = newX;
                    oldY = newY;
                    oldExit = board.tp.getTile(board.b[newX, newY].tileIndex).findExit(newExit);
                    newType = board.findNextTrackSection(oldX, oldY, oldExit, ref newX, ref newY, ref newExit);
                }

                if (loopCatcher >= loopLimit)
                {
                    System.Diagnostics.Debug.WriteLine("checkForCompletedTracks: infinite loop caught");
                    return TrackInsanity.ERROR;
                }

                /* If the destination isn't a station of some kind, the track isn't complete */
                if (board.b[newX, newY].type == BoardSquare.STATION ||
                   board.b[newX, newY].type == BoardSquare.CENTRAL_STATION)
                {
                    if (board.getTrackStatus(counter) != Board.TRACK_PROCESSED)
                    {
                        board.setTrackStatus(counter, Board.TRACK_COMPLETE);
                        processCompletedTrack(counter);
                    }
                }
            }

            return TrackInsanity.OK;
        }

        /****************************************************************************
        * processCompletedTrack
        *
        * Description:
        *   Used to determine the score given by the selected track, and to assign
        *   the score to the appropriate player. 
        *
        ****************************************************************************/
        public int processCompletedTrack(int station)
        {
            int stationX = -1, stationY = -1, stationExit = -1;
            int score, player, destination = -1;
            Nullable<int> passthru = null;

            if (board.getTrackStatus(station) != Board.TRACK_COMPLETE)
            {
                /* Do nothing, the track isn't complete */
                return TrackInsanity.OK;
            }

            /* If the number of players results in a board with stations that don't belong to anyone,
             * and one of those tracks is completed, ignore it. */
            if (board.playerStations[numPlayers, station] == (int)(Board.playerTrains.NO_TRAIN))
            {
                return TrackInsanity.OK;
            }

            if (station < 0 || station >= Board.NUM_STATIONS)
            {
                System.Diagnostics.Debug.WriteLine("processCompletedTracks: station index out of range");
                return TrackInsanity.ERROR;
            }

            score = board.calculateTrackScore(station, Tile.NONE, ref passthru, ref destination);

            /* Determine which player gets the points */
            player = board.playerStations[numPlayers, station];
            players[player - 1].score += score;
            players[player - 1].numStationsComplete++;

            /* Mark the starting station as empty and the track as processed */
            board.getStationInfo(station, ref stationX, ref stationY, ref stationExit);
            board.b[stationX, stationY].trainPresent = (int)Board.playerTrains.NO_TRAIN;
            board.setTrackStatus(station, Board.TRACK_PROCESSED);

            return TrackInsanity.OK;
        }

        /****************************************************************************
        * applyComputerMove
        *
        * Description:
        *   Takes the generated CPU move and processes it as if it were a human-made
        *   move.
        *
        ****************************************************************************/
        public void applyComputerMove()
        {
            ComputerAIPacket cur;

            cur = (ComputerAIPacket)SharedData.currentMove;

            switch (cur.moveType)
            {
                case (int)(ComputerAI.Action.DRAW):
                    /* Add a delay here */
                    if ( players[ curPlayer].currentTileId == Tile.NONE)
                    {
                         players[ curPlayer].currentTileId =  tilepool.drawRandomTile();
                         selectedMoveIsReserveTile = TrackInsanity.FALSE;
                         selectedMoveTileId =  players[ curPlayer].currentTileId;
                    }
                    else
                    {
                         players[ curPlayer].reserveTileId =  tilepool.drawRandomTile();
                         selectedMoveIsReserveTile = TrackInsanity.TRUE;
                         selectedMoveTileId =  players[ curPlayer].reserveTileId;
                    }
                    break;
                case (int)(ComputerAI.Action.PLAY):
                    /* Add a delay here */
                     selectedMoveTileX = cur.moveX;
                     selectedMoveTileY = cur.moveY;
                    if (cur.heldTile == ComputerAI.HELD_TILE_PRIMARY)
                    {
                         selectedMoveTileId =  players[ curPlayer].currentTileId;
                         selectedMoveIsReserveTile = TrackInsanity.FALSE;
                        if ( players[ curPlayer].reserveTileId != Tile.NONE)
                        {
                             players[ curPlayer].currentTileId =  players[ curPlayer].reserveTileId;
                             players[ curPlayer].reserveTileId = Tile.NONE;
                        }
                        else
                        {
                             players[ curPlayer].currentTileId = Tile.NONE;
                        }
                    }
                    else if (cur.heldTile == ComputerAI.HELD_TILE_RESERVE)
                    {
                         selectedMoveTileId =  players[ curPlayer].reserveTileId;
                         selectedMoveIsReserveTile = TrackInsanity.TRUE;
                         players[ curPlayer].reserveTileId = Tile.NONE;
                    }
                     board.markLegalMoves( tilepool.getTile( selectedMoveTileId));
                     board.placeTile( selectedMoveTileX,  selectedMoveTileY,  selectedMoveTileId);
                     players[ curPlayer].lastMoveX =  selectedMoveTileX;
                     players[ curPlayer].lastMoveY =  selectedMoveTileY;
                     checkForCompletedTracks();
                    break;
                case (int)(ComputerAI.Action.DISCARD):
                    /* Delay */
                    if (cur.heldTile == ComputerAI.HELD_TILE_PRIMARY)
                    {
                         selectedMoveTileId =  players[ curPlayer].currentTileId;
                         selectedMoveIsReserveTile = TrackInsanity.FALSE;
                    }
                    else if (cur.heldTile == ComputerAI.HELD_TILE_RESERVE)
                    {
                         selectedMoveTileId =  players[ curPlayer].reserveTileId;
                         selectedMoveIsReserveTile = TrackInsanity.TRUE;
                    }
                     discardTile();
                    break;
                case (int)(ComputerAI.Action.END_TURN):
                    /* Nothing -- the game never gets here */
                    break;
            }
        }

    }
}
