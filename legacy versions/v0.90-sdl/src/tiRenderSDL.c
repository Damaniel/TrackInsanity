/****************************************************************************
*
* tiRenderSDL.c - SDL graphics and input operations
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
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "tiMain.h"
#include "tiTiles.h"
#include "tiBoard.h"
#include "tiRenderSDL.h"
#include "tiCoords.h"
#include "tiPlayer.h"
#include "tiGame.h"
#include "tiComputerAI.h"

/****************************************************************************
* renderSharedDataInitialize - see tiRenderSDL.h for description
****************************************************************************/
TiSharedData *renderSharedDataInitialize(void)
{
    TiSharedData *s;
    int counter;

    s = malloc(sizeof(TiSharedData));
    if(s == NULL)
    {
        return NULL;
    }

    s->renderState = TI_STATE_NO_STATE;
    s->titleStartTicks = SDL_GetTicks();
    s->curTicks = 0;
    s->numFrames = 0;
    s->lastFrameTicks = 0;
    s->cyclesLeftOver = 0;

    s->dialogBacking = NULL;
    s->boardAreaBacking = NULL;
    s->currentPlayerBacking = NULL;
    s->currentTileBacking = NULL;

    renderResetSharedDataStructure(s);

    return s;
}

/****************************************************************************
* renderResetSharedDataStructure - see tiRenderSDL.h for description
****************************************************************************/
int renderResetSharedDataStructure(TiSharedData *data)
{
    int counter;

    /* Reset all of the game structures */
    data->selectedPlayers = TI_MIN_PLAYERS;
    data->playerState[0] = TI_PLAYER_HUMAN;
    for(counter=1;counter<TI_MAX_PLAYERS;counter++)
    {
        data->playerState[counter] = TI_PLAYER_COMPUTER;
    }
    data->refreshBG = TI_TRUE;
    data->refreshOptionsScreen = TI_FALSE;
    data->refreshPlayerScores = TI_FALSE;
    data->refreshPlayersList = TI_FALSE;
    data->refreshPlayerTiles = TI_FALSE;
    data->refreshPlayerStations = TI_FALSE;
    data->refreshPlayerTrains = TI_FALSE;
    data->refreshTrackOverlays = TI_FALSE;

    data->exitGame = TI_FALSE;

    if(data->dialogBacking != NULL)
    {
        SDL_FreeSurface(data->dialogBacking);
        data->dialogBacking = NULL;
    }
    if(data->boardAreaBacking != NULL)
    {
        SDL_FreeSurface(data->boardAreaBacking);
        data->boardAreaBacking = NULL;
    }
    if(data->currentPlayerBacking != NULL)
    {
        SDL_FreeSurface(data->currentPlayerBacking);
        data->currentPlayerBacking = NULL;
    }

    if(data->currentTileBacking != NULL)
    {
        SDL_FreeSurface(data->currentTileBacking);
        data->currentTileBacking = NULL;
    }

    data->dialogBackingX = 0;
    data->dialogBackingY = 0;
    data->dialogBackingW = 0;
    data->dialogBackingH = 0;
    data->currentTileBackingX = 0;
    data->currentTileBackingY = 0;
    data->currentTileBackingW = 0;
    data->currentTileBackingH = 0;

    data->currentPlayerBackingPlayerNum = 0;

    data->drawTileHighlighted = 0;
    data->prevDrawTileHighlighted = 0;
    data->refreshDrawTileHighlighted = TI_TRUE;

    data->drawDarkenMask = TI_FALSE;
    data->drawValidMoveMasks = TI_FALSE;
    data->restoreBoardArea = TI_FALSE;
    data->drawTileOnBoard = TI_FALSE;

    data->numFilesLoaded = 0;

    data->currentMove = NULL;
    data->previousMove = NULL;
    return TI_OK;
}

/****************************************************************************
* renderSharedDataDestroy - see tiRenderSDL.h for description
****************************************************************************/
int renderSharedDataDestroy(TiSharedData **data)
{
    if((*data)->dialogBacking != NULL)
    {
        SDL_FreeSurface((*data)->dialogBacking);
        (*data)->dialogBacking = NULL;
    }
    if((*data)->boardAreaBacking != NULL)
    {
        SDL_FreeSurface((*data)->boardAreaBacking);
        (*data)->boardAreaBacking = NULL;
    }
    if((*data)->currentPlayerBacking != NULL)
    {
        SDL_FreeSurface((*data)->currentPlayerBacking);
        (*data)->currentPlayerBacking = NULL;
    }

    if((*data)->currentTileBacking != NULL)
    {
        SDL_FreeSurface((*data)->currentTileBacking);
        (*data)->currentTileBacking = NULL;
    }

    free(*data);
    *data = NULL;
    return TI_OK;
}

/****************************************************************************
* renderInitialize - see tiRenderSDL.h for description
****************************************************************************/
TiScreen *renderInitialize(int xRes, int yRes, int bpp)
{
    TiScreen *s;

    s = malloc(sizeof(TiScreen));
    if(s == NULL)
    {
        return NULL;
    }

    s->xRes = xRes;
    s->yRes = yRes;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
    {
        free(s);
        return NULL;
    }

    s->screen = SDL_SetVideoMode(s->xRes, s->yRes,bpp ,SDL_SWSURFACE);
    if(s->screen == NULL)
    {
        free(s);
        return NULL;
    }

    /* Run full-screen and disable the mouse cursor on Nokia tablets */
#ifdef _NOKIA_N800_
    SDL_WM_ToggleFullScreen(s->screen);
    SDL_ShowCursor(SDL_DISABLE);
#endif

    SDL_WM_SetCaption("TrackInsanity", NULL);

    return s;
}

/****************************************************************************
* renderDestroy - see tiRenderSDL.h for description
****************************************************************************/
void renderDestroy(TiScreen **display)
{
    free(*display);
    *display = NULL;
    SDL_Quit();
    return;
}

/****************************************************************************
* renderLoadImage - see tiRenderSDL.h for description
****************************************************************************/
SDL_Surface *renderLoadImage(char *fileName)
{
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimizedImage = NULL;
    Uint32 colorkey;

    loadedImage = IMG_Load(fileName);
    if(loadedImage == NULL)
    {
        perror("renderLoadImage: image load failed:");
    }
    else
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0, 0xFF);
        SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
        SDL_FreeSurface(loadedImage);
    }

    return optimizedImage;
}

/****************************************************************************
* renderAssetsInitialize- see tiRenderSDL.h for description
****************************************************************************/
TiAssets *renderAssetsInitialize(void)
{
    TiAssets *a;
    a = malloc(sizeof(TiAssets));
    if(a == NULL)
    {
        return NULL;
    }
    a->logoScreenBG = NULL;
    a->titleScreenBG = NULL;
    a->trainBannerTop = NULL;
    a->trainBannerBottom = NULL;
    a->tapScreenMsg = NULL;
    a->titleMenu = NULL;
    a->newGameBG = NULL;
    a->numPlayersDigits = NULL;
    a->loadingDialog = NULL;
    a->progressBar = NULL;
    a->optionScreenBG = NULL;
    a->optionDigits = NULL;
    a->optionYesNo = NULL;
    a->optionShowLastMove = NULL;
    a->optionAILevel = NULL;
    a->humanCpu = NULL;
    a->gameplayBG = NULL;
    a->playerColors = NULL;
    a->playerLightBG = NULL;
    a->playerStationsChart = NULL;
    a->tileStripSmall = NULL;
    a->tileStripLarge = NULL;
    a->stationStatus = NULL;
    a->playerStrings = NULL;
    a->playerTypeStrings = NULL;
    a->playerScoreString = NULL;
    a->currentPlayer = NULL;
    a->currentTile = NULL;
    a->selectTileDialog = NULL;
    a->selectActionDialog = NULL;
    a->selectPlayActive = NULL;
    a->selectDrawActive = NULL;
    a->selectDiscardActive = NULL;
    a->selectPassActive = NULL;
    a->chooseDiscardDialog = NULL;
    a->confirmExitDialog = NULL;
    a->cancelPlayTileButton = NULL;
    a->computerThinkingDialog = NULL;
    a->gameFinishedDialog = NULL;
    a->drawTileHighlightLarge = NULL;
    a->darkenMask = NULL;
    a->validMoveMask = NULL;
    a->scoreBacking = NULL;
    a->digits = NULL;
    a->trains = NULL;
    a->stations = NULL;
    a->trackOverlays = NULL;
    a->lastMoveHighlights = NULL;
    a->resultsBackground = NULL;
    a->resultsSmallDigits = NULL;
    a->resultsLargeDigits = NULL;
    a->resultsSmallPlayers = NULL;
    a->resultsLargePlayers = NULL;

    return a;
}

void renderDisplayLoadingDialog(TiScreen *display, TiAssets *assets, TiSharedData *data)
{
    renderBlitSurface(0, 0, TI_RENDER_LOADING_DIALOG_X, TI_RENDER_LOADING_DIALOG_Y,
                      TI_RENDER_LOADING_DIALOG_WIDTH, TI_RENDER_LOADING_DIALOG_HEIGHT,
                      assets->loadingDialog, display->screen);
    SDL_UpdateRect(display->screen, TI_RENDER_LOADING_DIALOG_X, TI_RENDER_LOADING_DIALOG_Y,
                   TI_RENDER_LOADING_DIALOG_WIDTH, TI_RENDER_LOADING_DIALOG_HEIGHT);
}

void renderUpdateProgressBar(TiScreen *display, TiAssets *assets, TiSharedData *data)
{
    renderBlitSurface(0, 0, TI_RENDER_PROGRESS_BAR_X, TI_RENDER_PROGRESS_BAR_Y,
                      TI_RENDER_PROGRESS_BAR_WIDTH * (float)((float)data->numFilesLoaded / (float)TI_RENDER_LOADING_DIALOG_NUM_FILES),
                      TI_RENDER_PROGRESS_BAR_HEIGHT, assets->progressBar, display->screen);
    SDL_UpdateRect(display->screen, TI_RENDER_PROGRESS_BAR_X, TI_RENDER_PROGRESS_BAR_Y,
                   TI_RENDER_PROGRESS_BAR_WIDTH, TI_RENDER_PROGRESS_BAR_HEIGHT);
}

