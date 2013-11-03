/****************************************************************************
*
* ComputerAI.cs - implementation of computer player AI.
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
    /* ComputerAIPacket holds information about a possible computer move.  It holds the action
     * (draw, play, pass, discard), position on the board to play (if applicable), and the tile
     * to use when playing (primary or secondary). */
    public struct ComputerAIPacket
    {
        public int moveType { get; set; }
        public int moveX { get; set; }
        public int moveY { get; set; }
        public int heldTile { get; set; }
    };

    /* This holds information about the results of playing a particular tile in a location on 
     * the board.  'Value' contains the score for the move, used to determine the relative
     * strength of the move. */
    public class AIMoveEval : IComparable
    {
        public int tileType { get; set; }
        public int tileX { get; set; }
        public int tileY { get; set; }
        public int value { get; set; }

        int IComparable.CompareTo(object obj)
        {
            AIMoveEval e = (AIMoveEval)obj;
            if (this.value > e.value)
            {
                return 1;
            }
            if (this.value < e.value)
            {
                return -1;
            }

            return 0;
        }
    }

    public static class ComputerAI
    {
        public enum Action
        {
            DRAW,
            PLAY,
            DISCARD,
            END_TURN
        };

        public static readonly int HELD_TILE_PRIMARY = 0;
        public static readonly int HELD_TILE_RESERVE = 1;

        private static readonly float COMPLETE_TRACK_WEIGHT = 1.0f;
        private static readonly float INCOMPLETE_TRACK_WEIGHT = 3.0f;
        private static readonly float COMPLETE_CENTRAL_STATION_WEIGHT = 2.0f;

        private static List<AIMoveEval> evalList;

        /****************************************************************************
        * determineNextMove()
        *
        * Description:
        *   Generates a single move for the current computer player, based on the
        *   state of the board and held tiles.  Generates a move packet that
        *   describes the action to be taken.
        *
        ****************************************************************************/
        public static ComputerAIPacket determineNextMove(Game g, Nullable<ComputerAIPacket> lastMove)
        {
            ComputerAIPacket p;
            int tileQuantity;
            AIMoveEval selectedMove;
            Board boardCopy;
            int holdingPrimary, holdingSecondary, legalMoves, legalMoves2;
            int tilesInPool;

            /* How to decide on a move (V1):
             *
             *  The following steps are used to help decide on a move.
             *   (a temporary board should be created to test move strength)
             *
             * x 1.  If the computer has no tiles, draw one.
             * x 2.  If the computer has one tile, but it has no legal moves,
             *        draw one.
             *  3.  If the computer has two tiles, but neither has legal moves,
             *      discard one (pick one at random)
             *  4.  If the computer is to draw a tile, but no tiles remain,
             *      and any held tile cannot be played, then pass, otherwise
             *      play.
             *  5.  If the computer has made more than 2 moves in any particular
             *      round, then pass (this is to prevent an infinite move loop)
             *  6.  When the computer is to play, do the following for each held tile:
             *       - Place the tile in each legal location
             *       - For each such location:
             *          -  the value of the location starts at 0
             *          -  calculate the value of all incomplete tracks of the current
             *             player for every station that still has a train on it,
             *             *if the played tile makes up part of that track*, and add them
             *             together.  If no stations pass through it, the score will be
             *             zero at this point.  
             *          - Repeat the calculation for each of the incomplete tracks of
             *            other players that pass through that tile.  For each one that
             *            does, subtract the value of those incomplete tracks.
             *       - When finished, sort the values from high to low.  Depending on the
             *         AI level, pick a move that's either highly ranked, ranked in the
             *         middle, or lowest ranked.  If more than 5 moves exist, don't pick
             *         the absolute best or worst; pick the second best or second worst.
             */

            /*
             * How to store moves in order to allow them to be ranked:
             *
             * A structure with the following elements:
             *
             *   int tileType (primary or secondary)
             *   int tileX    (the x position of the tile)
             *   int tileY    (the y position of the tile)
             *   int value    (the value of the this move)
             *
             * Create a list of this structure and add an element for each
             * move evaluated.
             */
            boardCopy = g.board.Copy();

            /* Create an empty evalList and AI packet*/
            evalList = new List<AIMoveEval>();
            p = new ComputerAIPacket();

            /* Determine how many tiles the AI player is holding, and how many tiles are left */
            tileQuantity = 0;
            holdingPrimary = TrackInsanity.FALSE;
            holdingSecondary = TrackInsanity.FALSE;
            if (g.players[g.curPlayer].currentTileId != Tile.NONE)
            {
                tileQuantity++;
                holdingPrimary = TrackInsanity.TRUE;
            }
            if (g.players[g.curPlayer].reserveTileId != Tile.NONE)
            {
                tileQuantity++;
                holdingSecondary = TrackInsanity.TRUE;
            }
            tilesInPool = g.tilepool.numUnplayedTiles;

            /* The computer hasn't done anything yet */
            if (lastMove == null)
            {
                /* No tiles in hand? */
                if (tileQuantity == 0)
                {
                    /* If there are still tiles in the pool, then draw one */
                    if (tilesInPool > 0)
                    {
                        p.moveType = (int)(Action.DRAW);
                        System.Diagnostics.Debug.WriteLine("  - Computer's move is DRAW");
                    }
                    else
                    {
                        p.moveType = (int)(Action.END_TURN);
                        System.Diagnostics.Debug.WriteLine("  - Computer's move is END_TURN (pass)");
                    }
                }
                /* One tile in hand? */
                else if (tileQuantity == 1)
                {
                    if (holdingPrimary == TrackInsanity.TRUE)
                    {
                        legalMoves = boardCopy.markLegalMoves(boardCopy.tp.getTile(g.players[g.curPlayer].currentTileId));
                    }
                    else
                    {
                        legalMoves = boardCopy.markLegalMoves(boardCopy.tp.getTile(g.players[g.curPlayer].reserveTileId));
                    }
                    /* No legal moves with this tile */
                    if (legalMoves == 0)
                    {
                        /* If tiles remain, draw another */
                        if (tilesInPool > 0)
                        {
                            p.moveType = (int)(Action.DRAW);
                            System.Diagnostics.Debug.WriteLine("  - Computer's move is DRAW");
                        }
                        /* If no tiles remain, pass */
                        else
                        {
                            p.moveType = (int)(Action.END_TURN);
                            System.Diagnostics.Debug.WriteLine("  - Computer's move is END_TURN (pass)");
                        }
                    }
                    /* This tile has legal moves, -- analyze them and pick one */
                    else
                    {
                        /* Analyze all board positions with this tile and pick a spot to play */
                        if (holdingPrimary == TrackInsanity.TRUE)
                        {
                            analyzeMoves(g, boardCopy, g.players[g.curPlayer].currentTileId, HELD_TILE_PRIMARY);
                        }
                        else
                        {
                            analyzeMoves(g, boardCopy, g.players[g.curPlayer].reserveTileId, HELD_TILE_RESERVE);
                        }
                        evalList.Sort();
                        selectedMove = selectListMove(g);
                        p.moveType = (int)(Action.PLAY);
                        p.moveX = selectedMove.tileX;
                        p.moveY = selectedMove.tileY;
                        p.heldTile = selectedMove.tileType;
                        System.Diagnostics.Debug.WriteLine("  - Computer's move is PLAY ({0} at ({1}, {2}))", (p.heldTile == HELD_TILE_PRIMARY) ? "P" : "S", p.moveX, p.moveY);
                    }
                }
                /* If two tiles, play or discard depending on whether there are legal moves */
                else
                {
                    legalMoves = boardCopy.markLegalMoves(boardCopy.tp.getTile(g.players[g.curPlayer].currentTileId));

                    /* Analyze all legal moves for the primary tile */
                    if (legalMoves > 0)
                    {
                        analyzeMoves(g, boardCopy, g.players[g.curPlayer].currentTileId, HELD_TILE_PRIMARY);
                    }

                    legalMoves2 = boardCopy.markLegalMoves(boardCopy.tp.getTile(g.players[g.curPlayer].reserveTileId));
                    /* Analyze all legal moves for the secondary tile */
                    if (legalMoves2 > 0)
                    {
                        analyzeMoves(g, boardCopy, g.players[g.curPlayer].reserveTileId, HELD_TILE_RESERVE);
                    }

                    /* If no legal moves are available, discard the secondary tile */
                    if (legalMoves == 0 && legalMoves2 == 0)
                    {
                        p.moveType = (int)(Action.DISCARD);
                        p.heldTile = HELD_TILE_RESERVE;
                        System.Diagnostics.Debug.WriteLine("  - Computer's move is DISCARD (secondary)");
                    }
                    /* Legal moves are available, pick one */
                    else
                    {
                        evalList.Sort();
                        selectedMove = selectListMove(g);
                        p.moveType = (int)(Action.PLAY);
                        p.moveX = selectedMove.tileX;
                        p.moveY = selectedMove.tileY;
                        p.heldTile = selectedMove.tileType;
                        System.Diagnostics.Debug.WriteLine("  - Computer's move is PLAY ({0} at ({1}, {2}))", (p.heldTile == HELD_TILE_PRIMARY) ? "P" : "S", p.moveX, p.moveY);
                    }
                }
            }
            /* The computer has already made a move this turn */
            else
            {
                /* If the last move was play, pass or discard, the turn is over */
                if (lastMove.Value.moveType == (int)Action.PLAY ||
                   lastMove.Value.moveType == (int)Action.DISCARD ||
                   lastMove.Value.moveType == (int)Action.END_TURN)
                {
                    p.moveType = (int)Action.END_TURN;
                    System.Diagnostics.Debug.WriteLine("  - Computer's move is 'END_TURN'");
                    /* Delay */
                }
                else
                {
                    /* If no tiles, pass (this should never happen) */
                    if (tileQuantity == 0)
                    {
                        p.moveType = (int)Action.END_TURN;
                        System.Diagnostics.Debug.WriteLine("  - Computer's move is 'END_TURN'");
                        /* Delay */
                    }
                    else if (tileQuantity == 1)
                    {
                        if (holdingPrimary == TrackInsanity.TRUE)
                        {
                            legalMoves = boardCopy.markLegalMoves(g.tilepool.getTile(g.players[g.curPlayer].currentTileId));
                        }
                        else
                        {
                            legalMoves = boardCopy.markLegalMoves(g.tilepool.getTile(g.players[g.curPlayer].reserveTileId));
                        }
                        /* If the tile has no legal moves, pass */
                        if (legalMoves == 0)
                        {
                            p.moveType = (int)Action.END_TURN;
                            System.Diagnostics.Debug.WriteLine("  - Computer's move is 'END_TURN'");
                            /* Delay */
                        }
                        else
                        {
                            if (holdingPrimary == TrackInsanity.TRUE)
                            {
                                analyzeMoves(g, boardCopy, g.players[g.curPlayer].currentTileId, HELD_TILE_PRIMARY);
                            }
                            else
                            {
                                analyzeMoves(g, boardCopy, g.players[g.curPlayer].reserveTileId, HELD_TILE_RESERVE);
                            }
                            evalListSort();
                            selectedMove = selectListMove(g);
                            p.moveType = (int)(Action.PLAY);
                            p.moveX = selectedMove.tileX;
                            p.moveY = selectedMove.tileY;
                            p.heldTile = selectedMove.tileType;
                            System.Diagnostics.Debug.WriteLine("  - Computer's move is 'PLAY' ({0} at ({1}, {2}))\n", (p.heldTile == HELD_TILE_PRIMARY) ? 'P' : 'S', p.moveX, p.moveY);
                        }
                    }
                    else
                    {
                        legalMoves = boardCopy.markLegalMoves(g.tilepool.getTile(g.players[g.curPlayer].currentTileId));

                        /* analyze all legal moves for the primary tile */
                        if (legalMoves > 0)
                        {
                            analyzeMoves(g, boardCopy, g.players[g.curPlayer].currentTileId, HELD_TILE_PRIMARY);
                        }
                        legalMoves2 = boardCopy.markLegalMoves(g.tilepool.getTile(g.players[g.curPlayer].reserveTileId));
                        /* Analyze all legal moves for the secondary tile */
                        if (legalMoves2 > 0)
                        {
                            analyzeMoves(g, boardCopy, g.players[g.curPlayer].reserveTileId, HELD_TILE_RESERVE);
                        }
                        /* If no legal moves are available, discard the secondary tile */
                        if (legalMoves == 0 && legalMoves2 == 0)
                        {
                            p.moveType = (int)Action.DISCARD;
                            p.heldTile = HELD_TILE_RESERVE;
                            System.Diagnostics.Debug.WriteLine("  - Computer's move is DISCARD (secondary)");
                        }
                        /* Legal moves are available, pick one */
                        else
                        {
                            evalListSort();
                            selectedMove = selectListMove(g);
                            p.moveType = (int)Action.PLAY;
                            p.moveX = selectedMove.tileX;
                            p.moveY = selectedMove.tileY;
                            p.heldTile = selectedMove.tileType;
                            System.Diagnostics.Debug.WriteLine("  - Computer's move is 'PLAY' ({0} at ({1}, {2}))\n", (p.heldTile == HELD_TILE_PRIMARY) ? 'P' : 'S', p.moveX, p.moveY);
                        }
                    }
                }
            }

            return p;
        }

        /****************************************************************************
        * analyzeMoves()
        *
        * Description:
        *   Generates a list of potential moves using the tile in the computer
        *   player's hand, each with a location and score.
        *
        ****************************************************************************/
        public static int analyzeMoves(Game g, Board b, int tileIndex, int heldTile)
        {
            int counter, counter2, counter3;
            int value;
            int stationX = -1, stationY = -1, exit = -1;
            int score;
            Nullable<int> passThru = -1;
            int destination = -1;
            float weight;

            for (counter = 1; counter < Board.WIDTH - 1; counter++)
            {
                for (counter2 = 1; counter2 < Board.HEIGHT; counter2++)
                {
                    if (b.isLegalMove(counter, counter2) == TrackInsanity.TRUE)
                    {
                        value = 0;
                        /* Play the move */
                        b.placeTile(counter, counter2, tileIndex);
                        /* For each station, if a train is transitioned here, check for
                         * partial/complete tracks, and either add or subtract the score
                         * from the value, depending on who owns the station */
                        for (counter3 = 0; counter3 < Board.NUM_STATIONS; counter3++)
                        {
                            b.getStationInfo(counter3, ref stationX, ref stationY, ref exit);
                            if (b.b[stationX, stationY].trainPresent != (int)(Board.playerTrains.NO_TRAIN))
                            {
                                weight = 1.0f;
                                score = b.calculateTrackScore(counter3, tileIndex, ref passThru, ref destination);
                                if (passThru.Value == TrackInsanity.TRUE)
                                {
                                    if (destination == BoardSquare.TILE)
                                    {
                                        weight = INCOMPLETE_TRACK_WEIGHT;
                                    }
                                    else if (destination == BoardSquare.STATION)
                                    {
                                        weight = COMPLETE_TRACK_WEIGHT;
                                    }
                                    else if (destination == BoardSquare.CENTRAL_STATION)
                                    {
                                        weight = COMPLETE_CENTRAL_STATION_WEIGHT;
                                    }

                                    if (b.playerStations[g.numPlayers, counter3] == (g.curPlayer + 1))
                                    {                                        
                                        value += (int)(score * weight);
                                    }
                                    else
                                    {
                                        value -= (int)(score * weight);
                                    }
                                    System.Diagnostics.Debug.WriteLine("Value is now {0}", value);
                                }
                            }
                        }

                        b.removeTile(counter, counter2);
                        evalListAdd(heldTile, counter, counter2, value);
                    }
                }
            }

            return TrackInsanity.OK;
        }

        /****************************************************************************
        * selectListMove()
        *
        * Description:
        *   Picks a move from the list of potential moves based on the strength
        *   of the AI and the relative strength of each move. 
        *
        ****************************************************************************/
        public static AIMoveEval selectListMove(Game g)
        {
            int numMoves = evalList.Count();
            int aiLevel = g.players[g.curPlayer].computerAiLevel;
            int moveIndex;

            System.Diagnostics.Debug.WriteLine("Evaluating AI -- number of moves for player {0} is {1}", g.curPlayer, numMoves);
            for (int counter = 0; counter < numMoves; counter++)
            {
                System.Diagnostics.Debug.WriteLine("  - Move {0}: Value = {1}", counter, evalList[counter].value);
            }
            /* If there are less than 3 moves:
                - Best AI = best move
                - Medium AI = best move
                - Weak AI = worst move

               If there are more than 3 moves:
                - Best AI = best move
                - Medium AI = a move better than approx 2/3 of moves.  If this move
                  has a negative or zero score, pick the weakest move with a positive score (if one exists)
                - Weak AI = a move worse than approx 2/3 of moves, regardless of the
                  move's value
             */
            if (numMoves < 3)
            {
                if (aiLevel == Player.AI_SMARTEST || aiLevel == Player.AI_SMARTER)
                {

                    moveIndex = 0;
                }
                else
                {
                    moveIndex = numMoves - 1;
                }
            }
            else
            {
                if (aiLevel == Player.AI_SMARTEST)
                {
                    moveIndex = 0;
                }
                else if (aiLevel == Player.AI_SMARTER)
                {
                    moveIndex = (int)((float)(numMoves - 1) * 0.33);
                    if (evalList[moveIndex].value <= 0)
                    {
                        while (moveIndex > 0 && evalList[moveIndex].value <= 0)
                        {
                            moveIndex--;
                        }
                    }
                }
                else
                {
                    moveIndex = (int)((float)(numMoves - 1) * 0.67);
                }
            }

            System.Diagnostics.Debug.WriteLine("  Picking move {0} from a total list of {1}", moveIndex, numMoves);
            return evalList[moveIndex];
        }

        /****************************************************************************
        * evalListAdd()
        *
        * Description:
        *   Adds a evaluated move to the move list.
        *
        ****************************************************************************/
        public static void evalListAdd(int type, int x, int y, int val)
        {
            AIMoveEval a = new AIMoveEval();
            a.tileType = type;
            a.tileX = x;
            a.tileY = y;
            a.value = val;
            evalList.Add(a);
        }

        /****************************************************************************
        * evalListSort()
        *
        * Description:
        *   Sorts the move list in order from strongest to weakest move.  
        *
        ****************************************************************************/
        public static void evalListSort()
        {
            evalList.Sort();
            evalList.Reverse();
        }
    }

                    
}
