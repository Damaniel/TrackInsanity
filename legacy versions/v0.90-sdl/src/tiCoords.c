/****************************************************************************
 *
 * tiCoords.c - coordinates and extents for graphical elements
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

int TI_RENDER_PLAYER_OFFSETS[5][4] =
{
    {120, 226, 621, 161},
    {120, 266, 621, 121},
    {120, 306, 621, 81},
    {120, 346, 621, 41},
    {120, 386, 621, 1}
};

int TI_RENDER_NUM_PLAYER_OFFSETS[5][4] =
{
    {392, 100, 35, 35},
    {466, 100, 35, 35},
    {542, 100, 35, 35},
    {618, 100, 35, 35},
    {694, 100, 35, 35}
};

int TI_RENDER_HUMAN_OFFSETS[6][4] =
{
    {383, 148, 130, 40},
    {383, 188, 130, 40},
    {383, 228, 130, 40},
    {383, 267, 130, 40},
    {383, 309, 130, 40},
    {383, 348, 130, 40}
};

int TI_RENDER_CPU_OFFSETS[6][4] =
{
    {561, 148, 175, 40},
    {561, 188, 175, 40},
    {561, 228, 175, 40},
    {561, 267, 175, 40},
    {561, 309, 175, 40},
    {561, 348, 175, 40}
};

int TI_RENDER_OPTIONS_TRACKS_OFFSETS[2][4] = 
{
    {433, 123, 39, 26},
    {529, 122, 56, 26}
};

int TI_RENDER_OPTIONS_LEGAL_OFFSETS[2][4] = 
{
    {433, 168, 39, 26},
    {529, 167, 56, 26}
};

int TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[3][4] = 
{
    {329, 209, 39, 30},
    {405, 209, 169, 30},
    {612, 209, 166, 30}
};

int TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[3][4] =
{
    {346, 253, 68, 31},
    {501, 253, 108, 31},
    {696, 253, 70, 31}
};

int TI_RENDER_OPTIONS_MUSIC_OFFSETS[10][4] = 
{
    {392, 311, 22, 29},
    {430, 311, 17, 29},
    {464, 311, 23, 29},
    {503, 311, 23, 29},
    {541, 311, 22, 29},
    {579, 311, 24, 29},
    {620, 311, 23, 29},
    {661, 311, 18, 29},
    {695, 311, 23, 29},
    {732, 311, 23, 29}
};

int TI_RENDER_OPTIONS_SOUND_OFFSETS[10][4] = 
{
    {392, 355, 22, 29},
    {430, 355, 17, 29},
    {464, 355, 23, 29},
    {503, 355, 23, 29},
    {541, 355, 22, 29},
    {579, 355, 24, 29},
    {620, 355, 23, 29},
    {661, 355, 18, 29},
    {695, 355, 23, 29},
    {732, 355, 23, 29}
};

int TI_RENDER_OPTIONS_TRACKS_FILE_OFFSETS[2] = {0, 39};

int TI_RENDER_OPTIONS_LEGAL_FILE_OFFSETS[2] = {0, 39};

int TI_RENDER_OPTIONS_LAST_MOVE_FILE_OFFSETS[3] = {0, 39, 208};

int TI_RENDER_OPTIONS_AI_LEVEL_FILE_OFFSETS[3] = {0, 68, 176};

int TI_RENDER_OPTIONS_DIGITS_OFFSETS[10] = {0, 22, 39, 62, 85, 107, 131, 154, 172, 195};

int TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[6][4] =
{
    {6,   6,   46,  73},
    {6,   85,  46,  73},
    {6,   164, 46,  73},
    {6,   243, 46,  73},
    {6,   322, 46,  73},
    {6,   401, 46,  73}
};

int TI_RENDER_GAME_CUR_PLAYER_OFFSETS[6][4] = 
{
    {2, 2,   54, 81},
    {2, 81,  54, 81},
    {2, 160, 54, 81},
    {2, 239, 54, 81},
    {2, 318, 54, 81},
    {2, 397, 54, 81}
};

int TI_RENDER_GAME_PLAYER_BG_OFFSETS[6][4] =
{
    {58,  6,   165, 73},
    {58,  85,  165, 73},
    {58,  164, 165, 73},
    {58,  243, 165, 73},
    {58,  322, 165, 73},
    {58,  401, 165, 73}
};

int TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[6][4] =
{
    {229, 6,   40,  40},
    {229, 85,  40,  40},
    {229, 164, 40,  40},
    {229, 243, 40,  40},
    {229, 322, 40,  40},
    {229, 401, 40,  40}
};

int TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[6][4] =
{
    {275, 6,   40,  40},
    {275, 85,  40,  40},
    {275, 164, 40,  40},
    {275, 243, 40,  40},
    {275, 322, 40,  40},
    {275, 401, 40,  40}
};

int TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[6][4] =
{
    {225, 2,   48,  48},
    {225, 81,  48,  48},
    {225, 160, 48,  48},
    {225, 239, 48,  48},
    {225, 318, 48,  48},
    {225, 397, 48,  48}
};

int TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[6][4] =
{
    {271, 2,   48,  48},
    {271, 81,  48,  48},
    {271, 160, 48,  48},
    {271, 239, 48,  48},
    {271, 318, 48,  48},
    {271, 397, 48,  48}
};

int TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[6][4] =
{
    {229, 51,  86,  28},
    {229, 130, 86,  28},
    {229, 209, 86,  28},
    {229, 288, 86,  28},
    {229, 367, 86,  28},
    {229, 446, 86,  28}
};

int TI_RENDER_GAME_PLAYER_NAME_OFFSETS[6][4] =
{
    {63,  8,   96,  23},
    {63,  87,  96,  23},
    {63,  166, 96,  23},
    {63,  245, 96,  23},
    {63,  324, 96,  23},
    {63,  403, 96,  23}
};

int TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[6][4] =
{
    {63, 32,  123, 22},
    {63, 111, 123, 22},
    {63, 190, 123, 22},
    {63, 269, 123, 22},
    {63, 348, 123, 22},
    {63, 427, 123, 22}
};

int TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS[6][4] =
{
    {63, 59,  67,  18},
    {63, 138, 67,  18},
    {63, 217, 67,  18},
    {63, 296, 67,  18},
    {63, 375, 67,  18},
    {63, 454, 67,  18}
};

int TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[6][2] =
{
    {150, 58},
    {150, 137},
    {150, 216},
    {150, 295},
    {150, 374},
    {150, 453}
};

int TI_RENDER_GAME_DIGIT_WIDTHS[10] = {15, 11, 17, 16, 16, 17, 16, 13, 15, 15};

/* This array is tricky.  The first dimension represents the entry point, while
   the second represents the exit point.  The value of 
   TI_RENDER_GAME_TRACK_OVERLAY_EXITS[x][y] give the horizontal offset in the 
   track overlay tile strip of the appropriate overlay for this entry/exit
   combo.  A value of -1 means that the particular combination of entry and exit
   is not valid 
 */
