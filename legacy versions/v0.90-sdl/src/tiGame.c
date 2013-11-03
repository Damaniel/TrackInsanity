/****************************************************************************
*
* tiGame.c - manages instances of the game
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
* gameInitialize - see tiGame.h for description
****************************************************************************/
Game *gameInitialize(char *tileData, char *stationData)
{
    Game *g;

    g = malloc(sizeof(Game));
    if(g == NULL)
    {
        return NULL;
    }

    g->tilepool = NULL;
    g->board = NULL;

    g->highlightTracks = TI_GAME_OPTIONS_YES;
    g->highlightLegalMoves = TI_GAME_OPTIONS_YES;
    g->showLastMove = TI_GAME_OPTIONS_ALL_PLAYERS;
    g->defaultAILevel = TI_GAME_OPTIONS_AI_MEDIUM;
    g->musicVolume = 4;
    g->effectsVolume = 4;

    g->tmpHighlightTracks = TI_GAME_OPTIONS_YES;
    g->tmpHighlightLegalMoves = TI_GAME_OPTIONS_YES;
    g->tmpShowLastMove = TI_GAME_OPTIONS_ALL_PLAYERS;
    g->tmpDefaultAILevel = TI_GAME_OPTIONS_AI_MEDIUM;
    g->tmpMusicVolume = 4;
    g->tmpEffectsVolume = 4;

    if(gameResetGameStructure(g) == TI_ERROR)
    {
        return NULL;
    }

    return g;
}

/****************************************************************************
* gameResetGameStructure - see tiGame.h for description
****************************************************************************/
int gameResetGameStructure(Game *g)
{
    if(g->tilepool != NULL)
    {
        tilePoolDestroy(&(g->tilepool));
    }
    g->tilepool = tilePoolInitialize("data/tileData");
    if(g->tilepool == NULL)
    {
        perror("gameInitialize: failed to initialize tile pool");
        free(g);
        return TI_ERROR;
    }

    if(g->board != NULL)
    {
        boardDestroy(&(g->board));
    }
    g->board = boardInitialize(g->tilepool, "data/stationData");
    if(g->board == NULL)
    {
        perror("gameInitialize: failed to initialize board");
        free(g->tilepool);
        free(g);
        return TI_ERROR;
    }

    g->numPlayers = 0;
    g->curPlayer = 0;
    g->gameState = TI_GAME_STATE_DEFAULT;
    g->previousGameState = TI_GAME_STATE_DEFAULT;
    g->playerHasDrawn = TI_FALSE;
    g->playIsValid = TI_FALSE;
    g->drawIsValid = TI_FALSE;
    g->discardIsValid = TI_FALSE;
    g->passIsValid = TI_FALSE;
    g->numDrawTilesToDisplay = 0;
    g->selectedMoveTileX = -1;
    g->selectedMoveTileY = -1;
    g->selectedMoveTileId = TI_TILE_NO_TILE;
    g->selectedMoveIsReserveTile = TI_FALSE;

    g->deleteLastPlayerHighlight = TI_FALSE;
    return TI_OK;
}

/****************************************************************************
* gameInitializePlayersFromUi - see tiGame.h for description
****************************************************************************/
int gameInitializePlayersFromUi(Game *g, TiSharedData *data)
{
    int counter;
    int stationX, stationY, exit;
    int aiLevel;

    if(data->selectedPlayers < TI_MIN_PLAYERS || data->selectedPlayers > TI_MAX_PLAYERS)
    {
        return TI_ERROR;
    }

    if(g->defaultAILevel == TI_GAME_OPTIONS_AI_EASY)
    {
        aiLevel = TI_PLAYER_AI_DEFAULT;
    }
    else if(g->defaultAILevel == TI_GAME_OPTIONS_AI_MEDIUM)
    {
        aiLevel = TI_PLAYER_AI_SMARTER;
    }
    else
    {
        aiLevel = TI_PLAYER_AI_SMARTEST;
    }

    printf("Set AI level to %d\n", aiLevel);

    /* Initialize active players */
    for(counter=0; counter<data->selectedPlayers; counter++)
    {
        playerInitPlayer(&(g->players[counter]), data->playerState[counter],
                        0, aiLevel);
    }
    /* Initialize inactive players */
    for(counter=data->selectedPlayers; counter< TI_MAX_PLAYERS; counter++)
    {
        playerInitPlayer(&(g->players[counter]), TI_PLAYER_NOBODY, 0,
                        aiLevel);
    }

    /* Set the number of stations that each player receives (used pretty
    much for the UI) */
    g->numStationsPerPlayer = (int)(TI_BOARD_NUM_STATIONS / data->selectedPlayers);

    /* Set the number of players for this game */
    g->numPlayers = data->selectedPlayers;

    /* Set the curernt player to player 1 */
    g->curPlayer = 0;

    /* Populate all station tiles with the appropriate owner */
    for(counter=0; counter<TI_BOARD_NUM_STATIONS; counter++)
    {
        boardGetStationInfo(counter, &stationX, &stationY, &exit);
        g->board->b[stationX][stationY].trainPresent = g->board->playerStations[g->numPlayers][counter];
    }

    return TI_OK;
}