/****************************************************************************
* renderLoadAssets - see tiRenderSDL.h for description
****************************************************************************/
int renderLoadAssets(TiScreen *display, TiAssets *assets, TiSharedData *data)
{

    int status = TI_OK;

    /* Yay for lazy evaluation! */

    if(data->renderState == TI_STATE_COMPANY_LOGO)
    {
        if((assets->logoScreenBG == NULL) &&
        ((assets->logoScreenBG = renderLoadImage("data/title/holygoat.png")) == NULL))
        {
            status = TI_ERROR;
        }
    }

    /* The trains are also necessary at the 'Final results' screen, so they're placed in a 
       separate if block from other 'title screen' things */
    if(data->renderState == TI_STATE_TITLE_SCREEN ||
       data->renderState == TI_STATE_TITLE_MENU_SCREEN ||
       data->renderState == TI_STATE_NEW_GAME_SCREEN ||
       data->renderState == TI_STATE_OPTIONS_SCREEN ||
       data->renderState == TI_STATE_GAME_RESULTS_SCREEN)
    {
        if((assets->trainBannerTop == NULL) &&
        ((assets->trainBannerTop = renderLoadImage("data/title/trainbannertop.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->trainBannerBottom == NULL) &&
        ((assets->trainBannerBottom = renderLoadImage("data/title/trainbannerbottom.png")) == NULL))
        {
            status = TI_ERROR;
        }
    }

    if(data->renderState == TI_STATE_TITLE_SCREEN ||
    data->renderState == TI_STATE_TITLE_MENU_SCREEN ||
    data->renderState == TI_STATE_OPTIONS_SCREEN ||
    data->renderState == TI_STATE_NEW_GAME_SCREEN)
    {
        if((assets->titleScreenBG == NULL) &&
        ((assets->titleScreenBG = renderLoadImage("data/title/titleScreen.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->tapScreenMsg == NULL) &&
        ((assets->tapScreenMsg = renderLoadImage("data/title/tapscreen.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->titleMenu == NULL) &&
        ((assets->titleMenu = renderLoadImage("data/title/titlemenu.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->newGameBG == NULL) &&
        ((assets->newGameBG = renderLoadImage("data/newGame/newGameMenu.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->numPlayersDigits == NULL) &&
        ((assets->numPlayersDigits = renderLoadImage("data/newGame/numPlayersHighlighted.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->humanCpu == NULL) &&
        ((assets->humanCpu = renderLoadImage("data/newGame/humanCPUHighlighted.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->optionScreenBG == NULL) &&
        ((assets->optionScreenBG = renderLoadImage("data/options/optionScreen.png")) == NULL))
        {
            status = TI_ERROR;
        }        
        if((assets->optionDigits == NULL) &&
        ((assets->optionDigits = renderLoadImage("data/options/digitsHighlighted.png")) == NULL))
        {
            status = TI_ERROR;
        }        
        if((assets->optionYesNo == NULL) &&
        ((assets->optionYesNo = renderLoadImage("data/options/yesNoHighlighted.png")) == NULL))
        {
            status = TI_ERROR;
        }
        if((assets->optionShowLastMove == NULL) &&
        ((assets->optionShowLastMove = renderLoadImage("data/options/showLastMoveHighlighted.png")) == NULL))
        {
            status = TI_ERROR;
        }        
        if((assets->optionAILevel == NULL) &&
        ((assets->optionAILevel = renderLoadImage("data/options/aiLevelHighlighted.png")) == NULL))
        {
            status = TI_ERROR;
        }
    }

    if(data->renderState == TI_STATE_NEW_GAME_SCREEN)
    {
        if((assets->loadingDialog == NULL) &&
        ((assets->loadingDialog = renderLoadImage("data/title/loadingDialog.png")) == NULL))
        {
            status = TI_ERROR;
        }

        if((assets->progressBar == NULL) &&
        ((assets->progressBar = renderLoadImage("data/title/progressBar.png")) == NULL))
        {
            status = TI_ERROR;
        }
    }

    if(data->renderState == TI_STATE_IN_GAME)
    {
        renderDisplayLoadingDialog(display, assets, data);

        if((assets->gameplayBG == NULL) &&
        ((assets->gameplayBG = renderLoadImage("data/board/boardBG.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->playerColors == NULL) &&
        ((assets->playerColors = renderLoadImage("data/board/playerColorTabs.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->playerLightBG == NULL) &&
        ((assets->playerLightBG = renderLoadImage("data/board/activePlayerBG1.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->playerStationsChart == NULL) &&
        ((assets->playerStationsChart = renderLoadImage("data/board/activePlayerStations.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->tileStripSmall == NULL) &&
        ((assets->tileStripSmall = renderLoadImage("data/board/tileStrip40.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->tileStripLarge == NULL) &&
        ((assets->tileStripLarge = renderLoadImage("data/board/tileStrip64.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->stationStatus == NULL) &&
        ((assets->stationStatus = renderLoadImage("data/board/activePlayerStationStatus.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->playerStrings == NULL) &&
        ((assets->playerStrings = renderLoadImage("data/board/playerNumberText.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->playerTypeStrings == NULL) &&
        ((assets->playerTypeStrings = renderLoadImage("data/board/playerHumanComputer.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->playerScoreString == NULL) &&
        ((assets->playerScoreString = renderLoadImage("data/board/playerScoreText.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->currentPlayer == NULL) &&
        ((assets->currentPlayer = renderLoadImage("data/board/currentPlayer.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->currentTile == NULL) &&
        ((assets->currentTile = renderLoadImage("data/board/currentTile.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->selectTileDialog == NULL) &&
        ((assets->selectTileDialog = renderLoadImage("data/board/selectTile.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->selectActionDialog == NULL) &&
        ((assets->selectActionDialog = renderLoadImage("data/board/selectAction.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->selectPlayActive == NULL) &&
        ((assets->selectPlayActive = renderLoadImage("data/board/selectActionPlayTile.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->selectDrawActive == NULL) &&
        ((assets->selectDrawActive = renderLoadImage("data/board/selectActionDrawTile.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->selectDiscardActive == NULL) &&
        ((assets->selectDiscardActive = renderLoadImage("data/board/selectActionDiscard.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->selectPassActive == NULL) &&
        ((assets->selectPassActive = renderLoadImage("data/board/selectActionPass.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->chooseDiscardDialog == NULL) &&
        ((assets->chooseDiscardDialog = renderLoadImage("data/board/chooseDiscard.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->confirmExitDialog == NULL) &&
        ((assets->confirmExitDialog = renderLoadImage("data/board/confirmExit.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->cancelPlayTileButton == NULL) &&
        ((assets->cancelPlayTileButton = renderLoadImage("data/board/cancelButton.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->computerThinkingDialog == NULL) &&
        ((assets->computerThinkingDialog = renderLoadImage("data/board/cpuThinking.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->gameFinishedDialog == NULL) &&
        ((assets->gameFinishedDialog = renderLoadImage("data/board/gameFinished.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->drawTileHighlightLarge == NULL) &&
        ((assets->drawTileHighlightLarge = renderLoadImage("data/board/drawTileHighlight80.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->darkenMask == NULL) &&
        ((assets->darkenMask = renderLoadImage("data/board/darkenMask.png")) == NULL))
        {
            status = TI_ERROR;
        }
        SDL_SetAlpha(assets->darkenMask, SDL_SRCALPHA, 128);
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->validMoveMask == NULL) &&
        ((assets->validMoveMask = renderLoadImage("data/board/validMask.png")) == NULL))
        {
            status = TI_ERROR;
        }
        SDL_SetAlpha(assets->validMoveMask, SDL_SRCALPHA, 80);
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->scoreBacking == NULL) &&
        ((assets->scoreBacking = renderLoadImage("data/board/scoreBacking.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->digits == NULL) &&
        ((assets->digits = renderLoadImage("data/board/digits.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->trains == NULL) &&
        ((assets->trains = renderLoadImage("data/board/trains.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->stations == NULL) &&
        ((assets->stations = renderLoadImage("data/board/stations.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->trackOverlays == NULL) &&
        ((assets->trackOverlays = renderLoadImage("data/board/trackOverlays.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);

        if((assets->lastMoveHighlights == NULL) &&
        ((assets->lastMoveHighlights = renderLoadImage("data/board/lastMoveHighlights.png")) == NULL))
        {
            status = TI_ERROR;
        }
        data->numFilesLoaded++;
        renderUpdateProgressBar(display, assets, data);
    }

    if(data->renderState == TI_STATE_GAME_RESULTS_SCREEN)
    {
        if((assets->resultsBackground == NULL) &&
        ((assets->resultsBackground = renderLoadImage("data/results/resultsScreen.png")) == NULL))
        {
            status = TI_ERROR;
        }

        if((assets->resultsSmallDigits == NULL) &&
        ((assets->resultsSmallDigits = renderLoadImage("data/results/digitsSmall.png")) == NULL))
        {
            status = TI_ERROR;
        }

        if((assets->resultsLargeDigits == NULL) &&
        ((assets->resultsLargeDigits = renderLoadImage("data/results/digitsLarge.png")) == NULL))
        {
            status = TI_ERROR;
        }

        if((assets->resultsSmallPlayers == NULL) &&
        ((assets->resultsSmallPlayers = renderLoadImage("data/results/playersSmall.png")) == NULL))
        {
            status = TI_ERROR;
        }

        if((assets->resultsLargePlayers == NULL) &&
        ((assets->resultsLargePlayers = renderLoadImage("data/results/playersLarge.png")) == NULL))
        {
            status = TI_ERROR;
        }
    }

    return status;
}

/****************************************************************************
* renderFreeHelper- see tiRenderSDL.h for description
****************************************************************************/
void renderFreeHelper(SDL_Surface **s)
{
    if(*s != NULL)
    {
        SDL_FreeSurface(*s);
        *s = NULL;
    }
}

/****************************************************************************
* renderFreeAssets - see tiRenderSDL.h for description
****************************************************************************/
int renderFreeAssets(TiAssets *assets, TiSharedData *data)
{
    if(data->renderState != TI_STATE_COMPANY_LOGO)
    {
        renderFreeHelper(&(assets->logoScreenBG));
    }

    /* The train banners are also used at the 'final results' screen, so they
       have been moved to a separate if block from the other title screen
       stuff 
     */
    if(data->renderState != TI_STATE_TITLE_SCREEN &&
       data->renderState != TI_STATE_TITLE_MENU_SCREEN &&
       data->renderState != TI_STATE_NEW_GAME_SCREEN &&
       data->renderState != TI_STATE_GAME_RESULTS_SCREEN)
    {
        renderFreeHelper(&(assets->trainBannerTop));
        renderFreeHelper(&(assets->trainBannerBottom));
    }

    if(data->renderState != TI_STATE_TITLE_SCREEN &&
    data->renderState != TI_STATE_TITLE_MENU_SCREEN &&
    data->renderState != TI_STATE_OPTIONS_SCREEN &&
    data->renderState != TI_STATE_NEW_GAME_SCREEN)
    {
        renderFreeHelper(&(assets->titleScreenBG));
        renderFreeHelper(&(assets->tapScreenMsg));
        renderFreeHelper(&(assets->titleMenu));
        renderFreeHelper(&(assets->newGameBG));
        renderFreeHelper(&(assets->numPlayersDigits));
        renderFreeHelper(&(assets->humanCpu));
        renderFreeHelper(&(assets->optionScreenBG));
        renderFreeHelper(&(assets->optionDigits));
        renderFreeHelper(&(assets->optionYesNo));
        renderFreeHelper(&(assets->optionShowLastMove));
        renderFreeHelper(&(assets->optionAILevel));
    }

    if(data->renderState != TI_STATE_NEW_GAME_SCREEN &&
       data->renderState != TI_STATE_IN_GAME)
    {
        renderFreeHelper(&(assets->loadingDialog));
        renderFreeHelper(&(assets->progressBar));
    }

    if(data->renderState != TI_STATE_IN_GAME)
    {
        renderFreeHelper(&(assets->gameplayBG));
        renderFreeHelper(&(assets->playerColors));
        renderFreeHelper(&(assets->playerLightBG));
        renderFreeHelper(&(assets->playerStationsChart));
        renderFreeHelper(&(assets->tileStripSmall));
        renderFreeHelper(&(assets->tileStripLarge));
        renderFreeHelper(&(assets->stationStatus));
        renderFreeHelper(&(assets->playerStrings));
        renderFreeHelper(&(assets->playerTypeStrings));
        renderFreeHelper(&(assets->playerScoreString));
        renderFreeHelper(&(assets->currentPlayer));
        renderFreeHelper(&(assets->currentTile));

        renderFreeHelper(&(assets->selectTileDialog));
        renderFreeHelper(&(assets->selectActionDialog));
        renderFreeHelper(&(assets->selectPlayActive));
        renderFreeHelper(&(assets->selectDrawActive));
        renderFreeHelper(&(assets->selectDiscardActive));
        renderFreeHelper(&(assets->selectPassActive));
        renderFreeHelper(&(assets->chooseDiscardDialog));
        renderFreeHelper(&(assets->confirmExitDialog));
        renderFreeHelper(&(assets->cancelPlayTileButton));
        renderFreeHelper(&(assets->computerThinkingDialog));
        renderFreeHelper(&(assets->gameFinishedDialog));

        renderFreeHelper(&(assets->drawTileHighlightLarge));
        renderFreeHelper(&(assets->darkenMask));
        renderFreeHelper(&(assets->validMoveMask));
        renderFreeHelper(&(assets->scoreBacking));
        renderFreeHelper(&(assets->digits));
        renderFreeHelper(&(assets->trains));
        renderFreeHelper(&(assets->stations));
        renderFreeHelper(&(assets->trackOverlays));
        renderFreeHelper(&(assets->lastMoveHighlights));
    }

    if(data->renderState != TI_STATE_GAME_RESULTS_SCREEN)
    {
        renderFreeHelper(&(assets->resultsBackground));
        renderFreeHelper(&(assets->resultsSmallDigits));
        renderFreeHelper(&(assets->resultsLargeDigits));
        renderFreeHelper(&(assets->resultsSmallPlayers));
        renderFreeHelper(&(assets->resultsLargePlayers));
    }

    return TI_OK;
}

/****************************************************************************
* renderSetRenderState - see tiRenderSDL.h for description
****************************************************************************/
void renderSetRenderState(int curState, int prevState, TiScreen *display, TiAssets *assets, TiSharedData *data)
{
    Game *g;

    renderFreeAssets(assets, data);

    switch(curState)
    {
        case TI_STATE_COMPANY_LOGO:
            printf("Changing render state to TI_STATE_COMPANY_LOGO\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            break;
        case TI_STATE_TITLE_SCREEN:
            printf("Changing render state to TI_STATE_TITLE_SCREEN\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            data->topTrainX = TI_RENDER_TRAIN_INITIAL_X;
            data->topTrainDirection = TI_RENDER_TRAIN_DIR_RIGHT;
            data->bottomTrainX = TI_RENDER_TRAIN_FINAL_X;
            data->bottomTrainDirection = TI_RENDER_TRAIN_DIR_LEFT;               
            break;
        case TI_STATE_TITLE_MENU_SCREEN:
            printf("Changing render state to TI_STATE_TITLE_MENU_SCREEN\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            break;
        case TI_STATE_NEW_GAME_SCREEN:
            printf("Changing render state to TI_STATE_NEW_GAME_SCREEN\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            data->refreshPlayersList = TI_TRUE;
            break;
        case TI_STATE_OPTIONS_SCREEN:
            printf("Changing render state to TI_STATE_OPTIONS_SCREEN\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            data->refreshOptionsScreen = TI_TRUE;
            break;
        case TI_STATE_IN_GAME:
            printf("Changing render state to TI_STATE_IN_GAME\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            data->refreshPlayerTiles = TI_TRUE;
            data->refreshPlayerStations = TI_TRUE;
            data->refreshPlayerScores = TI_TRUE;
            data->refreshPlayerStations = TI_TRUE;
            data->refreshTrackOverlays = TI_TRUE;
            g = gameGetGlobalGameInstance();
            gameInitializePlayersFromUi(g, data);
            gameSetGameState(g, TI_GAME_STATE_SELECT_ACTION);
            break;
        case TI_STATE_GAME_RESULTS_SCREEN:
            printf("Changing render state to TI_STATE_GAME_RESULTS_SCREEN\n");
            data->renderState = curState;
            data->refreshBG = TI_TRUE;
            data->topTrainX = TI_RENDER_TRAIN_INITIAL_X;
            data->topTrainDirection = TI_RENDER_TRAIN_DIR_RIGHT;
            data->bottomTrainX = TI_RENDER_TRAIN_FINAL_X;
            data->bottomTrainDirection = TI_RENDER_TRAIN_DIR_LEFT;
            break;
        case TI_STATE_END_GAME:
            printf("Changing render state to TI_STATE_END_GAME\n");
            data->renderState = curState;
            data->exitGame = TI_TRUE;
            break;
        default:
            perror("renderSetRenderState: unknown render state");
            break;
    }

    if(renderLoadAssets(display, assets, data) == TI_ERROR)
    {
        perror("renderSetRenderState: Unable to load all images! Exiting game!\n");
        exit(0);
    }
    data->stateStartTicks = SDL_GetTicks();
}

/****************************************************************************
* renderCheckTimingConditions - see tiRenderSDL.h for description
****************************************************************************/
void renderCheckTimingConditions(TiScreen *display, TiAssets *assets, TiSharedData *data)
{
    Uint32 curTime;
    curTime = SDL_GetTicks();

    switch(data->renderState)
    {
        case TI_STATE_COMPANY_LOGO:
            if(curTime - data->stateStartTicks >= TI_RENDER_LOGO_DURATION)
            {
                renderSetRenderState(TI_STATE_TITLE_SCREEN, data->renderState, display, assets, data);
            }
            return;
            break;
        case TI_STATE_TITLE_SCREEN:
            break;
        default:
            break;
    }
}

/****************************************************************************
* renderApplySurface - see tiRenderSDL.h for description
****************************************************************************/
void renderApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *dest)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, dest, &offset);
}

/****************************************************************************
* renderBlitSurface - see tiRenderSDL.h for description
****************************************************************************/
void renderBlitSurface(int srcX, int srcY, int dstX, int dstY, int w, int h,
                    SDL_Surface *source, SDL_Surface *dest)
{
    SDL_Rect sourceRect;
    SDL_Rect destRect;
    sourceRect.x = srcX;
    sourceRect.y = srcY;
    sourceRect.w = w;
    sourceRect.h = h;
    destRect.x = dstX;
    destRect.y = dstY;
    destRect.w = w;
    destRect.h = h;
    SDL_BlitSurface(source, &sourceRect, dest, &destRect);
}

/****************************************************************************
* renderProcessComputerMove - see tiRenderSDL.h for description
****************************************************************************/
int renderProcessComputerMove(TiScreen *display, TiAssets *a,  TiSharedData *data)
{
    Game *g;
    ComputerAIPacket *cur;
    ComputerAIPacket *prev;
    int lastPlayer;

    g = gameGetGlobalGameInstance();
    cur = data->currentMove;
    prev = data->previousMove;

    /* At the first part of a computer move, make sure the appropriate stuff is
       updated on screen */
    if(prev == NULL)
    {
        renderDrawCurrentPlayerHighlight(display, a, data);
        renderDrawCurrentTileHighlight(display, a, data);
        data->refreshPlayerTiles = TI_TRUE;
        renderUpdateScreen(display, a, data);
    }

    switch(cur->moveType)
    {
        case TI_CPU_MOVE_DRAW:
            SDL_Delay(rand() % TI_CPU_DYNAMIC_DELAY);
            if(g->players[g->curPlayer].currentTileId == TI_TILE_NO_TILE)
            {
                g->players[g->curPlayer].currentTileId = tilePoolDrawRandomTile(g->tilepool);
                g->selectedMoveIsReserveTile = TI_FALSE;
                g->selectedMoveTileId = g->players[g->curPlayer].currentTileId;
            }
            else
            {
                g->players[g->curPlayer].reserveTileId = tilePoolDrawRandomTile(g->tilepool);
                g->selectedMoveIsReserveTile = TI_TRUE;
                g->selectedMoveTileId = g->players[g->curPlayer].reserveTileId;
            }
            renderDrawCurrentTileHighlight(display, a, data);
            data->refreshPlayerTiles = TI_TRUE;
            renderUpdateScreen(display, a, data);
            break;
        case TI_CPU_MOVE_PLAY:
            /* Sleep for a while to make CPU moves non-instantaneous */
            SDL_Delay((rand() % TI_CPU_DYNAMIC_DELAY) + TI_CPU_STATIC_DELAY);
            g->selectedMoveTileX = cur->moveX;
            g->selectedMoveTileY = cur->moveY;
            if(cur->heldTile == TI_CPU_HELD_TILE_PRIMARY)
            {
                g->selectedMoveTileId = g->players[g->curPlayer].currentTileId;
                g->selectedMoveIsReserveTile = TI_FALSE;
                if(g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
                {
                    g->players[g->curPlayer].currentTileId = g->players[g->curPlayer].reserveTileId;
                    g->players[g->curPlayer].reserveTileId = TI_TILE_NO_TILE;
                }
                else
                {
                    g->players[g->curPlayer].currentTileId = TI_TILE_NO_TILE;
                }
            }
            else if(cur->heldTile == TI_CPU_HELD_TILE_RESERVE)
            {
                g->selectedMoveTileId = g->players[g->curPlayer].reserveTileId;
                g->selectedMoveIsReserveTile = TI_TRUE;
                g->players[g->curPlayer].reserveTileId = TI_TILE_NO_TILE;
            }
            else
            {
                return TI_ERROR;
            }
            boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->selectedMoveTileId));
            if(boardPlaceTile(g->board, g->selectedMoveTileX, g->selectedMoveTileY, 
                              g->selectedMoveTileId) == TI_BOARD_ILLEGAL_MOVE)
            {
                perror("Invalid tile placement!\n");
                printf("Attempted placement of %d at (%d, %d)\n", g->selectedMoveTileId, g->selectedMoveTileX, g->selectedMoveTileY);
                exit(-1);
            }

            /* Undraw the previous player's move if the user has selected the 'Last Player' option */
            renderUndrawPreviousMove(display, a, data);
            /* 'Undraw' the current player's last move */
            renderDrawTile(display, a, data , g->players[g->curPlayer].lastMoveX,
                           g->players[g->curPlayer].lastMoveY,
                           g->board->b[g->players[g->curPlayer].lastMoveX][g->players[g->curPlayer].lastMoveY].tileIndex);
            g->players[g->curPlayer].lastMoveX = g->selectedMoveTileX;
            g->players[g->curPlayer].lastMoveY = g->selectedMoveTileY;
            renderDrawTileOnBoard(display, a, data);
            renderLastMoves(display, a, data);
            renderTrackOverlays(display, a, data);
            renderDrawCurrentTileHighlight(display, a, data);
            data->refreshPlayerTiles = TI_TRUE;
            renderUpdateScreen(display, a, data);
            gameCheckForCompletedTracks(g);
            break;
        case TI_CPU_MOVE_DISCARD:
            /* Sleep for a while to make CPU moves non-instantaneous */
            SDL_Delay((rand() % TI_CPU_DYNAMIC_DELAY) + TI_CPU_STATIC_DELAY);
            if(cur->heldTile == TI_CPU_HELD_TILE_PRIMARY)
            {
                g->selectedMoveTileId = g->players[g->curPlayer].currentTileId;
                g->selectedMoveIsReserveTile = TI_FALSE;
            }
            else if(cur->heldTile == TI_CPU_HELD_TILE_RESERVE)
            {
                g->selectedMoveTileId = g->players[g->curPlayer].reserveTileId;
                g->selectedMoveIsReserveTile = TI_TRUE;
            }
            else
            {
                return TI_ERROR;
            }
            if(gameDiscardTile(g) != TI_OK)
            {
                perror("Discard failed!\n");
                exit(-1);
            }
            break;
        case TI_CPU_MOVE_END_TURN:
            /* Nothing -- the code never gets here */
            break;
    };

    return TI_OK;
}

/****************************************************************************
* renderUpdateLogic - see tiRenderSDL.h for description
****************************************************************************/
void renderUpdateLogic(TiScreen *display, TiSharedData *data, TiAssets *a)
{

    Game *g;
    g = gameGetGlobalGameInstance();

    switch(data->renderState)
    {
        case TI_STATE_COMPANY_LOGO:
            break;
        case TI_STATE_TITLE_SCREEN:
        case TI_STATE_TITLE_MENU_SCREEN:
        case TI_STATE_NEW_GAME_SCREEN:
        case TI_STATE_GAME_RESULTS_SCREEN:
        case TI_STATE_OPTIONS_SCREEN:             
            if(data->topTrainDirection == TI_RENDER_TRAIN_DIR_LEFT)
            {
                data->topTrainX -= TI_RENDER_TRAIN_BANNER_SPEED;
                if(data->topTrainX <= TI_RENDER_TRAIN_INITIAL_X)
                {
                    data->topTrainDirection = TI_RENDER_TRAIN_DIR_RIGHT;
                }
            }
            else
            {
                data->topTrainX += TI_RENDER_TRAIN_BANNER_SPEED;
                if(data->topTrainX >= TI_RENDER_TRAIN_FINAL_X)
                {
                    data->topTrainDirection = TI_RENDER_TRAIN_DIR_LEFT;
                }
            }
            if(data->bottomTrainDirection == TI_RENDER_TRAIN_DIR_LEFT)
            {
                data->bottomTrainX -= TI_RENDER_TRAIN_BANNER_SPEED;
                if(data->bottomTrainX <= TI_RENDER_TRAIN_INITIAL_X)
                {
                    data->bottomTrainDirection = TI_RENDER_TRAIN_DIR_RIGHT;
                }
            }
            else
            {
                data->bottomTrainX += TI_RENDER_TRAIN_BANNER_SPEED;
                if(data->bottomTrainX >= TI_RENDER_TRAIN_FINAL_X)
                {
                    data->bottomTrainDirection = TI_RENDER_TRAIN_DIR_LEFT;
                }
            }
            break;
        case TI_STATE_IN_GAME:
            switch(g->gameState)
            {
                case TI_GAME_STATE_COMPUTER_MOVE:
                    data->currentMove = computerDetermineNextMove(NULL);
                    if(data->currentMove == NULL)
                    {
                        perror("Unable to get CPU move!\n");
                        exit(-1);
                    }
                    while(data->currentMove->moveType != TI_CPU_MOVE_END_TURN)
                    {
                        renderProcessComputerMove(display, a, data);
                        if(data->previousMove != NULL)
                        {
                            free(data->previousMove);
                        }
                        data->previousMove = data->currentMove;
                        data->currentMove = computerDetermineNextMove(data->previousMove);
                    }
                    if(data->previousMove != NULL)
                    {
                        free(data->previousMove);
                        data->previousMove = NULL;
                    }
                    if(data->currentMove != NULL)
                    {
                        free(data->currentMove);
                        data->currentMove = NULL;
                    }
                    gameSetGameState(g, TI_GAME_STATE_END_TURN);
                    break;
                case TI_GAME_STATE_GAME_FINISHED:
                    break;
                default:
                    break;
            }
            break;
        case TI_STATE_END_GAME:
            break;
        default:
            break;
    }
}

/****************************************************************************
* renderDrawAnimatedTrains - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawAnimatedTrains(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    SDL_Surface *backing;

    /* Grab the train backing from the correct bitmap depending on the current render state */
    if(data->renderState == TI_STATE_GAME_RESULTS_SCREEN)
    {
        backing = a->resultsBackground;
    }
    else
    {
        backing = a->titleScreenBG;
    }

    /* Clear the existing on-screen train */
    renderBlitSurface(0, 0, 0, 0, TI_GAME_XRES, 40,
                    backing, display->screen);
    renderBlitSurface(0, TI_GAME_YRES-40, 0, TI_GAME_YRES-40, TI_GAME_XRES,
                    40, backing, display->screen);
    /* Now redraw them */
    renderBlitSurface(0, 0, data->topTrainX, 9,
                    TI_RENDER_TRAIN_BANNER_WIDTH, 22,
                    a->trainBannerTop, display->screen);
    renderBlitSurface(0, 0, data->bottomTrainX,
                    TI_GAME_YRES-31, TI_RENDER_TRAIN_BANNER_WIDTH, 22,
                    a->trainBannerBottom, display->screen);
    SDL_UpdateRect(display->screen, 0, 0, TI_GAME_XRES-1, 40);
    SDL_UpdateRect(display->screen, 0, TI_GAME_YRES-40, TI_GAME_XRES-1,40);
}

/****************************************************************************
* renderTitleScreenDrawBase - see tiRenderSDL.h for description
****************************************************************************/
void renderTitleScreenDrawBase(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    if(data->refreshBG == TI_TRUE)
    {
        renderApplySurface(0,0, a->titleScreenBG, display->screen);
        SDL_UpdateRect(display->screen, 0, 0, 0, 0);
        data->refreshBG = TI_FALSE;
    }

    renderDrawAnimatedTrains(display, a, data);
    return;
}

/****************************************************************************
* renderNewGameScreenExtras - see tiRenderSDL.h for description
****************************************************************************/
void renderNewGameScreenExtras(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    int counter;

    if(data->refreshPlayersList == TI_TRUE)
    {
        renderApplySurface(0, 40, a->newGameBG, display->screen);
        /* Blank out the lines for the players that aren't currently
        applicable */
        renderBlitSurface(TI_RENDER_PLAYER_OFFSETS[data->selectedPlayers-2][0],
                          TI_RENDER_PLAYER_OFFSETS[data->selectedPlayers-2][1],
                          TI_RENDER_PLAYER_OFFSETS[data->selectedPlayers-2][0],
                          TI_RENDER_PLAYER_OFFSETS[data->selectedPlayers-2][1],
                          TI_RENDER_PLAYER_OFFSETS[data->selectedPlayers-2][2],
                          TI_RENDER_PLAYER_OFFSETS[data->selectedPlayers-2][3],
                          a->titleScreenBG, display->screen);
        /* Mark the number of players on the screen */
        counter = data->selectedPlayers - 2;
        renderBlitSurface(counter*TI_RENDER_NUM_PLAYER_OFFSETS[counter][2], 0,
                          TI_RENDER_NUM_PLAYER_OFFSETS[counter][0],
                          TI_RENDER_NUM_PLAYER_OFFSETS[counter][1],
                          TI_RENDER_NUM_PLAYER_OFFSETS[counter][2],
                          TI_RENDER_NUM_PLAYER_OFFSETS[counter][3],
                          a->numPlayersDigits, display->screen);
        /* For each player, mark human or CPU for each */
        for(counter=0;counter<data->selectedPlayers;counter++)
        {
            if(data->playerState[counter] == TI_PLAYER_HUMAN)
            {
                renderBlitSurface(0,0,
                                  TI_RENDER_HUMAN_OFFSETS[counter][0],
                                  TI_RENDER_HUMAN_OFFSETS[counter][1],
                                  TI_RENDER_HUMAN_OFFSETS[counter][2],
                                  TI_RENDER_HUMAN_OFFSETS[counter][3],
                                  a->humanCpu, display->screen);
            }
            else
            {
                renderBlitSurface(TI_RENDER_HUMAN_OFFSETS[counter][2], 0,
                                  TI_RENDER_CPU_OFFSETS[counter][0],
                                  TI_RENDER_CPU_OFFSETS[counter][1],
                                  TI_RENDER_CPU_OFFSETS[counter][2],
                                  TI_RENDER_CPU_OFFSETS[counter][3],
                                  a->humanCpu, display->screen);
            }
        }
        SDL_UpdateRect(display->screen, 0, 0, 0, 0);
        data->refreshPlayersList = TI_FALSE;
    }
}

/****************************************************************************
* renderOptionsScreenExtras - see tiRenderSDL.h for description
****************************************************************************/
void renderOptionsScreenExtras(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    Game *g = gameGetGlobalGameInstance();
    
    if(data->refreshOptionsScreen == TI_TRUE)
    {
        renderApplySurface(0, 40, a->optionScreenBG, display->screen);
        
        if(g->tmpHighlightTracks == TI_GAME_OPTIONS_YES)
        {
            renderBlitSurface(TI_RENDER_OPTIONS_TRACKS_FILE_OFFSETS[1], 0,
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][0],
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][1],
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][2],
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][3],
                              a->optionYesNo, display->screen);
        }
        else
        {
            renderBlitSurface(TI_RENDER_OPTIONS_TRACKS_FILE_OFFSETS[0], 0,
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][0],
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][1],
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][2],
                              TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][3],
                              a->optionYesNo, display->screen);
        }            
        
        if(g->tmpHighlightLegalMoves == TI_GAME_OPTIONS_YES)
        {
            renderBlitSurface(TI_RENDER_OPTIONS_LEGAL_FILE_OFFSETS[1], 0,
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][0],
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][1],
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][2],
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][3],
                              a->optionYesNo, display->screen);
        }
        else
        {
            renderBlitSurface(TI_RENDER_OPTIONS_LEGAL_FILE_OFFSETS[0], 0,
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][0],
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][1],
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][2],
                              TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][3],
                              a->optionYesNo, display->screen);
        }
                              
        if(g->tmpShowLastMove == TI_GAME_OPTIONS_NO)
        {
            renderBlitSurface(TI_RENDER_OPTIONS_LAST_MOVE_FILE_OFFSETS[0], 0,
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][0],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][1],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][2],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][3],
                              a->optionShowLastMove, display->screen);
        }
        else if(g->tmpShowLastMove == TI_GAME_OPTIONS_LAST_PLAYER)
        {
            renderBlitSurface(TI_RENDER_OPTIONS_LAST_MOVE_FILE_OFFSETS[1], 0,
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][0],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][1],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][2],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][3],
                              a->optionShowLastMove, display->screen);
        }
        else /* All players */
        {
            renderBlitSurface(TI_RENDER_OPTIONS_LAST_MOVE_FILE_OFFSETS[2], 0,
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][0],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][1],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][2],
                              TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][3],
                              a->optionShowLastMove, display->screen);
        }

        if(g->tmpDefaultAILevel == TI_GAME_OPTIONS_AI_EASY)
        {
            renderBlitSurface(TI_RENDER_OPTIONS_AI_LEVEL_FILE_OFFSETS[0], 0,
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][0],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][1],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][2],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][3],
                              a->optionAILevel, display->screen);
        }
        else if(g->tmpDefaultAILevel == TI_GAME_OPTIONS_AI_MEDIUM)
        {
            renderBlitSurface(TI_RENDER_OPTIONS_AI_LEVEL_FILE_OFFSETS[1], 0,
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][0],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][1],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][2],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][3],
                              a->optionAILevel, display->screen);
        }
        else
        {
            renderBlitSurface(TI_RENDER_OPTIONS_AI_LEVEL_FILE_OFFSETS[2], 0,
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][0],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][1],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][2],
                              TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][3],
                              a->optionAILevel, display->screen);
        }

        renderBlitSurface(TI_RENDER_OPTIONS_DIGITS_OFFSETS[g->tmpMusicVolume], 0,
                          TI_RENDER_OPTIONS_MUSIC_OFFSETS[g->tmpMusicVolume][0],
                          TI_RENDER_OPTIONS_MUSIC_OFFSETS[g->tmpMusicVolume][1],
                          TI_RENDER_OPTIONS_MUSIC_OFFSETS[g->tmpMusicVolume][2],
                          TI_RENDER_OPTIONS_MUSIC_OFFSETS[g->tmpMusicVolume][3],
                          a->optionDigits, display->screen);

        renderBlitSurface(TI_RENDER_OPTIONS_DIGITS_OFFSETS[g->tmpEffectsVolume], 0,
                          TI_RENDER_OPTIONS_SOUND_OFFSETS[g->tmpEffectsVolume][0],
                          TI_RENDER_OPTIONS_SOUND_OFFSETS[g->tmpEffectsVolume][1],
                          TI_RENDER_OPTIONS_SOUND_OFFSETS[g->tmpEffectsVolume][2],
                          TI_RENDER_OPTIONS_SOUND_OFFSETS[g->tmpEffectsVolume][3],
                          a->optionDigits, display->screen);

        SDL_UpdateRect(display->screen, 0, 0, 0, 0);
        data->refreshOptionsScreen = TI_FALSE;
    }     
}

void renderLastMoves(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    Game *g;
    int counter;
    int lastPlayer;

    g = gameGetGlobalGameInstance();

    for(counter=0;counter<g->numPlayers;counter++)
    {
        if(g->showLastMove == TI_GAME_OPTIONS_ALL_PLAYERS ||
           (g->showLastMove == TI_GAME_OPTIONS_LAST_PLAYER && counter == g->curPlayer))
        {
            if(g->players[counter].lastMoveX != TI_PLAYER_INVALID_LAST_MOVE &&
               g->players[counter].lastMoveY != TI_PLAYER_INVALID_LAST_MOVE)
            {
                renderBlitSurface(counter * TI_RENDER_SMALL_TILE_WIDTH, 0,
                                  TI_RENDER_BOARD_TILE_AREA_X + ((g->players[counter].lastMoveX - 1) * TI_RENDER_SMALL_TILE_WIDTH),
                                  TI_RENDER_BOARD_TILE_AREA_Y + ((g->players[counter].lastMoveY - 1) * TI_RENDER_SMALL_TILE_WIDTH),
                                  TI_RENDER_SMALL_TILE_WIDTH, TI_RENDER_SMALL_TILE_WIDTH,
                                  a->lastMoveHighlights, display->screen);
                SDL_UpdateRect(display->screen,
                               TI_RENDER_BOARD_TILE_AREA_X + (g->players[counter].lastMoveX * TI_RENDER_SMALL_TILE_WIDTH),
                               TI_RENDER_BOARD_TILE_AREA_Y + (g->players[counter].lastMoveY * TI_RENDER_SMALL_TILE_WIDTH),
                               TI_RENDER_SMALL_TILE_WIDTH, TI_RENDER_SMALL_TILE_WIDTH);
            }
        }
    }
}

/****************************************************************************
* renderTrackOverlay - see tiRenderSDL.h for description
****************************************************************************/
void renderTrackOverlay(TiScreen *display, TiAssets *a, TiSharedData *data, int station)
{

    Game *g;
    Board *b;
    int stationX, stationY, stationExit;
    int newX, newY, newExit, newType;
    int oldX, oldY, oldExit;
    int loopCatcher, loopLimit;
    int player;

    g = gameGetGlobalGameInstance();
    b = g->board;

    /* User doesn't want track overlays, so just return */
    if(g->highlightTracks == TI_GAME_OPTIONS_NO)
    {
        return;
    }

    player = g->board->playerStations[g->numPlayers][station];

    if(player == TI_BOARD_NO_TRAIN)
    {
        return;
    }

    /* Subtract one from the player number to get the correct vertical offset into the 
       track overlay file */
    player = player - 1;

    /* Determine the starting point */
    boardGetStationInfo(station, &stationX, &stationY, &stationExit);

    if(b->b[stationX][stationY].type != TI_BOARDSQUARE_TYPE_STATION)
    {
        perror("boardCalculateTrackScore: starting point isn't station");
        return;
    }

    /* If the train is already gone, don't draw the track */
    /*
    if(b->b[stationX][stationY].trainPresent == TI_BOARD_NO_TRAIN)
    {
        return;
    }
    */
    newType = boardFindNextTrackSection(b, stationX, stationY, stationExit,
                                        &newX, &newY, &newExit);

    loopCatcher = 0;
    loopLimit = 255;

    while((newType == TI_BOARDSQUARE_TYPE_PLAYED_TILE) &&
        (loopCatcher < loopLimit))
    {
        loopCatcher++;
        oldX = newX;
        oldY = newY;
        oldExit = tileGetExit(
                    &(b->tp->t[b->b[newX][newY].tileIndex]),
                    newExit);
       renderBlitSurface(TI_RENDER_GAME_TRACK_OVERLAY_EXITS[oldExit][newExit] * TI_RENDER_SMALL_TILE_WIDTH,
                          player * TI_RENDER_SMALL_TILE_WIDTH,
                          TI_RENDER_BOARD_TILE_AREA_X + ((oldX-1) * TI_RENDER_SMALL_TILE_WIDTH),
                          TI_RENDER_BOARD_TILE_AREA_Y + ((oldY-1) * TI_RENDER_SMALL_TILE_WIDTH),
                          TI_RENDER_SMALL_TILE_WIDTH,
                          TI_RENDER_SMALL_TILE_WIDTH,
                          a->trackOverlays, display->screen);
        newType = boardFindNextTrackSection(b, oldX, oldY, oldExit,
                                            &newX, &newY, &newExit);
    }

    if(loopCatcher >= loopLimit)  /* Broke out from possible infinite loop */
    {
        perror("boardCalculateTrackScore: infinite loop caught");
        return;
    }

}

/****************************************************************************
* renderTrackOverlays - see tiRenderSDL.h for description
****************************************************************************/
void renderTrackOverlays(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    int counter;

    for(counter=0;counter< TI_BOARD_NUM_STATIONS;counter++)
    {
        renderTrackOverlay(display, a, data, counter);
    }

    SDL_UpdateRect(display->screen,
                   TI_RENDER_BOARD_TILE_AREA_X,
                   TI_RENDER_BOARD_TILE_AREA_Y,
                   TI_RENDER_BOARD_TILE_AREA_MAX_X - TI_RENDER_BOARD_TILE_AREA_X,
                   TI_RENDER_BOARD_TILE_AREA_MAX_Y - TI_RENDER_BOARD_TILE_AREA_Y);
}

/****************************************************************************
* renderPlayerScore - see tiRenderSDL.h for description
****************************************************************************/
void renderPlayerScore(TiScreen *display, TiAssets *a, TiSharedData *data, int player)
{
    Game *g;
    int blitX;
    int blitY;
    int score;
    int numDigits;
    int digit;
    int counter, offset;

    g = gameGetGlobalGameInstance();

    /* Start with the most significant digit, and work toward the least */
    blitX = TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[player][0];
    blitY = TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[player][1];

    /* Get a copy of the score */
    score = g->players[player].score;

    /* The score should never be this high.  If it is, just don't draw it */
    if(score > 9999)
    {
        return;
    }
    else if(score > 999 && score <= 9999)
    {
        numDigits = 4;
    }
    else if(score > 99 && score <= 999)
    {
        numDigits = 3;
    }
    else if(score > 9 && score <= 99)
    {
        numDigits = 2;
    }
    else
    {
        numDigits = 1;
    }

    renderBlitSurface(0, 0, TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[player][0],
                      TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[player][1],
                      TI_RENDER_GAME_SCORE_BACKING_WIDTH, TI_RENDER_GAME_SCORE_BACKING_HEIGHT,
                      a->scoreBacking, display->screen);

    for(counter=numDigits-1;counter>=0;counter--)
    {
        digit = score / pow(10, counter);
        score = score - (digit * pow(10, counter));
        offset = digit * TI_RENDER_GAME_DIGIT_OFFSET;
        renderBlitSurface(offset, 0, blitX, blitY, TI_RENDER_GAME_DIGIT_WIDTHS[digit],
                          TI_RENDER_GAME_DIGIT_HEIGHT, a->digits, display->screen);
        blitX += (TI_RENDER_GAME_DIGIT_WIDTHS[digit] + TI_RENDER_GAME_LETTER_SPACING);
    }

    SDL_UpdateRect(display->screen, TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[player][0],
                   TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[player][1],
                   TI_RENDER_GAME_SCORE_BACKING_WIDTH, TI_RENDER_GAME_SCORE_BACKING_HEIGHT);

    return;
}

/****************************************************************************
* renderTrainsAndStations - see tiRenderSDL.h for description
****************************************************************************/
void renderTrainsAndStations(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    int counter;
    int stationX, stationY, exit, player;
    Game *g;

    g = gameGetGlobalGameInstance();

    for(counter=0;counter<TI_BOARD_NUM_STATIONS;counter++)
    {
        /* If a train is present, draw the train.  If not, draw the station */
        boardGetStationInfo(counter, &stationX, &stationY, &exit);
        if(g->board->b[stationX][stationY].trainPresent == TI_BOARD_NO_TRAIN)
        {
            renderBlitSurface(TI_RENDER_GAME_STATION_OFFSETS[counter][2] * TI_RENDER_STATION_WIDTH, 0,
                              TI_RENDER_GAME_STATION_OFFSETS[counter][0],
                              TI_RENDER_GAME_STATION_OFFSETS[counter][1],
                              TI_RENDER_STATION_WIDTH, TI_RENDER_STATION_HEIGHT,
                              a->stations, display->screen);
        }
        else
        {
            player = g->board->b[stationX][stationY].trainPresent - 1;
            renderBlitSurface(TI_RENDER_GAME_STATION_TRAIN_OFFSETS[counter][4] * TI_RENDER_TRAIN_TILE_WIDTH,
                              player * TI_RENDER_TRAIN_TILE_HEIGHT,
                              TI_RENDER_GAME_STATION_TRAIN_OFFSETS[counter][0],
                              TI_RENDER_GAME_STATION_TRAIN_OFFSETS[counter][1],
                              TI_RENDER_GAME_STATION_TRAIN_OFFSETS[counter][2],
                              TI_RENDER_GAME_STATION_TRAIN_OFFSETS[counter][3],
                              a->trains, display->screen);
        }
    }

    SDL_UpdateRect(display->screen, TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y,
                   TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT);
}

/****************************************************************************
* renderRestoreBoardBacking - see tiRenderSDL.h for description
****************************************************************************/
void renderRestoreBoardBacking(TiScreen *display, TiAssets *a, TiSharedData *data, int refreshScreen)
{
    renderBlitSurface(0, 0, TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y,
                      TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT,
                      data->boardAreaBacking, display->screen);
    if(refreshScreen == TI_TRUE)
    {
        SDL_UpdateRect(display->screen,
                       TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y,
                       TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT);
    }
}

/****************************************************************************
* renderDrawCurrentPlayerHighlight - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawCurrentPlayerHighlight(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    Uint32 rmask, gmask, bmask, amask;
    Game *g;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    g = gameGetGlobalGameInstance();

    /* Restore the original player backing */
    if(data->currentPlayerBacking != NULL)
    {
        renderBlitSurface(0, 0, 
                          TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][0],
                          TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][1],
                          TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][2],
                          TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][3],
                          data->currentPlayerBacking, display->screen);
        SDL_UpdateRect(display->screen,
                       TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][0],
                       TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][1],
                       TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][2],
                       TI_RENDER_GAME_CUR_PLAYER_OFFSETS[data->currentPlayerBackingPlayerNum][3]);
        SDL_FreeSurface(data->currentPlayerBacking);
        data->currentPlayerBacking = NULL;
    }

    /* Grab the new backing data */
    data->currentPlayerBacking = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][2],
                                                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][3],
                                                      32, rmask, gmask, bmask, amask);
    renderBlitSurface(TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][0],
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][1],
                      0, 0,
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][2],
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][3],
                      display->screen, data->currentPlayerBacking);

    /* Draw the player marker */
    renderBlitSurface(0, 0, 
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][0],
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][1],
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][2],
                      TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][3],
                      a->currentPlayer, display->screen);
    SDL_UpdateRect(display->screen,
                   TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][0],
                   TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][1],
                   TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][2],
                   TI_RENDER_GAME_CUR_PLAYER_OFFSETS[g->curPlayer][3]);

    data->currentPlayerBackingPlayerNum = g->curPlayer;
}

/****************************************************************************
* renderDrawCurrentTileHighlight - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawCurrentTileHighlight(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    Uint32 rmask, gmask, bmask, amask;
    Game *g;
    int x, y, w, h;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    g = gameGetGlobalGameInstance();

    /* Restore the original tile backing */
    if(data->currentTileBacking != NULL)
    {
        renderBlitSurface(0, 0, data->currentTileBackingX, data->currentTileBackingY,
                          data->currentTileBackingW, data->currentTileBackingH,
                          data->currentTileBacking, display->screen);
        SDL_UpdateRect(display->screen,
                       data->currentTileBackingX, data->currentTileBackingY,
                       data->currentTileBackingW, data->currentTileBackingH);
        SDL_FreeSurface(data->currentTileBacking);
        data->currentTileBacking = NULL;
    }

    if(g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE ||
       g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
    {
        /* Grab the new backing data */
        if(g->selectedMoveIsReserveTile == TI_TRUE)
        {
            x = TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[g->curPlayer][0];
            y = TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[g->curPlayer][1];
            w = TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[g->curPlayer][2];
            h = TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[g->curPlayer][3];
        }
        else
        {
            x = TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[g->curPlayer][0];
            y = TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[g->curPlayer][1];
            w = TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[g->curPlayer][2];
            h = TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[g->curPlayer][3];
        }

        data->currentTileBacking = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,
                                                        32, rmask, gmask, bmask, amask);
        renderBlitSurface(x, y, 0, 0, w, h, display->screen, data->currentTileBacking);

        /* Draw the highlight */
        renderBlitSurface(0, 0, x, y, w, h, a->currentTile, display->screen);
        SDL_UpdateRect(display->screen, x, y, w, h);

        data->currentTileBackingX = x;
        data->currentTileBackingY = y;
        data->currentTileBackingW = w;
        data->currentTileBackingH = h;
    }
}

/****************************************************************************
* renderDrawTile - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawTile(TiScreen *display, TiAssets *a, TiSharedData *data, int x, int y, int tile)
{
    Game *g;

    g = gameGetGlobalGameInstance();

    if(x< 1 || y < 1)
    {
        return;
    }

    renderBlitSurface(tilePoolGetTileIndexForTileId(g->tilepool, tile) * TI_RENDER_SMALL_TILE_WIDTH,
                      0, (TI_RENDER_BOARD_TILE_AREA_X + ((x - 1) * TI_RENDER_SMALL_TILE_WIDTH)),
                      (TI_RENDER_BOARD_TILE_AREA_Y + ((y - 1) * TI_RENDER_SMALL_TILE_WIDTH)),
                      TI_RENDER_SMALL_TILE_WIDTH, TI_RENDER_SMALL_TILE_WIDTH,
                      a->tileStripSmall, display->screen);
    SDL_UpdateRect(display->screen,
                   (TI_RENDER_BOARD_TILE_AREA_X + ((x - 1) * TI_RENDER_SMALL_TILE_WIDTH)),
                   (TI_RENDER_BOARD_TILE_AREA_Y + ((y - 1) * TI_RENDER_SMALL_TILE_WIDTH)),
                   TI_RENDER_SMALL_TILE_WIDTH, TI_RENDER_SMALL_TILE_WIDTH);
}

/****************************************************************************
* renderDrawTileOnBoard - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawTileOnBoard(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    Game *g;

    g = gameGetGlobalGameInstance();
    renderDrawTile(display, a, data, g->selectedMoveTileX, g->selectedMoveTileY, g->selectedMoveTileId);
}

/****************************************************************************
* renderInGameScreen - see tiRenderSDL.h for description
****************************************************************************/
void renderInGameScreen(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    int counter, counter2, counter3, offset, stationOffset, renderOffset;
    int lastPlayer;
    Game *g;

    /* Get a pointer to the global game instance.  Yuck. */
    g = gameGetGlobalGameInstance();

    /* If a background refresh was requested, do it.  Refreshing the background
    forces a refresh of the other screen components */
    if(data->refreshBG == TI_TRUE)
    {
        renderApplySurface(0, 0, a->gameplayBG, display->screen);

        for(counter=0;counter<TI_MAX_PLAYERS;counter++)
        {
            if(g->players[counter].controlledBy != TI_PLAYER_NOBODY)
            {
                /* Render player color swatches for each active player */
                renderBlitSurface(counter * TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[counter][2], 0,
                                  TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[counter][3],
                                  a->playerColors, display->screen);
                /* Render lighter background for each active player */
                renderBlitSurface(0, 0,
                                  TI_RENDER_GAME_PLAYER_BG_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_BG_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_BG_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_BG_OFFSETS[counter][3],
                                  a->playerLightBG, display->screen);
                /* Render station chart for each active player */
                renderBlitSurface(0, 0,
                                  TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][3],
                                  a->playerStationsChart, display->screen);
                /* Render player number/type/score string text for each active player */
                renderBlitSurface(0, counter * TI_RENDER_GAME_PLAYER_NAME_OFFSETS[counter][3],
                                  TI_RENDER_GAME_PLAYER_NAME_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_NAME_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_NAME_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_NAME_OFFSETS[counter][3],
                                  a->playerStrings, display->screen);
                /* Offset to 'human' or 'computer' depending on the player type */
                offset = (g->players[counter].controlledBy == TI_PLAYER_HUMAN) ? 0 : 1;
                renderBlitSurface(0, offset * TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[counter][3],
                                  TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[counter][3],
                                  a->playerTypeStrings, display->screen);
                renderBlitSurface(0, 0,
                                  TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS[counter][3],
                                  a->playerScoreString, display->screen);
            }
        }

        SDL_UpdateRect(display->screen, 0, 0, 0, 0);
        data->refreshBG = TI_FALSE;
        data->refreshPlayerTiles = TI_TRUE;
        data->refreshPlayerStations = TI_TRUE;
    }

    /* If a request to update the player active/reserve tiles was made, do it */
    if(data->refreshPlayerTiles == TI_TRUE)
    {
        /* Mark the current and reserve tiles for each player */
        for(counter=0;counter<TI_MAX_PLAYERS;counter++)
        {
            if(g->players[counter].controlledBy != TI_PLAYER_NOBODY)
            {
                /* If processing the current player, and that player is human, show the actual tiles.
                   Otherwise, show a generic 'holding tile' graphic for held tiles, and the tile 
                   back if a tile isn't held */
                if(counter == g->curPlayer && g->players[counter].controlledBy == TI_PLAYER_HUMAN)
                {
                    offset = (g->players[counter].currentTileId == TI_TILE_NO_TILE) ? 0 :
                              tilePoolGetTileIndexForTileId(g->tilepool, g->players[counter].currentTileId);
                }
                else
                {
                    offset = (g->players[counter].currentTileId == TI_TILE_NO_TILE) ? 0 : TI_RENDER_TILE_BACK_OFFSET;
                }

                renderBlitSurface(offset * TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][2], 0,
                                  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][3],
                                  a->tileStripSmall, display->screen);

                /* If processing the current player, and that player is human, show the actual tiles.
                   Otherwise, show a generic 'holding tile' graphic for held tiles, and the tile 
                   back if a tile isn't held */
                if(counter == g->curPlayer && g->players[counter].controlledBy == TI_PLAYER_HUMAN)
                {
                    offset = (g->players[counter].reserveTileId == TI_TILE_NO_TILE) ? 0 :
                              tilePoolGetTileIndexForTileId(g->tilepool, g->players[counter].reserveTileId);
                }
                else
                {
                    offset = (g->players[counter].reserveTileId == TI_TILE_NO_TILE) ? 0 : TI_RENDER_TILE_BACK_OFFSET;
                }

                renderBlitSurface(offset * TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][2], 0,
                                  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][0],
                                  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][1],
                                  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][2],
                                  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][3],
                                  a->tileStripSmall, display->screen);

                /* Make sure the changes make it onto the screen */
                SDL_UpdateRect(display->screen,
                               TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][0],
                               TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][1],
                               TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][2],
                               TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[counter][3]);
                SDL_UpdateRect(display->screen,
                               TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][0],
                               TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][1],
                               TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][2],
                               TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[counter][3]);
            }
        }
        data->refreshPlayerTiles = TI_FALSE;
    }

    /* If a request to refresh the player station status is made, do it.  This is always
    displayed for all players regardless of the player type.  This request will
    refresh station status for all players, even if it isn't needed, which is
    inefficient.  Plus, the code is ugly (could I nest loops any deeper?).  Meh.
    */
    if(data->refreshPlayerStations == TI_TRUE)
    {
        for(counter=0;counter<TI_MAX_PLAYERS;counter++)
        {
            if(g->players[counter].controlledBy != TI_PLAYER_NOBODY)
            {
                /* The stations are divided over multiple rows.  In the current game version,
                the 16 maximum possible player stations (in a 2 player game) are
                displayed in 2 rows of 8 each.  The nested loops are used to make sure
                the status block is drawn in the correct place
                */
                for(counter2=0;counter2<TI_RENDER_NUM_STATION_ROWS;counter2++)
                {
                    for(counter3=0;counter3<TI_RENDER_NUM_STATIONS_PER_ROW;counter3++)
                    {
                        stationOffset = ((counter2 * TI_RENDER_NUM_STATIONS_PER_ROW) + counter3);
                        if(stationOffset < g->numStationsPerPlayer)
                        {
                            /* Incomplete stations */
                            if(stationOffset >= g->players[counter].numStationsComplete)
                            {
                                renderOffset = TI_RENDER_STATION_STATUS_INCOMPLETE *
                                               TI_RENDER_STATION_STATUS_WIDTH;
                            }
                            /* Complete stations */
                            else
                            {
                                renderOffset = TI_RENDER_STATION_STATUS_COMPLETE *
                                               TI_RENDER_STATION_STATUS_WIDTH;
                            }
                        }
                        else
                        {
                            /* Stations not used in the game */
                            renderOffset = TI_RENDER_STATION_STATUS_NA *
                                        TI_RENDER_STATION_STATUS_WIDTH;
                        }
                        renderBlitSurface(renderOffset,0,
                                         (TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][0]) +
                                         (counter3 * (TI_RENDER_STATION_STATUS_WIDTH + 1)) +
                                         TI_RENDER_STATION_INITIAL_XOFF,
                                         (TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][1]) +
                                         (counter2 * (TI_RENDER_STATION_STATUS_HEIGHT + 1)) +
                                         TI_RENDER_STATION_INITIAL_YOFF,
                                         TI_RENDER_STATION_STATUS_WIDTH,
                                         TI_RENDER_STATION_STATUS_HEIGHT,
                                         a->stationStatus, display->screen);
                    }
                }
                SDL_UpdateRect(display->screen,
                               TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][0],
                               TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][1],
                               TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][2],
                               TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[counter][3]);
            }
        }//
        /* Mark the finished/unfinished stations for each player */
        data->refreshPlayerStations = TI_FALSE;
    }

    if(data->refreshPlayerScores == TI_TRUE)
    {
        for(counter=0;counter<TI_MAX_PLAYERS;counter++)
        {
            if(g->players[counter].controlledBy != TI_PLAYER_NOBODY)
            {
                renderPlayerScore(display, a, data, counter);
            }
        }
        data->refreshPlayerScores = TI_FALSE;
    }

    if(data->restoreBoardArea == TI_TRUE && data->boardAreaBacking != NULL)
    {
        renderRestoreBoardBacking(display, a, data, TI_TRUE);
        SDL_FreeSurface(data->boardAreaBacking);
        data->boardAreaBacking = NULL;
        data->restoreBoardArea = TI_FALSE;
    }

    if(data->refreshPlayerTrains == TI_TRUE)
    {
        renderTrainsAndStations(display, a, data);
        data->refreshPlayerTrains = TI_FALSE;
    }

    if(data->refreshTrackOverlays == TI_TRUE)
    {
        renderTrackOverlays(display, a, data);
        data->refreshTrackOverlays = TI_FALSE;
    }

    if(data->drawTileOnBoard == TI_TRUE)
    {
        /* 'Undraw' the last player's move if the 'Last Player' option was selected */
        renderUndrawPreviousMove(display, a, data);
        /* 'Undraw' the current player's move */
        renderDrawTile(display, a, data , g->players[g->curPlayer].lastMoveX,
                       g->players[g->curPlayer].lastMoveY,
                       g->board->b[g->players[g->curPlayer].lastMoveX][g->players[g->curPlayer].lastMoveY].tileIndex);
        g->players[g->curPlayer].lastMoveX = g->selectedMoveTileX;
        g->players[g->curPlayer].lastMoveY = g->selectedMoveTileY;
        renderDrawTileOnBoard(display, a, data);
        data->drawTileOnBoard = TI_FALSE;
        renderLastMoves(display, a, data);
        /* This shouldn't be here.  There's no event to latch to that otherwise signals this state change */
        g->selectedMoveTileX = -1;
        g->selectedMoveTileY = -1;
        g->selectedMoveTileId = TI_TILE_NO_TILE;
        gameSetGameState(g, TI_GAME_STATE_END_TURN);
    }

}

void renderUndrawPreviousMove(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    int lastPlayer;
    Game *g;

    g = gameGetGlobalGameInstance();

   /* 'Undraw' the last player's move if the user has set 'Show Player moves' to 'Last Player' */
   if(g->showLastMove == TI_GAME_OPTIONS_LAST_PLAYER)
   {
       lastPlayer = g->curPlayer - 1;
       if(lastPlayer < 0)
       {
           lastPlayer = g->numPlayers -1;
       }

       if(g->players[lastPlayer].lastMoveX != TI_PLAYER_INVALID_LAST_MOVE &&
          g->players[lastPlayer].lastMoveY != TI_PLAYER_INVALID_LAST_MOVE)
       {
          renderDrawTile(display, a, data , g->players[lastPlayer].lastMoveX,
                         g->players[lastPlayer].lastMoveY,
                         g->board->b[g->players[lastPlayer].lastMoveX][g->players[lastPlayer].lastMoveY].tileIndex);
       }
   }
}
/****************************************************************************
* renderTransitionSurface - see tiRenderSDL.h for description
****************************************************************************/
void renderTransitionSurface(TiScreen *display, TiSharedData *data,
                             SDL_Surface *surf, int x, int y, int w, int h,
                             int fromLeft)
{
    float *xVals;
    float *accels;
    int   *finished;

    int counter, counter2, numFinished;
    float minAccel = TI_RENDER_TRANSITION_MIN_ACCEL;
    float maxAccel = TI_RENDER_TRANSITION_MAX_ACCEL;

    xVals = malloc(h * sizeof(float));
    accels = malloc(h * sizeof(float));
    finished = malloc(h * sizeof(int));

    if(xVals == NULL || accels == NULL || finished == NULL)
    {
        perror("Out of memory!\n");
        exit(-1);
    }

    /* Speed things up on larger dialog boxes */
    if(w > 500 || h > 200)
    {
        minAccel *= 3.0;
        maxAccel *= 3.0;
    }

    numFinished=0;

    for(counter=0; counter<h; counter++)
    {
        if(fromLeft == TI_TRUE)
        {
            xVals[counter] = x;
        }
        else
        {
            xVals[counter] = (x + w - 1);
        }
        accels[counter] = rand() * ((maxAccel - minAccel) / RAND_MAX) + minAccel;
        finished[counter] = TI_FALSE;
    }

    while(numFinished < h)
    {
        counter2=0;
        for(counter=0;counter<h;counter++)
        {
            if(finished[counter] == TI_FALSE)
            {
                renderBlitSurface(xVals[counter]-x, counter, xVals[counter], y + counter,
                                  accels[counter]+1, 1, surf, display->screen);
                xVals[counter] = xVals[counter] + accels[counter];
                if(xVals[counter] >= x+w)
                {
                    finished[counter] = TI_TRUE;
                    numFinished++;
                }
            } 
        }
        SDL_UpdateRect(display->screen, x, y, w, h);
    }

    free(xVals);
    free(accels);
    free(finished);
}

/****************************************************************************
* renderRestoreDisplayBacking - see tiRenderSDL.h for description
****************************************************************************/
void renderRestoreDisplayBacking(TiScreen *display, TiAssets *a,
                                 TiSharedData *data)
{

    if(data->dialogBacking != NULL)
    {
        renderTransitionSurface(display, data, data->dialogBacking, 
                                data->dialogBackingX, data->dialogBackingY,
                                data->dialogBackingW, data->dialogBackingH,
                                TI_TRUE);
        SDL_FreeSurface(data->dialogBacking);
        data->dialogBacking = NULL;
    }
}

/****************************************************************************
* renderDisplayDialog - see tiRenderSDL.h for description
****************************************************************************/
int renderDisplayDialog(TiScreen *display, SDL_Surface *dialog, TiSharedData *data,
                         int x, int y, int w, int h)
{

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    if(data->dialogBacking != NULL)
    {
        SDL_FreeSurface(data->dialogBacking);
        data->dialogBacking = NULL;
    }

    /* Grab the contents behind the dialog */
    data->dialogBacking = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                               rmask, gmask, bmask, amask);
    if(data->dialogBacking == NULL)
    {
        return TI_ERROR;
    }

    renderBlitSurface(x, y, 0, 0, w, h, display->screen, data->dialogBacking);
    SDL_UpdateRect(data->dialogBacking, 0, 0, w, h);

    /* Blit the dialog to the screen */
    renderTransitionSurface(display, data, dialog, x, y, w, h, TI_TRUE);

    data->dialogBackingX = x;
    data->dialogBackingY = y;
    data->dialogBackingW = w;
    data->dialogBackingH = h;

    return TI_OK;
}

/****************************************************************************
* renderDrawDarkenMasks- see tiRenderSDL.h for description
****************************************************************************/
void renderDrawDarkenMasks(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    Game *g;
    int  counter, counter2;
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    g = gameGetGlobalGameInstance();

    if(data->drawDarkenMask == TI_TRUE)
    {
        if(data->boardAreaBacking != NULL)
        {
            SDL_FreeSurface(data->boardAreaBacking);
        }
        data->boardAreaBacking = SDL_CreateRGBSurface(SDL_SWSURFACE, TI_RENDER_BOARD_AREA_WIDTH, 
                                                      TI_RENDER_BOARD_AREA_HEIGHT, 32,
                                                      rmask, gmask, bmask, amask);
        if(data->boardAreaBacking == NULL)
        {
            perror("Unable to allocated surface!");
            exit(0);
        }

        renderBlitSurface(TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y, 0, 0,
                          TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT,
                          display->screen, data->boardAreaBacking);
        /* Now blit the darkening mask */
        renderBlitSurface(0, 0, TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y,
                          TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT,
                          a->darkenMask, display->screen);
        /* Avoid a second redraw for the move masks by only updating the screen if invalid 
           move masks are not to be redrawn */
        if(data->drawValidMoveMasks != TI_TRUE)
        {
            SDL_UpdateRect(display->screen,
                           TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y,
                           TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT);
            data->drawDarkenMask = TI_FALSE;
        }
    }

    /* If the player doesn't want legal moves drawn, then skip this part */
    if(g->highlightLegalMoves == TI_GAME_OPTIONS_YES)
    {
        /* Mark the legal moves */
        if(data->drawValidMoveMasks == TI_TRUE)
        {
            for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
            {
                for(counter2=1;counter2<TI_BOARD_HEIGHT-1;counter2++)
                {
                    if(boardIsLegalMove(g->board, counter, counter2) == TI_TRUE)
                    {
                        renderBlitSurface(0, 0,
                                          TI_RENDER_BOARD_TILE_AREA_X + ((counter-1)* TI_RENDER_SMALL_TILE_WIDTH),
                                          TI_RENDER_BOARD_TILE_AREA_Y + ((counter2-1) * TI_RENDER_SMALL_TILE_WIDTH),
                                          TI_RENDER_SMALL_TILE_WIDTH,
                                          TI_RENDER_SMALL_TILE_WIDTH,
                                          a->validMoveMask, display->screen);
                    }
                }
            }
            SDL_UpdateRect(display->screen,
                           TI_RENDER_BOARD_AREA_X, TI_RENDER_BOARD_AREA_Y,
                           TI_RENDER_BOARD_AREA_WIDTH, TI_RENDER_BOARD_AREA_HEIGHT);
            data->drawValidMoveMasks = TI_FALSE;
        }
    }
}

/****************************************************************************
* renderDrawPlayCancelButton - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawPlayCancelButton(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    renderBlitSurface(0, 0, TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X,
                      TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y,
                      TI_RENDER_PLAY_CANCEL_BUTTON_MAX_X - TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X,
                      TI_RENDER_PLAY_CANCEL_BUTTON_MAX_Y - TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y,
                      a->cancelPlayTileButton, display->screen);
    SDL_UpdateRect(display->screen,
                   TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X,
                   TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y,
                   TI_RENDER_PLAY_CANCEL_BUTTON_MAX_X - TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X,
                   TI_RENDER_PLAY_CANCEL_BUTTON_MAX_Y - TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y);
}

/****************************************************************************
* renderDrawGameStateSpecificDialogs - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawGameStateSpecificDialogs(TiScreen *display, TiAssets *a,
                                        TiSharedData *data)
{
    Game *g;
    int  counter, lastPlayer;
    int skipPlayTile = TI_TRUE;

    /* Ugh.  Again. */
    g = gameGetGlobalGameInstance();

    if(g->gameStateChanged == TI_FALSE)
    {
        /* Do nothing */
        return;
    }

    /* Restore the hidden screen bits from the previous state */
    renderRestoreDisplayBacking(display, a, data);

    /* Holy crap!  This is an ugly hack.  if restoring the backing from a 'discard' operation,
       and if the user is in 'Last Player' mode, delete all player highlights */
    if(g->deleteLastPlayerHighlight == TI_TRUE && g->showLastMove == TI_GAME_OPTIONS_LAST_PLAYER)
    {
        for(counter=0;counter<g->numPlayers;counter++)
        {
            renderDrawTile(display, a, data , g->players[counter].lastMoveX,
                           g->players[counter].lastMoveY,
                           g->board->b[g->players[counter].lastMoveX][g->players[counter].lastMoveY].tileIndex);
        }
        g->deleteLastPlayerHighlight = TI_FALSE;
    }

    /* Ugly, ugly hack alert!!!  The player tiles get refreshed here twice.  The first time,
       the tiles are drawn, then the backing from the previous highlighted tile is restored.
       This gets rid of the border from where it shouldn't be, but places the other contents
       of the backing here, which might not be correct.  Therefore, the tiles are redrawn
       again, making sure the tile displayed is correct.

       If the first redraw is omitted, when selecting between the two tiles in the player's
       hand, the second tile becomes blank.

       If the second redraw is omitted, when the second tile is played, it remains displayed
       in the player's hand.
    */

    /* START UGLY HACK */
    data->refreshPlayerTiles = TI_TRUE;
    data->refreshBG = TI_FALSE;
    data->refreshPlayersList = TI_FALSE;
    data->refreshPlayerStations = TI_TRUE;
    data->refreshPlayerScores = TI_TRUE;
    data->refreshPlayerTrains = TI_TRUE;
    data->refreshTrackOverlays = TI_TRUE;
    renderInGameScreen(display, a, data);

    if(g->gameState == TI_GAME_STATE_SELECT_ACTION ||
       g->previousGameState == TI_GAME_STATE_CONFIRM_EXIT)
    {
        renderDrawCurrentTileHighlight(display, a, data);
    }

    data->refreshPlayerTiles = TI_TRUE;
    renderInGameScreen(display, a, data);
    /* END UGLY HACK */

    /* Display the things specific to this state.  Save the contents
       of the screen hidden behind the dialog being displayed */
    switch(g->gameState)
    {
        case TI_GAME_STATE_SELECT_ACTION:
            /* Render the 'base' dialog */
            renderDisplayDialog(display, a->selectActionDialog, data, 
                                TI_RENDER_GAME_SELECT_ACTION_OFFSET[0],
                                TI_RENDER_GAME_SELECT_ACTION_OFFSET[1],
                                TI_RENDER_GAME_SELECT_ACTION_OFFSET[2],
                                TI_RENDER_GAME_SELECT_ACTION_OFFSET[3]);
            /* On top of it, display the valid options */
            /* Display play option (if required) */ 
            if(g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE || 
               g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
            {
                if(g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE && boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->players[g->curPlayer].currentTileId)) > 0)
                {
                    skipPlayTile = TI_FALSE;
                }
                if(g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE && boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->players[g->curPlayer].reserveTileId)) > 0)
                {
                    skipPlayTile = TI_FALSE;
                }

                if(skipPlayTile == TI_FALSE)
                {
                    renderBlitSurface(0, 0, TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][0],
                                      TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][1],
                                      TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][2],
                                      TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][3],
                                      a->selectPlayActive, display->screen);
                    SDL_UpdateRect(display->screen, 
                                   TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][0],
                                   TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][1],
                                   TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][2],
                                   TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PLAY][3]);
                    g->playIsValid = TI_TRUE;
                }
            }
            else
            {
                g->playIsValid = TI_FALSE;
            }
            /* Display draw option (if required) */
            if(g->playerHasDrawn == TI_FALSE && g->tilepool->numUnplayedTiles > 0)
            {
                renderBlitSurface(0, 0, TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][0],
                                  TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][1],
                                  TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][2],
                                  TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][3],
                                  a->selectDrawActive, display->screen);
                SDL_UpdateRect(display->screen, 
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][0],
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][1],
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][2],
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DRAW][3]);
                g->drawIsValid = TI_TRUE;
            }
            else 
            {
                g->drawIsValid = TI_FALSE;
            }
            /* Display discard option (if required) */
            if(g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE ||
               g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
            {
                renderBlitSurface(0, 0, TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][0],
                                  TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][1],
                                  TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][2],
                                  TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][3],
                                  a->selectDiscardActive, display->screen);
                SDL_UpdateRect(display->screen, 
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][0],
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][1],
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][2],
                               TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_DISCARD][3]);
                g->discardIsValid = TI_TRUE;
            }
            else 
            {
                g->discardIsValid = TI_FALSE;
            }


            /* Display pass option (always allowed) */
            renderBlitSurface(0, 0, TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][0],
                              TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][1],
                              TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][2],
                              TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][3],
                              a->selectPassActive, display->screen);
            SDL_UpdateRect(display->screen, 
                           TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][0],
                           TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][1],
                           TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][2],
                           TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[TI_OFFSET_PASS][3]);
            g->passIsValid = TI_TRUE;

            break;
        case TI_GAME_STATE_TILE_DRAW:
            /* Draw the 'Draw Tile' dialog */
            renderDisplayDialog(display, a->selectTileDialog, data, 
                                TI_RENDER_GAME_DRAW_TILE_OFFSET[0],
                                TI_RENDER_GAME_DRAW_TILE_OFFSET[1],
                                TI_RENDER_GAME_DRAW_TILE_OFFSET[2],
                                TI_RENDER_GAME_DRAW_TILE_OFFSET[3]);
            /* Display enough tiles to fill the box, unless there are less tiles left
               than would be necessary to do this; in that case, just draw the number
               of tiles remaining. */
            for(counter=0; counter<g->numDrawTilesToDisplay; counter++)
            {
                renderBlitSurface(TI_RENDER_TILE_BACK_OFFSET * TI_RENDER_LARGE_TILE_WIDTH, 0, 
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][0],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][1],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][2],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][3],
                                  a->tileStripLarge, display->screen);
                SDL_UpdateRect(display->screen, 
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][0],
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][1],
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][2],
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][3]);
            }
            break;
        case TI_GAME_STATE_TILE_SELECT:
            renderDrawDarkenMasks(display, a, data);
            renderDrawPlayCancelButton(display, a, data);
            break;
        case TI_GAME_STATE_DISCARD:
            renderDisplayDialog(display, a->chooseDiscardDialog, data, 
                                TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET[0],
                                TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET[1],
                                TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET[2],
                                TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET[3]);
            break;
        case TI_GAME_STATE_COMPUTER_MOVE:
            renderDisplayDialog(display, a->computerThinkingDialog, data,
                                TI_RENDER_GAME_CPU_THINKING_OFFSET[0],
                                TI_RENDER_GAME_CPU_THINKING_OFFSET[1],
                                TI_RENDER_GAME_CPU_THINKING_OFFSET[2],
                                TI_RENDER_GAME_CPU_THINKING_OFFSET[3]);
            break;
        case TI_GAME_STATE_GAME_FINISHED:
            renderDisplayDialog(display, a->gameFinishedDialog, data,
                                TI_RENDER_GAME_FINISHED_OFFSETS[0],
                                TI_RENDER_GAME_FINISHED_OFFSETS[1],
                                TI_RENDER_GAME_FINISHED_OFFSETS[2],
                                TI_RENDER_GAME_FINISHED_OFFSETS[3]);
            break;
        case TI_GAME_STATE_CONFIRM_EXIT:
            renderDisplayDialog(display, a->confirmExitDialog, data, 
                                TI_RENDER_GAME_CONFIRM_EXIT_OFFSET[0],
                                TI_RENDER_GAME_CONFIRM_EXIT_OFFSET[1],
                                TI_RENDER_GAME_CONFIRM_EXIT_OFFSET[2],
                                TI_RENDER_GAME_CONFIRM_EXIT_OFFSET[3]);
            break;
        default:
            break;
    }

    if(g->gameState == TI_GAME_STATE_SELECT_ACTION)
    {
        renderDrawCurrentPlayerHighlight(display, a, data);
    }

    g->gameStateChanged = TI_FALSE;

    return;
}

