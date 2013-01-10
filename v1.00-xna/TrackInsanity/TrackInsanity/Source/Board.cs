/****************************************************************************
*
* Board.cs - Game board access functions
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
    public class BoardSquare
    {
        public const int STATION = 0;
        public const int CENTRAL_STATION = 1;
        public const int TILE = 2;
        public const int CORNER = 3;
        public const int PLAYED_TILE = 4;

        public int type;
        public int trainPresent;
        public int tileIndex;

        /****************************************************************************
        * setTileIndex
        *
        * Description:
        *   Sets the tile index of a particular board square.  This function
        *   doesn't check to see if the move is legal (that's the responsibility
        *   of placeTile())
        *
        ****************************************************************************/
        public int setTileIndex(int index)
        {
            if (type == TILE)
            {
                tileIndex = index;
                type = PLAYED_TILE;
                return TrackInsanity.OK;
            }

            return TrackInsanity.ERROR;
        }

        /* Makes a deep copy of a BoardSquare */
        public BoardSquare Copy()
        {
            BoardSquare sq = new BoardSquare();
            sq.type = type;
            sq.trainPresent = trainPresent;
            sq.tileIndex = tileIndex;

            return sq;
        }
    };

    public class Board
    {
        public const int WIDTH = 10;
        public const int HEIGHT = 10;
        public const int NUM_STATIONS = 32;

        public const int TRACK_NOT_COMPLETE = 0;
        public const int TRACK_COMPLETE = 1;
        public const int TRACK_PROCESSED = 2;

        public const int ILLEGAL_MOVE = 0;
        public const int LEGAL_MOVE = 1;

        /* This array contains the list of stations owned by each player in a n-player game.  The 0th and 1st
         * rows in the array aren't used (and are set to zero).  Each item in the array represents one of the
         * 32 stations, and the player number that owns it */
        private static readonly int[,] TI_BOARD_STATION_DATA = 
        {
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
            { 2, 1, 3, 2, 3, 2, 1, 3, 1, 3 ,2, 1, 3 ,1, 2, 0, 0, 3 ,1 ,2, 3, 1, 2, 3, 2, 3 ,1, 2, 1, 3, 2, 1 },
            { 3, 4, 1, 2, 4, 3, 2, 1, 4, 3, 2, 1, 4, 3, 1, 2, 4, 3, 1, 2, 3, 4, 2, 1, 3, 4, 2, 1, 4, 3, 1, 2 },
            { 2, 4, 3, 5, 2, 1, 3, 5, 4, 2, 5, 1, 4, 2, 3, 0, 0, 1, 3, 5, 4, 2, 1, 5, 3, 4, 1, 2, 3, 4, 5, 1 },
            { 2, 1, 5, 3, 2, 4, 6, 3, 5, 2, 1, 6, 5, 3, 4, 0, 0, 1, 2, 4, 3, 6, 5, 4, 1, 3, 2, 6, 1, 5, 4, 6 }
        };

        public enum playerTrains
        {
            NO_TRAIN,
            P1_TRAIN,
            P2_TRAIN,
            P3_TRAIN,
            P4_TRAIN,
            P5_TRAIN,
            P6_TRAIN
        };

        /* Actual components of a board */
        public BoardSquare[,] b;           /* Type of each square in the board */
        private int[,] legalMove;           /* Move legality status for each square on the board */
        private int[] trackStatus;           /* Current contents of each square on the board */

        public int[,] playerStations;      /* List of player stations */
        public TilePool tp;

        public Board(TilePool t)
        {
            int counter, counter2;

            /* Initialize the arrays and class instances */
            b = new BoardSquare[WIDTH, HEIGHT];
            legalMove = new int[WIDTH, HEIGHT];
            trackStatus = new int[NUM_STATIONS];
            playerStations = new int[TrackInsanity.MAX_PLAYERS + 1, NUM_STATIONS];

            /* Set the board's tilepool */
            tp = t;

            /* Mark each station as having incomplete tracks */
            for (counter = 0; counter < NUM_STATIONS; counter++)
            {
                trackStatus[counter] = TRACK_NOT_COMPLETE;
            }

            /* Set each board position to 'illegal move' -- a function will be called for
             * each player turn that determines which moves are legal */
            for (counter = 0; counter < WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < HEIGHT; counter2++)
                {
                    legalMove[counter, counter2] = ILLEGAL_MOVE;
                }
            }

            /* Set up the board square data for each board position */
            for (counter = 0; counter < WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < HEIGHT; counter2++)
                {
                    b[counter, counter2] = new BoardSquare();
                    b[counter, counter2].tileIndex = Tile.NONE;
                    b[counter, counter2].trainPresent = (int)(Board.playerTrains.NO_TRAIN);
                }
            }

            /* Set the outer station squares */
            for (counter = 1; counter < WIDTH - 1; counter++)
            {
                b[counter, 0].type = BoardSquare.STATION;
                b[counter, HEIGHT - 1].type = BoardSquare.STATION;
            }
            for (counter = 1; counter < HEIGHT - 1; counter++)
            {
                b[0, counter].type = BoardSquare.STATION;
                b[WIDTH - 1, counter].type = BoardSquare.STATION;
            }

            /* Set the tile type squares */
            for (counter = 1; counter < WIDTH - 1; counter++)
            {
                for (counter2 = 1; counter2 < HEIGHT - 1; counter2++)
                {
                    b[counter, counter2].type = BoardSquare.TILE;
                }
            }

            /* Set the central station squares */
            b[4, 4].type = BoardSquare.CENTRAL_STATION;
            b[4, 5].type = BoardSquare.CENTRAL_STATION;
            b[5, 4].type = BoardSquare.CENTRAL_STATION;
            b[5, 5].type = BoardSquare.CENTRAL_STATION;

            /* Set the corner squares */
            b[0, 0].type = BoardSquare.CORNER;
            b[0, HEIGHT - 1].type = BoardSquare.CORNER;
            b[WIDTH - 1, 0].type = BoardSquare.CORNER;
            b[WIDTH - 1, HEIGHT - 1].type = BoardSquare.CORNER;

            /* Read station data */
            for (counter = TrackInsanity.MIN_PLAYERS; counter <= TrackInsanity.MAX_PLAYERS; counter++)
            {
                for (counter2 = 0; counter2 < NUM_STATIONS; counter2++)
                {
                    playerStations[counter, counter2] = TI_BOARD_STATION_DATA[counter, counter2];
                }
            }
        }

        /****************************************************************************
         * Copy
         *
         * Description:  Creates a copy of the board. 
         *               Note: This is a *deep* copy, not a shallow one!
         *
         ****************************************************************************/
        public Board Copy()
        {
            int counter, counter2;
            TilePool tpc = tp.Copy();
            Board brd = new Board(tpc);

            for (counter = 0; counter < WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < HEIGHT; counter2++)
                {
                    brd.b[counter, counter2] = b[counter, counter2].Copy();
                    brd.legalMove = legalMove;
                }
            }

            for (counter = 0; counter < NUM_STATIONS; counter++)
            {
                brd.trackStatus = trackStatus;
            }

            for(counter=0;counter<TrackInsanity.MAX_PLAYERS + 1; counter++)
            {
                for(counter2=0;counter2<NUM_STATIONS; counter2++)
                {
                    brd.playerStations[counter,counter2] = playerStations[counter,counter2];
                }
            }

            return brd;
        }

        /****************************************************************************
         * findNextTrackSection
         *
         * Description:
         *   Based on a particular board coordinate pair and exit point, determine
         *   the tile coordinate and entry point for the adjacent tile.
         *
         ****************************************************************************/
        public int findNextTrackSection(int x, int y, int exit, ref int newX, ref int newY, ref int enter)
        {
            /* Given the provided exit position, determine what new board section and entry point 
             * we end up on. */
            switch (exit)
            {
                case 0:
                    newX = x;
                    newY = y - 1;
                    enter = 5;
                    break;
                case 1:
                    newX = x;
                    newY = y - 1;
                    enter = 4;
                    break;
                case 2:
                    newX = x + 1;
                    newY = y;
                    enter = 7;
                    break;
                case 3:
                    newX = x + 1;
                    newY = y;
                    enter = 6;
                    break;
                case 4:
                    newX = x;
                    newY = y + 1;
                    enter = 1;
                    break;
                case 5:
                    newX = x;
                    newY = y + 1;
                    enter = 0;
                    break;
                case 6:
                    newX = x - 1;
                    newY = y;
                    enter = 3;
                    break;
                case 7:
                    newX = x - 1;
                    newY = y;
                    enter = 2;
                    break;
                default:
                    newX = x;
                    newY = y;
                    enter = exit;
                    break;
            }
       
            /* Return the square type of the new position */
            return b[newX, newY].type;
        }

        /****************************************************************************
        * markLegalMoves
        *
        * Description:
        *   Given a particular tile, determine and mark the legal places on the
        *   board in which the tile can be placed.
        *
        ****************************************************************************/
        public int markLegalMoves(Tile t)
        {
            int counter, counter2, legalMoves;

            /* Mark all moves as legal */
            for (counter = 0; counter < WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < HEIGHT; counter2++)
                {
                    legalMove[counter, counter2] = LEGAL_MOVE;
                }
            }

            /* Mark all of the stations and corner squares as illegal */
            for (counter = 0; counter < WIDTH; counter++)
            {
                legalMove[counter, 0] = ILLEGAL_MOVE;
                legalMove[counter, HEIGHT - 1] = ILLEGAL_MOVE;
            }
            for (counter = 0; counter < HEIGHT; counter++)
            {
                legalMove[0, counter] = ILLEGAL_MOVE;
                legalMove[WIDTH - 1, counter] = ILLEGAL_MOVE;
            }
            legalMove[4, 4] = ILLEGAL_MOVE;
            legalMove[4, 5] = ILLEGAL_MOVE;
            legalMove[5, 4] = ILLEGAL_MOVE;
            legalMove[5, 5] = ILLEGAL_MOVE;

            /* Mark all of the already played squares as illegal */
            for (counter = 1; counter < WIDTH - 1; counter++)
            {
                for (counter2 = 1; counter2 < HEIGHT - 1; counter2++)
                {
                    if (b[counter, counter2].type == BoardSquare.PLAYED_TILE)
                    {
                        legalMove[counter, counter2] = ILLEGAL_MOVE;
                    }
                }
            }

            /* Check the inner station squares for adjacent tiles */
            for (counter = 2; counter < WIDTH - 2; counter++)
            {
                for (counter2 = 2; counter2 < HEIGHT - 2; counter2++)
                {
                    if ((b[counter + 1, counter2].type != BoardSquare.PLAYED_TILE) &&
                        (b[counter - 1, counter2].type != BoardSquare.PLAYED_TILE) &&
                        (b[counter, counter2 - 1].type != BoardSquare.PLAYED_TILE) &&
                        (b[counter, counter2 + 1].type != BoardSquare.PLAYED_TILE))
                    {
                        legalMove[counter, counter2] = ILLEGAL_MOVE;
                    }
                }
            }

            /* Check the outer station squares for track lengths of 1.  This check will be performed with 8
             * individual checks -- 4 sides and each of the 4 corners. */
            for (counter = 1; counter < WIDTH - 1; counter++)
            {
                /* Check #1, #2 */
                if (t.findExit(0) == 1)
                {
                    legalMove[counter, 1] = ILLEGAL_MOVE;
                }
                if (t.findExit(5) == 4)
                {
                    legalMove[counter, HEIGHT - 2] = ILLEGAL_MOVE;
                }
            }
            for (counter = 1; counter < HEIGHT - 1; counter++)
            {
                /* Check #3, #4 */
                if (t.findExit(3) == 2)
                {
                    legalMove[WIDTH - 2, counter] = ILLEGAL_MOVE;
                }
                if (t.findExit(7) == 6)
                {
                    legalMove[1, counter] = ILLEGAL_MOVE;
                }
            }
            /* Check #5 (top left corner) */
            if (t.findExit(6) == 1 || t.findExit(0) == 7)
            {
                legalMove[1, 1] = ILLEGAL_MOVE;
            }
            /* Check #6 (top right corner) */
            if (t.findExit(2) == 1 || t.findExit(3) == 0)
            {
                legalMove[WIDTH - 2, 1] = ILLEGAL_MOVE;
            }
            /* Check #7 (bottom right corner) */
            if (t.findExit(4) == 3 || t.findExit(5) == 2)
            {
                legalMove[WIDTH - 2, HEIGHT - 2] = ILLEGAL_MOVE;
            }
            /* Check #8 (bottom left hand corner) */
            if (t.findExit(5) == 6 || t.findExit(4) == 7)
            {
                legalMove[1, HEIGHT - 2] = ILLEGAL_MOVE;
            }

            /* Count the legal moves */
            legalMoves = 0;
            for (counter = 0; counter < WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < HEIGHT; counter2++)
                {
                    if (legalMove[counter, counter2] == LEGAL_MOVE)
                    {
                        legalMoves++;
                    }
                }
            }

            /* If there are no legal moves, then check the tilepool.  If no cards
             * remain, mark all normally illegal empty spaces as legal, otherwise,
             * return the number of legal moves (0). */
            if (legalMoves == 0 && tp.numUnplayedTiles == 0)
            {
                for (counter = 1; counter < WIDTH - 1; counter++)
                {
                    for (counter2 = 1; counter2 < HEIGHT - 1; counter2++)
                    {
                        if (b[counter, counter2].type == BoardSquare.TILE)
                        {
                            legalMove[counter, counter2] = LEGAL_MOVE;
                        }
                    }
                }
            }
            else
            {
                return legalMoves;
            }

            /* count the legal moves (again) */
            legalMoves = 0;
            for (counter = 0; counter < WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < HEIGHT; counter2++)
                {
                    if (legalMove[counter, counter2] == LEGAL_MOVE)
                    {
                        legalMoves++;
                    }
                }
            }

            return legalMoves;
        }

        /****************************************************************************
        * getStationInfo
        *
        * Description:
        *   Given a station number, convert the value to a (x,y) coordinate pair
        *   that represent the location of the station on the board, and an exit
        *   point for that station.
        *
        ****************************************************************************/
        public void getStationInfo(int station, ref int x, ref int y, ref int exit)
        {
           
            /* Note: this code is specific to an 8x8 board with 32 stations */
            if (station >= 0 && station < 8)
            {
                x = 1 + station;
                y = 0;
                exit = 5;
            }
            else if (station >= 8 && station < 16)
            {
                x = 9;
                y = 1 + (station - 8);
                exit = 7;
            }
            else if (station >= 16 && station < 24)
            {
                x = 8 - (station - 16);
                y = 9;
                exit = 1;
            }
            else if (station >= 24 && station < 32)
            {
                x = 0;
                y = 8 - (station - 24);
                exit = 3;
            }
            else
            {
                x = 0;
                y = 0;
                exit = 0;
            }
        }

        /****************************************************************************
        * getStationNumber (b)
        *
        * Description:
        *   Given an (x,y) coordinate pair, return the station number (if a
        *   station exists).
        *
        ****************************************************************************/
        public int getStationNumber(int x, int y)
        {
            int station;

            /* Note: this code is specific to an 8x8 board with 32 stations */
            /* Left stations (24,31) */
            if (x == 0)
            {
                if (y >= 1 && y < HEIGHT - 1)
                {
                    station = 32 - y;
                }
                else
                {
                    station = -1;
                }
            }
            /* Right stations (8-15) */
            else if (x == WIDTH - 1)
            {
                if (y >= 1 && y < HEIGHT - 1)
                {
                    station = 7 + y;
                }
                else
                {
                    station = -1;
                }
            }
            /* Top stations (0-7) */
            else if (y == 0)
            {
                if (x >= 1 && x < WIDTH - 1)
                {
                    station = x - 1;
                }
                else
                {
                    station = -1;
                }
            }
            /* Bottom stations (16-23) */
            else if (y == HEIGHT - 1)
            {
                if (x >= 1 && x < WIDTH - 1)
                {
                    station = 24 - x;
                }
                else
                {
                    station = -1;
                }
            }
            else
            {
                station = -1;
            }

            return station;
        }

        /****************************************************************************
        * calculateTrackScore
        *
        * Description:
        *   For a given board and station, calculate the score obtained my moving
        *   the train at the station from the station to the destination at the
        *   other end of the track.
        *
        ****************************************************************************/
        public int calculateTrackScore(int station, int passThruTileId, ref Nullable<int> passThruTile, ref int destination)
        {
            int stationX = -1, stationY = -1, stationExit = -1;
            int newX = -1, newY = -1, newExit = -1, newType = -1;
            int oldX = -1, oldY = -1, oldExit = -1;
            int loopCatcher;
            int loopLimit = 255;     /* Catch infinite looping */
            int score = 0;

            if (passThruTileId != Tile.NONE && passThruTile != null)
            {
                passThruTile = TrackInsanity.FALSE;
            }

            /* Determine the starting point for calculating score */
            getStationInfo(station, ref stationX, ref stationY, ref stationExit);

            /* Are we starting at a station? */
            if (b[stationX, stationY].type != BoardSquare.STATION)
            {
                System.Diagnostics.Debug.WriteLine("calculateTrackScore: starting point isn't station");
                return TrackInsanity.ERROR;
            }

            /* Is there a train there? */
            if (b[stationX, stationY].trainPresent == (int)(playerTrains.NO_TRAIN))
            {
                System.Diagnostics.Debug.WriteLine("calculateTrackScore: no train at station");
                return TrackInsanity.ERROR;
            }

            newType = findNextTrackSection(stationX, stationY, stationExit, ref newX, ref newY, ref newExit);

            /* XXX Figure out what the hell this is supposed to do! */
            loopCatcher = 0;
            score = 0;
            while((newType == BoardSquare.PLAYED_TILE) && (loopCatcher < loopLimit))
            {
                if(passThruTileId != Tile.NONE &&
                    b[newX, newY].tileIndex == passThruTileId &&
                    passThruTile != null)
                {
                    passThruTile = TrackInsanity.TRUE;
                }
                score++;
                loopCatcher++;
                oldX = newX;
                oldY = newY;
                oldExit = tp.getTile(b[newX, newY].tileIndex).findExit(newExit);
                newType = findNextTrackSection(oldX, oldY, oldExit, ref newX, ref newY, ref newExit);
            }

            /* If a potential infinite loop was caught, break from it */
            if (loopCatcher >= loopLimit)
            {
                System.Diagnostics.Debug.WriteLine("calculateTrackScore: infinite loop caught");
                return TrackInsanity.ERROR;
            }

            /* Determine the type of the tile that the track stops at */
            destination = b[newX, newY].type;

            /* If the destination isn't a station of some kind, that's bad */
            if (b[newX, newY].type != BoardSquare.STATION &&
               b[newX, newY].type != BoardSquare.CENTRAL_STATION &&
                passThruTileId == Tile.NONE)
            {
                System.Diagnostics.Debug.WriteLine("calculateTrackScore: destination not station");
                return TrackInsanity.ERROR;
            }

            /* If the destination was a central station, double the score */
            if (b[newX, newY].type == BoardSquare.CENTRAL_STATION)
            {
                score = score * 2;
            }

            return score;
        }

        /****************************************************************************
        * placeTile
        *
        * Description:
        *   Places a tile specified by tileIndex on the specified board, after
        *   making sure that the chosen location is legal.
        *
        ****************************************************************************/
        public int placeTile(int x, int y, int index)
        {
            int result;

            /* If the indices are out of range, the move is illegal */
            if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
            {
                return ILLEGAL_MOVE;
            }

            /* If the square is a station, the move is illegal */
            if (b[x, y].type != BoardSquare.TILE)
            {
                return ILLEGAL_MOVE;
            }

            /* If the square is marked as an illegal move, don't place the tile */
            if (legalMove[x, y] != LEGAL_MOVE)
            {
                return ILLEGAL_MOVE;
            }

            /* As a sanity check, if a tile is already placed here, don't place the tile */
            if (b[x, y].tileIndex != Tile.NONE)
            {
                return ILLEGAL_MOVE;
            }

            /* If all of these checks pass, place the tile */
            result = b[x, y].setTileIndex(index);

            if (result == TrackInsanity.ERROR)
            {
                return ILLEGAL_MOVE;
            }

            return LEGAL_MOVE;           
        }

        /****************************************************************************
        * isLegalMove
        *
        * Description:
        *   Checks to see if playing a tile at the specified location is allowed.
        *
        ****************************************************************************/
        public int isLegalMove(int x, int y)
        {
            if (legalMove[x, y] == LEGAL_MOVE)
            {
                return TrackInsanity.TRUE;
            }

            return TrackInsanity.FALSE;
        }

        /****************************************************************************
        * getTrackStatus
        *
        * Description:
        *   get accessor for the trackStatus array.
        *
        ****************************************************************************/
        public int getTrackStatus(int index)
        {
            if (index < 0 || index > NUM_STATIONS)
            {
                return TrackInsanity.ERROR;
            }
         
            return trackStatus[index];
        }

        /****************************************************************************
        * setTrackStatus
        *
        * Description:
        *   set accessor for the trackStatus array.
        *
        ****************************************************************************/
        public void setTrackStatus(int index, int status)
        {
            if (index >= 0 && index < NUM_STATIONS)
            {
                trackStatus[index] = status;
            }
        }

        /****************************************************************************
        * removeTile
        *
        * Description:
        *   Removes a tile from the board
        *
        ****************************************************************************/
        public int removeTile(int x, int y)
        {
            /* If the indices are out of range, the movie is illegal */
            if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
            {
                return TrackInsanity.ERROR;
            }

            /* If the square is not a played tile, then the move is illegal */
            if (b[x, y].type != BoardSquare.PLAYED_TILE)
            {
                return TrackInsanity.ERROR;
            }

            /* If a tile isn't there, then it can't be removed */
            if (b[x, y].tileIndex == Tile.NONE)
            {
                return TrackInsanity.ERROR;
            }

            b[x, y].tileIndex = Tile.NONE;
            b[x, y].type = BoardSquare.TILE;

            return TrackInsanity.OK;
        }
    }
}
