/****************************************************************************
*
* TilePool.cs - information about the group of tiles that are used by 
*               the game, and their contents and played/unplayed status.
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
    public class TilePool
    {
        private const int NUM_TILES = 60;
        private static readonly int[,] TileData = new int[,] { 
                                                        { 1, 0, 3, 2, 5, 4, 7, 6, 1},
                                                        { 1, 0, 3, 2, 5, 4, 7, 6, 1},
                                                        { 5, 4, 7, 6, 1, 0, 3, 2, 2},
                                                        { 5, 4, 7, 6, 1, 0, 3, 2, 2},
                                                        { 5, 4, 7, 6, 1, 0, 3, 2, 2},
                                                        { 5, 4, 7, 6, 1, 0, 3, 2, 2},
                                                        { 1, 0, 7, 6, 5, 4, 3, 2, 3},
                                                        { 1, 0, 7, 6, 5, 4, 3, 2, 3},
                                                        { 7, 2, 1, 4, 3, 6, 5, 0, 4},
                                                        { 7, 2, 1, 4, 3, 6, 5, 0, 4},
                                                        { 7, 2, 1, 6, 5, 4, 3, 0, 5},
                                                        { 7, 2, 1, 6, 5, 4, 3, 0, 5},
                                                        { 7, 4, 3, 2, 1, 6, 5, 0, 6},
                                                        { 7, 4, 3, 2, 1, 6, 5, 0, 6},
                                                        { 5, 4, 3, 2, 1, 0, 7, 6, 7},
                                                        { 5, 4, 3, 2, 1, 0, 7, 6, 7},
                                                        { 5, 2, 1, 4, 3, 0, 7, 6, 8},
                                                        { 5, 2, 1, 4, 3, 0, 7, 6, 8},
                                                        { 1, 0, 7, 4, 3, 6, 5, 2, 9},
                                                        { 1, 0, 7, 4, 3, 6, 5, 2, 9},
                                                        { 7, 6, 5, 4, 3, 2, 1, 0, 10},
                                                        { 7, 6, 5, 4, 3, 2, 1, 0, 10},
                                                        { 7, 6, 5, 4, 3, 2, 1, 0, 10},
                                                        { 3, 2, 1, 0, 5, 4, 7, 6, 11},
                                                        { 3, 2, 1, 0, 5, 4, 7, 6, 11},
                                                        { 1, 0, 5, 4, 3, 2, 7, 6, 12},
                                                        { 1, 0, 5, 4, 3, 2, 7, 6, 12},
                                                        { 7, 6, 3, 2, 5, 4, 1, 0, 13},
                                                        { 7, 6, 3, 2, 5, 4, 1, 0, 13},
                                                        { 1, 0, 3, 2, 7, 6, 5, 4, 14},
                                                        { 1, 0, 3, 2, 7, 6, 5, 4, 14},
                                                        { 3, 6, 5, 0, 7, 2, 1, 4, 15},
                                                        { 3, 6, 5, 0, 7, 2, 1, 4, 15},
                                                        { 3, 6, 7, 0, 5, 4, 1, 2, 16},
                                                        { 3, 6, 7, 0, 5, 4, 1, 2, 16},
                                                        { 1, 0, 5, 6, 7, 2, 3, 4, 17},
                                                        { 1, 0, 5, 6, 7, 2, 3, 4, 17},
                                                        { 5, 2, 1, 6, 7, 0, 3, 4, 18},
                                                        { 5, 2, 1, 6, 7, 0, 3, 4, 18},
                                                        { 5, 2, 1, 6, 7, 0, 3, 4, 18},
                                                        { 5, 2, 1, 6, 7, 0, 3, 4, 18},
                                                        { 3, 4, 7, 0, 1, 6, 5, 2, 19},
                                                        { 3, 4, 7, 0, 1, 6, 5, 2, 19},
                                                        { 3, 4, 7, 0, 1, 6, 5, 2, 19},
                                                        { 3, 4, 7, 0, 1, 6, 5, 2, 19},
                                                        { 7, 4, 5, 6, 1, 2, 3, 0, 20},
                                                        { 7, 4, 5, 6, 1, 2, 3, 0, 20},
                                                        { 7, 4, 5, 6, 1, 2, 3, 0, 20},
                                                        { 7, 4, 5, 6, 1, 2, 3, 0, 20},
                                                        { 3, 4, 5, 0, 1, 2, 7, 6, 21},
                                                        { 3, 4, 5, 0, 1, 2, 7, 6, 21},
                                                        { 5, 6, 7, 4, 3, 0, 1, 2, 22},
                                                        { 5, 6, 7, 4, 3, 0, 1, 2, 22},
                                                        { 5, 6, 7, 4, 3, 0, 1, 2, 22},
                                                        { 5, 6, 7, 4, 3, 0, 1, 2, 22},
                                                        { 3, 2, 1, 0, 7, 6, 5, 4, 23},
                                                        { 3, 2, 1, 0, 7, 6, 5, 4, 23},
                                                        { 3, 2, 1, 0, 7, 6, 5, 4, 23},
                                                        { 5, 6, 3, 2, 7, 0, 1, 4, 24},
                                                        { 5, 6, 3, 2, 7, 0, 1, 4, 24}
                                                     };

        /* The list of individual tiles in the tile pool */
        private Tile[] t;

        /* The play status of each tile (played (on the board or in hand), or unplayed (still in the pool)) */
        private const int PLAYED = 0;
        private const int UNPLAYED = 1;

        /* The list of tile IDs that represent already played tiles */
        private int[] playedTiles;

        /* The list of tile IDs that represent unplayed tiles */
        private int[] unplayedTiles;

        /* The status (played or unplayed) of each tile in the pool */
        private int[] tileStatus;

        /* The specific number of played and unplayed tiles remaining */
        public int numPlayedTiles { get; set; }
        public int numUnplayedTiles { get; set; }

        /****************************************************************************
        * TilePool
        *
        * Description:
        *   Constructor.
        *
        ****************************************************************************/
        public TilePool()
        {
            t = new Tile[NUM_TILES];
            playedTiles = new int[NUM_TILES];
            unplayedTiles = new int[NUM_TILES];
            tileStatus = new int[NUM_TILES];

            /* Get the tile exits and tile strip offsets from the TileData structure */
            for (int tileIndex = 0; tileIndex < NUM_TILES; tileIndex++)
            {
                t[tileIndex] = new Tile();
                for (int direction = 0; direction < Tile.NUM_EXITS; direction++)
                {                  
                    t[tileIndex].setExit(direction, TileData[tileIndex, direction]);
                }
                tileStatus[tileIndex] = UNPLAYED;
                t[tileIndex].tileStripOffset = TileData[tileIndex, 8];

            }

            calculatePlayedTiles();
            calculateUnplayedTiles();
        }

        /****************************************************************************
        * Copy
        *
        * Description:
        *   Makes a deep copy of a TilePool structure. 
        *
        ****************************************************************************/
        public TilePool Copy()
        {
            int counter;
            TilePool p = new TilePool();

            for (counter = 0; counter < NUM_TILES; counter++)
            {
                p.t[counter] = t[counter].Copy();
                p.playedTiles[counter] = playedTiles[counter];
                p.unplayedTiles[counter] = unplayedTiles[counter];
                p.tileStatus[counter] = tileStatus[counter];
            }

            p.numPlayedTiles = numPlayedTiles;
            p.numUnplayedTiles = numUnplayedTiles;

            return p;
        }

        /****************************************************************************
        * getTile
        *
        * Description:
        *   An accessor for an individual tile in the pool. 
        *
        ****************************************************************************/
        public Tile getTile(int index)
        {
            if (index < 0 || index >= NUM_TILES)
            {
                return null;
            }
            return t[index];
        }

        /****************************************************************************
        * getTileIndexForTileId
        *
        * Description:
        *   Determine the offset into the tile strip for the specified tile index 
        *
        ****************************************************************************/
        public int getTileIndexForTileId(int id)
        {
            if (id < 0 || id >= NUM_TILES)
            {
                return Tile.NONE;
            }
            return t[id].tileStripOffset;
        }

        /****************************************************************************
        * drawRandomTile
        *
        * Description:
        *   Returns the index of an unplayed tile in the pool. 
        *
        ****************************************************************************/
        public int drawRandomTile()
        {
            int offset, index;

            if (numUnplayedTiles == 0)
            {
                return Tile.INVALID;
            }

            offset = RNG.GetRandomInt(0, numUnplayedTiles - 1);
            index = unplayedTiles[offset];

            /* This shouldn't happen! */
            if (tileStatus[index] == PLAYED)
            {
                System.Diagnostics.Debug.WriteLine("How did we get here!?  offset = {0}, index = {1}, unplayedTiles[offset] {2}, tileStatus = {3}",
                                                   offset, index, unplayedTiles[offset], tileStatus[index]);
                return Tile.ERROR;
            }

            /* Now that a new tile is in play, update the state of the tile pool. */
            tileStatus[index] = PLAYED;
            calculatePlayedTiles();
            calculateUnplayedTiles();

            return index;
        }

        /****************************************************************************
        * returnTileToPool
        *
        * Description:
        *   Adds a previously removed tile back into the pool.
        *
        ****************************************************************************/
        public int returnTileToPool(int index)
        {
            if (numPlayedTiles == 0)
            {
                return TrackInsanity.ERROR;
            }

            if (tileStatus[index] == UNPLAYED)
            {
                return TrackInsanity.ERROR;
            }

            /* Now that a new tile is in play, update the state of the tile pool. */
            tileStatus[index] = UNPLAYED;
            calculatePlayedTiles();
            calculateUnplayedTiles();

            return TrackInsanity.OK;
        }


        /****************************************************************************
        * calculatePlayedTiles
        *
        * Description:
        *   Calculate the number of tiles that have been removed from the tile pool.
        *
        ****************************************************************************/
        public void calculatePlayedTiles()
        {          
            numPlayedTiles = 0;
            for (int counter = 0; counter < NUM_TILES; counter++)
            {
                if (tileStatus[counter] == PLAYED)
                {
                    playedTiles[numPlayedTiles] = counter;
                    numPlayedTiles++;
                }
            }
        }

        /****************************************************************************
        * calculateUnplayedTiles
        *
        * Description:
        *   Calculate the number of tiles remaining in the tile pool.
        *
        ****************************************************************************/
        public void calculateUnplayedTiles()
        {
            numUnplayedTiles = 0;
            for (int counter = 0; counter < NUM_TILES; counter++)
            {
                if (tileStatus[counter] == UNPLAYED)
                {
                    unplayedTiles[numUnplayedTiles] = counter;
                    numUnplayedTiles++;
                }
            }
        }
    }
}