/****************************************************************************
* renderDrawGameStateDynamicElements - see tiRenderSDL.h for description
****************************************************************************/
void renderDrawGameStateDynamicElements(TiScreen *display, TiAssets *a,
                                        TiSharedData *data)
{
    Game *g;
    int  counter;

    /* Ugh.  Again. */
    g = gameGetGlobalGameInstance();

    switch(g->gameState)
    {
        case TI_GAME_STATE_SELECT_ACTION:
            break;
        case TI_GAME_STATE_TILE_DRAW:
            /* Erase the old highlight and draw the highlight around the specified tile if necessary */
            if(data->refreshDrawTileHighlighted == TI_TRUE)
            {
                /* Redraw the area around the old tile.  This is a hack.  Essentially, the code just
                   redraws the part of the dialog that was surrounded by the highlight, and places a
                   new blank tile on top of it.  A smarter function would just erase the parts that 
                   the highlight touched. 
                */
                renderBlitSurface(TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][0] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET - TI_RENDER_GAME_DRAW_TILE_OFFSET[0],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][1] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET - TI_RENDER_GAME_DRAW_TILE_OFFSET[1],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][0] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][1] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][2] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET),
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][3] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET),
                                  a->selectTileDialog, display->screen);
                renderBlitSurface(TI_RENDER_TILE_BACK_OFFSET * TI_RENDER_LARGE_TILE_WIDTH, 0, 
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][0],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][1],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][2],
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][3],
                                  a->tileStripLarge, display->screen);
                SDL_UpdateRect(display->screen, 
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][0] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][1] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][2] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET),
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->prevDrawTileHighlighted][3] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET));

                /* Draw the highlight around the new tile */
                renderBlitSurface(0,0, TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][0] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][1] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][2] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET),
                                  TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][3] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET),
                                  a->drawTileHighlightLarge, display->screen);
                SDL_UpdateRect(display->screen, 
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][0] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][1] - TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET,
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][2] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET),
                               TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[data->drawTileHighlighted][3] + (2*TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET));
                data->refreshDrawTileHighlighted = TI_FALSE;
            }
            break;
        default:
            break;
    }

    return;
}

