/****************************************************************************
*
* tiBoard.c - Game board access functions
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
* boardInitialize - see tiBoard.h for description
****************************************************************************/
Board *boardInitialize(TilePool *t, char *stationInitFile)
{
    Board *b;
    int counter, counter2;
    int result;
    FILE *stationFP;

    b = malloc(sizeof(Board));
    if(b == NULL)
    {
        return NULL;
    }

    /* Mark each of the stations as having incomplete tracks */
    for(counter=0;counter<TI_BOARD_NUM_STATIONS;counter++)
    {
        b->trackStatus[counter] = TI_BOARD_TRACK_NOT_COMPLETE;
    }

    /* Set each board position to 'illegal move' -- a function will be
    called for each player turn that determines which moves are legal */
    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_HEIGHT;counter2++)
        {
            b->legalMove[counter][counter2] = TI_BOARD_ILLEGAL_MOVE;
        }
    }

    /* Set up the BoardSquare data for each board position */
    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_HEIGHT;counter2++)
        {
            b->b[counter][counter2].tileIndex = TI_TILE_NO_TILE;
            b->b[counter][counter2].trainPresent = TI_BOARD_NO_TRAIN;
        }
    }

    /* Set the outer station squares */
    for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
    {
        b->b[counter][0].type = TI_BOARDSQUARE_TYPE_STATION;
        b->b[counter][TI_BOARD_HEIGHT-1].type = TI_BOARDSQUARE_TYPE_STATION;
    }
    for(counter=1;counter<TI_BOARD_HEIGHT-1;counter++)
    {
        b->b[0][counter].type = TI_BOARDSQUARE_TYPE_STATION;
        b->b[TI_BOARD_WIDTH-1][counter].type = TI_BOARDSQUARE_TYPE_STATION;
    }
    /* Set the tile type squares */
    for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
    {
        for(counter2=1;counter2<TI_BOARD_HEIGHT-1;counter2++)
        {
            b->b[counter][counter2].type = TI_BOARDSQUARE_TYPE_TILE;
        }
    }
    /* Set the central station squares */
    b->b[4][4].type = TI_BOARDSQUARE_TYPE_CENTRAL;
    b->b[4][5].type = TI_BOARDSQUARE_TYPE_CENTRAL;
    b->b[5][4].type = TI_BOARDSQUARE_TYPE_CENTRAL;
    b->b[5][5].type = TI_BOARDSQUARE_TYPE_CENTRAL;
    /* Set the corner squares */
    b->b[0][0].type = TI_BOARDSQUARE_TYPE_CORNER;
    b->b[0][TI_BOARD_HEIGHT-1].type = TI_BOARDSQUARE_TYPE_CORNER;
    b->b[TI_BOARD_WIDTH-1][0].type = TI_BOARDSQUARE_TYPE_CORNER;
    b->b[TI_BOARD_WIDTH-1][TI_BOARD_HEIGHT-1].type = TI_BOARDSQUARE_TYPE_CORNER;

    /* Assign the tile pool to the board */
    b->tp = t;

    /* If the name of a station data file was provided, load it */
    if(stationInitFile != NULL)
    {
        stationFP = fopen(stationInitFile, "r");
        if(stationFP == NULL)
        {
            perror("boardInitialize: unable to open station file");
            return NULL;
        }
        else
        {
            result = boardParseStationFile(b, stationFP);
            if(result == TI_ERROR)
            {
                perror("boardInitialize: unable to parse station file");
                fclose(stationFP);
                return NULL;
            }
            fclose(stationFP);
        }
    }

    /* Return the board pointer */
    return b;
}

/****************************************************************************
* boardDestroy - see tiBoard.h for description
****************************************************************************/
int boardDestroy(Board **b)
{
    free(*b);
    *b = NULL;
    return TI_OK;
}