/****************************************************************************
* gameSetGameState - see tiGame.h for description
****************************************************************************/
int gameSetGameState(Game *g, int state)
{
    int tileIdToUse;
    int numLegalMoves;
    int counter;

    switch(state)
    {
        case TI_GAME_STATE_DEFAULT:
            break;
        case TI_GAME_STATE_SELECT_ACTION:
            printf("  Changing game state to TI_GAME_STATE_SELECT_ACTION\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            /* Move into the computer move state */
            if(g->players[g->curPlayer].controlledBy == TI_PLAYER_COMPUTER)
            {
                gameSetGameState(g, TI_GAME_STATE_COMPUTER_MOVE);
            }
            else 
            {
                /* If a player holds 2 tiles, make sure 'Draw tile' is disabled */
                if(g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE &&
                   g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
                {
                    g->playerHasDrawn = TI_TRUE;
                }
                if(g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
                {
                    g->selectedMoveIsReserveTile = TI_TRUE;
                }
                else
                {
                    g->selectedMoveIsReserveTile = TI_FALSE;
                }
            }
            break;
        case TI_GAME_STATE_TILE_DRAW:
            printf("  Changing game state to TI_GAME_STATE_TILE_DRAW\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            if(g->tilepool->numUnplayedTiles < TI_RENDER_NUM_DRAW_TILES_TO_DISPLAY)
            {
                g->numDrawTilesToDisplay = g->tilepool->numUnplayedTiles;
            }
            else
            {
                g->numDrawTilesToDisplay = TI_RENDER_NUM_DRAW_TILES_TO_DISPLAY;
            }
            break;
        case TI_GAME_STATE_TILE_SELECT:
            printf("  Changing game state to TI_GAME_STATE_TILE_SELECT\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            /* Decide the tile that will be used as the 'active' tile.  If a tile is held
               as a reserve, it will be selected.  Otherwise, the main tile is selected.
               The player will be able to select a different tile if preferred */
            if(g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
            {
                tileIdToUse = g->players[g->curPlayer].reserveTileId;
                g->selectedMoveIsReserveTile = TI_TRUE;
            }
            else
            {
                tileIdToUse = g->players[g->curPlayer].currentTileId;
                g->selectedMoveIsReserveTile = TI_FALSE;
            }
            g->selectedMoveTileId = tileIdToUse;
            /* Determine the legal moves for the selected tile */
            numLegalMoves = boardMarkLegalMoves(g->board, tilePoolGetTile(g->tilepool, tileIdToUse));
            break;
        case TI_GAME_STATE_TILE_PLAY:
            printf("  Changing game state to TI_GAME_STATE_TILE_PLAY\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            if(boardPlaceTile(g->board, g->selectedMoveTileX, g->selectedMoveTileY, 
                              g->selectedMoveTileId) == TI_BOARD_ILLEGAL_MOVE)
            {
                perror("Invalid tile placement!\n");
                exit(1);
            }
            gameCheckForCompletedTracks(g);
            break;
        case TI_GAME_STATE_DISCARD:
            printf("  Changing game state to TI_GAME_STATE_DISCARD\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            break;
        case TI_GAME_STATE_END_TURN:
            printf("  Changing game state to TI_GAME_STATE_END_TURN\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            g->playerHasDrawn = TI_FALSE;
            /* increment the player number */
            g->curPlayer = g->curPlayer + 1;
            if(g->curPlayer >= g->numPlayers)
            {
                /* If we've reached the end of the player list, wrap around to player 1 */
                g->curPlayer = 0;
            }
            /* Check for end of game */
            if(gameCheckForEndOfGame(g) == TI_TRUE)
            {
                gameSetGameState(g, TI_GAME_STATE_GAME_FINISHED);
            }
            else
            {
                /* Set the initial game state now that the next player is up */
                gameSetGameState(g, TI_GAME_STATE_SELECT_ACTION);
            }
            break;
        case TI_GAME_STATE_CONFIRM_EXIT:
            printf("  Changing game state to TI_GAME_STATE_CONFIRM_EXIT\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            break;
        case TI_GAME_STATE_GAME_FINISHED:
            printf("  Changing game state to TI_GAME_STATE_GAME_FINISHED\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            break;
        case TI_GAME_STATE_COMPUTER_MOVE:
            printf("  Changing game state to TI_GAME_STATE_COMPUTER_MOVE\n");
            g->previousGameState = g->gameState;
            g->gameState = state;
            g->gameStateChanged = TI_TRUE;
            break;
        default:
            return TI_ERROR;
            break;
    }

    return TI_OK;
}

/****************************************************************************
* gameCheckForEndOfGame - see tiGame.h for description
****************************************************************************/
int gameCheckForEndOfGame(Game *g)
{
    int gameEnded = TI_TRUE;
    int counter;

    /* If there are tiles in the tilepool, the game isn't over yet */
    if(g->tilepool->numUnplayedTiles > 0)
    {
        gameEnded = TI_FALSE;
    }

    /* If any player still has tiles left, the game isn't over */
    for(counter=0; counter<g->numPlayers; counter++)
    {
        if(g->players[counter].currentTileId != TI_TILE_NO_TILE ||
           g->players[counter].reserveTileId != TI_TILE_NO_TILE)
        {
            gameEnded = TI_FALSE;
        }
    }

    return gameEnded;
}

/****************************************************************************
* gameDiscardTile - see tiGame.h for description
****************************************************************************/
int gameDiscardTile(Game *g)
{

    int result;

    result = tilePoolReturnTileToPool(g->tilepool, g->selectedMoveTileId);
    if(result != TI_OK)
    {
        return TI_ERROR;
    }

    if(g->selectedMoveIsReserveTile == TI_TRUE)
    {
        g->players[g->curPlayer].reserveTileId = TI_TILE_NO_TILE;
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
    }

    return TI_OK;
}

/****************************************************************************
* gameDestroy - see tiGame.h for description
****************************************************************************/
int gameDestroy(Game **g)
{
    tilePoolDestroy(&((*g)->board->tp));
    boardDestroy(&((*g)->board));
    free(*g);
    *g = NULL;
    return TI_OK;
}

/****************************************************************************
* gameCheckForCompletedTracks - see tiGame.h for description
****************************************************************************/
int gameCheckForCompletedTracks(Game *g)
{
    int counter, result;
    int stationX, stationY, stationExit;
    int oldX, oldY, oldExit;
    int newX, newY, newExit, newType;
    int loopCatcher;
    int loopLimit=255;

    for(counter=0; counter<TI_BOARD_NUM_STATIONS; counter++)
    {
        boardGetStationInfo(counter,&stationX,&stationY,&stationExit);
        if(g->board->b[stationX][stationY].type != TI_BOARDSQUARE_TYPE_STATION)
        {
            perror("boardCheckForCompletedTracks: starting point isn't station");
            return TI_ERROR;
        }
        newType = boardFindNextTrackSection(g->board, stationX, stationY, stationExit,
                                            &newX, &newY, &newExit);

        loopCatcher = 0;
        while((newType == TI_BOARDSQUARE_TYPE_PLAYED_TILE) &&
            (loopCatcher < loopLimit))
        {
            loopCatcher++;
            oldX = newX;
            oldY = newY;
            oldExit = tileGetExit(
                        &(g->board->tp->t[g->board->b[newX][newY].tileIndex]),
                        newExit);
            newType = boardFindNextTrackSection(g->board, oldX, oldY, oldExit,
                                                &newX, &newY, &newExit);
        }

        if(loopCatcher >= loopLimit)  /* Broke out from possible infinite loop */
        {
            perror("boardCheckForCompletedTracks: infinite loop caught");
            return TI_ERROR;
        }

        /* If the destination isn't a station of some kind, the track
        isn't complete  */
        if(g->board->b[newX][newY].type == TI_BOARDSQUARE_TYPE_STATION ||
        g->board->b[newX][newY].type == TI_BOARDSQUARE_TYPE_CENTRAL)
        {
            if(g->board->trackStatus[counter] != TI_BOARD_TRACK_PROCESSED)
            {
                g->board->trackStatus[counter] = TI_BOARD_TRACK_COMPLETE;
                gameProcessCompletedTrack(g, counter);
            }
        }
    }
    return TI_OK;
}

/****************************************************************************
* gameProcessCompletedTrack - see tiGame.h for description
****************************************************************************/
int gameProcessCompletedTrack(Game *g, int station)
{
    int stationX, stationY, stationExit;
    int score, player, destination;

    if(g->board->trackStatus[station] != TI_BOARD_TRACK_COMPLETE)
    {
        /* Do nothing, the track is incomplete or processed */
        return TI_OK;
    }

    /* If the number of players results in a board with stations that don't
       belong to anyone, and one of those tracks is completed, ignore it.
     */
    if(g->board->playerStations[g->numPlayers][station] == TI_BOARD_NO_TRAIN)
    {
        return TI_OK;
    }

    if(station < 0 || station >= TI_BOARD_NUM_STATIONS)
    {
        perror("boardProcessCompletedTrack: station index out of range");
        return TI_ERROR;
    }

    score = boardCalculateTrackScore(g->board, station, TI_TILE_NO_TILE, NULL, &destination);

    /* Determine which player gets the points */
    player = g->board->playerStations[g->numPlayers][station];
    g->players[player-1].score += score;
    g->players[player-1].numStationsComplete++;

    /* Mark the starting station as empty and the track as processed */
    boardGetStationInfo(station, &stationX, &stationY, &stationExit);
    g->board->b[stationX][stationY].trainPresent = TI_BOARD_NO_TRAIN;
    g->board->trackStatus[station] = TI_BOARD_TRACK_PROCESSED;

    return TI_OK;
}

/****************************************************************************
* gameGetGlobalGameInstance - see tiGame.h for description
****************************************************************************/
Game *gameGetGlobalGameInstance(void)
{
    return GameInstance;
}
