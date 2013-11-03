/****************************************************************************
 *
 * tiComputerAI.c - Controls computer opponent move selection
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
#include "tiComputerAI.h"
#include "tiBoard.h"
#include "tiRenderSDL.h"
#include "tiCoords.h"
#include "tiPlayer.h"
#include "tiGame.h"
#include "tiComputerAI.h"

/****************************************************************************
 * computerDetermineNextMove - see tiComputerAI.h for description
 ****************************************************************************/
ComputerAIPacket *computerDetermineNextMove(ComputerAIPacket *lastMove)
{
    ComputerAIPacket *p;
    Game *g;
    int tileQuantity;
    AIMoveEval *evalList, *selectedMove;
    Board *boardCopy;
    int holdingPrimary, holdingSecondary, legalMoves, legalMoves2;
    int tilesInPool;

    /* How to decide on a move (V1):
     *
     *  The following steps are used to help decide on a move.
     *   (a temporary board should be created to test move strength)
     *
     * x 1.  If the computer has no tiles, draw one.
     * x 2.  If the computer has one tile, but it has no legal moves,
     *        draw one.
     *  3.  If the computer has two tiles, but neither has legal moves,
     *      discard one (pick one at random)
     *  4.  If the computer is to draw a tile, but no tiles remain,
     *      and any held tile cannot be played, then pass, otherwise
     *      play.
     *  5.  If the computer has made more than 2 moves in any particular
     *      round, then pass (this is to prevent an infinite move loop)
     *  6.  When the computer is to play, do the following for each held tile:
     *       - Place the tile in each legal location
     *       - For each such location:
     *          -  the value of the location starts at 0
     *          -  calculate the value of all incomplete tracks of the current
     *             player for every station that still has a train on it,
     *             *if the played tile makes up part of that track*, and add them
     *             together.  If no stations pass through it, the score will be
     *             zero at this point.  
     *          - Repeat the calculation for each of the incomplete tracks of
     *            other players that pass through that tile.  For each one that
     *            does, subtract the value of those incomplete tracks.
     *       - When finished, sort the values from high to low.  Depending on the
     *         AI level, pick a move that's either highly ranked, ranked in the
     *         middle, or lowest ranked.  If more than 5 moves exist, don't pick
     *         the absolute best or worst; pick the second best or second worst.
     */

    /*
     * How to store moves in order to allow them to be ranked:
     *
     * A structure with the following elements:
     *
     *   int tileType (primary or secondary)
     *   int tileX    (the x position of the tile)
     *   int tileY    (the y position of the tile)
     *   int value    (the value of the this move)
     *
     * Create a linked list of this structure and add an element for each
     * move evaluated.
     */

    g = gameGetGlobalGameInstance();
    boardCopy = boardCopyBoard(g->board);

    p = malloc(sizeof(ComputerAIPacket));
    if(p == NULL)
    {
        return NULL;
    }

    evalList = NULL;

    tileQuantity = 0;
    holdingPrimary = TI_FALSE;
    holdingSecondary = TI_FALSE;
    if(g->players[g->curPlayer].currentTileId != TI_TILE_NO_TILE)
    {
        tileQuantity++;
        holdingPrimary = TI_TRUE;
    }
    if(g->players[g->curPlayer].reserveTileId != TI_TILE_NO_TILE)
    {
        tileQuantity++;
        holdingSecondary = TI_TRUE;
    }
    tilesInPool = g->tilepool->numUnplayedTiles;

    /* The computer hasn't done anything yet */
    if(lastMove == NULL)
    {
        /* No tiles in hand */
        if(tileQuantity == 0)
        {
            /* If there are tiles left, draw one */
            if(tilesInPool > 0)
            {
                p->moveType = TI_CPU_MOVE_DRAW;
                printf("    - Computer's move is 'TI_CPU_MOVE_DRAW'\n");
            }
            /* If there are no tiles left, pass */
            else
            {
                p->moveType = TI_CPU_MOVE_END_TURN;
                printf("    - Computer's move is 'TI_CPU_MOVE_END_TURN (pass)'\n");
            }
        }
        /* One tile in hand */
        else if(tileQuantity == 1)
        {
            if(holdingPrimary == TI_TRUE)
            {
                legalMoves = boardMarkLegalMoves(boardCopy, 
                               tilePoolGetTile(g->tilepool, g->players[g->curPlayer].currentTileId));
            }
            else
            {
                legalMoves = boardMarkLegalMoves(boardCopy,
                               tilePoolGetTile(g->tilepool, g->players[g->curPlayer].reserveTileId));
            }
            /* No legal moves with this tile */
            if(legalMoves == 0)
            {
                /* If tiles remain, draw another */
                if(tilesInPool > 0)
                {
                    p->moveType = TI_CPU_MOVE_DRAW;
                    printf("    - Computer's move is 'TI_CPU_MOVE_DRAW'\n");
                }
                /* If no tiles remain, pass */
                else
                {
                    p->moveType = TI_CPU_MOVE_END_TURN;
                    printf("    - Computer's move is 'TI_CPU_MOVE_END_TURN (pass)'\n");
                }
            }
            /* This tile has legal moves -- analyze them and pick one */
            else
            {
                /* Analyze all board positions with this tile and pick a spot to play */
                if(holdingPrimary == TI_TRUE)
                {
                    computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].currentTileId, TI_CPU_HELD_TILE_PRIMARY);
                }
                else
                {
                    computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].reserveTileId, TI_CPU_HELD_TILE_RESERVE);
                }
                computerMoveEvalListSort(&evalList);
                selectedMove = computerMoveSelectListMove(&evalList);
                p->moveType = TI_CPU_MOVE_PLAY;
                p->moveX = selectedMove->tileX;
                p->moveY = selectedMove->tileY;
                p->heldTile = selectedMove->tileType;
                printf("    - Computer's move is 'TI_CPU_MOVE_PLAY' (%c at (%d, %d))\n", (p->heldTile == TI_CPU_HELD_TILE_PRIMARY) ? 'P' : 'S', p->moveX, p->moveY);
            }
        }
        /* Two tiles in hand */
        else
        {
            legalMoves = boardMarkLegalMoves(boardCopy,
                           tilePoolGetTile(g->tilepool, g->players[g->curPlayer].currentTileId));

            /* Analyze all legal moves for the primary tile */
            if(legalMoves > 0)
            {
                computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].currentTileId, TI_CPU_HELD_TILE_PRIMARY);
            }
            legalMoves2 = boardMarkLegalMoves(boardCopy,
                           tilePoolGetTile(g->tilepool, g->players[g->curPlayer].reserveTileId));
            /* Analyze all legal moves for the secondary tile */
            if(legalMoves2 > 0)
            {
                computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].reserveTileId, TI_CPU_HELD_TILE_RESERVE);
            }

            /* If no legal moves are available, discard the secondary tile */
            if(legalMoves == 0 && legalMoves2 == 0)
            {
                p->moveType = TI_CPU_MOVE_DISCARD;
                p->heldTile = TI_CPU_HELD_TILE_RESERVE;
                printf("    - Computer's move is 'TI_CPU_MOVE_DISCARD (secondary)'\n");
            }
            /* Legal moves are available, pick one */
            else
            {
                computerMoveEvalListSort(&evalList);
                selectedMove = computerMoveSelectListMove(&evalList);
                p->moveType = TI_CPU_MOVE_PLAY;
                p->moveX = selectedMove->tileX;
                p->moveY = selectedMove->tileY;
                p->heldTile = selectedMove->tileType;
                printf("    - Computer's move is 'TI_CPU_MOVE_PLAY' (%c at (%d, %d))\n", (p->heldTile == TI_CPU_HELD_TILE_PRIMARY) ? 'P' : 'S', p->moveX, p->moveY);
            }
        }
    }
    /* The computer has already made a move this turn */
    else
    {
        /* If the last move was play, pass or discard, the turn is over */
        if(lastMove->moveType == TI_CPU_MOVE_PLAY ||
           lastMove->moveType == TI_CPU_MOVE_DISCARD ||
           lastMove->moveType == TI_CPU_MOVE_END_TURN)
        {
            p->moveType = TI_CPU_MOVE_END_TURN;
            printf("    - Computer's move is 'TI_CPU_MOVE_END_TURN'\n");
            SDL_Delay((rand() % TI_CPU_PASS_DYNAMIC_DELAY) + TI_CPU_PASS_STATIC_DELAY);
        }
        /* If the last move was draw, determine what to do now */
        else
        {
            /* If no tiles, pass. (This should never happen) */
            if(tileQuantity == 0)
            {
               p->moveType = TI_CPU_MOVE_END_TURN;
               printf("    - Computer's move is 'TI_CPU_MOVE_END_TURN'\n");
               SDL_Delay((rand() % TI_CPU_PASS_DYNAMIC_DELAY) + TI_CPU_PASS_STATIC_DELAY);
            }
            /* If one tile, either pass or play, depending on whether there are legal moves */
            else if(tileQuantity == 1)
            {
                if(holdingPrimary == TI_TRUE)
                {
                    legalMoves = boardMarkLegalMoves(boardCopy, 
                                 tilePoolGetTile(g->tilepool, g->players[g->curPlayer].currentTileId));
                }
                else
                {
                    legalMoves = boardMarkLegalMoves(boardCopy,
                                 tilePoolGetTile(g->tilepool, g->players[g->curPlayer].reserveTileId));
                }
                /* If the tile has no legal moves, pass */
                if(legalMoves == 0)
                {
                    p->moveType = TI_CPU_MOVE_END_TURN;
                    printf("    - Computer's move s 'TI_CPU_MOVE_END_TURN (pass)'\n");
                    SDL_Delay((rand() % TI_CPU_DYNAMIC_DELAY) + TI_CPU_STATIC_DELAY);
                }
                /* If the tile has legal moves, analyze the available moves and play */
                else
                {
                    if(holdingPrimary == TI_TRUE)
                    {
                        computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].currentTileId, TI_CPU_HELD_TILE_PRIMARY);
                    }
                    else
                    {
                        computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].reserveTileId, TI_CPU_HELD_TILE_RESERVE);
                    }
                    computerMoveEvalListSort(&evalList);
                    selectedMove = computerMoveSelectListMove(&evalList);
                    p->moveType = TI_CPU_MOVE_PLAY;
                    p->moveX = selectedMove->tileX;
                    p->moveY = selectedMove->tileY;
                    p->heldTile = selectedMove->tileType;
                    printf("    - Computer's move is 'TI_CPU_MOVE_PLAY' (%c at (%d, %d))\n", (p->heldTile == TI_CPU_HELD_TILE_PRIMARY) ? 'P' : 'S', p->moveX, p->moveY);
                }
            }
            /* If two tiles, play or discard, depending on whether there are legal
               moves */
            else
            {
                legalMoves = boardMarkLegalMoves(boardCopy,
                               tilePoolGetTile(g->tilepool, g->players[g->curPlayer].currentTileId));

                /* Analyze all legal moves for the primary tile */
                if(legalMoves > 0)
                {
                    computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].currentTileId, TI_CPU_HELD_TILE_PRIMARY);
                }
                legalMoves2 = boardMarkLegalMoves(boardCopy,
                                tilePoolGetTile(g->tilepool, g->players[g->curPlayer].reserveTileId));
                /* Analyze all legal moves for the secondary tile */
                if(legalMoves2 > 0)
                {
                    computerMoveAnalyzeMoves(&evalList, boardCopy, g->players[g->curPlayer].reserveTileId, TI_CPU_HELD_TILE_RESERVE);
                }

                /* If no legal moves are available, discard the secondary tile */
                if(legalMoves == 0 && legalMoves2 == 0)
                {
                    p->moveType = TI_CPU_MOVE_DISCARD;
                    p->heldTile = TI_CPU_HELD_TILE_RESERVE;
                    printf("    - Computer's move is 'TI_CPU_MOVE_DISCARD (secondary)'\n");
                }
                /* Legal moves are available, pick one. */
                else
                {
                    computerMoveEvalListSort(&evalList);
                    selectedMove = computerMoveSelectListMove(&evalList);
                    p->moveType = TI_CPU_MOVE_PLAY;
                    p->moveX = selectedMove->tileX;
                    p->moveY = selectedMove->tileY;
                    p->heldTile = selectedMove->tileType;
                    printf("    - Computer's move is 'TI_CPU_MOVE_PLAY' (%c at (%d, %d))\n", (p->heldTile == TI_CPU_HELD_TILE_PRIMARY) ? 'P' : 'S', p->moveX, p->moveY);
                }
            }
        }
    }

    tilePoolDestroy(&(boardCopy->tp));
    boardDestroy(&boardCopy);
    computerMoveEvalListFree(&evalList);
    return p;
}

