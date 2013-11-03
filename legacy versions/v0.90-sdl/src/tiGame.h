/****************************************************************************
*
* tiGame.h - Header for tiGame.c
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
#ifndef __TIGAME_H__
#define __TIGAME_H__

enum
{
    TI_GAME_STATE_DEFAULT,
    TI_GAME_STATE_SELECT_ACTION,
    TI_GAME_STATE_TILE_DRAW,
    TI_GAME_STATE_TILE_SELECT,
    TI_GAME_STATE_TILE_PLAY,
    TI_GAME_STATE_PASS,
    TI_GAME_STATE_DISCARD,
    TI_GAME_STATE_END_TURN,
    TI_GAME_STATE_GAME_FINISHED,
    TI_GAME_STATE_CONFIRM_EXIT,
    TI_GAME_STATE_COMPUTER_MOVE,
    TI_GAME_STATE_PAUSE,
};

#define TI_GAME_OPTIONS_NO      0
#define TI_GAME_OPTIONS_YES     1
#define TI_GAME_OPTIONS_LAST_PLAYER  2
#define TI_GAME_OPTIONS_ALL_PLAYERS  3
#define TI_GAME_OPTIONS_AI_EASY     0
#define TI_GAME_OPTIONS_AI_MEDIUM   1
#define TI_GAME_OPTIONS_AI_HARD     2

typedef struct
{
    Board       *board;
    TilePool    *tilepool;
    Player      players[TI_MAX_PLAYERS];
    int         numPlayers;
    int         numStationsPerPlayer;
    int         gameState;
    int         previousGameState;
    int         gameStateChanged;
    int         curPlayer;

    /* Options in the 'Options' screen */
    int        highlightTracks;
    int        highlightLegalMoves;
    int        showLastMove;
    int        musicVolume;
    int        effectsVolume;
    int        defaultAILevel;

    /* Temporary set options (we'll only change the values if 'ok' is selected) */
    int        tmpHighlightTracks;
    int        tmpHighlightLegalMoves;
    int        tmpShowLastMove;
    int        tmpMusicVolume;
    int        tmpEffectsVolume;
    int        tmpDefaultAILevel;

    /* Used by various game states to determine the validity of particular options */
    int         playerHasDrawn;
    int         playIsValid;
    int         drawIsValid;
    int         discardIsValid;
    int         passIsValid;
    int         numDrawTilesToDisplay;

    int         selectedMoveTileX;
    int         selectedMoveTileY;
    int         selectedMoveTileId;
    int         selectedMoveIsReserveTile;

    int         deleteLastPlayerHighlight;
} Game;

extern Game     *GameInstance;

/****************************************************************************
* gameInitialize
*
* Description:
*   Allocates memory for a Game and sets default values as necessary.
*
* Arguments:
*   char *tileData - the tile data file name
*   char *stationData - the station data file name
*
* Returns:
*   A pointer to the Game, or NULL if an error was detected.
*
****************************************************************************/
Game *gameInitialize(char *tileData, char *stationData);

/****************************************************************************
* gameInitializePlayersFromUi
*
* Description:
*   Sets the player data from the information obtained from the UI
*   'New Game' menu.
*
* Arguments:
*   Game *g - the game structure that holds the player structure to modify
*   TiSharedData *data - the shared data obtained from the UI
*
* Returns:
*   TI_OK.
*
****************************************************************************/
int gameInitializePlayersFromUi(Game *g, TiSharedData *data);

/****************************************************************************
* gameDestroy
*
* Description:
*   Frees the memory that a game instance is using
*
* Arguments:
*   Game *g - the game to destroy
*
* Returns:
*   TI_OK.
*
****************************************************************************/
int gameDestroy(Game **g);

/****************************************************************************
* gameResetGameStructure
*
* Description:
*   Returns a Game structure to default values without reloading station 
*   information and related data.  This is used to allow a new game to be
*   started after an existing game is finished.
*
* Arguments:
*   Game *g - the game to reset
*
* Returns:
*   TI_OK or TI_ERROR.
*
****************************************************************************/
int gameResetGameStructure(Game *g);

/****************************************************************************
* gameCheckForCompletedTracks
*
* Description:
*   Check each station to see if it has a completed track.  For each
*   completed track, process it and mark it as complete.
*
* Arguments:
*   Game *g - The game instance to check the board of.
*
* Returns:
*   The number of completed tracks found.
*
****************************************************************************/
int gameCheckForCompletedTracks(Game *g);

/****************************************************************************
* gameProcessCompletedTrack
*
* Description:
*   Using a board and station, calculates the score for the finished track,
*   updates the player train status and marks the track's station as
*   processed.
*
* Arguments:
*   Game *g - the game instance to process
*   int station - the station to be processed.
*
* Returns:
*   TI_OK if everything is ok, TI_ERROR otherwise.
*
****************************************************************************/
int gameProcessCompletedTrack(Game *g, int station);

/****************************************************************************
* gameGetGlobalGameInstance
*
* Description:
*   Returns a copy of the global Game instance.
*
* Arguments:
*   None.
*
* Returns:
*   A pointer to the current game instance (currently global).
*
****************************************************************************/
Game *gameGetGlobalGameInstance(void);

/****************************************************************************
* gameSetGameState
*
* Description:
*   Changes the current game state.
*
* Arguments:
*   Game *g - the Game instance that will change state
*   int state - the state to change to
*
* Returns:
*   TI_OK or TI_ERROR.
*
****************************************************************************/
int gameSetGameState(Game *g, int state);

/****************************************************************************
* gameCheckForEndOfGame
*
* Description:
*   Determines whether the game is over
*
* Arguments:
*   Game *g - the game to check the completion status of
*
* Returns:
*   TI_TRUE if the game is over, or TI_FALSE if not.
*
****************************************************************************/
int gameCheckForEndOfGame(Game *g);

/****************************************************************************
* gameDiscardTile
*
* Description:
*   Discards the tile of the current player as described by values in the
*    specified game instance.
*
* Arguments:
*   Game *g - the current game (used to determine which tile to discard)
*
* Returns:
*   TI_OK or TI_ERROR.
*
****************************************************************************/
int gameDiscardTile(Game *g);

#endif /* __TIGAME_H__ */