int TI_RENDER_GAME_TRACK_OVERLAY_EXITS[TI_TILE_NUM_EXITS][TI_TILE_NUM_EXITS] =
{
    {-1,  0, -1, 12, -1,  8, -1,  7},       /* Entry 0, exits 0-7 */
    { 0, -1,  4, -1,  9, -1, 15, -1},       /* Entry 1, exits 0-7 */
    {-1,  5, -1,  1, -1, 13, -1, 10},       /* Entry 2, exits 0-7 */
    {12, -1,  1, -1,  5, -1, 11, -1},       /* Entry 3, exits 0-7 */
    {-1,  9, -1,  5, -1,  2, -1, 14},       /* Entry 4, exits 0-7 */
    { 8, -1, 13, -1,  2, -1,  6, -1},       /* Entry 5, exits 0-7 */
    {-1, 15, -1, 11, -1,  6, -1,  3},       /* Entry 6, exits 0-7 */
    { 7, -1, 10, -1, 14, -1,  3, -1}        /* Entry 7, exits 0-7 */
};

/* The 5 values here are: x position, y position, width, height, and horizontal
   offset in the train tile bitmap of the appropriate image */
int TI_RENDER_GAME_STATION_TRAIN_OFFSETS[TI_BOARD_NUM_STATIONS][5] =
{
    {408, 56, 10, 20, 0},
    {448, 56, 10, 20, 0},
    {488, 56, 10, 20, 0},
    {528, 56, 10, 20, 0},
    {568, 56, 10, 20, 0},
    {608, 56, 10, 20, 0},
    {648, 56, 10, 20, 0},
    {688, 56, 10, 20, 0},

    {724, 88,  20, 10, 1},
    {724, 128, 20, 10, 1},
    {724, 168, 20, 10, 1},
    {724, 208, 20, 10, 1},
    {724, 248, 20, 10, 1},
    {724, 288, 20, 10, 1},
    {724, 328, 20, 10, 1},
    {724, 368, 20, 10, 1},

    {702, 404, 10, 20, 2},
    {662, 404, 10, 20, 2},
    {622, 404, 10, 20, 2},
    {582, 404, 10, 20, 2},
    {542, 404, 10, 20, 2},
    {502, 404, 10, 20, 2},
    {462, 404, 10, 20, 2},
    {422, 404, 10, 20, 2},

    {376, 382, 20, 10, 3},
    {376, 342, 20, 10, 3},
    {376, 302, 20, 10, 3},
    {376, 262, 20, 10, 3},
    {376, 222, 20, 10, 3},
    {376, 182, 20, 10, 3},
    {376, 142, 20, 10, 3},
    {376, 102, 20, 10, 3}
};