/****************************************************************************
 * computerMoveAnalyzeMoves - see tiComputerAI.h for description
 ****************************************************************************/
int computerMoveAnalyzeMoves(AIMoveEval **p, Board *b, int tileIndex, int heldTile)
{
    int counter, counter2, counter3;
    int value, stationX, stationY, exit, score, passThru, destination;
    Game *g;
    float weight;

    g = gameGetGlobalGameInstance();

    /* Loop through and check for each legal move */
    for(counter=1;counter<TI_BOARD_WIDTH-1;counter++)
    {
        for(counter2=1;counter2<TI_BOARD_HEIGHT-1;counter2++)
        {
            if(b->legalMove[counter][counter2] == TI_BOARD_LEGAL_MOVE)
            {
                value = 0;
                /* Play the move */
                boardPlaceTile(b, counter, counter2, tileIndex);
                /* For each station, if a train is positioned there, check for
                   partial/complete tracks, and either add or subtract the score
                   from the value, depending on who owns the station */
                for(counter3=0;counter3<TI_BOARD_NUM_STATIONS;counter3++)
                {
                    boardGetStationInfo(counter3, &stationX, &stationY, &exit);
                    if(b->b[stationX][stationY].trainPresent != TI_BOARD_NO_TRAIN)
                    {
                        weight = 1.0;
                        score = boardCalculateTrackScore(b, counter3, tileIndex, &passThru, &destination);
                        if(passThru == TI_TRUE)
                        {
                            if(destination == TI_BOARDSQUARE_TYPE_TILE)
                            {
                                weight = TI_CPU_WEIGHT_INCOMPLETE_TRACK;
                            }
                            else if(destination == TI_BOARDSQUARE_TYPE_STATION)
                            {
                                weight = TI_CPU_WEIGHT_COMPLETE_TRACK;
                            }
                            else if(destination == TI_BOARDSQUARE_TYPE_CENTRAL)
                            {
                                weight = TI_CPU_WEIGHT_COMPLETE_CENTRAL_STATION;
                            }

                            if(b->playerStations[g->numPlayers][counter3] == (g->curPlayer+1))
                            {
                                value += (score * weight);
                            }
                            else
                            {
                                value -= (score * weight);
                            }
                        }
                    }
                }

                boardRemoveTile(b, counter, counter2);
                computerMoveEvalListAdd(p, heldTile, counter, counter2, value);
            }
        }
    }

    return TI_OK;
}