/****************************************************************************
* boardParseStationFile - see tiBoard.h for description
****************************************************************************/
int boardParseStationFile(Board *b, FILE *fp)
{
    char curLine[TI_LINE_MAX];
    char *lineCopy, *tokenCopy, *indexToken, *stationsToken, *token;
    int  counter, result, tokenCount;
    int  playerNum, numPlayers;
    const char indexDelim[] = ":";
    const char delimiters[] = " \n";
    int  gotError = 0;

    while(fgets(curLine, TI_LINE_MAX, fp) != NULL)
    {
        if(curLine[0] == '#') /* Non-comment */
        {
            continue;
        }

        /* Separate each line into the two desired tokens */
        lineCopy = strdup(curLine);
        indexToken = strtok(lineCopy, indexDelim);
        result = sscanf(indexToken, "%d", &numPlayers);
        if(result != 1 || numPlayers < TI_MIN_PLAYERS || numPlayers > TI_MAX_PLAYERS)
        {
            free(curLine);
            free(lineCopy);
            perror("boardParseStationFile: Illegal number of players");
            return TI_ERROR;
        }
        stationsToken = strtok(NULL, indexDelim);
        /* If there are less than three, return an error */
        if((indexToken == NULL) || (stationsToken == NULL))
        {
            free(curLine);
            free(lineCopy);
            perror("boardParseStationFile: parse error");
            return TI_ERROR;
        }
        tokenCopy = strdup(stationsToken);
        token = strtok(tokenCopy, delimiters);
        tokenCount = 1;
        while(token != NULL && tokenCount <= TI_BOARD_NUM_STATIONS)
        {
            result = sscanf(token, "%d", &playerNum);
            if(result != 1 || playerNum < TI_BOARD_NO_TRAIN || playerNum > TI_BOARD_P6_TRAIN)
            {
                perror("boardParseStationFile: invalid line");
                free(curLine);
                free(lineCopy);
                free(tokenCopy);
                return TI_ERROR;
            }
            b->playerStations[numPlayers][tokenCount-1] = playerNum;
            tokenCount++;
            token = strtok(NULL, delimiters);
        }

        free(lineCopy);
        free(tokenCopy);
    }

    /* Return error if an error occurs while reading the file */
    if(ferror(fp))
    {
        free(curLine);
        perror("tilePoolParseTilePoolFile: file read error");
        return TI_ERROR;
    }

    return TI_OK;
}

/****************************************************************************
* boardFindNextTrackSection - see tiBoard.h for description
****************************************************************************/
int boardFindNextTrackSection(Board *b, int x, int y, int exit,
                            int *newX, int *newY, int *enter)
{
    switch(exit)
    {
        case 0:
            *newX = x;
            *newY = y - 1;
            *enter = 5;
            break;
        case 1:
            *newX = x;
            *newY = y - 1;
            *enter = 4;
            break;
        case 2:
            *newX = x+1;
            *newY = y;
            *enter = 7;
            break;
        case 3:
            *newX = x+1;
            *newY = y;
            *enter = 6;
            break;
        case 4:
            *newX = x;
            *newY = y+1;
            *enter = 1;
            break;
        case 5:
            *newX = x;
            *newY = y+1;
            *enter = 0;
            break;
        case 6:
            *newX = x - 1;
            *newY = y;
            *enter = 3;
            break;
        case 7:
            *newX = x - 1;
            *newY = y;
            *enter = 2;
            break;
        default:
            *newX = x;
            *newY = y;
            *enter = exit;
            break;
    }

    /* Now that the new x and y positions and entry point were found,
    * determine what the square type is and return it */
    return b->b[*newX][*newY].type;
}

