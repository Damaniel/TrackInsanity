/****************************************************************************
*
* SharedData.cs - A global set of parameters used by multiple sources.
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
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace TrackInsanity.Source
{
    public static class SharedData
    {
        /* Current part of the game we're drawing/executing */
        public static int renderState;

        /* Actual screen resolution */
        public static int xRes, yRes;

        /* Render state timer and main timer */
        public static int curTicks;
        public static int stateStartTicks;
        public static int cpuDelayTicks;

        public static bool cpuMoveAllowed;

        /* Should the game exit? */
        public static int exitGame;

        /* The number of players selected for the current game */
        public static int selectedPlayers;

        /* The type of player (human, computer) for each of the active players */
        public static int[] playerState;

        /* A frame counter (total frames executed since the start of the game) */
        public static int numFrames;

        /* Used for computer AI */
        public static ComputerAIPacket currentMove;
        public static ComputerAIPacket previousMove;

        public static GraphicsDevice graphics;

        public static int reset()
        {
            int counter;

            SharedData.selectedPlayers = TrackInsanity.MIN_PLAYERS;
            SharedData.playerState = new int[TrackInsanity.MAX_PLAYERS];

            SharedData.playerState[0] = Player.HUMAN;
            for (counter = 1; counter < TrackInsanity.MAX_PLAYERS; counter++)
            {
                SharedData.playerState[counter] = Player.COMPUTER;
            }
 
            SharedData.exitGame = TrackInsanity.FALSE;

            return TrackInsanity.OK;
        }
    }
}
