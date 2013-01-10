/****************************************************************************
*
* tiRenderSDL.h - Header for tiRenderSDL.c
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
#ifndef __TIRENDERSDL_H__
#define __TIRENDERSDL_H__

#include "tiComputerAI.h"

/* Aim for 60 FPS */
#define TI_RENDER_FRAME_RATE                60
#define TI_RENDER_FRAME_DURATION            (1000 / TI_RENDER_FRAME_RATE)
#define TI_RENDER_MINIMUM_FRAME_RATE        10
#define TI_MAX_CYCLES_PER_FRAME             (TI_RENDER_FRAME_RATE / TI_RENDER_MINIMUM_FRAME_RATE)

/* Show the 'tap to play' message 75 percent of the time */
#define TI_RENDER_TAP_MSG_FRAME_SHOWN       0
#define TI_RENDER_TAP_MSG_FRAME_HIDDEN      (TI_RENDER_FRAME_RATE) * 0.75

/* Train directions at the title screen */
#define TI_RENDER_TRAIN_DIR_LEFT            0
#define TI_RENDER_TRAIN_DIR_RIGHT           1
#define TI_RENDER_TRAIN_BANNER_SPEED        (60 / (TI_RENDER_FRAME_RATE))

/* Duration of the display of the 'Meatgoat Software' logo */
#define TI_RENDER_LOGO_DURATION             5000                        /* 5 seconds */

/* Used for the UI representation of the number of stations completed
for each player */
#define TI_RENDER_NUM_STATIONS_PER_ROW      8
#define TI_RENDER_NUM_STATION_ROWS          2
#define TI_RENDER_STATION_STATUS_NA         0
#define TI_RENDER_STATION_STATUS_COMPLETE   1
#define TI_RENDER_STATION_STATUS_INCOMPLETE 2

/* The maximum number of tiles to draw on the 'Draw Tile' dialog.
 * This actual number drawn may be reduced if there are less than this many tiles left in 
 * the tile pool.
 */
#define TI_RENDER_NUM_DRAW_TILES_TO_DISPLAY     12

#define TI_RENDER_LOADING_DIALOG_NUM_FILES      31

#ifdef _NOKIA_N800_
#define TI_RENDER_TRANSITION_MIN_ACCEL          10.0
#define TI_RENDER_TRANSITION_MAX_ACCEL          15.0
#else
#define TI_RENDER_TRANSITION_MIN_ACCEL          2.0
#define TI_RENDER_TRANSITION_MAX_ACCEL          6.0
#endif

/* A list of all of the possible render states for the game.  The items drawn
 * on-screen are dependent on the current render state
 */
enum
{
    TI_STATE_NO_STATE,
    TI_STATE_COMPANY_LOGO,
    TI_STATE_TITLE_SCREEN,
    TI_STATE_TITLE_MENU_SCREEN,
    TI_STATE_NEW_GAME_SCREEN,
    TI_STATE_OPTIONS_SCREEN,
    TI_STATE_IN_GAME,
    TI_STATE_GAME_RESULTS_SCREEN,
    TI_STATE_END_GAME
};

typedef struct
{
    /* Current part of the game we're executing */
    int renderState;
    /* Variables used to initiate refresh of key areas of the screen */
    int refreshBG;
    int refreshOptionsScreen;
    int refreshPlayersList;
    int refreshPlayerTiles;
    int refreshPlayerStations;
    int refreshPlayerScores;
    int refreshPlayerTrains;
    int refreshTrackOverlays;
    /* Number of frames rendered */
    int numFrames;
    /* Should the game exit? */
    int exitGame;
    /* Tick counters (since start, since state change, and since last frame) */
    Uint32 titleStartTicks;
    Uint32 curTicks;
    Uint32 stateStartTicks;
    Uint32 lastFrameTicks;
    Uint32 cyclesLeftOver;
    /* Used to control the position of the title screen animated trains */
    int topTrainX;
    int topTrainDirection;
    int bottomTrainX;
    int bottomTrainDirection;
    int selectedPlayers;
    int playerState[TI_MAX_PLAYERS];
    /* Holds the contents of the display behind a dialog */
    SDL_Surface *currentPlayerBacking;
    SDL_Surface *currentTileBacking;
    SDL_Surface *dialogBacking;
    /* Allows the restoration of the board after valid moves have been marked on it */
    SDL_Surface *boardAreaBacking;

    int dialogBackingX;
    int dialogBackingY;
    int dialogBackingW;
    int dialogBackingH;

    int currentTileBackingX;
    int currentTileBackingY;
    int currentTileBackingW;
    int currentTileBackingH;

    /* The player that the current player backing bitmap is holding */
    int currentPlayerBackingPlayerNum;

    /* For the 'Draw Tile' dialog */
    int drawTileHighlighted;
    int prevDrawTileHighlighted;
    int refreshDrawTileHighlighted;

    /* control the display of the background darkening mask */
    int drawDarkenMask;
    int drawValidMoveMasks;

    int restoreBoardArea;
    int drawTileOnBoard;

    /* Progress bar variables */
    int numFilesLoaded;

    /* Used for computer AI */
    ComputerAIPacket *currentMove;
    ComputerAIPacket *previousMove;
} TiSharedData;