/****************************************************************************
 * computerMoveEvalListSort - see tiComputerAI.h for description
 ****************************************************************************/
AIMoveEval *computerMoveEvalListSort(AIMoveEval **list)
{
    AIMoveEval *p, *q, *e, *tail;
    int insize, nmerges, psize, qsize, i;

    /* Perform a merge sort on the list to sort the list elements by value.
     */
    if(*list == NULL)
    {
        return NULL;
    }

    insize = 1;

    while(1)
    {
        p = *list;
        *list = NULL;
        tail = NULL;

        nmerges = 0;

        while(p != NULL)
        {
            nmerges++;
            q = p;
            psize = 0;
            for(i = 0; i < insize; i++)
            {
                psize++;
                q = q->next;
                if(q == NULL)
                {
                    break;
                }
            }

            qsize = insize;

            while(psize > 0 || (qsize > 0 && q != NULL)) 
            {
                if(psize == 0)
                {
                    e = q;
                    q = q->next;
                    qsize--;
                }
                else if(qsize == 0 || q == NULL)
                {
                    e = p;
                    p = p->next;
                    psize--;
                }
                else if(p->value >= q->value)
                {
                    e = p;
                    p = p->next;
                    psize--;
                }
                else
                {
                    e = q;
                    q = q->next;
                    qsize--;
                }

                if(tail != NULL)
                {
                    tail->next = e;
                }
                else
                {
                    *list = e;
                }
                tail = e;
            }

            p = q;
        }
        tail->next = NULL;

        if(nmerges<=1)
        {
            AIMoveEval *ptr = *list;
            while(ptr != NULL)
            {
                ptr = ptr->next;
            }
            return *list;
        }

        insize *=2;
    }
}