/****************************************************************************
* renderResultsScoreDigits - see tiRenderSDL.h for description
****************************************************************************/
void renderResultsScoreDigits(TiScreen *display, TiAssets *a, TiSharedData *data, int rank, int score)
{
    int blitX;
    int blitY;
    int numDigits;
    int digit;
    int counter, offset;
    SDL_Surface *digitSurface;
    int tmpScore;

    if(rank == 0)
    {
        digitSurface = a->resultsLargeDigits;
    }
    else
    {
        digitSurface = a->resultsSmallDigits;
    }

    /* Start with the most significant digit, and work toward the least */
    blitX = TI_RENDER_GAME_RESULTS_SCORE_OFFSETS[rank][0];
    blitY = TI_RENDER_GAME_RESULTS_SCORE_OFFSETS[rank][1];

    tmpScore = score;

    /* The score should never be this high.  If it is, just don't draw it */
    if(tmpScore > 9999)
    {
        return;
    }
    else if(tmpScore > 999 && tmpScore <= 9999)
    {
        numDigits = 4;
    }
    else if(tmpScore > 99 && tmpScore <= 999)
    {
        numDigits = 3;
    }
    else if(tmpScore > 9 && tmpScore <= 99)
    {
        numDigits = 2;
    }
    else
    {
        numDigits = 1;
    }

    for(counter=numDigits-1;counter>=0;counter--)
    {
        digit = tmpScore / pow(10, counter);
        tmpScore = tmpScore - (digit * pow(10, counter));
        if(rank == 0)
        {
            offset = digit * TI_RENDER_GAME_RESULTS_LARGE_DIGIT_OFFSET ;
            renderBlitSurface(offset, 0, blitX, blitY, TI_RENDER_GAME_RESULTS_LARGE_DIGIT_WIDTHS[digit],
                              TI_RENDER_GAME_RESULTS_LARGE_DIGIT_HEIGHT, digitSurface, display->screen);
            blitX += (TI_RENDER_GAME_RESULTS_LARGE_DIGIT_WIDTHS[digit] + TI_RENDER_GAME_RESULTS_LARGE_DIGIT_SPACING);
        }
        else
        {
            offset = digit * TI_RENDER_GAME_RESULTS_SMALL_DIGIT_OFFSET ;
            renderBlitSurface(offset, 0, blitX, blitY, TI_RENDER_GAME_RESULTS_SMALL_DIGIT_WIDTHS[digit],
                              TI_RENDER_GAME_RESULTS_SMALL_DIGIT_HEIGHT, digitSurface, display->screen);
            blitX += (TI_RENDER_GAME_RESULTS_SMALL_DIGIT_WIDTHS[digit] + TI_RENDER_GAME_RESULTS_SMALL_DIGIT_SPACING);
        }
    }
}

