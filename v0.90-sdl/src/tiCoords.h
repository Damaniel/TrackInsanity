/****************************************************************************
 *
 * tiCoords.h - coordinates and extents for graphical elements
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
#ifndef __TICOORDS_H__
#define __TICOORDS_H__

/*
 * Any elements that are dependant on the resolution of the screen go in
 * this file.  This file assumes an 800x480 resolution, used by the Nokia 770/
 * N800 internet tablets
 */

/* Start/end positions for the moving trains at the title screen */
#define TI_RENDER_TRAIN_INITIAL_X           -600
#define TI_RENDER_TRAIN_FINAL_X             (TI_GAME_XRES + 72)

/* Dimensions of the trains at the title screen */
#define TI_RENDER_TRAIN_BANNER_WIDTH        528
#define TI_RENDER_TRAIN_BANNER_HEIGHT       22

/* Size and position of the 'Tap to play' message */
#define TI_RENDER_TAP_MSG_WIDTH             400
#define TI_RENDER_TAP_MSG_HEIGHT            40
#define TI_RENDER_TAP_MSG_X                 ((TI_GAME_XRES / 2) - (TI_RENDER_TAP_MSG_WIDTH / 2))
#define TI_RENDER_TAP_MSG_Y                 (TI_GAME_YRES * 0.6)

/* Size and position of the title screen menu */
#define TI_RENDER_TITLE_MENU_WIDTH          178
#define TI_RENDER_TITLE_MENU_HEIGHT         130
#define TI_RENDER_TITLE_MENU_X              ((TI_GAME_XRES / 2) - (TI_RENDER_TITLE_MENU_WIDTH / 2))
#define TI_RENDER_TITLE_MENU_Y              (TI_GAME_YRES * 0.5)

/* Borders of the clickable areas of the title screen menu */
#define TI_RENDER_NEW_GAME_MIN_X            (TI_RENDER_TITLE_MENU_X)
#define TI_RENDER_NEW_GAME_MAX_X            (TI_RENDER_TITLE_MENU_X + TI_RENDER_TITLE_MENU_WIDTH)
#define TI_RENDER_NEW_GAME_MIN_Y            (TI_RENDER_TITLE_MENU_Y)
#define TI_RENDER_NEW_GAME_MAX_Y            (TI_RENDER_TITLE_MENU_Y + 42)
#define TI_RENDER_OPTIONS_MIN_X             (TI_RENDER_TITLE_MENU_X)
#define TI_RENDER_OPTIONS_MAX_X             (TI_RENDER_TITLE_MENU_X + TI_RENDER_TITLE_MENU_WIDTH)
#define TI_RENDER_OPTIONS_MIN_Y             (TI_RENDER_TITLE_MENU_Y + 43)
#define TI_RENDER_OPTIONS_MAX_Y             (TI_RENDER_TITLE_MENU_Y + 88)
#define TI_RENDER_EXIT_MIN_X                (TI_RENDER_TITLE_MENU_X)
#define TI_RENDER_EXIT_MAX_X                (TI_RENDER_TITLE_MENU_X + TI_RENDER_TITLE_MENU_WIDTH)
#define TI_RENDER_EXIT_MIN_Y                (TI_RENDER_TITLE_MENU_Y + 89)
#define TI_RENDER_EXIT_MAX_Y                (TI_RENDER_TITLE_MENU_Y + TI_RENDER_TITLE_MENU_HEIGHT)

#define TI_RENDER_BACK_TO_TITLE_MIN_X       20
#define TI_RENDER_BACK_TO_TITLE_MAX_X       265
#define TI_RENDER_BACK_TO_TITLE_MIN_Y       398
#define TI_RENDER_BACK_TO_TITLE_MAX_Y       432

#define TI_RENDER_START_GAME_MIN_X          560
#define TI_RENDER_START_GAME_MAX_X          780
#define TI_RENDER_START_GAME_MIN_Y          398
#define TI_RENDER_START_GAME_MAX_Y          432

#define TI_RENDER_LOADING_DIALOG_X          200
#define TI_RENDER_LOADING_DIALOG_Y          180
#define TI_RENDER_LOADING_DIALOG_WIDTH      400
#define TI_RENDER_LOADING_DIALOG_HEIGHT     120

#define TI_RENDER_PROGRESS_BAR_X            229
#define TI_RENDER_PROGRESS_BAR_Y            260
#define TI_RENDER_PROGRESS_BAR_WIDTH        342
#define TI_RENDER_PROGRESS_BAR_HEIGHT        15

