/****************************************************************************
*
* tiMain.c - entry point for TrackInsanity
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
#include <time.h>
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

TiSharedData *GameData;
TiScreen     *GameDisplay;
TiAssets     *GameAssetPool;
Game        *GameInstance;

int main(int argc, char **argv)
{
    int updateIterations;

    srand(time(NULL));

    GameDisplay = renderInitialize(TI_GAME_XRES, TI_GAME_YRES, TI_GAME_DEPTH);
    if(GameDisplay == NULL)
    {
        perror("Unable to initialize render structure");
        exit(1);
    }

    GameData = renderSharedDataInitialize();
    if(GameData == NULL)
    {
        perror("Unable to initialize shared data structure");
        renderDestroy(&GameDisplay);
        exit(1);
    }

    GameAssetPool = renderAssetsInitialize();
    if(GameAssetPool == NULL)
    {
        perror("Unable to initialize asset pool");
        renderSharedDataDestroy(&GameData);
        renderDestroy(&GameDisplay);
        exit(1);
    }
    if(renderLoadAssets(GameDisplay, GameAssetPool, GameData) == TI_ERROR)
    {
        perror("Unable to load all images");
        renderFreeAssets(GameAssetPool, GameData);
        renderSharedDataDestroy(&GameData);
        renderDestroy(&GameDisplay);
        exit(1);
    }

    GameInstance = gameInitialize(TI_TILE_DATA_FILE, TI_STATION_DATA_FILE);
    if(GameInstance == NULL)
    {
        perror("Unable to initialize Game structure");
        renderFreeAssets(GameAssetPool, GameData);
        renderSharedDataDestroy(&GameData);
        renderDestroy(&GameDisplay);
        exit(1);
    }

    /* Start up the state machine */
    renderSetRenderState(TI_STATE_COMPANY_LOGO, TI_STATE_NO_STATE, GameDisplay, GameAssetPool, GameData);

    while(GameData->exitGame == TI_FALSE)
    {
        GameData->curTicks = SDL_GetTicks();
        updateIterations = ((GameData->curTicks - GameData->lastFrameTicks) +
                            GameData->cyclesLeftOver);
        if(updateIterations > (TI_MAX_CYCLES_PER_FRAME * TI_RENDER_FRAME_DURATION))
        {
            updateIterations = (TI_MAX_CYCLES_PER_FRAME * TI_RENDER_FRAME_DURATION);
        }

        while(updateIterations > TI_RENDER_FRAME_DURATION)
        {
            updateIterations -= TI_RENDER_FRAME_DURATION;
            GameData->numFrames++;
            renderUpdateLogic(GameDisplay, GameData, GameAssetPool);
            renderProcessEvents(GameDisplay, GameAssetPool, GameData);
            renderCheckTimingConditions(GameDisplay, GameAssetPool, GameData);
        }

        GameData->cyclesLeftOver = updateIterations;
        GameData->lastFrameTicks = GameData->curTicks;

        renderUpdateScreen(GameDisplay, GameAssetPool, GameData);
        SDL_Delay(1);
    }

    /* Free resources here */
    gameDestroy(&GameInstance);
    /* This call will free any remaining assets that are still loaded */
    renderFreeAssets(GameAssetPool, GameData);
    renderDestroy(&GameDisplay);
    renderSharedDataDestroy(&GameData);
}
