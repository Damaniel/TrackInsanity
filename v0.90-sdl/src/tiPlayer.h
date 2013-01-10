/****************************************************************************
*
* tiPlayer.h - Header for tiPlayer.c
*
* Copyright 2007 Shaun Brandt / Holy Meatgoat Software
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
#ifndef __TIPLAYER_H__
#define __TIPLAYER_H__

#define TI_PLAYER_NOBODY        0
#define TI_PLAYER_HUMAN         1
#define TI_PLAYER_COMPUTER      2

#define TI_PLAYER_AI_DEFAULT    0
#define TI_PLAYER_AI_SMARTER    1
#define TI_PLAYER_AI_SMARTEST   2

#define TI_PLAYER_INVALID_LAST_MOVE     -1

typedef struct
{
    char name[16];
    int  controlledBy;
    int  score;
    int  currentTileId;
    int  reserveTileId;
    int  computerAiLevel;
    int  numStationsComplete;
    int  lastMoveX;
    int  lastMoveY;
} Player;

/****************************************************************************
* playerInitPlayer
*
* Description:
*   Initializes a player.
*
* Arguments:
*   Player *p - the player to initialize
*   int control - human or player control
*   int score - the initial score
*   int computerAI - the computer AI strength
*
* Returns:
*   Nothing.
*
****************************************************************************/
void playerInitPlayer(Player *p, int control, int score, int computerAI);

#endif /* __TIPLAYER_H__ */
