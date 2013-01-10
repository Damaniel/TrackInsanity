/****************************************************************************
*
* tiBoard.h - Header for tiBoard.c
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
#ifndef __TIBOARD_H__
#define __TIBOARD_H__

#define TI_BOARD_WIDTH                  10
#define TI_BOARD_HEIGHT                 10
#define TI_BOARD_NUM_STATIONS           32

#define TI_BOARD_TRACK_NOT_COMPLETE     0
#define TI_BOARD_TRACK_COMPLETE         1
#define TI_BOARD_TRACK_PROCESSED        2

#define TI_BOARD_ILLEGAL_MOVE           0
#define TI_BOARD_LEGAL_MOVE             1

#define TI_BOARDSQUARE_TYPE_STATION     0
#define TI_BOARDSQUARE_TYPE_CENTRAL     1
#define TI_BOARDSQUARE_TYPE_TILE        2
#define TI_BOARDSQUARE_TYPE_CORNER      3
#define TI_BOARDSQUARE_TYPE_PLAYED_TILE 4

enum playerTrains
{
    TI_BOARD_NO_TRAIN = 0,
    TI_BOARD_P1_TRAIN,
    TI_BOARD_P2_TRAIN,
    TI_BOARD_P3_TRAIN,
    TI_BOARD_P4_TRAIN,
    TI_BOARD_P5_TRAIN,
    TI_BOARD_P6_TRAIN
};

typedef struct {
    int type;
    int trainPresent;
    int tileIndex;
} BoardSquare;

typedef struct {
    BoardSquare b[TI_BOARD_WIDTH][TI_BOARD_HEIGHT];
    int legalMove[TI_BOARD_WIDTH][TI_BOARD_HEIGHT];
    int trackStatus[TI_BOARD_NUM_STATIONS];
    int playerStations[TI_MAX_PLAYERS+1][TI_BOARD_NUM_STATIONS];
    TilePool *tp;
} Board;

/* Function prototypes */

/****************************************************************************
* boardInitialize
*
* Description:
*   Allocates memory for a Board and sets default values as necessary.
*
* Arguments:
*   TilePool *t           - the TilePool that the board will use.
*   char *stationInitFile - the path and file name of the station file
*
* Returns:
*   A pointer to the Board, or NULL if an error was detected.
*
****************************************************************************/
Board *boardInitialize(TilePool *t, char *stationInitFile);

/****************************************************************************
* boardDestroy
*
* Description:
*   Frees the memory that a board is using
*
* Arguments:
*   Board **b    - the board to destroy
*
* Returns:
*   TI_OK.
*
****************************************************************************/
int boardDestroy(Board **b);

/****************************************************************************
* boardParseStationFile
*
* Description:
*   Reads the contents of a station data file and populates the provided
*   board with the station and player information
*
* Arguments:
*   Board *b - the board to be modified by this function
*   FILE *fp - a pointer to the station data file
*
* Returns:
*   TI_OK if all is well, TI_ERROR if a problem was detected.
*
****************************************************************************/
int boardParseStationFile(Board *b, FILE *fp);

/****************************************************************************
* boardFindNextTrackSection
*
* Description:
*   Based on a particular board coordinate pair and exit point, determine
*   the tile coordinate and entry point for the adjacent tile.
*
* Arguments:
*   Board *b - the board to mark legal moves on.
*   int x, y - the (x,y) coordinates of the starting board position
*   int exit - the exit point of the tile
*   int *newX, *newY - the (x,y) coordinates of the destination tile
*   int *enter - the entry point of the new tile
*
* Returns:
*   The type of BoardSquare (tile, station, blank) of the destination
*   tile.
*
****************************************************************************/
int boardFindNextTrackSection(Board *b, int x, int y, int exit,
                            int *newX, int *newY, int *enter);