/****************************************************************************
* renderResultsScores - see tiRenderSDL.h for description
****************************************************************************/
void renderResultsScores(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    int counter, counter2, temp;
    Game *g;
    int playerRanks[TI_MAX_PLAYERS];

    g = gameGetGlobalGameInstance();

    /* Sort the players from high score to low score */
    for(counter=0;counter<g->numPlayers;counter++)
    {
        playerRanks[counter] = counter;
    }
    for(counter=0;counter<g->numPlayers-1;counter++)
    {
        for(counter2=counter+1;counter2<g->numPlayers;counter2++)
        {
            if(g->players[playerRanks[counter2]].score > g->players[playerRanks[counter]].score)
            {
                temp = playerRanks[counter2];
                playerRanks[counter2] = playerRanks[counter];
                playerRanks[counter] = temp;
            }
        }
    }

    /* Display the highest ranking player at the top */
    renderBlitSurface(1 * TI_RENDER_GAME_RESULTS_LARGE_DIGIT_OFFSET, 0,
                      TI_RENDER_GAME_RESULTS_RANK_OFFSETS[0][0],
                      TI_RENDER_GAME_RESULTS_RANK_OFFSETS[0][1],
                      TI_RENDER_GAME_RESULTS_LARGE_DIGIT_WIDTHS[1],
                      TI_RENDER_GAME_RESULTS_LARGE_DIGIT_HEIGHT,
                      a->resultsLargeDigits, display->screen);
    renderBlitSurface(0, playerRanks[0] * TI_RENDER_GAME_RESULTS_LARGE_PLAYER_HEIGHT,
                      TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS[0][0],
                      TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS[0][1],
                      TI_RENDER_GAME_RESULTS_LARGE_PLAYER_WIDTH,
                      TI_RENDER_GAME_RESULTS_LARGE_PLAYER_HEIGHT,
                      a->resultsLargePlayers, display->screen);
    renderResultsScoreDigits(display, a, data, 0, g->players[playerRanks[0]].score);


    /* Display the remaining players below the top player */
    for(counter=1;counter<g->numPlayers;counter++)
    {
        renderBlitSurface((counter+1) * TI_RENDER_GAME_RESULTS_SMALL_DIGIT_OFFSET, 0,
                          TI_RENDER_GAME_RESULTS_RANK_OFFSETS[counter][0],
                          TI_RENDER_GAME_RESULTS_RANK_OFFSETS[counter][1],
                          TI_RENDER_GAME_RESULTS_SMALL_DIGIT_WIDTHS[(counter+1)],
                          TI_RENDER_GAME_RESULTS_SMALL_DIGIT_HEIGHT,
                          a->resultsSmallDigits, display->screen);
        renderBlitSurface(0, playerRanks[counter] * TI_RENDER_GAME_RESULTS_SMALL_PLAYER_HEIGHT,
                          TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS[counter][0],
                          TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS[counter][1],
                          TI_RENDER_GAME_RESULTS_SMALL_PLAYER_WIDTH,
                          TI_RENDER_GAME_RESULTS_SMALL_PLAYER_HEIGHT,
                          a->resultsSmallPlayers, display->screen);
        renderResultsScoreDigits(display, a, data, counter, g->players[playerRanks[counter]].score);
    }
}