/* Offset lists too awkward to place into individual #defines */
extern int TI_RENDER_PLAYER_OFFSETS[TI_MAX_PLAYERS-1][4];
extern int TI_RENDER_NUM_PLAYER_OFFSETS[TI_MAX_PLAYERS-1][4];
extern int TI_RENDER_HUMAN_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_CPU_OFFSETS[TI_MAX_PLAYERS][4];

/* Option screen offsets */
#define TI_RENDER_OPTIONS_BACK_TO_TITLE_MIN_X         10
#define TI_RENDER_OPTIONS_BACK_TO_TITLE_MAX_X         158
#define TI_RENDER_OPTIONS_BACK_TO_TITLE_MIN_Y         402
#define TI_RENDER_OPTIONS_BACK_TO_TITLE_MAX_Y         430

#define TI_RENDER_OPTIONS_OK_MIN_X                    696
#define TI_RENDER_OPTIONS_OK_MAX_X                    784
#define TI_RENDER_OPTIONS_OK_MIN_Y                    401
#define TI_RENDER_OPTIONS_OK_MAX_Y                    430

extern int TI_RENDER_OPTIONS_TRACKS_OFFSETS[2][4];
extern int TI_RENDER_OPTIONS_LEGAL_OFFSETS[2][4];
extern int TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS[3][4];
extern int TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS[3][4];
extern int TI_RENDER_OPTIONS_MUSIC_OFFSETS[10][4];
extern int TI_RENDER_OPTIONS_SOUND_OFFSETS[10][4];

extern int TI_RENDER_OPTIONS_TRACKS_FILE_OFFSETS[2];
extern int TI_RENDER_OPTIONS_LEGAL_FILE_OFFSETS[2];
extern int TI_RENDER_OPTIONS_LAST_MOVE_FILE_OFFSETS[3];
extern int TI_RENDER_OPTIONS_AI_LEVEL_FILE_OFFSETS[3];
extern int TI_RENDER_OPTIONS_DIGITS_OFFSETS[10];

/* In-game offsets */
extern int TI_RENDER_GAME_PLAYER_COLOR_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_CUR_PLAYER_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_BG_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_TILE1_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_TILE2_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_NAME_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_TYPE_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS[TI_MAX_PLAYERS][4];
extern int TI_RENDER_GAME_PLAYER_SCORE_OFFSETS[TI_MAX_PLAYERS][2];
extern int TI_RENDER_GAME_DIGIT_WIDTHS[10];

extern int TI_RENDER_GAME_TRACK_OVERLAY_EXITS[TI_TILE_NUM_EXITS][TI_TILE_NUM_EXITS];

extern int TI_RENDER_GAME_STATION_TRAIN_OFFSETS[TI_BOARD_NUM_STATIONS][5];
extern int TI_RENDER_GAME_STATION_OFFSETS[TI_BOARD_NUM_STATIONS][3];

extern int TI_RENDER_GAME_CONFIRM_EXIT_OFFSET[4];
extern int TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS[2][4];
extern int TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET[4];
extern int TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS[2][4];
extern int TI_RENDER_GAME_SELECT_ACTION_OFFSET[4];
extern int TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS[4][4];
extern int TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS[4][4];
extern int TI_RENDER_GAME_DRAW_TILE_OFFSET[4];
extern int TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS[2][4];
extern int TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS[TI_RENDER_NUM_DRAW_TILES_TO_DISPLAY][4];
extern int TI_RENDER_GAME_CPU_THINKING_OFFSET[4];
extern int TI_RENDER_GAME_FINISHED_OFFSETS[4];
extern int TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS[4];

/* Score digit heights and spaces between each digit */
#define TI_RENDER_GAME_DIGIT_HEIGHT         21
#define TI_RENDER_GAME_LETTER_SPACING       1
#define TI_RENDER_GAME_DIGIT_OFFSET         17

/* The size of the score backing bitmap */
#define TI_RENDER_GAME_SCORE_BACKING_WIDTH      71
#define TI_RENDER_GAME_SCORE_BACKING_HEIGHT     21

/* The 'end game' button in the upper right corner */
#define TI_RENDER_GAME_END_BUTTON_MIN_X     765
#define TI_RENDER_GAME_END_BUTTON_MAX_X     792
#define TI_RENDER_GAME_END_BUTTON_MIN_Y     0
#define TI_RENDER_GAME_END_BUTTON_MAX_Y     23