/****************************************************************************
 * computerMoveSelectListMove - see tiComputerAI.h for description
 ****************************************************************************/
AIMoveEval *computerMoveSelectListMove(AIMoveEval **evalList)
{
    AIMoveEval *ctr, *movePtr;
    int numMoves, moveIndex, aiLevel, index;
    Game *g;

    g = gameGetGlobalGameInstance();
    ctr = *evalList;
    numMoves = 0;
    aiLevel = g->players[g->curPlayer].computerAiLevel;

    while(ctr != NULL)
    {
        numMoves++;
        ctr = ctr->next;
    }

    if(numMoves <= 0)
    {
        return NULL;
    }

    /* Reset the list pointer */
    ctr = *evalList;

    /* If there are less than 3 moves:
        - Best AI = best move
        - Medium AI = best move
        - Weak AI = worst move

       If there are more than 3 moves:
        - Best AI = best move
        - Medium AI = a move better than approx 2/3 of moves.  If this move
          has a negative score, pick the weakest move with a 0 or positive score
        - Weak AI = a move worse than approx 2/3 of moves, regardless of the
          move's value
     */
    if(numMoves < 3)
    {
        if(aiLevel == TI_PLAYER_AI_SMARTEST ||
           aiLevel == TI_PLAYER_AI_SMARTER)
        {
            moveIndex = 0;
            movePtr = *evalList;
        }
        else
        {
            moveIndex = (numMoves - 1);
            movePtr = *evalList;
            for(index=0;index<moveIndex;index++)
            {
                movePtr = movePtr->next;
            }
        }
    }
    else
    {
        if(aiLevel == TI_PLAYER_AI_SMARTEST)
        {
            moveIndex = 0;
            movePtr = *evalList;
        }
        else if(aiLevel == TI_PLAYER_AI_SMARTER)
        {
            moveIndex = (int)((float)numMoves - 1.0) * 0.33;
            movePtr = *evalList;
            for(index=0;index<moveIndex;index++)
            {
                movePtr = movePtr->next;
            }
            if(movePtr->value <= 0)
            {
                moveIndex = 0;
                movePtr = *evalList;
                while(movePtr->next != NULL &&
                      movePtr->next->value > 0)
                {
                    moveIndex++;
                    movePtr = movePtr->next;
                }
            }
        }
        else
        {
            moveIndex = (int)((float)numMoves - 1.0) * 0.67;
            movePtr = *evalList;
            for(index=0;index<moveIndex;index++)
            {
                movePtr = movePtr->next;
            }
        }
    }

    return movePtr;
}

/****************************************************************************
 * computerMoveEvalListAdd - see tiComputerAI.h for description
 ****************************************************************************/
AIMoveEval *computerMoveEvalListAdd(AIMoveEval **p, int type, int x, int y, int val)
{
    AIMoveEval *a = malloc(sizeof(AIMoveEval));
    a->next = *p;
    *p = a;
    a->tileType = type;
    a->tileX = x;
    a->tileY = y;
    a->value = val;
    return a;
}

/****************************************************************************
 * computerMoveEvalListRemove - see tiComputerAI.h for description
 ****************************************************************************/
void computerMoveEvalListRemove(AIMoveEval **p)
{
    AIMoveEval *n;

    if(*p != NULL)
    {
        n = *p;
        *p = (*p)->next;
        free(n);
    }
}

/****************************************************************************
 * computerMoveEvalListFree - see tiComputerAI.h for description
 ****************************************************************************/
void computerMoveEvalListFree(AIMoveEval **p)
{
    while(*p != NULL)
    {
        computerMoveEvalListRemove(p);
    }
    *p = NULL;
}
