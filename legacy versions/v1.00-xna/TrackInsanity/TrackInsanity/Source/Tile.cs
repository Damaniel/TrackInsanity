/****************************************************************************
*
* Tile.cs -- information about tiles (contents, entrances/exits, etc).
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
    public class Tile
    {
        /* The number of entrances/exits from a single tile */
        public const int NUM_EXITS = 8;

        /* Error values */
        public const int INVALID = -1;
        public const int ERROR = -2;

        /* Used to represent a 'null' tile */
        public const int NONE = -1;

        private int[] exits;
        public int tileStripOffset;

        /****************************************************************************
        * Tile
        *
        * Description:
        *   Constructor.
        *
        ****************************************************************************/
        public Tile()
        {
            exits = new int[NUM_EXITS];
        }

        /****************************************************************************
        * Copy
        *
        * Description:
        *   Creates a deep copy of a Tile
        *
        ****************************************************************************/
        public Tile Copy()
        {
            int counter;
            Tile t = new Tile();

            for (counter = 0; counter < NUM_EXITS; counter++)
            {
                t.exits[counter] = exits[counter];
            }
            t.tileStripOffset = tileStripOffset;

            return t;
        }

        /****************************************************************************
        * findExit
        *
        * Description:
        *   Determine the exit point of a tile, given the entry point. 
        *
        ****************************************************************************/
        public int findExit(int entry)
        {
            if (entry < 0 || entry >= NUM_EXITS)
            {
                return ERROR;
            }

            return exits[entry];
        }

        /****************************************************************************
        * setExit
        *
        * Description:
        *   Accessor function for the exits array.  
        *
        ****************************************************************************/
        public void setExit(int index, int value)
        {
            exits[index] = value;
        }
    }
}