/* The 2 values here are: x position, y position, and offset of the station
   within the station image strip */
int TI_RENDER_GAME_STATION_OFFSETS[TI_BOARD_NUM_STATIONS][3] =
{
    {400, 40, 0},
    {440, 40, 0},
    {480, 40, 0},
    {520, 40, 0},
    {560, 40, 0},
    {600, 40, 0},
    {640, 40, 0},
    {680, 40, 0},

    {720, 80,  1},
    {720, 120, 1},
    {720, 160, 1},
    {720, 200, 1},
    {720, 240, 1},
    {720, 280, 1},
    {720, 320, 1},
    {720, 360, 1},

    {680, 400, 2},
    {640, 400, 2},
    {600, 400, 2},
    {560, 400, 2},
    {520, 400, 2},
    {480, 400, 2},
    {440, 400, 2},
    {400, 400, 2},

    {360, 360, 3},
    {360, 320, 3},
    {360, 280, 3},
    {360, 240, 3},
    {360, 200, 3},
    {360, 160, 3},
    {360, 120, 3},
    {360, 80,  3}
};

int TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET[4] = {360, 160, 400, 160};

/* These coordinates are relative to the screen, not the dialog window itself */
int TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[2][4] =
{
    {369, 264, 187, 47},
    {565, 264, 186, 47}
};

int TI_RENDER_GAME_CONFIRM_EXIT_OFFSET[4] = {200, 180, 400, 120};

/* These coordinates are relative to the screen, not the dialog window itself */
int TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[2][4] =
{
    {208, 244, 187, 47},
    {405, 244, 186, 47}
};

int TI_RENDER_GAME_SELECT_ACTION_OFFSET[4] = {320, 400, 480, 80};

/* These coordinates are relative to the screen, not the dialog window itself */
int TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[4][4] =
{
    {327, 443, 111, 32},    /* Draw */
    {445, 443, 111, 32},    /* Play */
    {563, 443, 111, 32},    /* Discard */
    {681, 443, 112, 32 }    /* Pass */
};

/* These coordinates are relative to the screen, not the dialog window itself */
/* Unlike the above coordinates, these only represent the text itself, and not the
   entire button */
int TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[4][4] =
{
    {337, 452, 94, 17},     /* Draw */
    {479, 452, 44, 18},     /* Play */
    {583, 451, 74, 19},     /* Discard */
    {717, 453, 46, 16}      /* Pass */
};

int TI_RENDER_GAME_DRAW_TILE_OFFSET[4] = {360, 40, 400, 400};

int TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[2][4] =
{
    {368, 392, 187, 39},
    {565, 392, 186, 39}
};

/* These coordinates are relative to the screen, not the dialog itself */
int TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[TI_RENDER_NUM_DRAW_TILES_TO_DISPLAY][4] =
{
    {384, 114, 64, 64},
    {480, 114 ,64, 64},
    {576, 114 ,64, 64},
    {672, 114 ,64, 64},
    {384, 210, 64, 64},
    {480, 210, 64, 64},
    {576, 210, 64, 64},
    {672, 210, 64, 64},
    {384, 306, 64, 64},
    {480, 306, 64, 64},
    {576, 306, 64, 64},
    {672, 306, 64, 64}
};

int TI_RENDER_GAME_CPU_THINKING_OFFSET[4] = {320, 400, 480, 80};

int TI_RENDER_GAME_FINISHED_OFFSETS[4] = {200, 180, 400, 120};

int TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[4] = {208, 244, 382, 46};

int TI_RENDER_GAME_RESULTS_RANK_OFFSETS[TI_MAX_PLAYERS][2] =
{
    {196, 161},
    {196, 202},
    {196, 235},
    {196, 268},
    {196, 301},
    {196, 334}
};

int TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS[TI_MAX_PLAYERS][2] =
{
    {269, 161},
    {271, 202},
    {271, 235},
    {271, 268},
    {271, 301},
    {271, 334}
};

int TI_RENDER_GAME_RESULTS_SCORE_OFFSETS[TI_MAX_PLAYERS][2] =
{
    {532, 161},
    {533, 202},
    {533, 235},
    {533, 268},
    {533, 301},
    {533, 334}
};

int TI_RENDER_GAME_RESULTS_SMALL_DIGIT_WIDTHS[10] = {13, 9, 14, 14, 13, 14, 13, 10, 13, 13};
int TI_RENDER_GAME_RESULTS_LARGE_DIGIT_WIDTHS[10] = {18, 12, 19, 20, 18, 20, 19, 15, 18, 20};
