/****************************************************************************
*
* Player.cs - Information about human and computer players.
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
    public class Player
    {
        /* Constants */
        public const int NOBODY = 0;
        public const int HUMAN = 1;
        public const int COMPUTER = 2;

        public const int AI_DEFAULT = 0;
        public const int AI_SMARTER = 1;
        public const int AI_SMARTEST = 2;

        public const int INVALID_LAST_MOVE = -1;

        public int controlledBy { get; set; }
        public int score { get; set; }
        public int currentTileId { get; set; }
        public int reserveTileId { get; set; }
        public int computerAiLevel { get; set; }
        public int numStationsComplete { get; set; }
        public int lastMoveX { get; set; }
        public int lastMoveY { get; set; }

        /****************************************************************************
        * Player
        *
        * Description:
        *   Constructor
        *
        ****************************************************************************/
        public Player(int control, int initialScore, int computerAI)
        {
            /* Set the type of player this particular player instance represents */
            if (control == NOBODY || control == HUMAN || control == COMPUTER)
            {
                controlledBy = control;
            }
            else
            {
                controlledBy = NOBODY;
            }

            /* Initialize the score */
            if (initialScore >= 0)
            {
                score = initialScore;
            }
            else
            {
                score = 0;
            }

            /* Set the current and reserve tiles to empty */
            currentTileId = Tile.NONE;
            reserveTileId = Tile.NONE;

            /* No stations are complete for any player at first */
            numStationsComplete = 0;

            /* Set either the provided or a default AI level, depending on what's passed in */
            if (computerAI == AI_DEFAULT || computerAI == AI_SMARTER || computerAI == AI_SMARTEST)
            {
                computerAiLevel = computerAI;
            }
            else
            {
                computerAiLevel = AI_DEFAULT;
            }

            /* Mark the last move data as invalid */
            lastMoveX = INVALID_LAST_MOVE;
            lastMoveY = INVALID_LAST_MOVE;
        }
    }
}