/****************************************************************************
* boardMarkLegalMoves - see tiBoard.h for description
****************************************************************************/
int boardMarkLegalMoves(Board *b, Tile *t)
{	
    int counter, counter2, legalMoves;

    /* Mark all moves as legal */
    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_HEIGHT;counter2++)
        {
            b->legalMove[counter][counter2] = TI_BOARD_LEGAL_MOVE;
        }
    }

    /* Mark all of the stations and corner squares as illegal */
    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        b->legalMove[counter][0] = TI_BOARD_ILLEGAL_MOVE;
        b->legalMove[counter][TI_BOARD_HEIGHT-1] = TI_BOARD_ILLEGAL_MOVE;
    }
    for(counter=0;counter<TI_BOARD_HEIGHT;counter++)
    {
        b->legalMove[0][counter] = TI_BOARD_ILLEGAL_MOVE;
        b->legalMove[TI_BOARD_WIDTH-1][counter] = TI_BOARD_ILLEGAL_MOVE;
    }
    b->legalMove[4][4] = TI_BOARD_ILLEGAL_MOVE;
    b->legalMove[4][5] = TI_BOARD_ILLEGAL_MOVE;
    b->legalMove[5][4] = TI_BOARD_ILLEGAL_MOVE;
    b->legalMove[5][5] = TI_BOARD_ILLEGAL_MOVE;

    /* Mark all of the already played squares as illegal */
    for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
    {
        for(counter2=1;counter2<TI_BOARD_HEIGHT-1;counter2++)
        {
            if(b->b[counter][counter2].type == TI_BOARDSQUARE_TYPE_PLAYED_TILE)
            {
                b->legalMove[counter][counter2] = TI_BOARD_ILLEGAL_MOVE;
            }
        }
    }

    /* Check the inner station squares for adjacent tiles */
    for(counter=2;counter<TI_BOARD_WIDTH-2;counter++)
    {
        for(counter2=2;counter2<TI_BOARD_HEIGHT-2;counter2++)
        {
            if((b->b[counter+1][counter2].type != TI_BOARDSQUARE_TYPE_PLAYED_TILE) &&
            (b->b[counter-1][counter2].type != TI_BOARDSQUARE_TYPE_PLAYED_TILE) &&
            (b->b[counter][counter2-1].type != TI_BOARDSQUARE_TYPE_PLAYED_TILE) &&
            (b->b[counter][counter2+1].type != TI_BOARDSQUARE_TYPE_PLAYED_TILE))
            {
                b->legalMove[counter][counter2] = TI_BOARD_ILLEGAL_MOVE;
            }
        }
    }

    /* Check the outer station squares for track lengths of 1
    This check will be performed as 8 different checks -- 4 sides and
    the 4 corners */
    for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
    {
        if(tileGetExit(t, 0) == 1)
        {
            b->legalMove[counter][1] = TI_BOARD_ILLEGAL_MOVE;
        }
        if(tileGetExit(t, 5) == 4)
        {
            b->legalMove[counter][TI_BOARD_HEIGHT-2] = TI_BOARD_ILLEGAL_MOVE;
        }
    }
    for(counter=1;counter<TI_BOARD_HEIGHT-1;counter++)
    {
        if(tileGetExit(t,3) == 2)
        {
            b->legalMove[TI_BOARD_WIDTH-2][counter] = TI_BOARD_ILLEGAL_MOVE;
        }
        if(tileGetExit(t,7) == 6)
        {
            b->legalMove[1][counter] = TI_BOARD_ILLEGAL_MOVE;
        }
    }
    if(tileGetExit(t,6) == 1 || tileGetExit(t,0) == 7)
    {
        b->legalMove[1][1] = TI_BOARD_ILLEGAL_MOVE;
    }
    if(tileGetExit(t,2) == 1 || tileGetExit(t,3) == 0)
    {
        b->legalMove[TI_BOARD_WIDTH-2][1] = TI_BOARD_ILLEGAL_MOVE;
    }
    if(tileGetExit(t,4) == 3 || tileGetExit(t,5) == 2)
    {
        b->legalMove[TI_BOARD_WIDTH-2][TI_BOARD_HEIGHT-2] = TI_BOARD_ILLEGAL_MOVE;
    }
    if(tileGetExit(t,5) == 6 || tileGetExit(t,4) == 7)
    {
        b->legalMove[1][TI_BOARD_HEIGHT-2] = TI_BOARD_ILLEGAL_MOVE;
    }

    /* count the legal moves */
    legalMoves = 0;
    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_HEIGHT;counter2++)
        {
            if(b->legalMove[counter][counter2] == TI_BOARD_LEGAL_MOVE)
            {
                legalMoves++;
            }
        }
    }

    /* If there are no legal moves, then check the tilepool.  If no cards
    remain, mark all normally illegal empty spaces as legal, otherwise,
    return the number of legal moves (0).	*/
    if(legalMoves == 0 && b->tp->numUnplayedTiles == 0)
    {
        for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
        {
            for(counter2=1;counter2<TI_BOARD_HEIGHT-1;counter2++)
            {
                if(b->b[counter][counter2].type == TI_BOARDSQUARE_TYPE_TILE)
                {
                    b->legalMove[counter][counter2] = TI_BOARD_LEGAL_MOVE;
                }
            }
        }
    }
    else
    {
        return legalMoves;
    }

    /* count the legal moves (again) */
    legalMoves = 0;
    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_HEIGHT;counter2++)
        {
            if(b->legalMove[counter][counter2] == TI_BOARD_LEGAL_MOVE)
            {
                legalMoves++;
            }
        }
    }

    return legalMoves;
}