typedef struct
{
    SDL_Surface *screen;
    int xRes;
    int yRes;
} TiScreen;

typedef struct
{
    /* Company logo */
    SDL_Surface *logoScreenBG;

    /* Title screen */
    SDL_Surface *titleScreenBG;
    SDL_Surface *trainBannerTop;
    SDL_Surface *trainBannerBottom;
    SDL_Surface *tapScreenMsg;
    SDL_Surface *titleMenu;

    /* New game screen */
    SDL_Surface *newGameBG;
    SDL_Surface *numPlayersDigits;
    SDL_Surface *humanCpu;
    SDL_Surface *loadingDialog;
    SDL_Surface *progressBar;

    /* Option screen */
    SDL_Surface *optionScreenBG;
    SDL_Surface *optionDigits;
    SDL_Surface *optionYesNo;
    SDL_Surface *optionShowLastMove;
    SDL_Surface *optionAILevel;

    /* In game screen */
    SDL_Surface *gameplayBG;
    SDL_Surface *playerColors;
    SDL_Surface *playerLightBG;
    SDL_Surface *playerStationsChart;
    SDL_Surface *tileStripSmall;
    SDL_Surface *tileStripLarge;
    SDL_Surface *stationStatus;
    SDL_Surface *playerStrings;
    SDL_Surface *playerTypeStrings;
    SDL_Surface *playerScoreString;
    SDL_Surface *currentPlayer;
    SDL_Surface *currentTile;

    /* In game dialogs */
    SDL_Surface *selectTileDialog;
    SDL_Surface *selectActionDialog;
    SDL_Surface *selectPlayActive;
    SDL_Surface *selectDrawActive;
    SDL_Surface *selectDiscardActive;
    SDL_Surface *selectPassActive;
    SDL_Surface *chooseDiscardDialog;
    SDL_Surface *confirmExitDialog;
    SDL_Surface *cancelPlayTileButton;
    SDL_Surface *computerThinkingDialog;
    SDL_Surface *gameFinishedDialog;

    /* Other in-game elements */
    SDL_Surface *drawTileHighlightLarge;
    SDL_Surface *darkenMask;
    SDL_Surface *validMoveMask;
    SDL_Surface *scoreBacking;              /* It's here because it's always the same */
    SDL_Surface *digits;
    SDL_Surface *trains;
    SDL_Surface *stations;
    SDL_Surface *trackOverlays;
    SDL_Surface *lastMoveHighlights;

    /* Game results screen (also uses trainBannerTop and trainBannerBottom) */
    SDL_Surface *resultsBackground;
    SDL_Surface *resultsSmallDigits;
    SDL_Surface *resultsLargeDigits;
    SDL_Surface *resultsSmallPlayers;
    SDL_Surface *resultsLargePlayers;

} TiAssets;

/****************************************************************************
* renderSharedDataInitialize
*
* Description:
*   Initializes the shared data structure used by the render code
*
* Arguments:
*   None
*
* Returns:
*   A pointer to the shared data structure, or NULL if an error was
*   detected.
*
****************************************************************************/
TiSharedData *renderSharedDataInitialize(void);

/****************************************************************************
* renderSharedDataDestroy
*
* Description:
*   Frees the memory used by the specified shared data structure
*
* Arguments:
*   TiSharedData **data	- the structure to delete.
*
* Returns:
*   TI_OK or TI_ERROR
*
****************************************************************************/
int renderSharedDataDestroy(TiSharedData **data);

