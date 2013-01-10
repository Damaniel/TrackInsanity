/****************************************************************************
 *
 * tiTiles.c - Tile and tile pool access functions
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
 * tilePoolInitialize - see tiTiles.h for description
 ****************************************************************************/
TilePool *tilePoolInitialize(char *poolInitFile)
{
    FILE *tilePoolFP;
    TilePool *pool;
    int   result, counter;

    /* Set up the needed memory for the tile pool*/
    pool = malloc(sizeof(TilePool));
    if(pool == NULL)
    {
        perror("tilePoolInitialize: malloc of tile pool failed");
        return NULL;
    }

    /* Attempt to open the tile pool description file */
    tilePoolFP = fopen(poolInitFile, "r");
    if(tilePoolFP == NULL)
    {
        perror("tilePoolInitialize: open of tile description file failed");
        return NULL;
    }

    result = tilePoolParseTilePoolFile(tilePoolFP, pool);

    /* Set all tiles as unplayed */
    for(counter=0;counter<TI_TILEPOOL_NUM_TILES;counter++)
    {
        pool->tileStatus[counter] = TI_TILE_UNPLAYED;
    }

    /* Assemble the initial played and unplayed tile lists */
    tilePoolCalculatePlayedTiles(pool);
    tilePoolCalculateUnplayedTiles(pool);

    /* Close and return */
    fclose(tilePoolFP);
    return pool;
}

/****************************************************************************
 * tilePoolDestroy - see tiTiles.h for description
 ****************************************************************************/
int tilePoolDestroy(TilePool **pool)
{
    free(*pool);
    *pool = NULL;
    return TI_OK;
}

/****************************************************************************
 * tilePoolParseTilePoolFile - see tiTiles.h for description
 ****************************************************************************/