/****************************************************************************
* renderResultsScreen - see tiRenderSDL.h for description
****************************************************************************/
void renderResultsScreen(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    /* Draw the background and scores once */
    if(data->refreshBG == TI_TRUE)
    {
        /* Background */
        renderApplySurface(0,0, a->resultsBackground, display->screen);

        /* Scores */
        renderResultsScores(display, a, data);

        SDL_UpdateRect(display->screen, 0, 0, 0, 0);
        data->refreshBG = TI_FALSE;
    }

    /* Draw the trains every frame*/
    renderDrawAnimatedTrains(display, a, data);
}

/****************************************************************************
* renderUpdateScreen - see tiRenderSDL.h for description
****************************************************************************/
void renderUpdateScreen(TiScreen *display, TiAssets *a, TiSharedData *data)
{
    switch(data->renderState)
    {
        case TI_STATE_COMPANY_LOGO:
            if(data->refreshBG == TI_TRUE)
            {
                renderApplySurface(0,0, a->logoScreenBG, display->screen);
                SDL_UpdateRect(display->screen, 0, 0, 0, 0);
                data->refreshBG = TI_FALSE;
            }
            break;
        case TI_STATE_TITLE_SCREEN:
            /* Draw the stuff common to the title screen */
            renderTitleScreenDrawBase(display, a, data);
            /* Draw either the tap screen message or the backing bitmap depending on
            the current frame */
            if((data->numFrames % TI_RENDER_FRAME_RATE) >= TI_RENDER_TAP_MSG_FRAME_SHOWN &&
            (data->numFrames % TI_RENDER_FRAME_RATE) <  TI_RENDER_TAP_MSG_FRAME_HIDDEN)
            {
                renderBlitSurface(0,0,TI_RENDER_TAP_MSG_X, TI_RENDER_TAP_MSG_Y,
                                TI_RENDER_TAP_MSG_WIDTH, TI_RENDER_TAP_MSG_HEIGHT,
                                a->tapScreenMsg, display->screen);
                SDL_UpdateRect(display->screen, TI_RENDER_TAP_MSG_X, TI_RENDER_TAP_MSG_Y,
                            TI_RENDER_TAP_MSG_WIDTH, TI_RENDER_TAP_MSG_HEIGHT);
            }
            if((data->numFrames % TI_RENDER_FRAME_RATE) >= TI_RENDER_TAP_MSG_FRAME_HIDDEN &&
            (data->numFrames % TI_RENDER_FRAME_RATE) < TI_RENDER_FRAME_RATE)
            {
                renderBlitSurface(TI_RENDER_TAP_MSG_X, TI_RENDER_TAP_MSG_Y,
                                TI_RENDER_TAP_MSG_X, TI_RENDER_TAP_MSG_Y,
                                TI_RENDER_TAP_MSG_WIDTH, TI_RENDER_TAP_MSG_HEIGHT,
                                a->titleScreenBG, display->screen);
                SDL_UpdateRect(display->screen, TI_RENDER_TAP_MSG_X, TI_RENDER_TAP_MSG_Y,
                                TI_RENDER_TAP_MSG_WIDTH, TI_RENDER_TAP_MSG_HEIGHT);
            }
            break;
        case TI_STATE_TITLE_MENU_SCREEN:
            /* Draw the stuff common to the title screen */
            renderTitleScreenDrawBase(display, a, data);
            /* Draw the menu screen */
            renderBlitSurface(0, 0, TI_RENDER_TITLE_MENU_X, TI_RENDER_TITLE_MENU_Y,
                            TI_RENDER_TITLE_MENU_WIDTH, TI_RENDER_TITLE_MENU_HEIGHT,
                            a->titleMenu, display->screen);
            SDL_UpdateRect(display->screen, TI_RENDER_TITLE_MENU_X, TI_RENDER_TITLE_MENU_Y,
                        TI_RENDER_TITLE_MENU_WIDTH, TI_RENDER_TITLE_MENU_HEIGHT);
            break;
        case TI_STATE_NEW_GAME_SCREEN:
            /* Draw the stuff common to the title screens */
            renderTitleScreenDrawBase(display, a, data);
            renderNewGameScreenExtras(display, a, data);
            break;
        case TI_STATE_OPTIONS_SCREEN:
            renderTitleScreenDrawBase(display, a, data);
            renderOptionsScreenExtras(display, a, data);
            break;
        case TI_STATE_IN_GAME:
            renderInGameScreen(display, a, data);
            renderDrawGameStateSpecificDialogs(display, a, data);
            renderDrawGameStateDynamicElements(display, a, data);
            break;
        case TI_STATE_GAME_RESULTS_SCREEN:
            renderResultsScreen(display, a, data);
            break;
        case TI_STATE_END_GAME:
            break;
        default:
            perror("renderUpdateScreen: unknown state");
            break;
    }
}

