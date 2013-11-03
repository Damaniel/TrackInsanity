/****************************************************************************
 *
 * tiTiles.h - Header for tiTiles.c
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
#ifndef __TITILES_H__
#define __TITILES_H__

#define TI_TILE_NUM_EXITS           8
#define TI_TILE_INVALID             -1
#define TI_TILE_ERROR               -2
#define TI_TILE_ORIENTATION_NORMAL  0
#define TI_TILE_ORIENTATION_90CW    1
#define TI_TILE_ORIENTATION_180CW   2
#define TI_TILE_ORIENTATION_270CW   3
#define TI_TILE_PLAYED              0
#define TI_TILE_UNPLAYED            1
#define TI_TILE_NO_TILE             -1

#define TI_TILEPOOL_NUM_TILES       60
#define TI_TILEPOOL_NUM_TILE_TYPES  25

/* Tile structures */
typedef struct {
    int         exits[TI_TILE_NUM_EXITS];
    int         defaultExits[TI_TILE_NUM_EXITS];
    int         orientation;
    int         tileStripOffset;
} Tile;

typedef struct {
    Tile    t[TI_TILEPOOL_NUM_TILES];
    int     playedTiles[TI_TILEPOOL_NUM_TILES];
    int     unplayedTiles[TI_TILEPOOL_NUM_TILES];
    int     tileStatus[TI_TILEPOOL_NUM_TILES];
    int     numPlayedTiles;
    int     numUnplayedTiles;
} TilePool;

/* Function prototypes */

/****************************************************************************
 * tilePoolInitialize
 *
 * Description:
 *   Allocates memory for a TilePool and coordinates loading of
 *   the tile data for each tile in the pool.
 *
 * Arguments:
 *   char *poolInitFile - the name of the file that contains information
 *                        about tiles in the tile pool.
 *
 * Returns:
 *   A pointer to the TilePool, or NULL if an error was detected.
 *
 ****************************************************************************/
TilePool *tilePoolInitialize(char *poolInitFile);

/****************************************************************************
 * tilePoolDestroy
 *
 * Description:
 *   Frees any memory associated with the specified TilePool.
 *
 * Arguments:
 *   TilePool **pool - a pointer to the TilePool to destroy.
 *
 * Returns:
 *   TI_OK or TI_ERROR.
 *
 ****************************************************************************/
int  tilePoolDestroy(TilePool **pool);

/****************************************************************************
 * tilePoolParseTilePoolFile
 *
 * Description:
 *   Reads and parses the contents of a tile pool description file and sets
 *   fields in the specified TilePool structure accordingly.
 *
 * Arguments:
 *   FILE *fp - a pointer to the tile pool description file stream.
 *   TilePool *pool - a pointer to the TilePool to populate with data.
 *
 * Returns:
 *   TI_OK or TI_ERROR.
 *
 ****************************************************************************/
int tilePoolParseTilePoolFile(FILE *fp, TilePool *pool);

/****************************************************************************
 * tilePoolGetTile
 *
 * Description:
 *   Returns the Tile structure associated with the specified index and
 *   TilePool.
 *
 * Arguments:
 *   TilePool *pool - a pointer to the TilePool to get the Tile from.
 *   int index - the offset into the tile pool.
 *   .
 *
 * Returns:
 *   A pointer to a Tile structure, or NULL if the provided index was
 *   invalid.
 *
 ****************************************************************************/
Tile *tilePoolGetTile(TilePool *pool, int index);

/****************************************************************************
 * tilePoolGetTileIndexForTileId
 *
 * Description:
 *  Given a particular tile, returns its index in the tile graphics file.
 *
 * Arguments:
 *  TilePool *pool - the tilepool containing the tile
 *  int id - the index of the tile
 *
 * Returns:
 *  The offset into the tile graphics file.
 *
 ****************************************************************************/
int tilePoolGetTileIndexForTileId(TilePool *pool, int id);

/****************************************************************************
 * tilePoolDrawRandomTile
 *
 * Description:
 *   Returns the index of a random undrawn tile in the specified TilePool.  
 *
 * Arguments:
 *   TilePool *pool - a pointer to the TilePool to get the Tile index from.
 *
 * Returns:
 *   The index of the selected tile, or TI_TILE_INVALID if no tiles remain.
 *
 ****************************************************************************/
int  tilePoolDrawRandomTile(TilePool *pool);

/****************************************************************************
 * tilePoolReturnTileToPool
 *
 * Description:
 *   Takes a played tile and places it back in the tile pool.  This is
 *   necessary if the player discards a tile.
 *
 * Arguments:
 *   TilePool *pool - a pointer to the TilePool to restore the tile to
 *   int index - the tile to place back in the pool
 *   .
 *
 * Returns:
 *   TI_OK if the operation was successful, or TI_ERROR if not.
 *
 ****************************************************************************/
int tilePoolReturnTileToPool(TilePool *pool, int index);

/****************************************************************************
 * tilePoolCalculatePlayedTiles
 *
 * Description:
 *   Assemble a list of tiles that have already been played on the board.  
 *
 * Arguments:
 *   TilePool *pool - a pointer to the TilePool to examine
 *
 * Returns:
 *   Nothing.
 *
 ****************************************************************************/
void tilePoolCalculatePlayedTiles(TilePool *p);

/****************************************************************************
 * tilePoolCalculateUnplayedTiles
 *
 * Description:
 *   Assemble a list of tiles that have not yet been played on the board.  
 *
 * Arguments:
 *   TilePool *pool - a pointer to the TilePool to examine
 *
 * Returns:
 *   Nothing.
 *
 ****************************************************************************/
void tilePoolCalculateUnplayedTiles(TilePool *p);

/****************************************************************************
 * tileGetExit
 *
 * Description:
 *   Returns the exit point for the specified tile and entry point.  
 *
 * Arguments:
 *   Tile *t - the tile to get the exit from.
 *   int entry - the entry point of the tile (0-7)
 *
 * Returns:
 *   The exit point for the provided entry point, or TI_TILE_ERROR if the
 *   entry point is invalid.
 *
 * Notes:
 *   The exit point is based on the list of orientation-adjusted exits, 
 *   not the 'default' exits.  In most cases, these will be the same anyway 
 *   (the exception is if the player selects arbitrary piece orientation as 
 *   a game option).
 *
 ****************************************************************************/
int tileGetExit(Tile *t, int entry);

/****************************************************************************
 * tileSetRotatedExits
 *
 * Description:
 *   Recalculate the exit points for a tile when placed in the specified
 *   orientation. 
 *
 * Arguments:
 *   Tile *t - the tile to get the exit from.
 *   int orientation - the orientation of the tile
 *
 * Returns:
 *   Nothing.
 *
 ****************************************************************************/
void tileSetRotatedExits(Tile *t, int orientation);

#endif /* __TITILES_H__ */
