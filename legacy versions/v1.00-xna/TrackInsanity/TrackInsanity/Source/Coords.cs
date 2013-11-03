/****************************************************************************
 *
 * Coords.cs - coordinates and extents for graphical elements
 *
 * Copyright 2007-2010 Shaun Brandt / Holy Meatgoat Software
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TrackInsanity.Source
{
    /*
     * A list of coordinates used by various displays in the game.  Ported from tiCoords.c/tiCoords.h.
     * Created as a struct to make things easier to organize (the old code was just a bunch of 
     * #defines).
     */
    struct Coords
    {
        /* The game resolution.  Assumes a default Windows 7 Mobile series device.  */
        public const int TI_GAME_XRES = 800;
        public const int TI_GAME_YRES = 480;

        /*******************************************************************************************************
         *  TITLE MENU
         *******************************************************************************************************/
        /* Start/end positions for the moving trains at the title screen */
        public const int TI_RENDER_TRAIN_INITIAL_X = -600;
        public const int TI_RENDER_TRAIN_FINAL_X = (TI_GAME_XRES + 72);

        /* Dimensions of the trains at the title screen */
        public const int TI_RENDER_TRAIN_BANNER_WIDTH = 528;
        public const int TI_RENDER_TRAIN_BANNER_HEIGHT = 22;

        /* Size and position of the 'Tap to play' message */
        public const int TI_RENDER_TAP_MSG_WIDTH = 400;
        public const int TI_RENDER_TAP_MSG_HEIGHT = 40;
        public const int TI_RENDER_TAP_MSG_X = ((TI_GAME_XRES / 2) - (TI_RENDER_TAP_MSG_WIDTH / 2));
        public const int TI_RENDER_TAP_MSG_Y = ((int)((float)TI_GAME_YRES * 0.6f));

        /* Size and position of the title screen menu */
        public const int TI_RENDER_TITLE_MENU_WIDTH = 178;
        public const int TI_RENDER_TITLE_MENU_HEIGHT = 130;
        public const int TI_RENDER_TITLE_MENU_X = ((TI_GAME_XRES / 2) - (TI_RENDER_TITLE_MENU_WIDTH / 2));
        public const int TI_RENDER_TITLE_MENU_Y = ((int)((float)TI_GAME_YRES * 0.5f));

        /* Size and position of the clickable areas of the title screen menu */
        /* 'New Game' */
        public const int TI_RENDER_NEW_GAME_MIN_X = (TI_RENDER_TITLE_MENU_X);
        public const int TI_RENDER_NEW_GAME_MAX_X = (TI_RENDER_TITLE_MENU_X + TI_RENDER_TITLE_MENU_WIDTH);
        public const int TI_RENDER_NEW_GAME_MIN_Y = (TI_RENDER_TITLE_MENU_Y);
        public const int TI_RENDER_NEW_GAME_MAX_Y = (TI_RENDER_TITLE_MENU_Y + 42);
        /* 'Options' */
        public const int TI_RENDER_OPTIONS_MIN_X = (TI_RENDER_TITLE_MENU_X);
        public const int TI_RENDER_OPTIONS_MAX_X = (TI_RENDER_TITLE_MENU_X + TI_RENDER_TITLE_MENU_WIDTH);
        public const int TI_RENDER_OPTIONS_MIN_Y = (TI_RENDER_TITLE_MENU_Y + 43);
        public const int TI_RENDER_OPTIONS_MAX_Y = (TI_RENDER_TITLE_MENU_Y + 88);
        /* 'Exit' */
        public const int TI_RENDER_EXIT_MIN_X = (TI_RENDER_TITLE_MENU_X);
        public const int TI_RENDER_EXIT_MAX_X = (TI_RENDER_TITLE_MENU_X + TI_RENDER_TITLE_MENU_WIDTH);
        public const int TI_RENDER_EXIT_MIN_Y = (TI_RENDER_TITLE_MENU_Y + 89);
        public const int TI_RENDER_EXIT_MAX_Y = (TI_RENDER_TITLE_MENU_Y + TI_RENDER_TITLE_MENU_HEIGHT);

        /*******************************************************************************************************
         *  'NEW GAME' MENU
         *******************************************************************************************************/
        /* Size and position of the clickable areas of the 'New Game' options screen */
        /* 'Back to title screen' */
        public const int TI_RENDER_BACK_TO_TITLE_MIN_X = 20;
        public const int TI_RENDER_BACK_TO_TITLE_MAX_X = 265;
        public const int TI_RENDER_BACK_TO_TITLE_MIN_Y = 398;
        public const int TI_RENDER_BACK_TO_TITLE_MAX_Y = 432;
        /* Start game */
        public const int TI_RENDER_START_GAME_MIN_X = 560;
        public const int TI_RENDER_START_GAME_MAX_X = 780;
        public const int TI_RENDER_START_GAME_MIN_Y = 398;
        public const int TI_RENDER_START_GAME_MAX_Y = 432;
        
        /* Size and position of the loading dialog */
        public const int TI_RENDER_LOADING_DIALOG_X = 200;
        public const int TI_RENDER_LOADING_DIALOG_Y = 180;
        public const int TI_RENDER_LOADING_DIALOG_WIDTH = 400;
        public const int TI_RENDER_LOADING_DIALOG_HEIGHT = 120;

        /* Size and position of the loading progress bar */
        public const int TI_RENDER_PROGRESS_BAR_X = 229;
        public const int TI_RENDER_PROGRESS_BAR_Y = 260;
        public const int TI_RENDER_PROGRESS_BAR_WIDTH = 342;
        public const int TI_RENDER_PROGRESS_BAR_HEIGHT = 15;

        /* Size and position of the player numbers (i.e. 'Player 2') */
        public static readonly int[,] TI_RENDER_PLAYER_OFFSETS = 
        {
            { 120, 226, 621, 161 },
            { 120, 266, 621, 121 },
            { 120, 306, 621, 81 },
            { 120, 346, 621, 41 },
            { 120, 386, 621, 1 }
        };

        /* Size and position of the 'number of player' digits */
        public static readonly int[,] TI_RENDER_NUM_PLAYER_OFFSETS = 
        {
            { 392, 100, 35, 35 },
            { 466, 100, 35, 35 },
            { 542, 100, 35, 35 },
            { 618, 100, 35, 35 },
            { 694, 100, 35, 35 }
        };

        /* Size and position of the 'Human' options */
        public static readonly int[,] TI_RENDER_HUMAN_OFFSETS =  
        {
            { 383, 148, 130, 40 },
            { 383, 188, 130, 40 },
            { 383, 228, 130, 40 },
            { 383, 267, 130, 40 },
            { 383, 309, 130, 40 },
            { 383, 348, 130, 40 }
        };

        /* Size and position of the 'Computer' options */
        public static readonly int[,] TI_RENDER_CPU_OFFSETS =  
        {
            { 561, 148, 175, 40 },
            { 561, 188, 175, 40 },
            { 561, 228, 175, 40 },
            { 561, 267, 175, 40 },
            { 561, 309, 175, 40 },
            { 561, 348, 175, 40 }
        };

        /*******************************************************************************************************
         *  'OPTIONS' MENU
         *******************************************************************************************************/
        /* Size and position of the 'Back to Title' button */
        public const int TI_RENDER_OPTIONS_BACK_TO_TITLE_MIN_X = 10;
        public const int TI_RENDER_OPTIONS_BACK_TO_TITLE_MAX_X = 158;
        public const int TI_RENDER_OPTIONS_BACK_TO_TITLE_MIN_Y = 402;
        public const int TI_RENDER_OPTIONS_BACK_TO_TITLE_MAX_Y = 430;

        /* Size and position of the 'OK' button */
        public const int TI_RENDER_OPTIONS_OK_MIN_X = 696;
        public const int TI_RENDER_OPTIONS_OK_MAX_X = 784;
        public const int TI_RENDER_OPTIONS_OK_MIN_Y = 401;
        public const int TI_RENDER_OPTIONS_OK_MAX_Y = 430;

        /* Size and position of the 'Highlight player tracks' options (No, Yes) */
        public static readonly int[,] TI_RENDER_OPTIONS_TRACKS_OFFSETS = 
        {
            { 433, 123, 39, 26 },
            { 529, 122, 56, 26 }
        };

        /* Size and position of the 'Highlight legal moves' options (No, Yes) */
        public static readonly int[,] TI_RENDER_OPTIONS_LEGAL_OFFSETS = 
        {
            { 433, 168, 39, 26 },
            { 529, 167, 56, 26 }
        };

        /* Size and position of the 'Show last move' options (No, Last Player, All Players) */
        public static readonly int[,] TI_RENDER_OPTIONS_LAST_MOVE_OFFSETS = 
        {
            { 329, 209, 39, 30 },
            { 405, 209, 169, 30 },
            { 612, 209, 166, 30 }
        };

        /* Size and position of the 'Computer AI level' options (Easy, Normal, Hard) */
        public static readonly int[,] TI_RENDER_OPTIONS_AI_LEVEL_OFFSETS = 
        {
            { 346, 253, 68, 31 },
            { 501, 253, 108, 31 },
            { 696, 253, 70, 31 }
        };

        /* Size and position of the 'Music volume' options (0 - 9) */
        public static readonly int[,] TI_RENDER_OPTIONS_MUSIC_OFFSETS = 
        {
            { 392, 311, 22, 29},
            { 430, 311, 17, 29},
            { 464, 311, 23, 29},
            { 503, 311, 23, 29}, 
            { 541, 311, 22, 29},
            { 579, 311, 24, 29},
            { 620, 311, 23, 29},
            { 661, 311, 18, 29},
            { 695, 311, 23, 29},
            { 732, 311, 23, 29}
        };

        /* Size and position of the 'Sound Effects volume' options (0 -9) */
        public static readonly int[,] TI_RENDER_OPTIONS_SOUND_OFFSETS = 
        {
            { 392, 355, 22, 29 },
            { 430, 355, 17, 29 },
            { 464, 355, 23, 29 },
            { 503, 355, 23, 29 },
            { 541, 355, 22, 29 },
            { 579, 355, 24, 29 },
            { 620, 355, 23, 29 },
            { 661, 355, 18, 29 },
            { 695, 355, 23, 29 },
            { 732, 355, 23, 29 }
        };

        /*******************************************************************************************************
         *  MAIN GAME INTERFACE OFFSETS
         *******************************************************************************************************/

        /* Used for the UI representation of the 'station status' for each player. */
        public const int TI_RENDER_STATION_STATUS_WIDTH = 9;
        public const int TI_RENDER_STATION_STATUS_HEIGHT = 12;
        public const int TI_RENDER_STATION_INITIAL_XOFF = 3;
        public const int TI_RENDER_STATION_INITIAL_YOFF = 1;

        /* Size and position of the board area of the screen (approximately the right 2/3 of the display) */
        public const int TI_RENDER_BOARD_AREA_X = 320;
        public const int TI_RENDER_BOARD_AREA_Y = 0;
        public const int TI_RENDER_BOARD_AREA_WIDTH = 480;
        public const int TI_RENDER_BOARD_AREA_HEIGHT = 480;

        /* Size and position of the 'tile area' (places where tiles can be played ) */
        public const int TI_RENDER_BOARD_TILE_AREA_X = 400;
        public const int TI_RENDER_BOARD_TILE_AREA_Y = 80;
        public const int TI_RENDER_BOARD_TILE_AREA_MAX_X = 720;
        public const int TI_RENDER_BOARD_TILE_AREA_MAX_Y = 400;

        /* Size and position of the X in the upper corner of the screen (used to quit the game) */
        public const int TI_RENDER_GAME_END_BUTTON_MIN_X = 765;
        public const int TI_RENDER_GAME_END_BUTTON_MAX_X = 792;
        public const int TI_RENDER_GAME_END_BUTTON_MIN_Y = 0;
        public const int TI_RENDER_GAME_END_BUTTON_MAX_Y = 23;

        /* Size and position of the cancel button in the corner of the screen (while playing a tile) */
        public const int TI_RENDER_PLAY_CANCEL_BUTTON_MIN_X = 650;
        public const int TI_RENDER_PLAY_CANCEL_BUTTON_MAX_X = 800;
        public const int TI_RENDER_PLAY_CANCEL_BUTTON_MIN_Y = 430;
        public const int TI_RENDER_PLAY_CANCEL_BUTTON_MAX_Y = 480;

        /* Size and position of the color boxes for each player on the left side of the screen */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_COLOR_OFFSETS = 
        {
            { 6, 6, 46, 73 },
            { 6, 85, 46, 73 },
            { 6, 164, 46, 73 },
            { 6, 243, 46, 73 },
            { 6, 322, 46, 73 },
            { 6, 401, 46, 73 }
        };

        /* Size and position of the highlighted box around the current player */
        public static readonly int[,] TI_RENDER_GAME_CUR_PLAYER_OFFSETS = 
        {
            { 2, 2, 54, 81 },
            { 2, 81, 54, 81 },
            { 2, 160, 54, 81 },
            { 2, 239, 54, 81 },
            { 2, 318, 54, 81 },
            { 2, 397, 54, 81 }
        };

        /* Size and position of each player's status box (name, score) */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_BG_OFFSETS = 
        {
            { 58, 6, 165, 73 },
            { 58, 85, 165, 73 },
            { 58, 164, 165, 73 },
            { 58, 243, 165, 73 },
            { 58, 322, 165, 73 },
            { 58, 401, 165, 73 }
        };

        /* Size and position of each player's primary tile */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_TILE1_OFFSETS = 
        {
            { 229, 6, 40, 40 },
            { 229, 85, 40, 40 },
            { 229, 164, 40, 40 },
            { 229, 243, 40, 40 },
            { 229, 322, 40, 40 },
            { 229, 401, 40, 40 }
        };

        /* Size and position of each player's secondary tile */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_TILE2_OFFSETS = 
        {
            { 275, 6, 40, 40 },
            { 275, 85, 40, 40 },
            { 275, 164, 40, 40 },
            { 275, 243, 40, 40 },
            { 275, 322, 40, 40 },
            { 275, 401, 40, 40 }
        };

        /* Size and position of the highlighted box around the player's primary tile */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS = 
        {
            { 225, 2, 48, 48 },
            { 225, 81, 48, 48 },
            { 225, 160, 48, 48 },
            { 225, 239, 48, 48 },
            { 225, 318, 48, 48 },
            { 225, 397, 48, 48 }
        };

        /* Size and position of the highlighted box around the player's secondary tile */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS = 
        {
            { 271, 2, 48, 48 },
            { 271, 81, 48, 48 },
            { 271, 160, 48, 48 },
            { 271, 239, 48, 48 },
            { 271, 318, 48, 48 },
            { 271, 397, 48, 48 }
        };

        /* Size and position of the area containing each player's completed stations */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_STATIONS_OFFSETS = 
        {
            { 229, 51, 86, 28 },
            { 229, 130, 86, 28 },
            { 229, 209, 86, 28 },
            { 229, 288, 86, 28 },
            { 229, 367, 86, 28 },
            { 229, 446, 86, 28 }
        };

        /* Size and position of the player's name text ('Player 1') */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_NAME_OFFSETS = 
        {
            { 63, 8, 96, 23 },
            { 63, 87, 96, 23 },
            { 63, 166, 96, 23 },
            { 63, 245, 96, 23 },
            { 63, 324, 96, 23 },
            { 63, 403, 96, 23 }
        };

        /* Size and position of the player's type text ('CPU/Human') */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_TYPE_OFFSETS = 
        {
            { 63, 32, 123, 22 },
            { 63, 111, 123, 22 },
            { 63, 190, 123, 22 },
            { 63, 269, 123, 22 },
            { 63, 348, 123, 22 },
            { 63, 427, 123, 22 }
        };

        /* Size and position of the player's score text */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_SCORE_STR_OFFSETS = 
        {
            { 63, 59, 67, 18 },
            { 63, 138, 67, 18 },
            { 63, 217, 67, 18 },
            { 63, 296, 67, 18 },
            { 63, 375, 67, 18 },
            { 63, 454, 67, 18 }
        };

        /* Size and position of the player's score */
        public static readonly int[,] TI_RENDER_GAME_PLAYER_SCORE_OFFSETS = 
        {
            { 150, 58 },
            { 150, 137 },
            { 150, 216 },
            { 150, 295 },
            { 150, 374 },
            { 150, 453 }
        };

        /* The 5 values here are: x position, y position, width, height, and horizontal
                 * offset in the train tile bitmap of the appropriate image */
        public static readonly int[,] TI_RENDER_GAME_STATION_TRAIN_OFFSETS = 
        {
            /* Top of board */
            { 408, 56, 10, 20, 0 },
            { 448, 56, 10, 20, 0 },
            { 488, 56, 10, 20, 0 },
            { 528, 56, 10, 20, 0 },
            { 568, 56, 10, 20, 0 },
            { 608, 56, 10, 20, 0 },
            { 648, 56, 10, 20, 0 },
            { 688, 56, 10, 20, 0 },

            /* Right side of board */
            { 724, 88, 20, 10, 1 },
            { 724, 128, 20, 10, 1 },
            { 724, 168, 20, 10, 1 },
            { 724, 208, 20, 10, 1 },
            { 724, 248, 20, 10, 1 },
            { 724, 288, 20, 10, 1 },
            { 724, 328, 20, 10, 1 },
            { 724, 368, 20, 10, 1 },

            /* Bottom of board */
            { 702, 404, 10, 20, 2 },
            { 662, 404, 10, 20, 2 },
            { 622, 404, 10, 20, 2 },
            { 582, 404, 10, 20, 2 },
            { 542, 404, 10, 20, 2 },
            { 502, 404, 10, 20, 2 },
            { 462, 404, 10, 20, 2 },
            { 422, 404, 10, 20, 2 },

            /* Left side of board */
            { 376, 382, 20, 10, 3 },
            { 376, 342, 20, 10, 3 },
            { 376, 302, 20, 10, 3 },
            { 376, 262, 20, 10, 3 },
            { 376, 222, 20, 10, 3 },
            { 376, 182, 20, 10, 3 },
            { 376, 142, 20, 10, 3 },
            { 376, 102, 20, 10, 3 }
        };

        /* The three values here are: x position, y position, and x offset of the station within the image bitmap */
        public static readonly int[,] TI_RENDER_GAME_STATION_OFFSETS = 
        {
            /* Top stations */
            { 400, 40, 0 },
            { 440, 40, 0 },
            { 480, 40, 0 },
            { 520, 40, 0 },
            { 560, 40, 0 },
            { 600, 40, 0 },
            { 640, 40, 0 },
            { 680, 40, 0 },

            /* Right stations */
            { 720, 80, 1 },
            { 720, 120, 1 },
            { 720, 160, 1 },
            { 720, 200, 1 },
            { 720, 240, 1 },
            { 720, 280, 1 },
            { 720, 320, 1 },
            { 720, 360, 1 },

            /* Bottom stations */
            { 680, 400, 2 },
            { 640, 400, 2 },
            { 600, 400, 2 },
            { 560, 400, 2 },
            { 520, 400, 2 },
            { 480, 400, 2 },
            { 440, 400, 2 },
            { 400, 400, 2 },

            /* Left stations */
            { 360, 360, 3 },
            { 360, 320, 3 },
            { 360, 280, 3 },
            { 360, 240, 3 },
            { 360, 200, 3 },
            { 360, 160, 3 },
            { 360, 120, 3 },
            { 360, 80, 3 }
        };

        /* Size and position of the 'choose tile to discard' dialog */
        public static readonly int[] TI_RENDER_GAME_CHOOSE_DISCARD_OFFSET = new int[] { 360, 160, 400, 160 };

        /* Size and position of the buttons on the 'choose tile to discard' dialog.  These are relative to the screen,
         * not to the corner of the dialog itself. */
        public static readonly int[,] TI_RENDER_GAME_CHOOSE_DISCARD_BUTTON_OFFSETS = 
        {
            { 369, 264, 187, 47 },
            { 565, 264, 186, 47 }
        };

        /* Size and position of the 'Really exit?' dialog */
        public static readonly int[] TI_RENDER_GAME_CONFIRM_EXIT_OFFSET = new int[] { 200, 180, 400, 120 };

        /* Size and position of the buttons on the 'Really exit?' dialog.  These are relative to the screen, 
         * not to the corner of the dialog itself. */
        public static readonly int[,] TI_RENDER_GAME_CONFIRM_EXIT_BUTTON_OFFSETS = 
        {
            { 208, 244, 187, 47 },
            { 405, 244, 186, 47 }
        };

        /* Size and position of the 'Select action' dialog */
        public static readonly int[] TI_RENDER_GAME_SELECT_ACTION_OFFSET = new int[] { 320, 400, 480, 80 };

        /* Size and position of the buttons on the 'Select action' dialog.  These are relative to the screen,
         * not to the corner of the dialog itself. */
        public static readonly int[,] TI_RENDER_GAME_SELECT_ACTION_BUTTON_OFFSETS = 
        {
            { 327, 443, 111, 32 },
            { 445, 443, 111, 32 },
            { 563, 443, 111, 32 },
            { 681, 443, 112, 32 }
        };

        /* Size and position of the text on the 'Select action' dialog buttons.  These are relative to the
         * screen, not to the corner of the dialog itself. */
        public static readonly int[,] TI_RENDER_GAME_SELECT_ACTION_TEXT_OFFSETS = 
        {
            { 337, 452, 94, 17 },
            { 479, 452, 44, 18 },
            { 583, 451, 74, 19 },
            { 717, 453, 46, 16 }
        };

        public static readonly int[] TI_RENDER_GAME_DRAW_TILE_OFFSET = new int[] { 360, 40, 400, 400 };

        public static readonly int[,] TI_RENDER_GAME_DRAW_TILE_BUTTON_OFFSETS = 
        {
            { 368, 392, 187, 39 },
            { 565, 392, 186, 39 }
        };

        public static readonly int[,] TI_RENDER_GAME_DRAW_TILE_TILE_OFFSETS = 
        {
            /* Row 1 */
            { 384, 114, 64, 64 },
            { 480, 114, 64, 64 }, 
            { 576, 114, 64, 64 },
            { 672, 114, 64, 64 },

            /* Row 2 */
            { 384, 210, 64, 64 },
            { 480, 210, 64, 64 },
            { 576, 210, 64, 64 },
            { 672, 210, 64, 64 },

            /* Row 3 */
            { 384, 306, 64, 64 },
            { 480, 306, 64, 64 },
            { 576, 306, 64, 64 },
            { 672, 306, 64, 64 }
        };

        public static readonly int[] TI_RENDER_GAME_CPU_THINKING_OFFSET = new int[] { 320, 400, 480, 80 };

        /*******************************************************************************************************
         *  END OF GAME OFFSETS
         *******************************************************************************************************/        

        /* Size and position of the 'Game is finished' dialog */
        public static readonly int[] TI_RENDER_GAME_FINISHED_OFFSETS = new int[] { 200, 180, 400, 120 };

        /* Size and position of the 'Ok' button on the 'Game is finished' dialog */
        public static readonly int[] TI_RENDER_GAME_FINISHED_BUTTON_OFFSETS = new int[] { 208, 244, 382, 46 };

        /* Size and position of the rank numbers on the final score screen */
        public static readonly int[,] TI_RENDER_GAME_RESULTS_RANK_OFFSETS = 
        {
            { 196, 161 },
            { 196, 202 },
            { 196, 235 },
            { 196, 268 },
            { 196, 301 },
            { 196, 334 }
        };

        /* Size and position of the player names on the final score screen */
        public static readonly int[,] TI_RENDER_GAME_RESULTS_PLAYER_NAME_OFFSETS = 
        {
            { 269, 161 },
            { 271, 202 },
            { 271, 235 },
            { 271, 268 },
            { 271, 301 }, 
            { 271, 334 }
        };

        /* Size and position of the player scores on the final score screen */
        public static readonly int[,] TI_RENDER_GAME_RESULTS_SCORE_OFFSETS = 
        {
            { 532, 161 },
            { 533, 202 },
            { 533, 235 },
            { 533, 268 },
            { 533, 301 },
            { 533, 334 }
        };

        public const int TI_RENDER_GAME_RESULTS_TO_TITLE_MIN_X = 545;
        public const int TI_RENDER_GAME_RESULTS_TO_TITLE_MAX_X = 780;
        public const int TI_RENDER_GAME_RESULTS_TO_TITLE_MIN_Y = 395;
        public const int TI_RENDER_GAME_RESULTS_TO_TITLE_MAX_Y = 430;

        /*******************************************************************************************************
         *  BITMAP OFFSETS
         *******************************************************************************************************/
        /* Offsets of the 'Highlight player tracks' option text in the bitmap ('Yes'/'No') */
        public static readonly int[] TI_RENDER_OPTIONS_TRACKS_FILE_OFFSETS = new int[] { 0, 39 };

        /* Offsets of the 'Highlight legal moves' option text in the bitmap ('Yes'/'No') */
        public static readonly int[] TI_RENDER_OPTIONS_LEGAL_FILE_OFFSETS = new int[] { 0, 39 };

        public static readonly int[] TI_RENDER_OPTIONS_LAST_MOVE_FILE_OFFSETS = new int[] { 0, 39, 208 };

        public static readonly int[] TI_RENDER_OPTIONS_AI_LEVEL_FILE_OFFSETS = new int[] { 0, 68, 176 };

        public static readonly int[] TI_RENDER_OPTIONS_DIGITS_OFFSETS = new int[] { 0, 22, 39, 62, 85, 107, 131, 154, 172, 195 };

        public static readonly int[] TI_RENDER_GAME_DIGIT_WIDTHS = new int[] { 15, 11, 17, 16, 16, 17, 16, 13, 15, 15 };

        /* This array is tricky.  The first dimension represents the entry point, while
         * the second represents the exit point.  The value of 
         * TI_RENDER_GAME_TRACK_OVERLAY_EXITS[x][y] give the horizontal offset in the 
         * track overlay tile strip of the appropriate overlay for this entry/exit
         * combo.  A value of -1 means that the particular combination of entry and exit
         * is not valid */
        public static readonly int[,] TI_RENDER_GAME_TRACK_OVERLAY_EXITS = 
        {
            { -1, 0, -1, 12, -1, 8, -1, 7 },      /* Entry 0, exits 0-7 */
            { 0, -1, 4, -1, 9, -1, 15, -1 },      /* Entry 1, exits 0-7 */
            { -1, 5, -1, 1, -1, 13, -1, 10 },     /* Entry 2, exits 0-7 */
            { 12, -1, 1, -1, 5, -1, 11, -1 },     /* Entry 3, exits 0-7 */
            { -1, 9, -1, 5, -1, 2, -1, 14 },      /* Entry 4, exits 0-7 */
            { 8, -1, 13, -1, 2, -1, 6, -1 },      /* Entry 5, exits 0-7 */
            { -1, 15, -1, 11, -1, 6, -1, 3 },     /* Entry 6, exits 0-7 */
            { 7, -1, 10, -1, 14, -1, 3, -1 }      /* Entry 7, exits 0-7 */
        };        

        /* Widths of the digits in the two font sizes used on the final score screen */
        public static readonly int[] TI_RENDER_GAME_RESULTS_SMALL_DIGIT_WIDTHS = new int[] { 13, 9, 14, 14, 13, 14, 13, 10, 13, 13 };
        public static readonly int[] TI_RENDER_GAME_RESULTS_LARGE_DIGIT_WIDTHS = new int[] { 18, 12, 19, 20, 18, 20, 19, 15, 18, 20 };

        /* Digit sizes and spacing */
        public const int TI_RENDER_GAME_DIGIT_HEIGHT = 21;
        public const int TI_RENDER_GAME_LETTER_SPACING = 1;
        public const int TI_RENDER_GAME_DIGIT_OFFSET = 17;

        /* Size of the backing area used to cover old score data */
        public const int TI_RENDER_GAME_SCORE_BACKING_WIDTH = 71;
        public const int TI_RENDER_GAME_SCORE_BACKING_HEIGHT = 21;

        public const int TI_RENDER_GAME_RESULTS_SMALL_DIGIT_SPACING = 1;
        public const int TI_RENDER_GAME_RESULTS_SMALL_DIGIT_OFFSET = 14;
        public const int TI_RENDER_GAME_RESULTS_SMALL_DIGIT_HEIGHT = 19;

        public const int TI_RENDER_GAME_RESULTS_LARGE_DIGIT_SPACING = 1;
        public const int TI_RENDER_GAME_RESULTS_LARGE_DIGIT_OFFSET = 21;
        public const int TI_RENDER_GAME_RESULTS_LARGE_DIGIT_HEIGHT = 25;

        public const int TI_RENDER_GAME_RESULTS_SMALL_PLAYER_WIDTH = 100;
        public const int TI_RENDER_GAME_RESULTS_SMALL_PLAYER_HEIGHT = 20;

        public const int TI_RENDER_GAME_RESULTS_LARGE_PLAYER_WIDTH = 130;
        public const int TI_RENDER_GAME_RESULTS_LARGE_PLAYER_HEIGHT = 28;

        /*******************************************************************************************************
         *  MISCELLANEOUS
         *******************************************************************************************************/

        /* There are two different tile sizes: one for the game board, and a larger one for the 
         * 'select tile' dialog.  */
        public const int TI_RENDER_SMALL_TILE_WIDTH = 40;
        public const int TI_RENDER_LARGE_TILE_WIDTH = 64;

        /* The 'offset' (number of tiles into the tile bitmap) of the tile back */
        public const int TI_RENDER_TILE_BACK_OFFSET = 25;

        /* When displaying the highlighted tile, this is the offset to use to place the highlight correctly
         * over the tile.  This value should be subtracted from the tile offsets. */
        public const int TI_RENDER_LARGE_TILE_HIGHLIGHT_OFFSET = 8;

        /* All of the train graphics are placed in a single file.  Each train is contained in a grid square with
         * the dimensions of the following variables */
        public const int TI_RENDER_TRAIN_TILE_WIDTH = 20;
        public const int TI_RENDER_TRAIN_TILE_HEIGHT = 20;

        /* Width and height of the station tiles */
        public const int TI_RENDER_STATION_WIDTH = 40;
        public const int TI_RENDER_STATION_HEIGHT = 40;
        
        /* Used to reference offsets in the TI_RENDER_GAME_SELECT_ACTION_*_OFFSETS arrays */
        public const int TI_OFFSET_DRAW = 0;
        public const int TI_OFFSET_PLAY = 1;
        public const int TI_OFFSET_DISCARD = 2;
        public const int TI_OFFSET_PASS = 3;
        
    }
}