/****************************************************************************
* renderProcessInGameEvents - see tiRenderSDL.h for description
****************************************************************************/
void renderProcessInGameEvents(TiScreen *display, TiAssets *assets, TiSharedData *data, SDL_Event *event)
{
    Game *g;
    int xPos, yPos, counter;
    int tileX, tileY;

    /* Ugh.  Again. */
    g = gameGetGlobalGameInstance();
    xPos = event->button.x;
    yPos = event->button.y;

    /* Common to all states */
    /* Check to see if the 'X' in the upper corner was pressed */
    if(g->gameState != TI_GAME_STATE_CONFIRM_EXIT && 
       xPos >= TI_RENDER_GAME_END_BUTTON_MIN_X && xPos <= TI_RENDER_GAME_END_BUTTON_MAX_X &&
       yPos >= TI_RENDER_GAME_END_BUTTON_MIN_Y && yPos <= TI_RENDER_GAME_END_BUTTON_MAX_Y)
    {
       if(g->gameState == TI_GAME_STATE_TILE_SELECT)
       {
            renderRestoreBoardBacking(display, assets, data, TI_FALSE);
       }
       gameSetGameState(g, TI_GAME_STATE_CONFIRM_EXIT);
    }

    /* State specific */
    switch(g->gameState)
    {
        case TI_GAME_STATE_SELECT_ACTION:
            /* Check if 'draw tile' was selected */
            if(xPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DRAW][0] && 
               xPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DRAW][0] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DRAW][2] &&
               yPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DRAW][1] &&
               yPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DRAW][1] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DRAW][3])
            {
                if(g->drawIsValid == TI_TRUE)
                {
                    gameSetGameState(g, TI_GAME_STATE_TILE_DRAW);
                }
                else
                {
                }
            } 
            /* Check if 'play tile' was selected */
            if(xPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PLAY][0] && 
               xPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PLAY][0] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PLAY][2] &&
               yPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PLAY][1] &&
               yPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PLAY][1] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PLAY][3])
            {
                if(g->playIsValid == TI_TRUE)
                {
                    data->drawDarkenMask = TI_TRUE;
                    data->drawValidMoveMasks = TI_TRUE;
                    gameSetGameState(g, TI_GAME_STATE_TILE_SELECT);
                }
                else
                {
                }
            } 
            /* Check if 'Discard' was selected */
            if(xPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DISCARD][0] && 
               xPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DISCARD][0] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DISCARD][2] &&
               yPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DISCARD][1] &&
               yPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DISCARD][1] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_DISCARD][3])
            {
                if(g->discardIsValid == TI_TRUE)
                {
                    gameSetGameState(g, TI_GAME_STATE_DISCARD);
                }
                else
                {
                }
            } 
            /* Check if 'Pass' was selected */
            if(xPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PASS][0] && 
               xPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PASS][0] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PASS][2] &&
               yPos >= TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PASS][1] &&
               yPos < TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PASS][1] + TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[TI_OFFSET_PASS][3])
            {
                if(g->passIsValid == TI_TRUE)
                {
                    renderUndrawPreviousMove(display, assets, data);
                    gameSetGameState(g, TI_GAME_STATE_END_TURN);
                }
                else
                {
                }
            } 
            break;
        case TI_GAME_STATE_TILE_DRAW:
            /* Check each valid tile coordinate to see if the mouse has been clicked.  If so, update the
               highlighted tile and redraw */
            for(counter = 0; counter < g->numDrawTilesToDisplay; counter++)
            {
                if(xPos >= TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][0] && 
                xPos < TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][0] + TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][2] &&
                yPos >= TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][1] &&
                yPos < TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][1] + TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[counter][3])
                {
                    /* Optimization - only proceed if the new highlighted tile is different from the old one */
                    if(counter != data->drawTileHighlighted)
                    {
                        data->prevDrawTileHighlighted = data->drawTileHighlighted;
                        data->drawTileHighlighted = counter;
                        data->refreshDrawTileHighlighted = TI_TRUE;
                    }
                }
            }
            /* Check to see if 'OK' was selected */
            if(xPos >= TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[0][0] && 
               xPos < TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[0][0] + TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[0][2] &&
               yPos >= TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[0][1] &&
               yPos < TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[0][1] + TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[0][3])
            {
                /* Pick a random tile and give it to the player.  If the player has a tile already, put it
                   in the reserve slot */
                if(g->players[g->curPlayer].currentTileId == TI_TILE_NO_TILE)
                {
                    g->players[g->curPlayer].currentTileId = tilePoolDrawRandomTile(g->tilepool);
                    g->selectedMoveIsReserveTile = TI_FALSE;
                    g->selectedMoveTileId = g->players[g->curPlayer].currentTileId;
                }
                else
                {
                    g->players[g->curPlayer].reserveTileId = tilePoolDrawRandomTile(g->tilepool);
                    g->selectedMoveIsReserveTile = TI_TRUE;
                    g->selectedMoveTileId = g->players[g->curPlayer].reserveTileId;
                }
                data->drawTileHighlighted = 0;
                data->prevDrawTileHighlighted = 0;
                data->refreshDrawTileHighlighted = TI_TRUE;
                g->playerHasDrawn = TI_TRUE;
                gameSetGameState(g, TI_GAME_STATE_SELECT_ACTION);
            }

            /* Check to see if 'Cancel' was selected. */
            if(xPos >= TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[1][0] && 
               xPos < TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[1][0] + TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[1][2] &&
               yPos >= TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[1][1] &&
               yPos < TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[1][1] + TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[1][3])
            {
                /* Restore everything as it was and go back to the 'select action state' */
                data->drawTileHighlighted = 0;
                data->prevDrawTileHighlighted = 0;
                data->refreshDrawTileHighlighted = TI_TRUE;
                gameSetGameState(g, TI_GAME_STATE_SELECT_ACTION);
            }
            break;
        case TI_GAME_STATE_TILE_SELECT:
            /* Check to see if one of the player's tiles has been pressed.  If so, and a tile 
               exists there, make that the 'active' tile */
            if(xPos >= TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][0] &&
               xPos <  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][0] + TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][2] &&
               yPos >= TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][1] &&
               yPos <  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][1] + TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][3])
            {
                if(g->selectedMoveIsReserveTile == TI_TRUE && g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE)
                {
                    g->selectedMoveIsReserveTile = TI_FALSE;
                    g->selectedMoveTileId = g->players[g->curPlayer].currentTileId;
                    data->drawDarkenMask = TI_TRUE;
                    data->drawValidMoveMasks = TI_TRUE;
                    boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->selectedMoveTileId));
                    renderRestoreBoardBacking(display, assets, data, TI_FALSE);
                    renderDrawDarkenMasks(display, assets, data);
                    renderDrawPlayCancelButton(display, assets, data);
                    renderDrawCurrentTileHighlight(display, assets, data);
                }
            }
            if(xPos >= TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][0] &&
               xPos <  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][0] + TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][2] &&
               yPos >= TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][1] &&
               yPos <  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][1] + TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][3])
            {
                if(g->selectedMoveIsReserveTile == TI_FALSE && g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
                {
                    g->selectedMoveIsReserveTile = TI_TRUE;
                    g->selectedMoveTileId = g->players[g->curPlayer].reserveTileId;
                    data->drawDarkenMask = TI_TRUE;
                    data->drawValidMoveMasks = TI_TRUE;
                    boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->selectedMoveTileId));
                    renderRestoreBoardBacking(display, assets, data, TI_FALSE);
                    renderDrawDarkenMasks(display, assets, data);
                    renderDrawPlayCancelButton(display, assets, data);
                    renderDrawCurrentTileHighlight(display, assets, data);
                }
            }

            /* Check to see if a location on the board has been clicked */
            if(xPos >= TI_RENDER_BOARD_TILE_AREA_X && xPos < TI_RENDER_BOARD_TILE_AREA_MAX_X &&
               yPos >= TI_RENDER_BOARD_TILE_AREA_Y && yPos < TI_RENDER_BOARD_TILE_AREA_MAX_Y)
            {
                tileX = ((xPos - TI_RENDER_BOARD_TILE_AREA_X) / TI_RENDER_SMALL_TILE_WIDTH) + 1;
                tileY = ((yPos - TI_RENDER_BOARD_TILE_AREA_Y) / TI_RENDER_SMALL_TILE_WIDTH) + 1;
                if(boardIsLegalMove(g->board, tileX, tileY))
                {
                    /* A move has been made.  Reset the board state and prepare to switch states */
                    g->selectedMoveTileX = tileX;
                    g->selectedMoveTileY = tileY;
                    /* Take the tile out of the player's inventory here, so we can update the screen 
                       right away */
                    if(g->selectedMoveIsReserveTile == TI_TRUE)
                    {
                        g->players[g->curPlayer].reserveTileId = TI_TILE_NO_TILE;
                        data->refreshPlayerTiles = TI_TRUE;
                    }
                    else
                    {
                        if(g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
                        {
                            g->players[g->curPlayer].currentTileId = g->players[g->curPlayer].reserveTileId;
                            g->players[g->curPlayer].reserveTileId = TI_TILE_NO_TILE;
                        }
                        else
                        {
                            g->players[g->curPlayer].currentTileId = TI_TILE_NO_TILE;
                        }
                        data->refreshPlayerTiles = TI_TRUE;
                    }
                    data->restoreBoardArea = TI_TRUE;
                    data->drawTileOnBoard = TI_TRUE;
                    gameSetGameState(g, TI_GAME_STATE_TILE_PLAY);
                }
                else
                {
                    /* Do something for an illegal move (sound effect?) */
                }
            }
            /* Check if the cancel button was pressed.  If so, cancel and go back to
               the 'Select Action' dialog */
            if(xPos >= TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X && xPos < TI_RENDER_PLAY_CANCEL_BUTTON_MAX_X &&
               yPos >= TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y && yPos < TI_RENDER_PLAY_CANCEL_BUTTON_MAX_Y)
            {
                /* Restore everything as it was and go back to the 'select action state' */
                renderRestoreBoardBacking(display, assets, data, TI_FALSE);
                gameSetGameState(g, TI_GAME_STATE_SELECT_ACTION);
            }
            break;
        case TI_GAME_STATE_DISCARD:
            /* Check to see if one of the player's tiles has been selected */
            if(xPos >= TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][0] &&
               xPos <  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][0] + TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][2] &&
               yPos >= TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][1] &&
               yPos <  TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][1] + TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[g->curPlayer][3])
            {
                if(g->selectedMoveIsReserveTile == TI_TRUE && g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE)
                {
                    g->selectedMoveIsReserveTile = TI_FALSE;
                    g->selectedMoveTileId = g->players[g->curPlayer].currentTileId;
                    boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->selectedMoveTileId));
                    renderDrawCurrentTileHighlight(display, assets, data);
                }
            }
            if(xPos >= TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][0] &&
               xPos <  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][0] + TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][2] &&
               yPos >= TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][1] &&
               yPos <  TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][1] + TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[g->curPlayer][3])
            {
                if(g->selectedMoveIsReserveTile == TI_FALSE && g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
                {
                    g->selectedMoveIsReserveTile = TI_TRUE;
                    g->selectedMoveTileId = g->players[g->curPlayer].reserveTileId;
                    boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, g->selectedMoveTileId));
                    renderDrawCurrentTileHighlight(display, assets, data);
                }
            }
            /* Check to see if 'Ok' was selected */
            if(xPos >= TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[0][0] && 
               xPos < TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[0][0] + TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[0][2] &&
               yPos >= TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[0][1] &&
               yPos < TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[0][1] + TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[0][3])
            {
                /* Restore the tile to the tile pool and end the turn */
                if(gameDiscardTile(g) != TI_OK)
                {
                    perror("Discard failed!\n");
                    exit(-1);
                }
                /* 'Undraw' the last player's move if the 'Last Player' option was selected */
                renderUndrawPreviousMove(display, assets, data);
                g->deleteLastPlayerHighlight = TI_TRUE;
                gameSetGameState(g, TI_GAME_STATE_END_TURN);
            }
            /* Check to see if 'Cancel' was selected. */
            if(xPos >= TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[1][0] && 
               xPos < TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[1][0] + TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[1][2] &&
               yPos >= TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[1][1] &&
               yPos < TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[1][1] + TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[1][3])
            {
                /* Restore everything as it was and go back to the 'select action state' */
                gameSetGameState(g, TI_GAME_STATE_SELECT_ACTION);
            }
            break;
        case TI_GAME_STATE_GAME_FINISHED:
            if(xPos >= TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[0] && 
               xPos < TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[0] + TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[2] &&
               yPos >= TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[1] &&
               yPos < TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[1]+ TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[3])
            {
                /* Go to the game results screen */
                renderSetRenderState(TI_STATE_GAME_RESULTS_SCREEN, data->renderState, display, assets, data);
            }
            break;
        case TI_GAME_STATE_CONFIRM_EXIT:
            /* Check to see if 'yes' was selected */
           if(xPos >= TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[0][0] && 
               xPos < TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[0][0] + TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[0][2] &&
               yPos >= TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[0][1] &&
               yPos < TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[0][1] + TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[0][3])
            {
                /* Exit the game and return to the main menu */
                renderResetSharedDataStructure(data);
                gameResetGameStructure(g);
                data->topTrainX = TI_RENDER_TRAIN_INITIAL_X;
                data->topTrainDirection = TI_RENDER_TRAIN_DIR_RIGHT;
                data->bottomTrainX = TI_RENDER_TRAIN_FINAL_X;
                data->bottomTrainDirection = TI_RENDER_TRAIN_DIR_LEFT;                
                renderSetRenderState(TI_STATE_TITLE_MENU_SCREEN, data->renderState, display, assets, data);
            }
            /* Check to see if 'no' was selected */
            if(xPos >= TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[1][0] && 
               xPos < TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[1][0] + TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[1][2] &&
               yPos >= TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[1][1] &&
               yPos < TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[1][1] + TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[1][3])
            {
                /* Restore everything as it was before the exit button was clicked */
                if(g->previousGameState == TI_GAME_STATE_TILE_DRAW)
                {
                    data->refreshDrawTileHighlighted = TI_TRUE;
                }
                if(g->previousGameState == TI_GAME_STATE_TILE_SELECT)
                {
                    data->drawDarkenMask = TI_TRUE;
                    data->drawValidMoveMasks = TI_TRUE;
                }
                gameSetGameState(g, g->previousGameState);
            }
            break;
        default:
            break;
    }

}