/****************************************************************************
* boardGetStationInfo - see tiBoard.h for description
****************************************************************************/
void boardGetStationInfo(int station, int *x, int *y, int *exit)
{
    /* Note: this code is specific to an 8x8 board with 32 stations */
    if(station >=0 && station < 8)
    {
        *x = 1 + station;
        *y = 0;
        *exit = 5;
    }
    if(station >=8 && station < 16)
    {
        *x = 9;
        *y = 1 + (station-8);
        *exit = 7;
    }
    if(station >=16 && station < 24)
    {
        *x = 8 - (station - 16);
        *y = 9;
        *exit = 1;
    }
    if(station >=24 && station < 32)
    {
        *x = 0;
        *y = 8 - (station - 24);
        *exit = 3;
    }
}

/****************************************************************************
* boardGetStationNumber - see tiBoard.h for description
****************************************************************************/
int boardGetStationNumber(int x, int y)
{
    int station;

    /* Note: this code is specific to an 8x8 board with 32 stations */
    /* Left stations (24-31) */
    if(x == 0)
    {
        if(y >= 1 && y < TI_BOARD_HEIGHT-1)
        {
            station = 32 - y;
        }
        else
        {
            station = -1;
        }
    }
    /* Right stations (8-15) */
    else if(x == TI_BOARD_WIDTH - 1)
    {
        if(y >=1 && y < TI_BOARD_HEIGHT-1)
        {
            station = 7 + y;
        }
        else
        {
            station = -1;
        }
    }
    /* Top stations (0-7) */
    else if(y == 0)
    {
        if(x >= 1 && x < TI_BOARD_WIDTH-1)
        {
            station = x - 1;
        }
        else
        {
            station = -1;
        }
    }
    /* Bottom stations (16-23) */
    else if(y == TI_BOARD_HEIGHT - 1)
    {
        if(x >=1 && x < TI_BOARD_WIDTH-1)
        {
            station = 24 - x;
        }
        else
        {
            station = -1;
        }
    }
    /* Not a station */
    else
    {
        station = -1;
    }

    return station;
}