/****************************************************************************
* renderInitialize
*
* Description:
*   Initializes the display data structure
*
* Arguments:
*   int xRes - the horizontal resolution of the display
*   int yRes - the vertical resolution of the display
*   int bpp  - the bit depth of the display
*
* Returns:
*   A pointer to the display data structure, or NULL if an error was
*   detected.
*
****************************************************************************/
TiScreen *renderInitialize(int xRes, int yRes, int bpp);

/****************************************************************************
* renderDestroy
*
* Description:
*   Destroys the display data structure
*
* Arguments:
*   TiScreen **display - the screen structure to destroy
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderDestroy(TiScreen **display);

/****************************************************************************
* renderLoadImage
*
* Description:
*   Loads the specified image into an SDL_Surface
*
* Arguments:
*   char *fileName - the name of the file to load
*
* Returns:
*   A pointer to the SDL_Surface loaded by the function, or NULL if an
*   error was detected.
*
****************************************************************************/
SDL_Surface *renderLoadImage(char *fileName);

/****************************************************************************
* renderAssetsInitialize
*
* Description:
*   Creates and initializes an TiAssets structure
*
* Arguments:
*   None.
*
* Returns:
*   A pointer to the TiAssets structure created by the function, or NULL
*   if a problem was detected.
*
****************************************************************************/
TiAssets *renderAssetsInitialize(void);

/****************************************************************************
* renderLoadAssets
*
* Description:
*   Loads the images and sounds needed for the current render state
*
* Arguments:
*   TiScreen *display - the screen data
*   TiAssets *assets - the asset pool to load the data into
*   TiSharedData *data - the shared data structure that contains the
*                        current render state.
*
* Returns:
*   TI_OK or TI_ERROR
*
****************************************************************************/
int renderLoadAssets(TiScreen *display, TiAssets *assets, TiSharedData *data);

/****************************************************************************
* renderFreeHelper
*
* Description:
*   If necessary, frees the specified SDL_Surface and sets it to NULL.
*
* Arguments:
*   SDL_Surface **s - a pointer to a SDL_Surface pointer that points to
*                     a loaded image.  A pointer to a pointer is used since
*                     the value of the pointer will be changed by the
*                     function.
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderFreeHelper(SDL_Surface **s);

/****************************************************************************
* renderFreeAssets
*
* Description:
*   If necessary, frees the specified SDL_Surface and sets it to NULL.
*
* Arguments:
*   SDL_Surface **s - a pointer to a SDL_Surface pointer that points to
*                     a loaded image.  A pointer to a pointer is used since
*                     the value of the pointer will be changed by the
*                     function.
*
* Returns:
*   Nothing.
*
****************************************************************************/
int renderFreeAssets(TiAssets *assets, TiSharedData *data);

/****************************************************************************
* renderSetRenderState
*
* Description:
*   Changes the current render state, which controls what gets displayed on
*   the screen.
*
* Arguments:
*   int curState - the state to change to
*   int prevState - the state prior to the state change
*   TiScreen *display - the display data
*   TiAssets *assets - the asset pool (used to help load/unload images
*                      when the state changes)
*   TiSharedData *data - the shared data structure containing state
*                        information and other useful data
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderSetRenderState(int curState, int prevState, TiScreen *display, TiAssets *assets, TiSharedData *data);

/****************************************************************************
* renderCheckTimingConditions
*
* Description:
*   Checks for any timed actions during the current render state, and
*   responds accordingly.  Currently, this is only used to display the
*   Holy Meatgoat logo for 5 seconds before switching to the title screen.
*
* Arguments:
*   TiScreen *display - the display structure
*   TiAssets *assets - the asset pool
*   TiSharedData *data - the shared data structure containing state
*                        information and other useful data
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderCheckTimingConditions(TiScreen *display, TiAssets *assets, TiSharedData *data);

/****************************************************************************
* renderApplySurface
*
* Description:
*   Draws an entire SDL_Surface to the specified location within the
*   destination SDL_Surface
*
* Arguments:
*   int x - the x position of the destination SDL_Surface to draw in
*   int y - the y position of the destination SDL_Surface to draw in
*   SDL_Surface *source - the source bitmap
*   SDL_Surface *dest   - the destination bitmap
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *dest);

/****************************************************************************
* renderBlitSurface
*
* Description:
*   Draws a subset of a SDL_Surface to the specified location within the
*   destination SDL_Surface
*
* Arguments:
*   int srcX - the x position of the source bitmap to draw from
*   int srcY - the y position of the source bitmap to draw from
*   int dstX - the x position of the destination SDL_Surface to draw in
*   int dstY - the y position of the destination SDL_Surface to draw in
*   int w - the width of the area to blit
*   int h - the height of the area to blit
*   SDL_Surface *source - the source bitmap
*   SDL_Surface *dest   - the destination bitmap
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderBlitSurface(int srcX, int srcY, int dstX, int dstY, int w, int h,
                    SDL_Surface *source, SDL_Surface *dest);

/****************************************************************************
* renderUpdateLogic
*
* Description:
*   Update any animated elements that need it.
*
* Arguments:
*   TiScreen *display - a display structure
*   TiSharedData *data - the shared data structure
*   TiAssets     *a - the asset pool
* Returns:
*   Nothing.
*
****************************************************************************/
void renderUpdateLogic(TiScreen *display, TiSharedData *data, TiAssets *a);

