/****************************************************************************
*
* tiPlayer.c - functions to handle player data structures
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
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <SDL/SDL.h>
#include "tiMain.h"
#include "tiTiles.h"
#include "tiBoard.h"
#include "tiRenderSDL.h"
#include "tiCoords.h"
#include "tiPlayer.h"
#include "tiGame.h"
#include "tiComputerAI.h"

/****************************************************************************
* playerInitPlayer - see tiPlayer.h for description
****************************************************************************/
void playerInitPlayer(Player *p, int control, int score, int computerAI)
{
    /* The name isn't used for now */
    p->name[0] = '\0';

    /* Set the type of player this particular player instance represents */
    if(control == TI_PLAYER_NOBODY || control == TI_PLAYER_HUMAN ||
    control == TI_PLAYER_COMPUTER)
    {
        p->controlledBy = control;
    }
    else
    {
        p->controlledBy = TI_PLAYER_NOBODY;
    }

    /* Initialize the score (it generally should be set to 0 */
    if(score >= 0)
    {
        p->score = score;
    }
    else
    {
        p->score = 0;
    }

    /* Set the current and reserve tiles to empty */
    p->currentTileId = TI_TILE_NO_TILE;
    p->reserveTileId = TI_TILE_NO_TILE;

    /* No stations are complete for any player at first */
    p->numStationsComplete = 0;

    /* Set either the provided or a default AI level, depending what's passed in */
    if(computerAI == TI_PLAYER_AI_DEFAULT || computerAI == TI_PLAYER_AI_SMARTER ||
    computerAI == TI_PLAYER_AI_SMARTEST)
    {
        p->computerAiLevel = computerAI;
    }
    else
    {
        p->computerAiLevel = TI_PLAYER_AI_DEFAULT;
    }

    p->lastMoveX = TI_PLAYER_INVALID_LAST_MOVE;
    p->lastMoveY = TI_PLAYER_INVALID_LAST_MOVE;
}