/****************************************************************************
* boardCalculateTrackScore - see tiBoard.h for description
****************************************************************************/
int boardCalculateTrackScore(Board *b, int station, int passThruTileId, 
                             int *passThruTile, int *destination)
{
    int stationX, stationY, stationExit;
    int newX, newY, newExit, newType;
    int oldX, oldY, oldExit, oldType;
    int loopCatcher;
    int loopLimit = 255;		/* Catch infinite looping */
    int score = 0;

    if(passThruTileId != TI_TILE_NO_TILE && passThruTile != NULL)
    {
        *passThruTile = TI_FALSE;
    }

    /* Determine the starting point for calculating score */
    boardGetStationInfo(station, &stationX, &stationY, &stationExit);

    if(b->b[stationX][stationY].type != TI_BOARDSQUARE_TYPE_STATION)
    {
        perror("boardCalculateTrackScore: starting point isn't station");
        return TI_ERROR;
    }

    if(b->b[stationX][stationY].trainPresent == TI_BOARD_NO_TRAIN)
    {
        perror("boardCalculateTrackStore: no train at station");
        return TI_ERROR;
    }

    newType = boardFindNextTrackSection(b, stationX, stationY, stationExit,
                                        &newX, &newY, &newExit);

    loopCatcher = 0;
    score = 0;
    while((newType == TI_BOARDSQUARE_TYPE_PLAYED_TILE) &&
        (loopCatcher < loopLimit))
    {
        if(passThruTileId != TI_TILE_NO_TILE &&
           b->b[newX][newY].tileIndex == passThruTileId &&
           passThruTile != NULL)
        {
            *passThruTile = TI_TRUE;
        }
        score++;
        loopCatcher++;
        oldX = newX;
        oldY = newY;
        oldExit = tileGetExit(
                    &(b->tp->t[b->b[newX][newY].tileIndex]),
                    newExit);
        newType = boardFindNextTrackSection(b, oldX, oldY, oldExit,
                                            &newX, &newY, &newExit);
    }

    if(loopCatcher >= loopLimit)  /* Broke out from possible infinite loop */
    {
        perror("boardCalculateTrackScore: infinite loop caught");
        return TI_ERROR;
    }

    /* Determine the type of tile the track stops at */
    *destination = b->b[newX][newY].type;

    /* If the destination isn't a station of some kind, that's bad */
    if(b->b[newX][newY].type != TI_BOARDSQUARE_TYPE_STATION &&
       b->b[newX][newY].type != TI_BOARDSQUARE_TYPE_CENTRAL &&
       passThruTileId == TI_TILE_NO_TILE)
    {
        perror("boardCalculateTrackScore: destination not station");
        return TI_ERROR;
    }

    /* If the destination was a central station, double the score */
    if(b->b[newX][newY].type == TI_BOARDSQUARE_TYPE_CENTRAL)
    {
        score = score * 2;
    }

    return score;
}

/****************************************************************************
* boardPlaceTile - see tiBoard.h for description
****************************************************************************/
int boardPlaceTile(Board *b, int x, int y, int tileIndex)
{
    int result;

    /* If the indices are out of range, the move is illegal */
    if(x < 0 || y <0 || x >= TI_BOARD_WIDTH || y >= TI_BOARD_HEIGHT)
    {
        return TI_BOARD_ILLEGAL_MOVE;
    }

    /* If the square is a station, then the move is illegal */
    if(b->b[x][y].type != TI_BOARDSQUARE_TYPE_TILE)
    {
        return TI_BOARD_ILLEGAL_MOVE;
    }

    /* If the square is marked as an illegal move, then don't place
    the tile */
    if(b->legalMove[x][y] != TI_BOARD_LEGAL_MOVE)
    {
        return TI_BOARD_ILLEGAL_MOVE;
    }

    /* As a sanity check, if a tile is already placed here, don't
    place the tile */
    if(b->b[x][y].tileIndex != TI_TILE_NO_TILE)
    {
        return TI_BOARD_ILLEGAL_MOVE;
    }

    /* If all of these checks pass, place the tile */
    result = boardSquareSetTileIndex(&(b->b[x][y]), tileIndex);

    if(result == TI_ERROR)
    {
        return TI_BOARD_ILLEGAL_MOVE;
    }

    return TI_BOARD_LEGAL_MOVE;
}

