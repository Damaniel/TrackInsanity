/****************************************************************************
 *
 * tiComputerAI.h - Header for tiComputerAI.c
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

#ifndef __TICOMPUTERAI_H__
#define __TICOMPUTERAI_H__

#include "tiTiles.h"
#include "tiBoard.h"

/* The move types are as follows:

    TI_CPU_MOVE_DRAW        - draw a tile from the tile pool
    TI_CPU_MOVE_PLAY        - play a held tile onto the board
    TI_CPU_MOVE_DISCARD     - discard a tile back into the tilepool
    TI_CPU_MOVE_END_TURN    - a 'dummy' move that specifies that specifies that
                              the computer's turn is over.  This move type is
                              also used when the computer passes.

    With the TI_CPU_MOVE_PLAY type, moveX and moveY specify where to place
    the tile, while held tile specifies whether the primary or secondary
    tile is used. 

    With the TI_CPU_MOVE_DISCARD type, only heldTile is used; it specifies
    whether the primary or secondary tile is discarded.

    TI_CPU_MOVE_END_TURN uses no other parts of the structure.

*/

enum {
    TI_CPU_MOVE_DRAW,
    TI_CPU_MOVE_PLAY,
    TI_CPU_MOVE_DISCARD,
    TI_CPU_MOVE_END_TURN
};

/* Used to determine whether the CPU is acting on his primary or secondary tile */
#define TI_CPU_HELD_TILE_PRIMARY    0
#define TI_CPU_HELD_TILE_RESERVE    1

/* Weights for various CPU AI goals.  The higher the weight, the more important
   the class of move.  As an example, a play that creates an incomplete segment
   of track is worth more than a play that completes a track of the same length,
   because a complete track has no further potential for scoring, while an
   incomplete track does.  On the other hand, completing a track that terminates
   at a central station is fairly valuable due to the double point bonus.  The
   multiplier isn't as high as that of an incomplete track, since completed tracks
   to a central station already have a 2x multipler (creating a net 4x multiplier
   with the current weights)
 */
#define TI_CPU_WEIGHT_COMPLETE_TRACK            1.0
#define TI_CPU_WEIGHT_INCOMPLETE_TRACK          3.0
#define TI_CPU_WEIGHT_COMPLETE_CENTRAL_STATION  2.0
#define TI_CPU_WEIGHT_BLOCK_OPPONENT            1.5

#define TI_CPU_STATIC_DELAY                     400
#define TI_CPU_DYNAMIC_DELAY                    1100
#define TI_CPU_PASS_DYNAMIC_DELAY               200
#define TI_CPU_PASS_STATIC_DELAY                100

/* This structure holds everything necessary to determine the move that
   a computer opponent will make, including move type, tile to use, and
   location to place it */
typedef struct {
    int moveType;
    int moveX;
    int moveY;
    int heldTile;
} ComputerAIPacket;

/* This list is used to group together move analysis for further sorting and
   move selection. */
typedef struct AIEvalElem {
    int tileType;
    int tileX;
    int tileY;
    int value;
    struct AIEvalElem *next;
} AIMoveEval;


/****************************************************************************
 * computerDetermineNextMove
 *
 * Description:
 *   Determines the next action that the current computer AI player should 
 *   take.
 *
 * Arguments:
 *   ComputerAIPacket *lastMove - the last action taken (or NULL if no action
 *                                has been taken yet)
 *
 * Returns:
 *   A ComputerAIPacket that contains the computer's next move.
 *
 * Notes:
 *   The AI uses a copy of the global game instance to determine the current
 *  state of the game.  The AI has no knowledge of the tiles that the player
 *  holds; in fact, the amount of information available to the computer is
 *  equivalent to that of a human player. 
 *
 ****************************************************************************/
ComputerAIPacket *computerDetermineNextMove(ComputerAIPacket *lastMove);

/****************************************************************************
 * computerMoveAnalyzeMoves
 *
 * Description:
 *   Determines the 'value' of every legal move on the board -- that is, how
 *   useful the move is to the computer and how detrimental to other players.
 *
 * Arguments:
 *   AIMoveEval **p - a pointer to a AIMoveEval list.  This list will contain
 *                    the analysis for all legal moves when this function
 *                    completes. 
 *   int tileIndex -  the tile to place in all legal board locations
 *   int heldTile  -  used to determine if the tile is the computer's 
 *                    primary or secondary tile.
 *
 * Returns:
 *   TI_OK.
 *
 ****************************************************************************/
int computerMoveAnalyzeMoves(AIMoveEval **p, Board *b, int tileIndex, int heldTile);

/****************************************************************************
 * computerMoveEvalListAdd
 *
 * Description:
 *   Adds an entry to the AIMoveEval linked list.
 *
 * Arguments:
 *   AIMoveEval **p - a pointer to a AIMoveEval list to append the new value to.
 *   int x - the x position of the move on the board
 *   int y - the y position of the move on the board
 *   int val - the value of this move

 * Returns:
 *   A pointer to the newly added element.
 *
 ****************************************************************************/
AIMoveEval *computerMoveEvalListAdd(AIMoveEval **p, int type, int x, int y, int val);

/****************************************************************************
 * computerMoveEvalListFree
 *
 * Description:
 *   Deletes all entries in an AIMoveEval linked list. 
 *
 * Arguments:
 *   AIMoveEval **p - a pointer to a AIMoveEval list to delete.

 * Returns:
 *   Nothing.
 *
 ****************************************************************************/
void computerMoveEvalListFree(AIMoveEval **p);

/****************************************************************************
 * computerMoveEvalListRemove
 *
 * Description:
 *   Deletes the head of an AIMoveEval linked list. 
 *
 * Arguments:
 *   AIMoveEval **p - a pointer to a AIMoveEval list head to delete

 * Returns:
 *   Nothing.
 *
 ****************************************************************************/
void computerMoveEvalListRemove(AIMoveEval **p);

/****************************************************************************
 * computerMoveEvalListSort
 *
 * Description:
 *   Sorts all entries in an AIMoveEval linked list.
 *
 * Arguments:
 *   AIMoveEval **p - a pointer to a AIMoveEval list to sort.

 * Returns:
 *   A pointer to the head of the sorted list
 *
 * Notes:
 *   Based on 'Mergesort for Linked Lists' by Simon Tatham
 *    (http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html)
 *   This implementation allows use for any purpose, (presumably) including
 *   use in this software.
 ****************************************************************************/
AIMoveEval *computerMoveEvalListSort(AIMoveEval **list);

/****************************************************************************
 * computerMoveSelectListMove
 *
 * Description:
 *   Given a sorted list of moves (from best to worst), picks a move 
 *   depending on the strength of the computer AI.
 *
 * Arguments:
 *   AIMoveEval **p - a pointer to a AIMoveEval list to take a move from.

 * Returns:
 *   A pointer to the move that the computer will make.
 *
 ****************************************************************************/
AIMoveEval *computerMoveSelectListMove(AIMoveEval **evalList);

#endif /* __TI_COMPUTERAI_H__ */