/* The 'cancel' button at the bottom of the screen when playing a tile */
#define TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X      650
#define TI_RENDER_PLAY_CANCEL_BUTTON_MAX_X      800
#define TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y      430
#define TI_RENDER_PLAY_CANCEL_BUTTON_MAX_Y      480

/* Defines for the game results screen */
extern int TI_RENDER_GAME_RESULTS_RANK_OFFSETS[TI_MAX_PLAYERS][2];
extern int TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS[TI_MAX_PLAYERS][2];
extern int TI_RENDER_GAME_RESULTS_SCORE_OFFSETS[TI_MAX_PLAYERS][2];

extern int TI_RENDER_GAME_RESULTS_SMALL_DIGIT_WIDTHS[10];
extern int TI_RENDER_GAME_RESULTS_LARGE_DIGIT_WIDTHS[10];

#define TI_RENDER_GAME_RESULTS_SMALL_DIGIT_SPACING      1
#define TI_RENDER_GAME_RESULTS_SMALL_DIGIT_OFFSET       14
#define TI_RENDER_GAME_RESULTS_SMALL_DIGIT_HEIGHT       19

#define TI_RENDER_GAME_RESULTS_LARGE_DIGIT_SPACING      1
#define TI_RENDER_GAME_RESULTS_LARGE_DIGIT_OFFSET       21
#define TI_RENDER_GAME_RESULTS_LARGE_DIGIT_HEIGHT       25

#define TI_RENDER_GAME_RESULTS_SMALL_PLAYER_WIDTH       100
#define TI_RENDER_GAME_RESULTS_SMALL_PLAYER_HEIGHT      20

#define TI_RENDER_GAME_RESULTS_LARGE_PLAYER_WIDTH       130
#define TI_RENDER_GAME_RESULTS_LARGE_PLAYER_HEIGHT      28

/* The game results screen 'Back to title' button */
#define TI_RENDER_GAME_RESULTS_TO_TITLE_MIN_X       545
#define TI_RENDER_GAME_RESULTS_TO_TITLE_MIN_Y       395
#define TI_RENDER_GAME_RESULTS_TO_TITLE_MAX_X       780
#define TI_RENDER_GAME_RESULTS_TO_TITLE_MAX_Y       430

/*
 * There are two different tile sizes; one for the game board, and
 * one for the 'tile select' interface when drawing a tile.  These
 * are the sizes of the two different tiles
 */
#define TI_RENDER_SMALL_TILE_WIDTH          40
#define TI_RENDER_LARGE_TILE_WIDTH          64

#define TI_RENDER_TILE_BACK_OFFSET          25

/* When displaying the highlighted tile, this is the offset to use 
   to place the highlight correctly over the tile.  This value
   should be *subtracted* from the tile offsets.
 */
#define TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET  8

/* All of the train graphics are placed in a single file.  Each
   train is contained in a grid square with the dimensions of
   the two following variables */
#define TI_RENDER_TRAIN_TILE_WIDTH             20
#define TI_RENDER_TRAIN_TILE_HEIGHT            20
#define TI_RENDER_STATION_WIDTH                40
#define TI_RENDER_STATION_HEIGHT               40

/*
 * Used for the UI representation of the 'station status' for
 * each player.
 */
#define TI_RENDER_STATION_STATUS_WIDTH      9
#define TI_RENDER_STATION_STATUS_HEIGHT     12
#define TI_RENDER_STATION_INITIAL_XOFF      3
#define TI_RENDER_STATION_INITIAL_YOFF      1

#define TI_RENDER_BOARD_AREA_X              320
#define TI_RENDER_BOARD_AREA_Y              0
#define TI_RENDER_BOARD_AREA_WIDTH          480
#define TI_RENDER_BOARD_AREA_HEIGHT         480

#define TI_RENDER_BOARD_TILE_AREA_X         400
#define TI_RENDER_BOARD_TILE_AREA_Y         80
#define TI_RENDER_BOARD_TILE_AREA_MAX_X     720
#define TI_RENDER_BOARD_TILE_AREA_MAX_Y     400

/* Used to reference offsets in the TI_RENDER_GAME_SELECT_ACTION_*_OFFSETS arrays */
#define TI_OFFSET_DRAW                      0
#define TI_OFFSET_PLAY                      1
#define TI_OFFSET_DISCARD                   2
#define TI_OFFSET_PASS                      3

#endif /* __TICOORDS_H__ */