/****************************************************************************
* boardRemoveTile - see tiBoard.h for description
****************************************************************************/
int boardRemoveTile(Board *b, int x, int y)
{
    int result;

    /* If the indices are out of range, the move is illegal */
    if(x < 0 || y <0 || x >= TI_BOARD_WIDTH || y >= TI_BOARD_HEIGHT)
    {
        return TI_ERROR;
    }

    /* If the square is not a played tile, then the move is illegal */
    if(b->b[x][y].type != TI_BOARDSQUARE_TYPE_PLAYED_TILE)
    {
        return TI_ERROR;
    }

    /* If a tile isn't there, then it can't be removed */
    if(b->b[x][y].tileIndex == TI_TILE_NO_TILE)
    {
        return TI_ERROR;
    }

    b->b[x][y].tileIndex = TI_TILE_NO_TILE;
    b->b[x][y].type = TI_BOARDSQUARE_TYPE_TILE;

    return TI_OK;
}

/****************************************************************************
* boardIsLegalMove - see tiBoard.h for description
****************************************************************************/
int boardIsLegalMove(Board *b, int x, int y)
{
    return (b->legalMove[x][y] == TI_BOARD_LEGAL_MOVE) ? TI_TRUE : TI_FALSE;
}

/****************************************************************************
* boardSquareSetTileIndex - see tiBoard.h for description
****************************************************************************/
int boardSquareSetTileIndex(BoardSquare *bsq, int tileIndex)
{
    if(bsq->type == TI_BOARDSQUARE_TYPE_TILE)
    {
        bsq->tileIndex = tileIndex;
        bsq->type = TI_BOARDSQUARE_TYPE_PLAYED_TILE;
        return TI_OK;
    }

    return TI_ERROR;
}

/****************************************************************************
* boardCopyBoard- see tiBoard.h for description
****************************************************************************/
Board *boardCopyBoard(Board *b)
{
    Board *newBoard;
    int counter, counter2;

    /* Copy all elements of the given board into a new one */
    newBoard = malloc(sizeof(Board));
    if(newBoard == NULL)
    {
        return NULL;
    }

    newBoard->tp = malloc(sizeof(TilePool));
    if(newBoard->tp == NULL)
    {
        free(newBoard);
        return NULL;
    }

    for(counter=0;counter<TI_BOARD_WIDTH;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_HEIGHT;counter2++)
        {
            newBoard->b[counter][counter2].type = b->b[counter][counter2].type;
            newBoard->b[counter][counter2].trainPresent = b->b[counter][counter2].trainPresent;
            newBoard->b[counter][counter2].tileIndex = b->b[counter][counter2].tileIndex;
            newBoard->legalMove[counter][counter2] = b->legalMove[counter][counter2];
        }
    }

    for(counter=0;counter<TI_BOARD_NUM_STATIONS;counter++)
    {
        newBoard->trackStatus[counter] = b->trackStatus[counter];
    }

    for(counter=0;counter<TI_MAX_PLAYERS+1;counter++)
    {
        for(counter2=0;counter2<TI_BOARD_NUM_STATIONS;counter2++)
        {
            newBoard->playerStations[counter][counter2] = b->playerStations[counter][counter2];
        }
    }

    for(counter=0;counter<TI_TILEPOOL_NUM_TILES;counter++)
    {
        newBoard->tp->playedTiles[counter] = b->tp->playedTiles[counter];
        newBoard->tp->unplayedTiles[counter] = b->tp->unplayedTiles[counter];
        newBoard->tp->tileStatus[counter] = b->tp->tileStatus[counter];
        newBoard->tp->t[counter].orientation = b->tp->t[counter].orientation;
        newBoard->tp->t[counter].tileStripOffset = b->tp->t[counter].tileStripOffset;
        for(counter2=0;counter2<TI_TILE_NUM_EXITS;counter2++)
        {
            newBoard->tp->t[counter].exits[counter2] =
                b->tp->t[counter].exits[counter2];
            newBoard->tp->t[counter].defaultExits[counter2] =
                b->tp->t[counter].exits[counter2];
        }
    }
    newBoard->tp->numPlayedTiles = b->tp->numPlayedTiles;
    newBoard->tp->numUnplayedTiles = b->tp->numUnplayedTiles;

    return newBoard;
}