/****************************************************************************
* renderProcessEvents - see tiRenderSDL.h for description
****************************************************************************/
void renderProcessEvents(TiScreen *display, TiAssets *assets, TiSharedData *data)
{
    SDL_Event event;
    int xPos, yPos, counter;
    Game *g;

    while(SDL_PollEvent(&event))
    {
        /* Check for mouse clicks */
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            xPos = event.button.x;
            yPos = event.button.y;
            switch(data->renderState)
            {
                /* A button press automatically forces the company logo screen
                to change to the title screen */
                case TI_STATE_COMPANY_LOGO:
                    renderSetRenderState(TI_STATE_TITLE_SCREEN, data->renderState, display, assets, data);
                    break;
                case TI_STATE_TITLE_SCREEN:
                    renderSetRenderState(TI_STATE_TITLE_MENU_SCREEN, data->renderState, display, assets, data);
                    break;
                case TI_STATE_TITLE_MENU_SCREEN:
                    /* New game */
                    if(xPos >= TI_RENDER_NEW_GAME_MIN_X && xPos <= TI_RENDER_NEW_GAME_MAX_X &&
                    yPos >= TI_RENDER_NEW_GAME_MIN_Y && yPos <= TI_RENDER_NEW_GAME_MAX_Y)
                    {
                        renderSetRenderState(TI_STATE_NEW_GAME_SCREEN, data->renderState, display, assets, data);
                    }
                    /* Options menu */
                    if(xPos >= TI_RENDER_OPTIONS_MIN_X && xPos <= TI_RENDER_OPTIONS_MAX_X &&
                    yPos >= TI_RENDER_OPTIONS_MIN_Y && yPos <= TI_RENDER_OPTIONS_MAX_Y)
                    {
                        renderSetRenderState(TI_STATE_OPTIONS_SCREEN, data->renderState, display, assets, data);
                    }
                    /* End game */
                    if(xPos >= TI_RENDER_EXIT_MIN_X && xPos <= TI_RENDER_EXIT_MAX_X &&
                    yPos >= TI_RENDER_EXIT_MIN_Y && yPos <= TI_RENDER_EXIT_MAX_Y)
                    {
                        renderSetRenderState(TI_STATE_END_GAME, data->renderState, display, assets, data);
                    }
                    break;
                case TI_STATE_NEW_GAME_SCREEN:
                    if(xPos >= TI_RENDER_START_GAME_MIN_X && xPos <= TI_RENDER_START_GAME_MAX_X &&
                    yPos >= TI_RENDER_START_GAME_MIN_Y && yPos <= TI_RENDER_START_GAME_MAX_Y)
                    {
                        renderSetRenderState(TI_STATE_IN_GAME, data->renderState, display, assets, data);
                    }
                    if(xPos >= TI_RENDER_BACK_TO_TITLE_MIN_X && xPos <= TI_RENDER_BACK_TO_TITLE_MAX_X &&
                    yPos >= TI_RENDER_BACK_TO_TITLE_MIN_Y && yPos <= TI_RENDER_BACK_TO_TITLE_MAX_Y)
                    {
                        renderSetRenderState(TI_STATE_TITLE_MENU_SCREEN, data->renderState, display, assets, data);
                    }
                    for(counter=0;counter<5;counter++)
                    {
                        if(xPos >= TI_RENDER_NUM_PLAYER_OFFSETS[counter][0] &&
                           xPos < (TI_RENDER_NUM_PLAYER_OFFSETS[counter][0] + TI_RENDER_NUM_PLAYER_OFFSETS[counter][2]) &&
                           yPos >= TI_RENDER_NUM_PLAYER_OFFSETS[counter][1] &&
                           yPos < (TI_RENDER_NUM_PLAYER_OFFSETS[counter][1] + TI_RENDER_NUM_PLAYER_OFFSETS[counter][3]))
                        {
                            data->selectedPlayers = counter+2;
                            data->refreshPlayersList = TI_TRUE;
                        }
                    }
                    for(counter=0;counter<data->selectedPlayers;counter++)
                    {
                        if(xPos >= TI_RENDER_HUMAN_OFFSETS[counter][0] &&
                           xPos < (TI_RENDER_HUMAN_OFFSETS[counter][0] + TI_RENDER_HUMAN_OFFSETS[counter][2]) &&
                           yPos >= TI_RENDER_HUMAN_OFFSETS[counter][1] &&
                           yPos < (TI_RENDER_HUMAN_OFFSETS[counter][1] + TI_RENDER_HUMAN_OFFSETS[counter][3]))
                        {
                            data->playerState[counter] = TI_PLAYER_HUMAN;
                            data->refreshPlayersList = TI_TRUE;
                        }
                    }
                    for(counter=1;counter<data->selectedPlayers;counter++)
                    {
                        if(xPos >= TI_RENDER_CPU_OFFSETS[counter][0] &&
                           xPos < (TI_RENDER_CPU_OFFSETS[counter][0] + TI_RENDER_CPU_OFFSETS[counter][2]) &&
                           yPos >= TI_RENDER_CPU_OFFSETS[counter][1] &&
                           yPos < (TI_RENDER_CPU_OFFSETS[counter][1] + TI_RENDER_CPU_OFFSETS[counter][3]))
                        {
                            data->playerState[counter] = TI_PLAYER_COMPUTER;
                            data->refreshPlayersList = TI_TRUE;
                        }
                    }
                    break;
                case TI_STATE_OPTIONS_SCREEN:
                    g = gameGetGlobalGameInstance();
                    if(xPos >= TI_RENDER_OPTIONS_BACK_TO_TITLE_MIN_X && xPos <= TI_RENDER_OPTIONS_BACK_TO_TITLE_MAX_X &&
                       yPos >= TI_RENDER_OPTIONS_BACK_TO_TITLE_MIN_Y && yPos <= TI_RENDER_OPTIONS_BACK_TO_TITLE_MAX_Y)
                    {
                        /* Restore the original 'good' settings to the temp values */
                        g->tmpHighlightTracks = g->highlightTracks;
                        g->tmpHighlightLegalMoves = g->highlightLegalMoves;
                        g->tmpShowLastMove = g->showLastMove;
                        g->tmpMusicVolume = g->musicVolume;
                        g->tmpEffectsVolume = g->effectsVolume;
                        g->tmpDefaultAILevel = g->defaultAILevel;
                        renderSetRenderState(TI_STATE_TITLE_MENU_SCREEN, data->renderState, display, assets, data);
                    }     
                    if(xPos >= TI_RENDER_OPTIONS_OK_MIN_X && xPos <= TI_RENDER_OPTIONS_OK_MAX_X &&
                       yPos >= TI_RENDER_OPTIONS_OK_MIN_Y && yPos <= TI_RENDER_OPTIONS_OK_MAX_Y)
                    {
                        /* Set the good values to the 'temp' value settings */
                        g->highlightTracks = g->tmpHighlightTracks;
                        g->highlightLegalMoves = g->tmpHighlightLegalMoves;
                        g->showLastMove = g->tmpShowLastMove;
                        g->musicVolume = g->tmpMusicVolume;
                        g->effectsVolume = g->tmpEffectsVolume;
                        g->defaultAILevel = g->tmpDefaultAILevel;
                        renderSetRenderState(TI_STATE_TITLE_MENU_SCREEN, data->renderState, display, assets, data);                        
                    }
                    if(xPos >= TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][0] &&
                       xPos < (TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][0] + TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][2]) &&
                       yPos >= TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][1] &&
                       yPos < (TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][1] + TI_RENDER_OPTIONS_TRACKS_OFFSETS[0][3]))
                    {
                        g->tmpHighlightTracks = TI_GAME_OPTIONS_NO;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][0] &&
                       xPos < (TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][0] + TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][2]) &&
                       yPos >= TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][1] &&
                       yPos < (TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][1] + TI_RENDER_OPTIONS_TRACKS_OFFSETS[1][3]))
                    {
                        g->tmpHighlightTracks = TI_GAME_OPTIONS_YES;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][0] &&
                       xPos < (TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][0] + TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][2]) &&
                       yPos >= TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][1] &&
                       yPos < (TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][1] + TI_RENDER_OPTIONS_LEGAL_OFFSETS[0][3]))
                    {
                        g->tmpHighlightLegalMoves = TI_GAME_OPTIONS_NO;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][0] &&
                       xPos < (TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][0] + TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][2]) &&
                       yPos >= TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][1] &&
                       yPos < (TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][1] + TI_RENDER_OPTIONS_LEGAL_OFFSETS[1][3]))
                    {
                        g->tmpHighlightLegalMoves = TI_GAME_OPTIONS_YES;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][0] &&
                       xPos < (TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][0] + TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][2]) &&
                       yPos >= TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][1] &&
                       yPos < (TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][1] + TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[0][3]))
                    {
                        g->tmpShowLastMove = TI_GAME_OPTIONS_NO;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][0] &&
                       xPos < (TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][0] + TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][2]) &&
                       yPos >= TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][1] &&
                       yPos < (TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][1] + TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[1][3]))
                    {
                        g->tmpShowLastMove = TI_GAME_OPTIONS_LAST_PLAYER;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][0] &&
                       xPos < (TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][0] + TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][2]) &&
                       yPos >= TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][1] &&
                       yPos < (TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][1] + TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[2][3]))
                    {
                        g->tmpShowLastMove = TI_GAME_OPTIONS_ALL_PLAYERS;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][0] &&
                       xPos < (TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][0] + TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][2]) &&
                       yPos >= TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][1] &&
                       yPos < (TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][1] + TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[0][3]))
                    {
                        g->tmpDefaultAILevel = TI_GAME_OPTIONS_AI_EASY;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][0] &&
                       xPos < (TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][0] + TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][2]) &&
                       yPos >= TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][1] &&
                       yPos < (TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][1] + TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[1][3]))
                    {
                        g->tmpDefaultAILevel = TI_GAME_OPTIONS_AI_MEDIUM;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    if(xPos >= TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][0] &&
                       xPos < (TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][0] + TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][2]) &&
                       yPos >= TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][1] &&
                       yPos < (TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][1] + TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[2][3]))
                    {
                        g->tmpDefaultAILevel = TI_GAME_OPTIONS_AI_HARD;
                        data->refreshOptionsScreen = TI_TRUE;
                    }
                    for(counter=0;counter<10;counter++)
                    {
                        if(xPos >= TI_RENDER_OPTIONS_MUSIC_OFFSETS[counter][0] &&
                           xPos < (TI_RENDER_OPTIONS_MUSIC_OFFSETS[counter][0] + TI_RENDER_OPTIONS_MUSIC_OFFSETS[counter][2]) &&
                           yPos >= TI_RENDER_OPTIONS_MUSIC_OFFSETS[counter][1] &&
                           yPos < (TI_RENDER_OPTIONS_MUSIC_OFFSETS[counter][1] + TI_RENDER_OPTIONS_MUSIC_OFFSETS[counter][3]))
                        {
                            g->tmpMusicVolume = counter;
                            data->refreshOptionsScreen = TI_TRUE;
                        }
                        if(xPos >= TI_RENDER_OPTIONS_SOUND_OFFSETS[counter][0] &&
                           xPos < (TI_RENDER_OPTIONS_SOUND_OFFSETS[counter][0] + TI_RENDER_OPTIONS_SOUND_OFFSETS[counter][2]) &&
                           yPos >= TI_RENDER_OPTIONS_SOUND_OFFSETS[counter][1] &&
                           yPos < (TI_RENDER_OPTIONS_SOUND_OFFSETS[counter][1] + TI_RENDER_OPTIONS_SOUND_OFFSETS[counter][3]))
                        {
                            g->tmpEffectsVolume = counter;
                            data->refreshOptionsScreen = TI_TRUE;
                        }
                    }
                    break;                                             
                case TI_STATE_IN_GAME:
                    renderProcessInGameEvents(display, assets, data, &event);
                    break;
                case TI_STATE_GAME_RESULTS_SCREEN:
                        if(xPos >= TI_RENDER_GAME_RESULTS_TO_TITLE_MIN_X &&
                           xPos < TI_RENDER_GAME_RESULTS_TO_TITLE_MAX_X  &&
                           yPos >= TI_RENDER_GAME_RESULTS_TO_TITLE_MIN_Y &&
                           yPos < TI_RENDER_GAME_RESULTS_TO_TITLE_MAX_Y)
                        {
                            g=gameGetGlobalGameInstance();
                            renderResetSharedDataStructure(data);
                            gameResetGameStructure(g);
                            renderSetRenderState(TI_STATE_TITLE_MENU_SCREEN, data->renderState, display, assets, data);
                        }
                        break;
                default:
                    break;
            }
        }
    }
    return;
}