/****************************************************************************
* boardMarkLegalMoves
*
* Description:
*   Given a particular tile, determine and mark the legal places on the
*   board in which the tile can be placed.
*
* Arguments:
*   Board *b - the board to mark legal moves on.
*   Tile  *t - the tile to use in determining legal moves.
*
* Returns:
*   The number of legal moves found.
*
****************************************************************************/
int boardMarkLegalMoves(Board *b, Tile *t);

/****************************************************************************
* boardGetStationInfo   (a)
* boardGetStationNumber (b)
*
* Description:
*   (a) Given a station number, convert the value to a (x,y) coordinate pair
*   that represent the location of the station on the board, and an exit
*   point for that station.
*
*   (b) Given an (x,y) coordinate pair, return the station number (if a
*   station exists).
*
* Arguments:
*   int station - the station index
*   int *x, *y - the (x,y) coordinate of the station on the board
*   int *exit - the exit point of the track of the station
*
* Returns:
*   (a) Nothing.
*   (b) the station number, or -1 if the square is not a station
*
****************************************************************************/
void boardGetStationInfo(int station, int *x, int *y, int *exit);
int boardGetStationNumber(int x, int y);

/****************************************************************************
* boardCalculateTrackScore
*
* Description:
*   For a given board and station, calculate the score obtained my moving
*   the train at the station from the station to the destination at the
*   other end of the track.
*
* Arguments:
*   Board *b - the board to use.
*   int station - the station to be processed.
*   int passThruTileId - If checking a partial track (for AI purposes), then
*                        set this value to the id of the tile to look for,
*                        otherwise set it to TI_TILE_NO_TILE
*   int *passThruTile  - set to TI_TRUE if the track passes through the specified
*                        tile, and TI_FALSE otherwise.  Set to NULL if not checking
*                        for partial tracks.
*   int *destination   - the type of board square that lies at the destination
*                        (empty, station, central station)
*
* Returns:
*   The calculated score.
*
****************************************************************************/
int boardCalculateTrackScore(Board *b, int station, int passThruTileId, 
                             int *passThruTile, int *destination);

/****************************************************************************
* boardPlaceTile
*
* Description:
*   Places a tile specified by tileIndex on the specified board, after
*   making sure that the chosen location is legal.
*
* Arguments:
*   Board *b - the board to place the tile on
*   int x,y - the horizontal and vertical offset (from the top left) of
*             the placed tile
*   int tileIndex - the index of the tile to place.
*
* Returns:
*   TI_BOARD_LEGAL_MOVE if the tile was placed, or
*   TI_BOARD_ILLEGAL_MOVE if the tile could not be placed.
*
****************************************************************************/
int boardPlaceTile(Board *b, int x, int y, int tileIndex);

/****************************************************************************
* boardIsLegalMove
*
* Description:
*
****************************************************************************/
int boardIsLegalMove(Board *b, int x, int y);

/****************************************************************************
* boardSquareSetTileIndex
*
* Description:
*   Sets the tile index of a particular board square.  This function
*   doesn't check to see if the move is legal (that's the responsibility
*   of boardPlaceTile())
*
* Arguments:
*   BoardSquare *bsq - the board square to modify
*   int tileIndex - the index of the tile to place.
*
* Returns:
*   TI_OK or TI_ERROR.
*
****************************************************************************/
int boardSquareSetTileIndex(BoardSquare *bsq, int tileIndex);

/****************************************************************************
* boardCopyBoard
*
* Description:
*   Creates a copy of a specified Board.
*
* Arguments:
*   Board *b - the board to copy
*
* Returns:
*   A pointer to the copy of the board.
*
****************************************************************************/
Board *boardCopyBoard(Board *b);

/****************************************************************************
* boardRemoveTile
*
* Description:
*   Removes a tile from the board
*
* Arguments:
*   Board *b - the board to remove the tile from
*   int x, y - the coordinates of the spot to remove the tile from.
*
* Returns:
*   TTI_OK or TI_ERROR.
*
****************************************************************************/
int boardRemoveTile(Board *b, int x, int y);

#endif /* __TIBOARD_H__ */