/****************************************************************************
* renderTitleScreenDrawBase
*
* Description:
*   A helper function to draw the constant parts of the title screen
*
* Arguments:
*   TiScreen     *display - the display to draw to
*   TiAssets     *a - the asset pool to grab images from
*   TiSharedData *data - the shared data structure
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderTitleScreenDrawBase(TiScreen *display, TiAssets *a, TiSharedData *data);

/****************************************************************************
* renderTitleScreenDrawBase
*
* Description:
*   A helper function to draw the images specific to the 'New Game' screen
*
* Arguments:
*   TiScreen     *display - the display to draw to
*   TiAssets     *a - the asset pool to grab images from
*   TiSharedData *data - the shared data structure
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderNewGameScreenExtras(TiScreen *display, TiAssets *a, TiSharedData *data);

/****************************************************************************
* renderInGameScreen
*
* Description:
*   A helper function to draw the main game interface
*
* Arguments:
*   TiScreen     *display - the display to draw to
*   TiAssets     *a - the asset pool to grab images from
*   TiSharedData *data - the shared data structure
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderInGameScreen(TiScreen *display, TiAssets *a, TiSharedData *data);

/****************************************************************************
* renderUpdateScreen
*
* Description:
*   This is the function that gets called once per iteration of the game
*   loop, and is responsible for making sure the display is correct
*   regardless of the current render state.
*
* Arguments:
*   TiScreen     *display - the display to draw to
*   TiAssets     *a - the asset pool to grab images from
*   TiSharedData *data - the shared data structure
*
* Returns:
*   Nothing.
*
****************************************************************************/
void renderUpdateScreen(TiScreen *display, TiAssets *a, TiSharedData *data);

void renderOptionsScreenExtras(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderProcessEvents(TiScreen *display, TiAssets *assets, TiSharedData *data);
void renderProcessInGameEvents(TiScreen *display, TiAssets *assets, TiSharedData *data, SDL_Event *event);
void renderTrainsAndStations(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderDrawTileOnBoard(TiScreen *display, TiAssets *a, TiSharedData *data);
int renderProcessComputerMove(TiScreen *display, TiAssets *a,  TiSharedData *data);
void renderDrawAnimatedTrains(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderTrackOverlay(TiScreen *display, TiAssets *a, TiSharedData *data, int station);
void renderRestoreBoardBacking(TiScreen *display, TiAssets *a, TiSharedData *data, int refreshScreen);
void renderDrawCurrentTileHighlight(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderDrawCurrentPlayerHighlight(TiScreen *display, TiAssets *a, TiSharedData *data);
int renderDisplayDialog(TiScreen *display, SDL_Surface *dialog, TiSharedData *data,
                         int x, int y, int w, int h);
void renderDrawDarkenMasks(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderDrawPlayCancelButton(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderResultsScores(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderResultsScoreDigits(TiScreen *display, TiAssets *a, TiSharedData *data, int rank, int score);
void renderResultsScreen(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderPlayerScore(TiScreen *display, TiAssets *a, TiSharedData *data, int player);
int renderResetSharedDataStructure(TiSharedData *data);
void renderTrackOverlays(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderLastMoves(TiScreen *display, TiAssets *a, TiSharedData *data);
void renderDrawTile(TiScreen *display, TiAssets *a, TiSharedData *data, int x, int y, int tile);
void renderUndrawPreviousMove(TiScreen *display, TiAssets *a, TiSharedData *data);

#endif /* __TIRENDERSDL_H__ */