int tilePoolParseTilePoolFile(FILE *fp, TilePool *pool)
{
    char curLine[TI_LINE_MAX];
    char *lineCopy, *indexToken, *exitsToken, *fileToken;
    int  result, counter;
    const char delimiters[] = ",\n";
    int  tileIndex, tileOffset, curExit[TI_TILE_NUM_EXITS];
    int  gotError = 0;

    while(fgets(curLine, TI_LINE_MAX, fp) != NULL)
    {
        if(curLine[0] == '#') /* Non-comment */
        {
            continue;
        }

        /* Separate each line into the three desired tokens */
        lineCopy = strdup(curLine);
        indexToken = strtok(lineCopy, delimiters);
        exitsToken = strtok(NULL, delimiters);
        fileToken = strtok(NULL, delimiters);
        /* If there are less than three, return an error */
        if((indexToken == NULL) || (exitsToken == NULL)
        || (fileToken == NULL))
        {
            free(curLine);
            free(lineCopy);
            perror("tilePoolparseTilePoolFile: parse error");
            return TI_ERROR;
        }
        /* Get the tile index from the first token */
        result = sscanf(indexToken, "%d", &tileIndex);
        /* If the token wasn't read, or if the value of the token
        is out of range, return an error */
        if(result != 1 || tileIndex < 0 || tileIndex >= TI_TILEPOOL_NUM_TILES)
        {
            free(curLine);
            free(lineCopy);
            perror("tilePoolParseTilePoolFile: parsed invalid tile index");
            return TI_ERROR;
        }

        /* Read the exits from the second token */
        result = sscanf(exitsToken, "%d%d%d%d%d%d%d%d",
                        &curExit[0], &curExit[1], &curExit[2],
                        &curExit[3], &curExit[4], &curExit[5],
                        &curExit[6], &curExit[7]);
        /* If the token's weren't all read, or if any token values are
        out of range, return an error */
        if(result != TI_TILE_NUM_EXITS)
        {
            gotError = 1;
        }
        for(counter=0;counter<TI_TILE_NUM_EXITS;counter++)
        {
            if(curExit[counter] < 0 || curExit[counter] >= TI_TILE_NUM_EXITS)
            {
                gotError = 1;
            }
        }
        if(gotError)
        {
            free(curLine);
            free(lineCopy);
            perror("tilePoolParseTilePoolFile: parsed invalid exit");
            return TI_ERROR;
        }
        /* The tokens are ok, set the values in the tilePool accordingly */
        for(counter=0;counter<TI_TILE_NUM_EXITS;counter++)
        {
            pool->t[tileIndex].defaultExits[counter] = curExit[counter];
            pool->t[tileIndex].exits[counter] = curExit[counter];
            pool->t[tileIndex].orientation = TI_TILE_ORIENTATION_NORMAL;
        }

        /* Get the offset into the tile strip for the tile graphic */
        result = sscanf(fileToken, "%d", &tileOffset);
        /* If the token wasn't read, or if the value of the token
        is out of range, return an error */
        if(result != 1 || tileOffset < 0 || tileOffset >= TI_TILEPOOL_NUM_TILE_TYPES)
        {
            free(curLine);
            free(lineCopy);
            perror("tilePoolParseTilePoolFile: parsed invalid tile index");
            return TI_ERROR;
        }
        pool->t[tileIndex].tileStripOffset = tileOffset;

        free(lineCopy);
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
* tilePoolGetTile - see tiTiles.h for description
****************************************************************************/
Tile *tilePoolGetTile(TilePool *pool, int index)
{
    if(index < 0 || index >= TI_TILEPOOL_NUM_TILES)
    {
        return NULL;
    }

    return &(pool->t[index]);
}

/****************************************************************************
* tilePoolGetTileIndexForTileId - see tiTiles.h for description
****************************************************************************/
int tilePoolGetTileIndexForTileId(TilePool *pool, int id)
{
    if(id < 0 || id >= TI_TILEPOOL_NUM_TILES)
    {
        return TI_TILE_NO_TILE;
    }

    return pool->t[id].tileStripOffset;
}

/****************************************************************************
* tilePoolDrawRandomTile - see tiTiles.h for description
****************************************************************************/
int tilePoolDrawRandomTile(TilePool *pool)
{
    int offset, index;

    /* To draw a random tile, grab a random index from the unplayed tile
    * list, mark the tile as played in the tileStatus structure, then
    * recalculate the played and unplayed tile lists */

    if(pool->numUnplayedTiles == 0)
    {
        return TI_TILE_INVALID;
    }

    offset = rand() % pool->numUnplayedTiles;
    index = pool->unplayedTiles[offset];

    /* This shouldn't happen! */
    if(pool->tileStatus[index] == TI_TILE_PLAYED)
    {
        perror("tilePoolDrawRandomTile: tile already played!");
        return TI_TILE_ERROR;
    }

    pool->tileStatus[index] = TI_TILE_PLAYED;
    tilePoolCalculatePlayedTiles(pool);
    tilePoolCalculateUnplayedTiles(pool);

    return index;
}

/****************************************************************************
* tilePoolReturnTileToPool - see tiTiles.h for description
****************************************************************************/
int tilePoolReturnTileToPool(TilePool *pool, int index)
{

    if(pool->numPlayedTiles == 0)
    {
        return TI_ERROR;
    }

    if(pool->tileStatus[index] == TI_TILE_UNPLAYED)
    {
        return TI_ERROR;
    }

    pool->tileStatus[index] = TI_TILE_UNPLAYED;
    tilePoolCalculatePlayedTiles(pool);
    tilePoolCalculateUnplayedTiles(pool);

    return TI_OK;
}

/****************************************************************************
* tilePoolCalculatePlayedTiles - see tiTiles.h for description
****************************************************************************/
void tilePoolCalculatePlayedTiles(TilePool *p)
{
    int counter;

    p->numPlayedTiles = 0;
    for(counter=0;counter<TI_TILEPOOL_NUM_TILES;counter++)
    {
        if(p->tileStatus[counter] == TI_TILE_PLAYED)
        {
            p->playedTiles[p->numPlayedTiles] = counter;
            p->numPlayedTiles++;
        }
    }
}

/****************************************************************************
* tilePoolCalculateUnplayedTiles - see tiTiles.h for description
****************************************************************************/
void tilePoolCalculateUnplayedTiles(TilePool *p)
{
    int counter;

    p->numUnplayedTiles = 0;
    for(counter=0;counter<TI_TILEPOOL_NUM_TILES;counter++)
    {
        if(p->tileStatus[counter] == TI_TILE_UNPLAYED)
        {
            p->unplayedTiles[p->numUnplayedTiles] = counter;
            p->numUnplayedTiles++;
        }
    }
}

/****************************************************************************
* tileGetExit - see tiTiles.h for description
****************************************************************************/
int tileGetExit(Tile *t, int entry)
{
    if(entry < 0 || entry >= TI_TILE_NUM_EXITS)
    {
        return TI_TILE_ERROR;
    }

    return t->exits[entry];
}

/****************************************************************************
* tileSetRotatedExits - see tiTiles.h for description
****************************************************************************/
void tileSetRotatedExits(Tile *t, int orientation)
{
    int counter;

    /* If the orientation is non-default, swizzle the exit values to
    reflect the desired orientation.  As an example, for 90 degrees of
    rotation, the value of the exit point 90 degrees away from the
    'default' exit point will contain the value of the default exit
    point, plus the 90 degrees of rotation.  The same type of formula
    works for other rotation amounts as well.
    */
    switch(orientation)
    {
        case TI_TILE_ORIENTATION_NORMAL:
            for(counter=0;counter<TI_TILE_NUM_EXITS;counter++)
            {
                t->exits[counter] = t->defaultExits[counter];
            }
            break;
        case TI_TILE_ORIENTATION_90CW:
            for(counter=0;counter<TI_TILE_NUM_EXITS;counter++)
            {
                t->exits[(counter+2)%TI_TILE_NUM_EXITS] =
                    (t->defaultExits[counter]+2) % TI_TILE_NUM_EXITS;
            }
            break;
        case TI_TILE_ORIENTATION_180CW:
            for(counter=0;counter<TI_TILE_NUM_EXITS;counter++)
            {
                t->exits[(counter+4)%TI_TILE_NUM_EXITS] =
                    (t->defaultExits[counter]+4) % TI_TILE_NUM_EXITS;
            }
            break;
        case TI_TILE_ORIENTATION_270CW:
            for(counter=0;counter<TI_TILE_NUM_EXITS;counter++)
            {
                t->exits[(counter+6)%TI_TILE_NUM_EXITS] =
                    (t->defaultExits[counter]+6) % TI_TILE_NUM_EXITS;
            }
            break;
        default:
            /* Do nothing */
            break;
    }

}
