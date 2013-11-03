/****************************************************************************
*
* Render.cs - Functions related to drawing game elements on the display.
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
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Xna.Framework.Input;

namespace TrackInsanity.Source
{
    public class Render
    {
        /* This structure contains values that relate to the locations of elements on the screen at different
         * resolutions.  The only set of values defined now is for a Windows 7 Mobile series device (800x480).
         * In the future, one of two things may happen to support other platforms:
         *   - Additional values for common resolutions (480p, 720p, 1080p, a few PC settings) will be created.
         *   - (Better choice) All values here will be defined as relative to a single base resolution. */
        private class Coords
        {
            /* The game resolution.  Assumes a default Windows 7 Mobile series device.  */
            public const int GAME_XRES = 800;
            public const int GAME_YRES = 480;

            /*******************************************************************************************************
             *  TITLE MENU
             *******************************************************************************************************/

            /* The individual words 'Track' and 'Insanity' */
            public const int TRACK_BAR_HEIGHT = 40;

            public const int TRACK_WORD_INITIAL_X = -443;
            public const int TRACK_WORD_FINAL_X = 57;

            public const int INSANITY_WORD_INITIAL_X = (GAME_XRES + 100);
            public const int INSANITY_WORD_FINAL_X = 351;

            public const int TRACK_WORD_Y = 77;

            /* Start/end positions for the moving trains at the title screen */
            public const int TRAIN_INITIAL_X = -600;
            public const int TRAIN_FINAL_X = (GAME_XRES + 72);

            /* Dimensions of the trains at the title screen */
            public const int TRAIN_BANNER_WIDTH = 528;
            public const int TRAIN_BANNER_HEIGHT = 22;

            /* Size and position of the 'Tap to play' message */
            public const int TITLE_SCREEN_TAP_TO_PLAY_WIDTH = 400;
            public const int TITLE_SCREEN_TAP_TO_PLAY_HEIGHT = 40;
            public const int TITLE_SCREEN_TAP_TO_PLAY_X = ((GAME_XRES / 2) - (TITLE_SCREEN_TAP_TO_PLAY_WIDTH / 2));
            public const int TITLE_SCREEN_TAP_TO_PLAY_Y = ((int)((float)GAME_YRES * 0.6f));

            /* Size and position of the title screen menu */
            public const int TITLE_SCREEN_MENU_WIDTH = 178;
            public const int TITLE_SCREEN_MENU_HEIGHT = 130;
            public const int TITLE_SCREEN_MENU_X = ((GAME_XRES / 2) - (TITLE_SCREEN_MENU_WIDTH / 2));
            public const int TITLE_SCREEN_MENU_Y = ((int)((float)GAME_YRES * 0.5f));

            public const int TITLE_COPYRIGHT_TEXT_X = 0;
            public const int TITLE_COPYRIGHT_TEXT_Y = 403;
            public const int TITLE_COPYRIGHT_TEXT_WIDTH = 800;
            public const int TITLE_COPYRIGHT_TEXT_HEIGHT = 31;

            /* Size and position of the clickable areas of the title screen menu */
            /* 'New Game' */
            public const int NEW_GAME_MIN_X = (TITLE_SCREEN_MENU_X);
            public const int NEW_GAME_MAX_X = (TITLE_SCREEN_MENU_X + TITLE_SCREEN_MENU_WIDTH);
            public const int NEW_GAME_MIN_Y = (TITLE_SCREEN_MENU_Y);
            public const int NEW_GAME_MAX_Y = (TITLE_SCREEN_MENU_Y + 42);
            /* 'Options' */
            public const int OPTIONS_MIN_X = (TITLE_SCREEN_MENU_X);
            public const int OPTIONS_MAX_X = (TITLE_SCREEN_MENU_X + TITLE_SCREEN_MENU_WIDTH);
            public const int OPTIONS_MIN_Y = (TITLE_SCREEN_MENU_Y + 43);
            public const int OPTIONS_MAX_Y = (TITLE_SCREEN_MENU_Y + 88);
            /* 'Exit' */
            public const int EXIT_MIN_X = (TITLE_SCREEN_MENU_X);
            public const int EXIT_MAX_X = (TITLE_SCREEN_MENU_X + TITLE_SCREEN_MENU_WIDTH);
            public const int EXIT_MIN_Y = (TITLE_SCREEN_MENU_Y + 89);
            public const int EXIT_MAX_Y = (TITLE_SCREEN_MENU_Y + TITLE_SCREEN_MENU_HEIGHT);

            /*******************************************************************************************************
             *  'NEW GAME' MENU
             *******************************************************************************************************/
            /* Size and position of the clickable areas of the 'New Game' options screen */
            /* 'Back to title screen' */
            public const int NEW_GAME_BACK_TO_TITLE_MIN_X = 20;
            public const int NEW_GAME_BACK_TO_TITLE_MAX_X = 265;
            public const int NEW_GAME_BACK_TO_TITLE_MIN_Y = 398;
            public const int NEW_GAME_BACK_TO_TITLE_MAX_Y = 432;
            /* Start game */
            public const int NEW_GAME_START_GAME_MIN_X = 560;
            public const int NEW_GAME_START_GAME_MAX_X = 780;
            public const int NEW_GAME_START_GAME_MIN_Y = 398;
            public const int NEW_GAME_START_GAME_MAX_Y = 432;


            /* Size and position of the player numbers (i.e. 'Player 2') */
            public static readonly int[] NEW_GAME_PLAYER_NAME_OFFSETS = { 226, 266, 306, 346, 386 };
            public const int NEW_GAME_FONT_HEIGHT = 40;

            /* Size and position of the 'number of player' digits */
            public static readonly int[,] NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS = 
            {
                { 392, 100, 35, 35 },
                { 466, 100, 35, 35 },
                { 542, 100, 35, 35 },
                { 618, 100, 35, 35 },
                { 694, 100, 35, 35 }
            };

            /* Size and position of the 'Human' options */
            public static readonly int[,] NEW_GAME_HUMAN_PLAYER_OFFSETS =  
            {
                { 383, 148, 130, 40 },
                { 383, 188, 130, 40 },
                { 383, 228, 130, 40 },
                { 383, 267, 130, 40 },
                { 383, 309, 130, 40 },
                { 383, 348, 130, 40 }
            };

            /* Size and position of the 'Computer' options */
            public static readonly int[,] NEW_GAME_CPU_PLAYER_OFFSETS =  
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

            /* Size and position of the 'OK' button */
            public const int OPTIONS_SCREEN_OK_MIN_X = 696;
            public const int OPTIONS_SCREEN_OK_MAX_X = 784;
            public const int OPTIONS_SCREEN_OK_MIN_Y = 401;
            public const int OPTIONS_SCREEN_OK_MAX_Y = 430;

            /* Size and position of the 'Highlight player tracks' options (No, Yes) */
            public static readonly int[,] OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS = 
            {
                { 433, 123, 39, 26 },
                { 529, 122, 56, 26 }
            };

            /* Size and position of the 'Highlight legal moves' options (No, Yes) */
            public static readonly int[,] OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS = 
            {
                { 433, 168, 39, 26 },
                { 529, 167, 56, 26 }
            };

            /* Size and position of the 'Show last move' options (No, Last Player, All Players) */
            public static readonly int[,] OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS = 
            {
                { 329, 209, 39, 30 },
                { 405, 209, 169, 30 },
                { 612, 209, 166, 30 }
            };

            /* Size and position of the 'Computer AI level' options (Easy, Normal, Hard) */
            public static readonly int[,] OPTIONS_SCREEN_AI_LEVEL_OFFSETS = 
            {
                { 346, 253, 68, 31 },
                { 501, 253, 108, 31 },
                { 696, 253, 70, 31 }
            };  

            /* Size and position of the 'Music volume' options (0 - 9) */
            public static readonly int[,] OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS = 
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
            public static readonly int[,] OPTIONS_SCREEN_SFX_VOLUME_OFFSETS = 
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
            public const int IN_GAME_STATION_STATUS_WIDTH = 9;
            public const int IN_GAME_STATION_STATUS_HEIGHT = 12;
            public const int IN_GAME_STATION_INITIAL_XOFF = 3;
            public const int IN_GAME_STATION_INITIAL_YOFF = 1;

            /* Size and position of the board area of the screen (approximately the right 2/3 of the display) */
            public const int IN_GAME_BOARD_AREA_X = 320;
            public const int IN_GAME_BOARD_AREA_Y = 0;
            public const int IN_GAME_BOARD_AREA_WIDTH = 480;
            public const int IN_GAME_BOARD_AREA_HEIGHT = 480;

            /* Size and position of the 'tile area' (places where tiles can be played ) */
            public const int IN_GAME_BOARD_TILE_AREA_X = 400;
            public const int IN_GAME_BOARD_TILE_AREA_Y = 80;
            public const int IN_GAME_BOARD_TILE_AREA_MAX_X = 720;
            public const int IN_GAME_BOARD_TILE_AREA_MAX_Y = 400;

            /* Size and position of the X in the upper corner of the screen (used to quit the game) */
            public const int IN_GAME_END_GAME_BUTTON_MIN_X = 765;
            public const int IN_GAME_END_GAME_BUTTON_MAX_X = 792;
            public const int IN_GAME_END_GAME_BUTTON_MIN_Y = 0;
            public const int IN_GAME_END_GAME_BUTTON_MAX_Y = 23;

            /* Size and position of the cancel button in the corner of the screen (while playing a tile) */
            public const int IN_GAME_CANCEL_ACTION_BUTTON_MIN_X = 650;
            public const int IN_GAME_CANCEL_ACTION_BUTTON_MAX_X = 800;
            public const int IN_GAME_ACTION_CANCEL_BUTTON_MIN_Y = 430;
            public const int IN_GAME_ACTION_CANCEL_BUTTON_MAX_Y = 480;

            /* Size and position of the color boxes for each player on the left side of the screen */
            public static readonly int[,] IN_GAME_PLAYER_COLOR_REGION_OFFSETS = 
            {
                { 6, 6, 46, 73 },
                { 6, 85, 46, 73 },
                { 6, 164, 46, 73 },
                { 6, 243, 46, 73 },
                { 6, 322, 46, 73 },
                { 6, 401, 46, 73 }
            };

            /* Size and position of the highlighted box around the current player */
            public static readonly int[,] IN_GAME_CURRENT_PLAYER_HIGHLIGHT_OFFSETS = 
            {
                { 2, 2, 54, 81 },
                { 2, 81, 54, 81 },
                { 2, 160, 54, 81 },
                { 2, 239, 54, 81 },
                { 2, 318, 54, 81 },
                { 2, 397, 54, 81 }
            };

            /* Size and position of each player's status box (name, score) */
            public static readonly int[,] IN_GAME_PLAYER_BG_OFFSETS = 
            {
                { 58, 6, 165, 73 },
                { 58, 85, 165, 73 },
                { 58, 164, 165, 73 },
                { 58, 243, 165, 73 },
                { 58, 322, 165, 73 },
                { 58, 401, 165, 73 }
            };

            /* Size and position of each player's primary tile */
            public static readonly int[,] IN_GAME_PLAYER_TILE1_OFFSETS = 
            {
                { 229, 6, 40, 40 },
                { 229, 85, 40, 40 },
                { 229, 164, 40, 40 },
                { 229, 243, 40, 40 },
                { 229, 322, 40, 40 },
                { 229, 401, 40, 40 }
            };

            /* Size and position of each player's secondary tile */
            public static readonly int[,] IN_GAME_PLAYER_TILE2_OFFSETS = 
            {
                { 275, 6, 40, 40 },
                { 275, 85, 40, 40 },
                { 275, 164, 40, 40 },
                { 275, 243, 40, 40 },
                { 275, 322, 40, 40 },
                { 275, 401, 40, 40 }
            };

            /* Size and position of the highlighted box around the player's primary tile */
            public static readonly int[,] IN_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS = 
            {
                { 225, 2, 48, 48 },
                { 225, 81, 48, 48 },
                { 225, 160, 48, 48 },
                { 225, 239, 48, 48 },
                { 225, 318, 48, 48 },
                { 225, 397, 48, 48 }
            };

            /* Size and position of the highlighted box around the player's secondary tile */
            public static readonly int[,] IN_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS = 
            {
                { 271, 2, 48, 48 },
                { 271, 81, 48, 48 },
                { 271, 160, 48, 48 },
                { 271, 239, 48, 48 },
                { 271, 318, 48, 48 },
                { 271, 397, 48, 48 }
            };

            /* Size and position of the area containing each player's completed stations */
            public static readonly int[,] IN_GAME_PLAYER_STATIONS_OFFSETS = 
            {
                { 229, 51, 86, 28 },
                { 229, 130, 86, 28 },
                { 229, 209, 86, 28 },
                { 229, 288, 86, 28 },
                { 229, 367, 86, 28 },
                { 229, 446, 86, 28 }
            };

            /* Size and position of the player's name text ('Player 1') */
            public static readonly int[,] IN_GAME_PLAYER_NAME_OFFSETS = 
            {
                { 63, 8, 96, 23 },
                { 63, 87, 96, 23 },
                { 63, 166, 96, 23 },
                { 63, 245, 96, 23 },
                { 63, 324, 96, 23 },
                { 63, 403, 96, 23 }
            };

            /* Size and position of the player's type text ('CPU/Human') */
            public static readonly int[,] IN_GAME_PLAYER_TYPE_OFFSETS = 
            {
                { 63, 32, 123, 22 },
                { 63, 111, 123, 22 },
                { 63, 190, 123, 22 },
                { 63, 269, 123, 22 },
                { 63, 348, 123, 22 },
                { 63, 427, 123, 22 }
            };

            /* Size and position of the player's score text */
            public static readonly int[,] IN_GAME_PLAYER_SCORE_STR_OFFSETS = 
            {
                { 63, 59, 67, 18 },
                { 63, 138, 67, 18 },
                { 63, 217, 67, 18 },
                { 63, 296, 67, 18 },
                { 63, 375, 67, 18 },
                { 63, 454, 67, 18 }
            };

            /* Size and position of the player's score */
            public static readonly int[,] IN_GAME_PLAYER_SCORE_OFFSETS = 
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
            public static readonly int[,] IN_GAME_STATION_TRAIN_OFFSETS = 
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
            public static readonly int[,] IN_GAME_STATION_OFFSETS = 
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
            public static readonly int[] IN_GAME_CHOOSE_DISCARD_DIALOG_OFFSET = new int[] { 360, 160, 400, 160 };

            /* Size and position of the buttons on the 'choose tile to discard' dialog.  These are relative to the 
             * upper left corner of the dialog box and are specified as (minX, minY, maxX, maxY) */
            public static readonly int[,] IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS = 
            {
                { 9, 104, 196, 151 },
                { 205, 104, 391, 151 }
            };

            /* Size and position of the 'Really exit?' dialog */
            public static readonly int[] IN_GAME_CONFIRM_EXIT_DIALOG_OFFSET = new int[] { 200, 180, 400, 120 };

            /* Size and position of the buttons on the 'Really exit?' dialog.  These are relative to the upper
             * left corner of the dialog box and are specified as (minX, minY, maxX, maxY) */
            public static readonly int[,] IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS = 
            {
                { 8, 64, 195, 111 },
                { 205, 64, 391, 111 }
            };

            /* Size and position of the 'Select action' dialog */
            public static readonly int[] IN_GAME_SELECT_ACTION_DIALOG_OFFSET = new int[] { 320, 400, 480, 80 };

            /* Size and position of the 'Showing board' dialog */
            public static readonly int[,] IN_GAME_SHOWING_BOARD_OFFSET = 
            {
                { 320, 0, 480, 40},
                { 320, 440, 480, 40 }
            };

            /* Size and position of the 'Player n's turn' dialog */
            public static readonly int[] IN_GAME_PLAYER_TURN_DIALOG_OFFSET = new int[] { 320, 400, 480, 80 };

            /* Size and position of the buttons on the 'Select action' dialog.  These are relative to the upper left
             * corner of the dialog, and are specified as (minX, minY, maxX, maxY) */
            public static readonly int[,] IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS = 
            {
                { 27, 43, 118, 75 },
                { 125, 43, 236, 75 },
                { 243, 43, 354, 75 },
                { 361, 43, 473, 75 }
            };

            /* Size and position of the text on the 'Select action' dialog buttons.  These are relative to the
             * upper left corner of the dialog, and are specified as (x, y, width, height) */
            public static readonly int[,] IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS = 
            {
                { 17, 52, 94, 17 },
                { 159, 52, 44, 18 },
                { 263, 51, 74, 19 },
                { 397, 53, 46, 16 }
            };

            /* Size and position of the 'Draw Tile' dialog. */
            public static readonly int[] IN_GAME_DRAW_TILE_DIALOG_OFFSET = new int[] { 360, 40, 400, 400 };

            /* Size and position of the 'OK' and 'Cancel' buttons in the 'Draw Tile' dialog */
            public static readonly int[,] IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS = 
            {
                { 8, 352, 195, 391 },
                { 205, 352, 391, 391 }
            };

            /* Size and position of the individual tiles in the 'Draw Tile' dialog */
            public static readonly int[,] IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS = 
            {
                /* Row 1 */
                { 24, 74, 64, 64 },
                { 120, 74, 64, 64 }, 
                { 216, 74, 64, 64 },
                { 312, 74, 64, 64 },

                /* Row 2 */
                { 24, 170, 64, 64 },
                { 120, 170, 64, 64 },
                { 216, 170, 64, 64 },
                { 312, 170, 64, 64 },

                /* Row 3 */
                { 24, 266, 64, 64 },
                { 120, 266, 64, 64 },
                { 216, 266, 64, 64 },
                { 312, 266, 64, 64 }
            };

            /* Size and position of the individual tile clickable areas in the 'Draw Tile' dialog */
            public static readonly int[,] IN_GAME_DRAW_TILE_DIALOG_TILE_CLICKABLE_AREA_OFFSETS = 
            {
                /* Row 1 */
                { 24, 74, 88, 138 },
                { 120, 74, 184, 138 },
                { 216, 74, 280, 138 },
                { 312, 74, 376, 138 },

                /* Row 2 */
                { 24, 170, 88, 234 },
                { 120, 170, 184, 234 },
                { 216, 170, 280, 234 },
                { 312, 170, 376, 234 },

                /* Row 3 */
                { 24, 266, 88, 330 },
                { 120, 266, 184, 330 },
                { 216, 266, 280, 330 },
                { 312, 266, 376, 330 }
            };

            public static readonly int[] IN_GAME_CPU_THINKING_DIALOG_OFFSET = new int[] { 320, 400, 480, 80 };

            /*******************************************************************************************************
             *  END OF GAME OFFSETS
             *******************************************************************************************************/

            /* Size and position of the 'Game is finished' dialog */
            public static readonly int[] GAME_FINISHED_DIALOG_OFFSETS = new int[] { 200, 180, 400, 120 };

            /* Size and position of the buttons on the 'Game is finished' dialog.  These are relative to the upper
             * left corner of the dialog box and are specified as (minX, minY, maxX, maxY) */
            public static readonly int[,] GAME_FINISHED_DIALOG_BUTTON_OFFSETS = 
            {
                { 8, 64, 195, 111 },
                { 205, 64, 391, 111 }
            };

            /* Size and position of the rank numbers on the final score screen */
            public static readonly int[,] END_OF_GAME_RESULTS_RANK_OFFSETS = 
            {
                { 196, 161 },
                { 196, 202 },
                { 196, 235 },
                { 196, 268 },
                { 196, 301 },
                { 196, 334 }
            };

            /* Size and position of the player names on the final score screen */
            public static readonly int[,] END_OF_GAME_RESULTS_PLAYER_NAME_OFFSETS = 
            {
                { 269, 161 },
                { 271, 202 },
                { 271, 235 },
                { 271, 268 },
                { 271, 301 }, 
                { 271, 334 }
            };

            /* Size and position of the player scores on the final score screen */
            public static readonly int[,] END_OF_GAME_RESULTS_SCORE_OFFSETS = 
            {
                { 532, 161 },
                { 533, 202 },
                { 533, 235 },
                { 533, 268 },
                { 533, 301 },
                { 533, 334 }
            };

            /* Size and position of the 'Back to title' button on the game results screen */
            public const int END_GAME_BACK_TO_TITLE_MIN_X = 545;
            public const int END_GAME_BACK_TO_TITLE_MAX_X = 780;
            public const int END_GAME_BACK_TO_TITLE_MIN_Y = 395;
            public const int END_GAME_BACK_TO_TITLE_MAX_Y = 430;

            /*******************************************************************************************************
             *  BITMAP OFFSETS
             *******************************************************************************************************/
            /* Offsets of the 'Highlight player tracks' option text in the bitmap ('Yes'/'No') */
            public static readonly int[] OPTIONS_SCREEN_HIGHLIGHT_TRACKS_FILE_OFFSETS = new int[] { 0, 39 };

            /* Offsets of the 'Highlight legal moves' option text in the bitmap ('Yes'/'No') */
            public static readonly int[] OPTIONS_SCREEN_HIGHLIGHT_LEGAL_FILE_OFFSETS = new int[] { 0, 39 };

            public static readonly int[] OPTIONS_SCREEN_SHOW_LAST_MOVE_FILE_OFFSETS = new int[] { 0, 39, 208 };

            public static readonly int[] OPTIONS_SCREEN_AI_LEVEL_FILE_OFFSETS = new int[] { 0, 68, 176 };

            public static readonly int[] OPTIONS_SCREEN_DIGIT_OFFSETS = new int[] { 0, 22, 39, 62, 85, 107, 131, 154, 172, 195 };

            public static readonly int[] OPTION_SCREEN_DIGIT_WIDTHS = new int[] { 15, 11, 17, 16, 16, 17, 16, 13, 15, 15 };

            /* This array is tricky.  The first dimension represents the entry point, while
             * the second represents the exit point.  The value of 
             * GAME_TRACK_OVERLAY_EXITS[x][y] give the horizontal offset in the 
             * track overlay tile strip of the appropriate overlay for this entry/exit
             * combo.  A value of -1 means that the particular combination of entry and exit
             * is not valid */
            public static readonly int[,] GAME_TRACK_OVERLAY_EXITS = 
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
            public static readonly int[] GAME_RESULTS_SMALL_DIGIT_WIDTHS = new int[] { 13, 9, 14, 14, 13, 14, 13, 10, 13, 13 };
            public static readonly int[] GAME_RESULTS_LARGE_DIGIT_WIDTHS = new int[] { 18, 12, 19, 20, 18, 20, 19, 15, 18, 20 };

            /* Digit sizes and spacing */
            public const int IN_GAME_DIGIT_HEIGHT = 21;
            public const int IN_GAME_LETTER_SPACING = 1;
            public const int IN_GAME_DIGIT_OFFSET = 17;
            
            /* Size of the small digits (2nd-6th place) on the game results screen */
            public const int GAME_RESULTS_SMALL_DIGIT_SPACING = 1;
            public const int GAME_RESULTS_SMALL_DIGIT_OFFSET = 14;
            public const int GAME_RESULTS_SMALL_DIGIT_HEIGHT = 19;

            /* Size of the large digits (1st place) on the game results screen */
            public const int GAME_RESULTS_LARGE_DIGIT_SPACING = 1;
            public const int GAME_RESULTS_LARGE_DIGIT_OFFSET = 21;
            public const int GAME_RESULTS_LARGE_DIGIT_HEIGHT = 25;

            /* Size of the small player name font (2nd-6th place) on the game results screen */
            public const int GAME_RESULTS_SMALL_PLAYER_WIDTH = 100;
            public const int GAME_RESULTS_SMALL_PLAYER_HEIGHT = 20;

            /* Size of the large player name font (1st place) on the game results screen */
            public const int GAME_RESULTS_LARGE_PLAYER_WIDTH = 130;
            public const int GAME_RESULTS_LARGE_PLAYER_HEIGHT = 28;

            /*******************************************************************************************************
             *  MISCELLANEOUS
             *******************************************************************************************************/

            /* There are two different tile sizes: one for the game board, and a larger one for the 
             * 'select tile' dialog.  */
            public const int SMALL_TILE_WIDTH = 40;
            public const int LARGE_TILE_WIDTH = 64;

            /* The 'offset' (number of tiles into the tile bitmap) of the tile back */
            public const int TILE_BACK_OFFSET = 25;

            /* When displaying the highlighted tile, this is the offset to use to place the highlight correctly
             * over the tile.  This value should be subtracted from the tile offsets. */
            public const int LARGE_TILE_HIGHLIGHT_OFFSET = 8;

            /* All of the train graphics are placed in a single file.  Each train is contained in a grid square with
             * the dimensions of the following variables */
            public const int TRAIN_TILE_WIDTH = 20;
            public const int TRAIN_TILE_HEIGHT = 20;

            /* Width and height of the station tiles */
            public const int STATION_TILE_WIDTH = 40;
            public const int STATION_TILE_HEIGHT = 40;

            /* Used to reference offsets in the TI_RENDER_GAME_SELECT_ACTION_*_OFFSETS arrays */
            public const int OFFSET_DRAW = 0;
            public const int OFFSET_PLAY = 1;
            public const int OFFSET_DISCARD = 2;
            public const int OFFSET_PASS = 3;

        }

        /* Windows 7 Mobile series defaults to 30fps */
        public const int FRAME_RATE = 30;

        /* The speed to fade in the 'Holy Meatgoat' logo */
        private const int LOGO_FADE_IN_SPEED = 4;

        /* Show the 'tap to play' message 75 percent of the time */
        private const int TAP_MSG_FRAME_SHOWN_TIME = 0;
        private const int TAP_MSG_FRAME_HIDDEN_TIME = (int)(FRAME_RATE * 0.75);

        /* Train directions at the title screen */
        private const int TRAIN_DIR_LEFT = 0;
        private const int TRAIN_DIR_RIGHT = 1;
        private const int TRAIN_BANNER_SPEED = (int)(60 / FRAME_RATE);

        /* How long to show a blank (white) screen before fading in the logo */
        private const int LOGO_CLEAR_BEFORE_FADE_IN = 1000;
        /* Duration of the display of the 'Holy Meatgoat Software' logo */
        private const int LOGO_DISPLAY_DURATION = 5000;                     /* 5 seconds */

        /* The amount of time to wait between drawing the blank title background and starting each half 
         * of the 'TrackInsanity' word from moving */
        private const int TITLE_WORD_MOVE_DELAY = 1000;

        /* The amount to pause after displaying 'Insanity' before fully bringing up the title screen */
        private const int TITLE_FINAL_PAUSE_DELAY = 500;

        /* The amount of time to hold before flashing the 'Tap screen to play' message */
        private const int TAP_SCREEN_PRE_FLASH_DELAY = 1000;

        /* Used for the UI representation of the number of stations completed for each player */
        private const int NUM_STATIONS_PER_ROW = 8;
        private const int NUM_STATION_ROWS = 2;

        /* The maximum number of tiles to draw on the 'Draw Tile' dialog.  The actual number
         * to draw from may be reduced if there are less than this many tiles in the tile pool */
        private const int NUM_DRAW_TILES_TO_DISPLAY = 12;

        /* Logo screen bitmaps */
        private Texture2D logoScreenBG;
        private Texture2D titleScreenBlankBG;
        private Texture2D trackWord;
        private Texture2D insanityWord;
        
        /* Title screen bitmaps */
        private Texture2D trainBannerTop;
        private Texture2D trainBannerBottom;
        private Texture2D tapScreenMsg;
        private Texture2D titleMenu;
        private Texture2D copyrightText;

        /* New game screen bitmaps */
        private Texture2D newGameBG;
        private Texture2D numPlayersDigits;
        private Texture2D humanCpu;

        /* Option screen bitmaps */
        private Texture2D optionScreenBG;
        private Texture2D optionDigits;
        private Texture2D optionYesNo;
        private Texture2D optionShowLastMove;
        private Texture2D optionAILevel;

        /* In-game screen bitmaps */
        private Texture2D gameplayBG;
        private Texture2D playerColors;
        private Texture2D playerLightBG;
        private Texture2D playerStationsChart;
        private Texture2D tileStripSmall;
        private Texture2D tileStripLarge;
        private Texture2D tileBackLarge;
        private Texture2D stationStatus;
        private Texture2D playerStrings;
        private Texture2D playerTypeStrings;
        private Texture2D playerScoreString;
        private Texture2D currentPlayer;
        private Texture2D currentTile;

        /* In-game dialog bitmaps */
        private Texture2D selectTileDialog;
        private Texture2D selectActionDialog;
        private Texture2D selectPlayActive;
        private Texture2D selectDrawActive;
        private Texture2D selectDiscardActive;
        private Texture2D selectPassActive;
        private Texture2D chooseDiscardDialog;
        private Texture2D confirmExitDialog;
        private Texture2D cancelPlayTileButton;
        private Texture2D computerThinkingDialog;
        private Texture2D gameFinishedDialog;
        private Texture2D showingBoardDialog;
        private Texture2D showingBoardDialog2;
        private Texture2D[] playerTurnDialogs;

        /* Other in-game elements */
        private Texture2D drawTileHighlightLarge;
        private Texture2D darkenMask;
        private Texture2D validMoveMask;
        private Texture2D digits;
        private Texture2D trains;
        private Texture2D stations;
        private Texture2D trackOverlays;
        private Texture2D lastMoveHighlights;

        /* Game results screen */
        private Texture2D resultsBackground;
        private Texture2D resultsSmallDigits;
        private Texture2D resultsLargeDigits;
        private Texture2D resultsSmallPlayers;
        private Texture2D resultsLargePlayers;

        /* When the user presses the touchpad, this variable is set to false.  
         * When the touchpad is released, it's set back to true.  This is used
         * to limit the processing of input to one event per screen press. */
        private bool acceptInput = true;

        /* A SpriteBatch and Game instance tied to the Render object.
         * Each Render object will be responsible for creating the display for
         * a single Game, and will target one SpriteBatch (set to the actual
         * screen) */
        private SpriteBatch renderBatch;
        private Game gameInstance;

        /* Used to control the position of the title screen animated trains */
        private int topTrainX;
        private int topTrainDirection;
        private int bottomTrainX;
        private int bottomTrainDirection;

        /* Used to determine which tile is currently highlighted on the 'draw tile' screen */
        private int drawTileHighlighted;
        private int prevDrawTileHighlighted;

        private Dialog chooseDiscard;
        private Dialog selectAction;
        private Dialog confirmExit;
        private Dialog computerThinking;
        private Dialog selectTile;
        private Dialog gameFinished;
        private Dialog showingBoard;
        private Dialog[] playerTurn;

        /* A list of all of the game dialogs.  Only one can be active at a time. */
        private List<Dialog> dialogList;

        /* Tick counters and flags used to move and display animated graphical elements. */
        private bool startLogoFadeIn;
        private int titleAlpha;
        private int titleMenuAlpha;
        private bool startTitleDropDown;
        private float titleDropDownY;
        private float titleDropDownSpeed;
        private bool moveTrackWord;
        private bool moveInsanityWord;
        private float moveTrackWordX;
        private float moveInsanityWordX;
        private float moveTrackWordSpeed;
        private float moveInsanityWordSpeed;
        private int moveWordTicks;
        private int startTitleScreenTicks;
        private bool pauseBeforeFinalTitleScreen;
        private int titleScreenPauseFlashTicks;
        
        private int titleScreenBackgroundMovingX;
        
        /* Used to control movment of the title and options screens */
        private bool titleMovingToOptions;
        private bool optionsCancelMovingToTitle;
        private bool optionsOkMovingToTitle;
        private float titleScreenMovingX;
        private float optionScreenMovingX;
        private float titleScreenMovingSpeed;

        /* Used to control movement of the new game screen */
        private bool titleMovingToNewGame;
        private bool newGameMovingToTitle;
        private float newGameScreenMovingX;
        private bool newGameMovingToInGame;

        private bool inGameMovingToTitle;

        private bool inGameMovingToFinished;
        private bool finishedMovingToTitle;
        private float finishedMovingX;

        /* States for the Render state machine.  Used to determine what to draw
         * on the screen at any given time, depending on where in the game the
         * player currently is. */
        private enum RenderState
        {
            NO_STATE,
            COMPANY_LOGO,
            TITLE_SCREEN,
            TITLE_MENU_SCREEN,
            NEW_GAME_SCREEN,
            OPTIONS_SCREEN,
            IN_GAME,
            GAME_RESULTS_SCREEN,
            END_GAME
        };

        /* Contains offsets within the station status bitmap for each status.
         * This is the set of boxes displayed next to the player's tiles -- these
         * show how many of the player's trains have been taken off the board due
         * to completed tracks. */
        private enum StationDialogDisplayStatus
        {
            NA,
            COMPLETE,
            INCOMPLETE
        };


        /****************************************************************************
        * Render
        *
        * Description:
        *   Constructor.
        *
        *****************************************************************************/
        public Render()
        {
            drawTileHighlighted = 0;
            prevDrawTileHighlighted = 0;
            dialogList = new List<Dialog>();
            playerTurnDialogs = new Texture2D[TrackInsanity.MAX_PLAYERS];
            playerTurn = new Dialog[TrackInsanity.MAX_PLAYERS];
        }


        /****************************************************************************
        * setSpriteBatch
        *
        * Description:
        *   Sets the SpriteBatch that will be used to draw the game's sprites. 
        *
        *****************************************************************************/
        public void setSpriteBatch(SpriteBatch b)
        {
            renderBatch = b;
        }

        /****************************************************************************
        * setGameInstance
        *
        * Description:
        *   Set the particular Game instance that this render object is responsible 
        *   for drawing.
        *
        *****************************************************************************/
        public void setGameInstance(Game g)
        {
            gameInstance = g;
        }

        /****************************************************************************
        * setInitialRenderState
        *
        * Description:
        *   Starts the render engine by placing it into the intial (display company
        *   logo) state.
        *
        *****************************************************************************/
        public void setInitialRenderState()
        {
            titleAlpha = 0;
            titleDropDownY = -1 * SharedData.yRes;
            titleDropDownSpeed = 2.0f;
            startLogoFadeIn = false;
            moveTrackWord = false;
            moveInsanityWord = false;
            pauseBeforeFinalTitleScreen = false;
            moveTrackWordX = Coords.TRACK_WORD_INITIAL_X;
            moveTrackWordSpeed = 2.0f;
            moveInsanityWordX = Coords.INSANITY_WORD_INITIAL_X;
            moveInsanityWordSpeed = -2.0f;

            changeState((int)(RenderState.COMPANY_LOGO), (int)(RenderState.NO_STATE));
        }

        /****************************************************************************
        * changeState
        *
        * Description:
        *   Changes the current render state. 
        *
        *****************************************************************************/
        private void changeState(int curState, int prevState)
        {
            switch (curState)
            {
                case (int)(RenderState.COMPANY_LOGO):
                    System.Diagnostics.Debug.WriteLine("Changing render state to COMPANY_LOGO");
                    SharedData.renderState = curState;
                    break;
                case (int)(RenderState.TITLE_SCREEN):
                    System.Diagnostics.Debug.WriteLine("Changing render state to TITLE_SCREEN");
                    SharedData.renderState = curState;
                    titleScreenPauseFlashTicks = SharedData.curTicks;
                    topTrainX = Coords.TRAIN_INITIAL_X;
                    topTrainDirection = TRAIN_DIR_RIGHT;
                    bottomTrainX = Coords.TRAIN_FINAL_X;
                    bottomTrainDirection = TRAIN_DIR_LEFT;
                    titleScreenBackgroundMovingX = 0;
                    break;
                case (int)(RenderState.TITLE_MENU_SCREEN):
                    System.Diagnostics.Debug.WriteLine("Changing render state to TITLE_MENU_SCREEN");
                    SharedData.renderState = curState;
                    titleMenuAlpha = 0;
                    break;
                case (int)(RenderState.NEW_GAME_SCREEN):
                    System.Diagnostics.Debug.WriteLine("Changing render state to NEW_GAME_SCREEN");
                    SharedData.renderState = curState;
                    titleMovingToNewGame = true;
                    newGameMovingToTitle = false;
                    newGameMovingToInGame = false;
                    titleScreenMovingX = 0;                 
                    newGameScreenMovingX = SharedData.xRes;
                    titleScreenMovingSpeed = 2.0f;
                    break;
                case (int)(RenderState.OPTIONS_SCREEN):
                    System.Diagnostics.Debug.WriteLine("Changing render state to OPTIONS_SCREEN");
                    SharedData.renderState = curState;
                    titleMovingToOptions = true;
                    optionsCancelMovingToTitle = false;
                    optionsOkMovingToTitle = false;
                    titleScreenMovingX = 0;
                    optionScreenMovingX = SharedData.xRes;
                    titleScreenMovingSpeed = 2.0f;
                    break;
                case (int)(RenderState.IN_GAME):
                    System.Diagnostics.Debug.WriteLine("Changing render state to IN_GAME");
                    SharedData.renderState = curState;
                    /* Reset the visibility status of every dialog */
                    foreach (Dialog d in dialogList)
                    {
                        d.reset();
                    }
                    gameInstance.changeState((int)(Game.GameState.NEXT_PLAYER_TURN));
                    break;
                case (int)(RenderState.GAME_RESULTS_SCREEN):
                    System.Diagnostics.Debug.WriteLine("Changing render state to GAME_RESULTS_SCREEN");
                    SharedData.renderState = curState;
                    topTrainX = Coords.TRAIN_INITIAL_X;
                    topTrainDirection = TRAIN_DIR_RIGHT;
                    bottomTrainX = Coords.TRAIN_FINAL_X;
                    bottomTrainDirection = TRAIN_DIR_LEFT;
                    break;
                case (int)(RenderState.END_GAME):
                    System.Diagnostics.Debug.WriteLine("Changing render state to END_GAME");
                    SharedData.renderState = curState;
                    SharedData.exitGame = TrackInsanity.TRUE;
                    break;                  
                default:
                    System.Diagnostics.Debug.WriteLine("Unknown render state");
                    break;
            }

            SharedData.stateStartTicks = SharedData.curTicks;
        }

        /****************************************************************************
        * loadContent
        *
        * Description:
        *   Loads all image data into the game via the supplied ContentManager.
        *
        *****************************************************************************/
        public void loadContent(ContentManager c)
        {
            /* For now, just load it all and forget it! */

            /* Title screen */
            logoScreenBG = c.Load<Texture2D>("title/holygoat");
            titleScreenBlankBG = c.Load<Texture2D>("title/titleScreenBlank");
            trackWord = c.Load<Texture2D>("title/trackWord");
            insanityWord = c.Load<Texture2D>("title/insanityWord");
            trainBannerTop = c.Load<Texture2D>("title/trainbannertop");
            trainBannerBottom = c.Load<Texture2D>("title/trainbannerbottom");
            tapScreenMsg = c.Load<Texture2D>("title/tapscreen");
            titleMenu = c.Load<Texture2D>("title/titlemenu");
            copyrightText = c.Load<Texture2D>("title/copyright");

            /* New game screen */
            newGameBG = c.Load<Texture2D>("newGame/newGameMenu");
            numPlayersDigits = c.Load<Texture2D>("newGame/numPlayersHighlighted");
            humanCpu = c.Load<Texture2D>("newGame/humanCPUHighlighted");

            /* Option screen */
            optionScreenBG = c.Load<Texture2D>("options/optionScreen");
            optionDigits = c.Load<Texture2D>("options/digitsHighlighted");
            optionYesNo = c.Load<Texture2D>("options/yesNoHighlighted");
            optionShowLastMove = c.Load<Texture2D>("options/showLastMoveHighlighted");
            optionAILevel = c.Load<Texture2D>("options/aiLevelHighlighted");

            /* In game screen */
            gameplayBG = c.Load<Texture2D>("board/boardBG");
            playerColors = c.Load<Texture2D>("board/playerColorTabs");
            playerLightBG = c.Load<Texture2D>("board/activePlayerBG1");
            playerStationsChart = c.Load<Texture2D>("board/activePlayerStations");
            tileStripSmall = c.Load<Texture2D>("board/tileStrip40");
            tileStripLarge = c.Load<Texture2D>("board/tileStrip64");
            tileBackLarge = c.Load<Texture2D>("board/tileBackLarge");
            stationStatus = c.Load<Texture2D>("board/activePlayerStationStatus");
            playerStrings = c.Load<Texture2D>("board/playerNumberText");
            playerTypeStrings = c.Load<Texture2D>("board/playerHumanComputer");
            playerScoreString = c.Load<Texture2D>("board/playerScoreText");
            currentPlayer = c.Load<Texture2D>("board/currentPlayer");
            currentTile = c.Load<Texture2D>("board/currentTile");
            playerTurnDialogs[0] = c.Load<Texture2D>("board/P1Turn");
            playerTurnDialogs[1] = c.Load<Texture2D>("board/P2Turn");
            playerTurnDialogs[2] = c.Load<Texture2D>("board/P3Turn");
            playerTurnDialogs[3] = c.Load<Texture2D>("board/P4Turn");
            playerTurnDialogs[4] = c.Load<Texture2D>("board/P5Turn");
            playerTurnDialogs[5] = c.Load<Texture2D>("board/P6Turn");

            /* In-game dialogs */
            selectTileDialog = c.Load<Texture2D>("board/selectTile");
            selectActionDialog = c.Load<Texture2D>("board/selectAction");
            selectPlayActive = c.Load<Texture2D>("board/selectActionPlayTile");
            selectDrawActive = c.Load<Texture2D>("board/selectActionDrawTile");
            selectDiscardActive = c.Load<Texture2D>("board/selectActionDiscard");
            selectPassActive = c.Load<Texture2D>("board/selectActionPass");
            chooseDiscardDialog = c.Load<Texture2D>("board/chooseDiscard");
            confirmExitDialog = c.Load<Texture2D>("board/confirmExit");
            cancelPlayTileButton = c.Load<Texture2D>("board/cancelButton");
            computerThinkingDialog = c.Load<Texture2D>("board/cpuThinking");
            gameFinishedDialog = c.Load<Texture2D>("board/gameFinished");
            showingBoardDialog = c.Load<Texture2D>("board/showingBoard");
            showingBoardDialog2 = c.Load<Texture2D>("board/showingBoard2");

            /* Other in-game elements */
            drawTileHighlightLarge = c.Load<Texture2D>("board/drawTileHighlight80");
            darkenMask = c.Load<Texture2D>("board/darkenMask");
            validMoveMask = c.Load<Texture2D>("board/validMask");
            digits = c.Load<Texture2D>("board/digits");
            trains = c.Load<Texture2D>("board/trains");
            stations = c.Load<Texture2D>("board/stations");
            trackOverlays = c.Load<Texture2D>("board/trackOverlays");
            lastMoveHighlights = c.Load<Texture2D>("board/lastMoveHighlights");

            /* Game results screen */
            resultsBackground = c.Load<Texture2D>("results/resultsScreen");
            resultsSmallDigits = c.Load<Texture2D>("results/digitsSmall");
            resultsLargeDigits = c.Load<Texture2D>("results/digitsLarge");
            resultsSmallPlayers = c.Load<Texture2D>("results/playersSmall");
            resultsLargePlayers = c.Load<Texture2D>("results/playersLarge");


        }


        public void createDialogs()
        {
            /* === Create the 'choose tile to discard' dialog ============================================ */
            chooseDiscard = new Dialog(Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_OFFSET[0],
                                       Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_OFFSET[1]);
            chooseDiscard.addElement(0, 
                                     0, 
                                     Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_OFFSET[2],
                                     Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_OFFSET[3],
                                     chooseDiscardDialog, 
                                     "main",
                                     true);

            /* Add the button regions */
            chooseDiscard.addVirtualElement("ok", true);
            chooseDiscard.addButtonExtents("ok",
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[0, 0],
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[0, 1],
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[0, 2],
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[0, 3]);

            chooseDiscard.addVirtualElement("cancel", true);
            chooseDiscard.addButtonExtents("cancel",
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[1, 0],
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[1, 1],
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[1, 2],
                                           Coords.IN_GAME_CHOOSE_DISCARD_DIALOG_BUTTON_OFFSETS[1, 3]);
            dialogList.Add(chooseDiscard);
            /* =========================================================================================== */

            /* === Create the 'Select Action' dialog ===================================================== */
            selectAction = new Dialog(Coords.IN_GAME_SELECT_ACTION_DIALOG_OFFSET[0],
                                      Coords.IN_GAME_SELECT_ACTION_DIALOG_OFFSET[1]);
            selectAction.addElement(0,
                                    0,
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_OFFSET[2],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_OFFSET[3], 
                                    selectActionDialog, 
                                    "selectAction", 
                                    true);

            /* Add the 'Draw' button element */
            selectAction.addElement(Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[0, 0],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[0, 1],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[0, 2],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[0, 3],
                                    selectDrawActive, 
                                    "selectDrawActive", 
                                    true);
            selectAction.addButtonExtents("selectDrawActive",
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[0, 0],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[0, 1],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[0, 2],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[0, 3]);

            /* Add the 'Play' button element */
            selectAction.addElement(Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[1, 0],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[1, 1],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[1, 2],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[1, 3],
                                    selectPlayActive, 
                                    "selectPlayActive", 
                                    true);
            selectAction.addButtonExtents("selectPlayActive",
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[1, 0],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[1, 1],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[1, 2],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[1, 3]);

            /* Add the 'Discard' button element */
            selectAction.addElement(Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[2, 0],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[2, 1],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[2, 2],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[2, 3],
                                    selectDiscardActive, 
                                    "selectDiscardActive", 
                                    true);
            selectAction.addButtonExtents("selectDiscardActive", 
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[2, 0],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[2, 1],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[2, 2],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[2, 3]);

            /* Add the 'Pass' button element */
            selectAction.addElement(Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[3, 0],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[3, 1],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[3, 2],
                                    Coords.IN_GAME_SELECT_ACTION_DIALOG_TEXT_OFFSETS[3, 3],
                                    selectPassActive, 
                                    "selectPassActive", 
                                    true);
            selectAction.addButtonExtents("selectPassActive", 
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[3, 0],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[3, 1],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[3, 2],
                                          Coords.IN_GAME_SELECT_ACTION_DIALOG_BUTTON_OFFSETS[3, 3]);
            dialogList.Add(selectAction);
            /* =========================================================================================== */

            /* === Create the 'Confirm Exit' dialog ==-=================================================== */
            confirmExit = new Dialog(Coords.IN_GAME_CONFIRM_EXIT_DIALOG_OFFSET[0],
                                     Coords.IN_GAME_CONFIRM_EXIT_DIALOG_OFFSET[1]);
            confirmExit.addElement(0,
                                   0,
                                   Coords.IN_GAME_CONFIRM_EXIT_DIALOG_OFFSET[2],
                                   Coords.IN_GAME_CONFIRM_EXIT_DIALOG_OFFSET[3],
                                   confirmExitDialog,
                                   "main",
                                   true);
            
            /* Add the 'OK' and no buttons */
            confirmExit.addVirtualElement("yes", true);
            confirmExit.addButtonExtents("yes",
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[0, 0],
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[0, 1],
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[0, 2],
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[0, 3]);
            confirmExit.addVirtualElement("no", true);
            confirmExit.addButtonExtents("no",
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[1, 0],
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[1, 1],
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[1, 2],
                                         Coords.IN_GAME_CONFIRM_EXIT_DIALOG_BUTTON_OFFSETS[1, 3]);
            dialogList.Add(confirmExit);
            /* =========================================================================================== */

            /* === Create the 'Computer is thinking' dialog ============================================== */
            computerThinking = new Dialog(Coords.IN_GAME_CPU_THINKING_DIALOG_OFFSET[0],
                                          Coords.IN_GAME_CPU_THINKING_DIALOG_OFFSET[1]);
            computerThinking.addElement(0,
                                        0,
                                        Coords.IN_GAME_CPU_THINKING_DIALOG_OFFSET[2],
                                        Coords.IN_GAME_CPU_THINKING_DIALOG_OFFSET[3],
                                        computerThinkingDialog,
                                        "main",
                                        true);
            dialogList.Add(computerThinking);
            /* =========================================================================================== */

            /* === Create the 'Draw tile' dialog ========================================================= */
            /* Add the main window */
            selectTile = new Dialog(Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[0],
                                    Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[1]);
            selectTile.addElement(0,
                                  0,
                                  Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[2],
                                  Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[3],
                                  selectTileDialog,
                                  "main",
                                  true);

            /* Add the 'OK' button */
            selectTile.addVirtualElement("ok", true);
            selectTile.addButtonExtents("ok",
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[0, 0],
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[0, 1],
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[0, 2],
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[0, 3]);

            /* Add the 'Cancel' button */
            selectTile.addVirtualElement("cancel", true);
            selectTile.addButtonExtents("cancel",
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[1, 0],
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[1, 1],
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[1, 2],
                                        Coords.IN_GAME_DRAW_TILE_DIALOG_BUTTON_OFFSETS[1, 3]);

            /* Add the tile graphics and buttons */
            for(int counter=0;counter<NUM_DRAW_TILES_TO_DISPLAY; counter++)
            {
                selectTile.addElement(Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[counter, 0],
                                      Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[counter, 1],
                                      Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[counter, 2],
                                      Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[counter, 3],
                                      tileBackLarge,
                                      string.Concat("tile", counter),
                                      true);
                selectTile.addButtonExtents(string.Concat("tile", counter),
                                            Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_CLICKABLE_AREA_OFFSETS[counter, 0],
                                            Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_CLICKABLE_AREA_OFFSETS[counter, 1],
                                            Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_CLICKABLE_AREA_OFFSETS[counter, 2],
                                            Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_CLICKABLE_AREA_OFFSETS[counter, 3]);
            }
            dialogList.Add(selectTile);
            /* ============================================================================================ */

            /* === Create the 'Player n's turn' dialogs =================================================== */
            /* They start at 0,0 so we can create a virtual element that covers the entire screen, but
             * the actual contents of the dialog start in the lower corner */
            for (int counter = 0; counter < TrackInsanity.MAX_PLAYERS; counter++)
            {
                playerTurn[counter] = new Dialog(0,
                                                 0);
                playerTurn[counter].addElement(Coords.IN_GAME_PLAYER_TURN_DIALOG_OFFSET[0],
                                               Coords.IN_GAME_PLAYER_TURN_DIALOG_OFFSET[1],                                              
                                               Coords.IN_GAME_PLAYER_TURN_DIALOG_OFFSET[2],
                                               Coords.IN_GAME_PLAYER_TURN_DIALOG_OFFSET[3],
                                               playerTurnDialogs[counter],
                                               string.Concat("P", counter, "Turn"),
                                               true);
                playerTurn[counter].addButtonExtents(string.Concat("P", counter, "Turn"),
                                                     0,
                                                     0,
                                                     SharedData.xRes,
                                                     SharedData.yRes);
                dialogList.Add(playerTurn[counter]);
            }
            /* ============================================================================================ */

            /* === Create the 'Game finished' dialog ====================================================== */
            gameFinished = new Dialog(Coords.GAME_FINISHED_DIALOG_OFFSETS[0],
                                      Coords.GAME_FINISHED_DIALOG_OFFSETS[1]);

            gameFinished.addElement(0,
                                    0,
                                    Coords.GAME_FINISHED_DIALOG_OFFSETS[2],
                                    Coords.GAME_FINISHED_DIALOG_OFFSETS[3],
                                    gameFinishedDialog,
                                    "main",
                                    true);
            gameFinished.addVirtualElement("showBoard", true);
            gameFinished.addButtonExtents("showBoard",
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[0, 0],
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[0, 1],
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[0, 2],
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[0, 3]);
            gameFinished.addVirtualElement("showScores", true);
            gameFinished.addButtonExtents("showScores",
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[1, 0],
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[1, 1],
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[1, 2],
                                          Coords.GAME_FINISHED_DIALOG_BUTTON_OFFSETS[1, 3]);
            dialogList.Add(gameFinished);
            /* ============================================================================================ */

            /* === Create the 'Showing board' dialog ====================================================== */
            showingBoard = new Dialog(0,
                                      0);
            showingBoard.addElement(Coords.IN_GAME_SHOWING_BOARD_OFFSET[0, 0],
                                    Coords.IN_GAME_SHOWING_BOARD_OFFSET[0, 1],
                                    Coords.IN_GAME_SHOWING_BOARD_OFFSET[0, 2],
                                    Coords.IN_GAME_SHOWING_BOARD_OFFSET[0, 3],
                                    showingBoardDialog,
                                    "showingBoard",
                                    true);
            showingBoard.addElement(Coords.IN_GAME_SHOWING_BOARD_OFFSET[1, 0],
                                    Coords.IN_GAME_SHOWING_BOARD_OFFSET[1, 1],
                                    Coords.IN_GAME_SHOWING_BOARD_OFFSET[1, 2],
                                    Coords.IN_GAME_SHOWING_BOARD_OFFSET[1, 3],
                                    showingBoardDialog2,
                                    "clickToContinue",
                                    true);
            showingBoard.addVirtualElement("ok", true);
            showingBoard.addButtonExtents("ok",
                                          0,
                                          0,
                                          SharedData.xRes,
                                          SharedData.yRes);
            dialogList.Add(showingBoard);
            /* ============================================================================================ */
        }

        /****************************************************************************
        * checkTimingConditions
        *
        * Description:
        *   Checks for any timed actions during the current render state, and
        *   responds accordingly.  Currently, this is only used to display the
        *   Holy Meatgoat logo for 5 seconds before switching to the title screen.
        *
        *****************************************************************************/
        public void checkTimingConditions()
        {
            switch (SharedData.renderState)
            {
                case (int)(RenderState.COMPANY_LOGO):
                    if (SharedData.curTicks - SharedData.stateStartTicks >= LOGO_CLEAR_BEFORE_FADE_IN)
                    {
                        startLogoFadeIn = true;
                    }
                    if (SharedData.curTicks - SharedData.stateStartTicks >= LOGO_CLEAR_BEFORE_FADE_IN + LOGO_DISPLAY_DURATION)
                    {
                        startTitleDropDown = true;
                    }
                    if (moveTrackWord == true && SharedData.curTicks - moveWordTicks >= TITLE_WORD_MOVE_DELAY)
                    {
                        if (moveInsanityWord == false)
                        {
                            System.Diagnostics.Debug.WriteLine("Junp on in!");
                            moveTrackWord = false;
                            moveInsanityWord = true;
                            moveWordTicks = SharedData.curTicks;
                        }
                    }
                    if (moveInsanityWord == true && SharedData.curTicks - moveWordTicks >= TITLE_WORD_MOVE_DELAY)
                    {
                        if (pauseBeforeFinalTitleScreen == false)
                        {
                            pauseBeforeFinalTitleScreen = true;
                            startTitleScreenTicks = SharedData.curTicks;
                        }
                    }
                    if (pauseBeforeFinalTitleScreen == true && SharedData.curTicks - startTitleScreenTicks >= TITLE_FINAL_PAUSE_DELAY)
                    {
                        changeState((int)(RenderState.TITLE_SCREEN), SharedData.renderState);
                    }
                    break;
                case (int)(RenderState.IN_GAME):
                    switch(gameInstance.gameState)
                    {
                        case (int)(Game.GameState.COMPUTER_MOVE):
                            /* Until the CPU delay tick counter is reduced to zero, block the computer from moving */
                            if (SharedData.curTicks >= SharedData.cpuDelayTicks)
                            {
                                System.Diagnostics.Debug.WriteLine("Allowing move -- curTicks = {0}, delayTicks = {1}", SharedData.curTicks, SharedData.cpuDelayTicks);
                                SharedData.cpuMoveAllowed = true;
                            }
                            break;
                    }
                    break;
            }
        }

        /****************************************************************************
        * applySurface
        *
        * Description:
        *   Draws an entire Texture2D to the current render surface
        *
        ****************************************************************************/
        private void applySurface(int x, int y, Texture2D texture)
        {        
            Rectangle r = new Rectangle(x, y, texture.Width, texture.Height);
            renderBatch.Draw(texture, r, Color.White);
        }

        /****************************************************************************
        * drawAnimatedTrains
        *
        * Description:
        *   Draws the two train banners at their current positions.  
        *
        *****************************************************************************/
        private void drawAnimatedTrains()
        {
            blitSurface(0, 0, topTrainX, 9, Coords.TRAIN_BANNER_WIDTH, 22, trainBannerTop);
            blitSurface(0, 0, bottomTrainX, SharedData.yRes - 31, Coords.TRAIN_BANNER_WIDTH, 22, trainBannerBottom);
        }

        /****************************************************************************
        * renderTitleScreenDrawBase
        *
        * Description:
        *   Draws the components common to the title, new game and options screens.
        *
        *****************************************************************************/
        private void renderTitleScreenDrawBase()
        {
            if(titleScreenBackgroundMovingX < -SharedData.xRes)
            {
                titleScreenBackgroundMovingX = SharedData.xRes - (-SharedData.xRes - titleScreenBackgroundMovingX);
            }
            if(titleScreenBackgroundMovingX > SharedData.xRes)
            {
                titleScreenBackgroundMovingX = -SharedData.xRes + (titleScreenBackgroundMovingX - SharedData.xRes);
            }

            if (titleScreenBackgroundMovingX == 0)
            {
                blitSurface(0,
                            0,
                            0,
                            0,
                            titleScreenBlankBG.Width,
                            titleScreenBlankBG.Height,
                            titleScreenBlankBG);
            }
            else if (titleScreenBackgroundMovingX > 0)
            {
                blitSurface(0,
                            0,
                            titleScreenBackgroundMovingX,
                            0,
                            titleScreenBlankBG.Width,
                            titleScreenBlankBG.Height,
                            titleScreenBlankBG);
                blitSurface(0,
                            0,
                            titleScreenBackgroundMovingX - titleScreenBlankBG.Width,
                            0,
                            titleScreenBlankBG.Width,
                            titleScreenBlankBG.Height,
                            titleScreenBlankBG);
            }
            else
            {
                blitSurface(0,
                            0,
                            titleScreenBackgroundMovingX,
                            0,
                            titleScreenBlankBG.Width,
                            titleScreenBlankBG.Height,
                            titleScreenBlankBG);
                blitSurface(0,
                            0,
                            titleScreenBackgroundMovingX + titleScreenBlankBG.Width,
                            0,
                            titleScreenBlankBG.Width,
                            titleScreenBlankBG.Height,
                            titleScreenBlankBG);
            }
            drawAnimatedTrains();
            return;
        }

        /****************************************************************************
        * blitSurface
        *
        * Description:
        *   Draws a subset of a Texture2D to the specified location within the
        *   destination render surface
        *
        * Arguments:
        *   int srcX - the x position of the source bitmap to draw from
        *   int srcY - the y position of the source bitmap to draw from
        *   int destX - the x position of the destination SDL_Surface to draw in
        *   int destY - the y position of the destination SDL_Surface to draw in
        *   int width - the width of the area to blit
        *   int height - the height of the area to blit
        *   Texture2D texture - the texture to draw
        *
        ****************************************************************************/
        public void blitSurface(int srcX, int srcY, int destX, int destY, int width, int height, Texture2D texture)
        {
            Rectangle srcRect = new Rectangle(srcX, srcY, width, height);
            Rectangle destRect = new Rectangle(destX, destY, width, height);
            renderBatch.Draw(texture, destRect, srcRect, Color.White);
        }

        /****************************************************************************
        * blitAlphaSurface
        *
        * Description:
        *   Draws a subset of a Texture2D to the specified location within the
        *   destination render surface, using the specified transparency level
        *
        * Arguments:
        *   int srcX - the x position of the source bitmap to draw from
        *   int srcY - the y position of the source bitmap to draw from
        *   int destX - the x position of the destination SDL_Surface to draw in
        *   int destY - the y position of the destination SDL_Surface to draw in
        *   int width - the width of the area to blit
        *   int height - the height of the area to blit
        *   int trans - the transparency value to use (0-100, 100 is fully opaque)
        *   Texture2D texture - the texture to draw
        *
        ****************************************************************************/
        public void blitAlphaSurface(int srcX, int srcY, int destX, int destY, int width, int height, int trans, Texture2D texture)
        {
            int red = (int)(255.0f * (float)((float)trans / 100.0f));
            int green = (int)(255.0f * (float)((float)trans / 100.0f));
            int blue = (int)(255.0f * (float)((float)trans / 100.0f));
            int alpha = (int)(255.0f * (float)((float)trans / 100.0f));

            Rectangle srcRect = new Rectangle(srcX, srcY, width, height);
            Rectangle destRect = new Rectangle(destX, destY, width, height);
            renderBatch.Draw(texture, destRect, srcRect, new Color(red, green, blue, alpha));
        }

        /****************************************************************************
        * drawNewGameScreenExtras
        *
        * Description:
        *   Draws the components specific to the 'New Game' screen.
        *
        ****************************************************************************/
        private void drawNewGameScreenExtras(int offset)
        {
            int counter;

            /* Mark the number of players on the screen */
            counter = SharedData.selectedPlayers - 2;

            blitSurface(0,
                        0,
                        offset,
                        Coords.TRACK_BAR_HEIGHT,
                        newGameBG.Width,
                        Coords.NEW_GAME_PLAYER_NAME_OFFSETS[counter] - Coords.TRACK_BAR_HEIGHT,
                        newGameBG);

            blitSurface(0,
                        newGameBG.Height - 40,
                        offset,
                        newGameBG.Height,
                        newGameBG.Width,
                        Coords.NEW_GAME_FONT_HEIGHT,
                        newGameBG);

            blitSurface(counter * Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 2], 
                        0, 
                        Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 0] + offset, 
                        Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 1], 
                        Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 2], 
                        Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 3], 
                        numPlayersDigits);

            /* For each player, mark human or CPU */
            for (counter = 0; counter < SharedData.selectedPlayers; counter++)
            {
                if (SharedData.playerState[counter] == Player.HUMAN)
                {
                    blitSurface(0, 
                                0, 
                                Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 0] + offset, 
                                Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 1], 
                                Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 2], 
                                Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 3], 
                                humanCpu);
                }
                else
                {
                    blitSurface(Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 2], 
                                0, 
                                Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 0]  + offset,
                                Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 1], 
                                Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 2], 
                                Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 3], 
                                humanCpu);
                }
            }
        }

        /****************************************************************************
        * drawOptionsScreenExtras
        *
        * Description:
        *   Draws the components specific to the Game Options screen.
        *
        ****************************************************************************/
        private void drawOptionsScreenExtras(int offset)
        {
            /* Draw the menu background */
            blitSurface(0,
                        0,
                        offset,
                        Coords.TRACK_BAR_HEIGHT,
                        optionScreenBG.Width,
                        optionScreenBG.Height,
                        optionScreenBG);

            /* Highlight the correct setting for the 'highlight tracks option' */
            if (gameInstance.tmpHighlightTracks == Game.OPTION_YES)
            {
                blitSurface(Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_FILE_OFFSETS[1], 
                            0,
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 3], 
                            optionYesNo);
            }
            else
            {
                blitSurface(Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_FILE_OFFSETS[0], 
                            0, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 3], 
                            optionYesNo);
            }

            /* Highlight the correct setting for the 'highlight legal moves' option */
            if (gameInstance.tmpHighlightLegalMoves == Game.OPTION_YES)
            {
                blitSurface(Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_FILE_OFFSETS[1], 
                            0, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 3], 
                            optionYesNo);
            }
            else
            {
                blitSurface(Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_FILE_OFFSETS[0],
                            0, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 3], 
                            optionYesNo);
            }

            /* Highlight the correct setting for the 'show move' option */
            if (gameInstance.tmpShowLastMove == Game.OPTION_NO)
            {
                blitSurface(Coords.OPTIONS_SCREEN_SHOW_LAST_MOVE_FILE_OFFSETS[0], 
                            0, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 3], 
                            optionShowLastMove);
            }
            else if (gameInstance.tmpShowLastMove == Game.OPTION_LAST_PLAYER)
            {
                blitSurface(Coords.OPTIONS_SCREEN_SHOW_LAST_MOVE_FILE_OFFSETS[1], 
                            0,
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 3], 
                            optionShowLastMove);
            }
            else
            {
                blitSurface(Coords.OPTIONS_SCREEN_SHOW_LAST_MOVE_FILE_OFFSETS[2], 
                            0,
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 0] + offset, 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 1], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 2], 
                            Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 3],
                            optionShowLastMove);
            }

            /* Highlight the correct setting for the 'AI level' option */
            if (gameInstance.tmpDefaultAILevel == Game.OPTION_AI_EASY)
            {
                blitSurface(Coords.OPTIONS_SCREEN_AI_LEVEL_FILE_OFFSETS[0], 
                            0, 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 0] + offset, 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 1], 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 2], 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 3], 
                            optionAILevel);
            }
            else if (gameInstance.tmpDefaultAILevel == Game.OPTION_AI_MEDIUM)
            {
                blitSurface(Coords.OPTIONS_SCREEN_AI_LEVEL_FILE_OFFSETS[1], 
                            0, 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 0] + offset, 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 1], 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 2], 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 3], 
                            optionAILevel);
            }
            else
            {
                blitSurface(Coords.OPTIONS_SCREEN_AI_LEVEL_FILE_OFFSETS[2], 
                            0, 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 0] + offset,
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 1], 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 2], 
                            Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 3], 
                            optionAILevel);
            }

            /* Display the highlight for the music and sound effect volume options */
            blitSurface(Coords.OPTIONS_SCREEN_DIGIT_OFFSETS[gameInstance.tmpMusicVolume], 
                        0, 
                        Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[gameInstance.tmpMusicVolume, 0] + offset, 
                        Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[gameInstance.tmpMusicVolume, 1], 
                        Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[gameInstance.tmpMusicVolume, 2], 
                        Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[gameInstance.tmpMusicVolume, 3], 
                        optionDigits);
            blitSurface(Coords.OPTIONS_SCREEN_DIGIT_OFFSETS[gameInstance.tmpEffectsVolume], 
                        0,
                        Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[gameInstance.tmpEffectsVolume, 0] + offset, 
                        Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[gameInstance.tmpEffectsVolume, 1], 
                        Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[gameInstance.tmpEffectsVolume, 2], 
                        Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[gameInstance.tmpEffectsVolume, 3], 
                        optionDigits);
        }

        /****************************************************************************
        * drawInGameScreen
        *
        * Description:
        *   Draws the components seen by the player while a game is in progress.
        *
        ****************************************************************************/
        private void drawInGameScreen()
        {
            int counter, counter2, counter3, offset, stationOffset, renderOffset;

            /* Draw the background */
            applySurface(0, 0, gameplayBG);

            for (counter = 0; counter < TrackInsanity.MAX_PLAYERS; counter++)
            {
                if (gameInstance.players[counter].controlledBy != Player.NOBODY)
                {
                    /* Render player color swatches for each active player */
                    blitSurface(counter * Coords.IN_GAME_PLAYER_COLOR_REGION_OFFSETS[counter, 2], 
                                0, 
                                Coords.IN_GAME_PLAYER_COLOR_REGION_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_COLOR_REGION_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_COLOR_REGION_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_COLOR_REGION_OFFSETS[counter, 3], 
                                playerColors);

                    /* Render lighter background for each player */
                    blitSurface(0, 
                                0, 
                                Coords.IN_GAME_PLAYER_BG_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_BG_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_BG_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_BG_OFFSETS[counter, 3], 
                                playerLightBG);

                    /* Render station chart for each active player */
                    blitSurface(0, 
                                0, 
                                Coords.IN_GAME_PLAYER_STATIONS_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_STATIONS_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_STATIONS_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_STATIONS_OFFSETS[counter, 3], 
                                playerStationsChart);

                    /* Render player name for each player */
                    blitSurface(0,
                                counter * Coords.IN_GAME_PLAYER_NAME_OFFSETS[counter, 3], 
                                Coords.IN_GAME_PLAYER_NAME_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_NAME_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_NAME_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_NAME_OFFSETS[counter, 3], 
                                playerStrings);

                    /* Offset to 'human' or 'computer' depending on the player type */
                    offset = (gameInstance.players[counter].controlledBy == Player.HUMAN) ? 0 : 1;
                    blitSurface(0,
                                offset * Coords.IN_GAME_PLAYER_TYPE_OFFSETS[counter, 3], 
                                Coords.IN_GAME_PLAYER_TYPE_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_TYPE_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_TYPE_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_TYPE_OFFSETS[counter, 3], 
                                playerTypeStrings);

                    /* Render the score string for each player */
                    blitSurface(0,
                                0, 
                                Coords.IN_GAME_PLAYER_SCORE_STR_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_SCORE_STR_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_SCORE_STR_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_SCORE_STR_OFFSETS[counter, 3], 
                                playerScoreString);
                }
            }

            /* Mark the current and reserve tiles for each player */
            for (counter = 0; counter < TrackInsanity.MAX_PLAYERS; counter++)
            {
                if (gameInstance.players[counter].controlledBy != Player.NOBODY)
                {
                    /* If processing the current player, and that player is human, show the actual tiles.
                     * Otherwise, show a generic 'holding a tile' graphics for held tiles, and the tile
                     * back if a tile isn't held */
                    if (counter == gameInstance.curPlayer && gameInstance.players[counter].controlledBy == Player.HUMAN && gameInstance.gameState != (int)Game.GameState.NEXT_PLAYER_TURN)
                    {
                        offset = (gameInstance.players[counter].currentTileId == Tile.NONE) ? 0 : gameInstance.tilepool.getTileIndexForTileId(gameInstance.players[counter].currentTileId);
                    }
                    else
                    {
                        offset = (gameInstance.players[counter].currentTileId == Tile.NONE) ? 0 : Coords.TILE_BACK_OFFSET;
                    }
                    blitSurface(offset * Coords.IN_GAME_PLAYER_TILE1_OFFSETS[counter, 2], 
                                0,
                                Coords.IN_GAME_PLAYER_TILE1_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_TILE1_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_TILE1_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_TILE1_OFFSETS[counter, 3], 
                                tileStripSmall);

                    /* If processing the current player, and that player is human, show the actual tiles.
                     * Otherwise, show a generic 'holding a tile' graphics for held tiles, and the tile
                     * back if a tile isn't held */
                    if (counter == gameInstance.curPlayer && gameInstance.players[counter].controlledBy == Player.HUMAN)
                    {
                        offset = (gameInstance.players[counter].reserveTileId == Tile.NONE) ? 0 : gameInstance.tilepool.getTileIndexForTileId(gameInstance.players[counter].reserveTileId);
                    }
                    else
                    {
                        offset = (gameInstance.players[counter].reserveTileId == Tile.NONE) ? 0 : Coords.TILE_BACK_OFFSET;
                    }
                    blitSurface(offset * Coords.IN_GAME_PLAYER_TILE2_OFFSETS[counter, 2], 
                                0,
                                Coords.IN_GAME_PLAYER_TILE2_OFFSETS[counter, 0], 
                                Coords.IN_GAME_PLAYER_TILE2_OFFSETS[counter, 1], 
                                Coords.IN_GAME_PLAYER_TILE2_OFFSETS[counter, 2], 
                                Coords.IN_GAME_PLAYER_TILE2_OFFSETS[counter, 3], 
                                tileStripSmall);
                }
            }

            /* If a request to refresh the player station status is made, do it.  This is always
            displayed for all players regardless of the player type.  This request will
            refresh station status for all players, even if it isn't needed, which is
            inefficient.  Plus, the code is ugly (could I nest loops any deeper?).  Meh.
            */
            for (counter = 0; counter < TrackInsanity.MAX_PLAYERS; counter++)
            {
                if (gameInstance.players[counter].controlledBy != Player.NOBODY)
                {
                    /* The stations are divided over multiple rows.  In the current game version,
                    the 16 maximum possible player stations (in a 2 player game) are
                    displayed in 2 rows of 8 each.  The nested loops are used to make sure
                    the status block is drawn in the correct place
                    */
                    for (counter2 = 0; counter2 < NUM_STATION_ROWS; counter2++)
                    {
                        for (counter3 = 0; counter3 < NUM_STATIONS_PER_ROW; counter3++)
                        {
                            stationOffset = ((counter2 * NUM_STATIONS_PER_ROW) + counter3);
                            if (stationOffset < gameInstance.numStationsPerPlayer)
                            {
                                /* Incomplete stations */
                                if (stationOffset >= gameInstance.players[counter].numStationsComplete)
                                {
                                    renderOffset = (int)(StationDialogDisplayStatus.INCOMPLETE) * Coords.IN_GAME_STATION_STATUS_WIDTH;
                                }
                                /* Complete stations */
                                else
                                {
                                    renderOffset = (int)(StationDialogDisplayStatus.COMPLETE) * Coords.IN_GAME_STATION_STATUS_WIDTH;
                                }
                            }
                            else
                            {
                                /* Stations not used */
                                renderOffset = (int)(StationDialogDisplayStatus.NA) * Coords.IN_GAME_STATION_STATUS_WIDTH;
                            }
                            blitSurface(renderOffset, 
                                        0, 
                                        (Coords.IN_GAME_PLAYER_STATIONS_OFFSETS[counter, 0]) + (counter3 * (Coords.IN_GAME_STATION_STATUS_WIDTH + 1)) + Coords.IN_GAME_STATION_INITIAL_XOFF,
                                        (Coords.IN_GAME_PLAYER_STATIONS_OFFSETS[counter, 1]) + (counter2 * (Coords.IN_GAME_STATION_STATUS_HEIGHT + 1)) + Coords.IN_GAME_STATION_INITIAL_YOFF,
                                        Coords.IN_GAME_STATION_STATUS_WIDTH,
                                        Coords.IN_GAME_STATION_STATUS_HEIGHT, 
                                        stationStatus);
                        }
                    }
                }
            }

            /* Draw each players' current score */
            for (counter = 0; counter < TrackInsanity.MAX_PLAYERS; counter++)
            {
                if (gameInstance.players[counter].controlledBy != Player.NOBODY)
                {
                    drawInGamePlayerScore(counter);
                }
            }
      
            /* Draw all tiles on the board */            
            for (counter = 0; counter < Board.WIDTH; counter++)
            {
                for (counter2 = 0; counter2 < Board.HEIGHT; counter2++)
                {
                    if (gameInstance.board.b[counter, counter2].type == BoardSquare.PLAYED_TILE)
                    {
                        drawTile(counter, counter2, gameInstance.board.b[counter, counter2].tileIndex);
                    }
                }
            }

            /* Draw the remainder of the in-game components */
            drawTrainsAtStations();
            drawLastMoves();
            drawAllTrackOverlays();
            drawCurrentPlayerHighlight();
            if (gameInstance.gameState != (int)Game.GameState.NEXT_PLAYER_TURN)
            {
                drawActiveTileHighlight();
            }
        }

        /****************************************************************************
        * drawLastMoves
        *
        * Description:
        *   Draws a highlight around the last moves made by each player, if 
        *   the current game settings request it.  
        *
        ****************************************************************************/
        private void drawLastMoves()
        {
            int counter;

            for(counter=0;counter<gameInstance.numPlayers;counter++)
            {
                if(gameInstance.showLastMove == Game.OPTION_ALL_PLAYERS || (gameInstance.showLastMove == Game.OPTION_LAST_PLAYER && counter == gameInstance.curPlayer))
                {
                    if (gameInstance.players[counter].lastMoveX >= 1 && gameInstance.players[counter].lastMoveY >= 1)
                    {
                        blitSurface(counter * Coords.SMALL_TILE_WIDTH, 
                                    0,
                                    Coords.IN_GAME_BOARD_TILE_AREA_X + ((gameInstance.players[counter].lastMoveX - 1) * Coords.SMALL_TILE_WIDTH), 
                                    Coords.IN_GAME_BOARD_TILE_AREA_Y + ((gameInstance.players[counter].lastMoveY - 1) * Coords.SMALL_TILE_WIDTH), 
                                    Coords.SMALL_TILE_WIDTH, 
                                    Coords.SMALL_TILE_WIDTH,
                                    lastMoveHighlights);
                    }
                }
            }
        }

        /****************************************************************************
        * drawTile
        *
        * Description:
        *   Draws the specified tile graphics to the specified portion of the board. 
        *
        ****************************************************************************/
        private void drawTile(int x, int y, int tile)
        {
            if (x < 1 || y < 1)
            {
                return;
            }

            blitSurface(gameInstance.tilepool.getTileIndexForTileId(tile) * Coords.SMALL_TILE_WIDTH, 
                        0, 
                        (Coords.IN_GAME_BOARD_TILE_AREA_X + ((x - 1) * Coords.SMALL_TILE_WIDTH)), 
                        (Coords.IN_GAME_BOARD_TILE_AREA_Y + ((y - 1) * Coords.SMALL_TILE_WIDTH)),
                        Coords.SMALL_TILE_WIDTH, 
                        Coords.SMALL_TILE_WIDTH, 
                        tileStripSmall);
        }

        /****************************************************************************
        * drawTrainsAtStations
        *
        * Description:
        *   Draws the station graphics around the board, and the trains that are
        *   stationed at each one.
        *
        ****************************************************************************/
        private void drawTrainsAtStations()
        {
            int counter;
            int stationX = -1, stationY = -1, exit = -1;
            int player;

            for (counter = 0; counter < Board.NUM_STATIONS; counter++)
            {
                /* If a train is present, draw the train. */
                gameInstance.board.getStationInfo(counter, ref stationX, ref stationY, ref exit);
                if (gameInstance.board.b[stationX, stationY].trainPresent != (int)(Board.playerTrains.NO_TRAIN))
                {
                    player = gameInstance.board.b[stationX, stationY].trainPresent - 1;
                    blitSurface(Coords.IN_GAME_STATION_TRAIN_OFFSETS[counter, 4] * Coords.TRAIN_TILE_WIDTH, 
                                player * Coords.TRAIN_TILE_HEIGHT, 
                                Coords.IN_GAME_STATION_TRAIN_OFFSETS[counter, 0], 
                                Coords.IN_GAME_STATION_TRAIN_OFFSETS[counter, 1], 
                                Coords.IN_GAME_STATION_TRAIN_OFFSETS[counter, 2], 
                                Coords.IN_GAME_STATION_TRAIN_OFFSETS[counter, 3], 
                                trains);
                }
            }
        }

        /****************************************************************************
        * drawTrackOverlay
        *
        * Description:
        *   Highlights the 'active' (scoring) part of the track leading out from
        *   the specified station using the player's color.  
        *
        ****************************************************************************/
        private void drawTrackOverlay(int station)
        {
            int stationX = -1, stationY = -1, stationExit = -1;
            int newX = -1, newY = -1, newExit = -1;
            int newType;
            int oldX, oldY, oldExit;
            int loopCatcher, loopLimit;
            int player;

            /* Don't draw the overlay if the user doesn't want it */
            if(gameInstance.highlightTracks == Game.OPTION_NO)
            {
                return;
            }

            /* If there's no train at the station, don't draw an overlay */
            player = gameInstance.board.playerStations[gameInstance.numPlayers, station];
            if(player == (int)(Board.playerTrains.NO_TRAIN))
            {
                return;
            }

            /* Subtract 1 from the player number to get the correct vertical offset into the track overlay file */
            player = player - 1;

            /* Determine the starting point */
            gameInstance.board.getStationInfo(station, ref stationX, ref stationY, ref stationExit);
            if(gameInstance.board.b[stationX, stationY].type != BoardSquare.STATION)
            {
                System.Diagnostics.Debug.WriteLine("warning: starting point isn't station");
                return;
            }

            newType = gameInstance.board.findNextTrackSection(stationX, stationY, stationExit, ref newX, ref newY, ref newExit);

            loopCatcher = 0;
            loopLimit = 255;

            while((newType == BoardSquare.PLAYED_TILE) && (loopCatcher < loopLimit))
            {
                loopCatcher++;
                oldX = newX;
                oldY = newY;

                oldExit = gameInstance.board.tp.getTile(gameInstance.board.b[newX, newY].tileIndex).findExit(newExit);
                blitSurface(Coords.GAME_TRACK_OVERLAY_EXITS[oldExit, newExit] * Coords.SMALL_TILE_WIDTH, 
                            player * Coords.SMALL_TILE_WIDTH, 
                            Coords.IN_GAME_BOARD_TILE_AREA_X + ((oldX - 1) * Coords.SMALL_TILE_WIDTH), 
                            Coords.IN_GAME_BOARD_TILE_AREA_Y + ((oldY - 1) * Coords.SMALL_TILE_WIDTH), 
                            Coords.SMALL_TILE_WIDTH, 
                            Coords.SMALL_TILE_WIDTH,
                            trackOverlays);
                newType = gameInstance.board.findNextTrackSection(oldX, oldY, oldExit, ref newX, ref newY, ref newExit);
            }

            if(loopCatcher >= loopLimit)
            {
                System.Diagnostics.Debug.WriteLine("Infinite loop caught while drawing overlays");
            }
        }

        /****************************************************************************
        * drawAllTrackOverlays
        *
        * Description:
        *   Draws track overlays for every station on the board. 
        *
        ****************************************************************************/
        private void drawAllTrackOverlays()
        {
            int counter;

            for(counter=0; counter<Board.NUM_STATIONS; counter++)
            {
                drawTrackOverlay(counter);
            }

        }

        /****************************************************************************
        * drawCurrentPlayerHighlight
        *
        * Description:
        *   Draws the box and arrow around the active player's name.  
        *
        ****************************************************************************/
        private void drawCurrentPlayerHighlight()
        {
            blitSurface(0, 
                        0, 
                        Coords.IN_GAME_CURRENT_PLAYER_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 0], 
                        Coords.IN_GAME_CURRENT_PLAYER_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 1], 
                        Coords.IN_GAME_CURRENT_PLAYER_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 2], 
                        Coords.IN_GAME_CURRENT_PLAYER_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 3], 
                        currentPlayer);
        }

        /****************************************************************************
        * drawActiveTileHighlight
        *
        * Description:
        *   Draws a box around the player's currently selected (active) tile.  
        *
        ****************************************************************************/
        private void drawActiveTileHighlight()
        {
            int x, y, w, h;

            if(gameInstance.players[gameInstance.curPlayer].currentTileId != Tile.NONE ||
               gameInstance.players[gameInstance.curPlayer].reserveTileId != Tile.NONE)
            {
                if(gameInstance.selectedMoveIsReserveTile == TrackInsanity.TRUE)
                {
                    x = Coords.IN_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 0];
                    y = Coords.IN_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 1];
                    w = Coords.IN_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 2];
                    h = Coords.IN_GAME_PLAYER_TILE2_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 3];
                }
                else
                {
                    x = Coords.IN_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 0];
                    y = Coords.IN_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 1];
                    w = Coords.IN_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 2];
                    h = Coords.IN_GAME_PLAYER_TILE1_HIGHLIGHT_OFFSETS[gameInstance.curPlayer, 3];
                }

                blitSurface(0, 0, x, y, w, h, currentTile);
            }
        }

        /****************************************************************************
        * drawDarkenMasks
        *
        * Description:
        *   Draws the two semi-transparent masks on screen when choosing a move 
        *   to play -- the 'legal moves' highlights, and a mask, drawn around the 
        *   board, that darkens the non-board area.
        *
        ****************************************************************************/
        private void drawDarkenMasks()
        {
            int counter, counter2;

            blitAlphaSurface(0, 
                             0, 
                             Coords.IN_GAME_BOARD_AREA_X, 
                             Coords.IN_GAME_BOARD_AREA_Y, 
                             Coords.IN_GAME_BOARD_AREA_WIDTH,
                             Coords.IN_GAME_BOARD_AREA_HEIGHT,
                             50, 
                             darkenMask);

            /* Only draw the legal move masks if the player requests it */
            if (gameInstance.highlightLegalMoves == Game.OPTION_YES)
            {
                for (counter = 1; counter < Board.WIDTH - 1; counter++)
                {
                    for (counter2 = 1; counter2 < Board.HEIGHT - 1; counter2++)
                    {
                        if (gameInstance.board.isLegalMove(counter, counter2) == TrackInsanity.TRUE)
                        {
                            blitAlphaSurface(0, 
                                             0, 
                                             Coords.IN_GAME_BOARD_TILE_AREA_X + ((counter - 1) * Coords.SMALL_TILE_WIDTH), 
                                             Coords.IN_GAME_BOARD_TILE_AREA_Y + ((counter2 - 1) * Coords.SMALL_TILE_WIDTH), 
                                             Coords.SMALL_TILE_WIDTH, 
                                             Coords.SMALL_TILE_WIDTH, 
                                             25, 
                                             validMoveMask);
                        }
                    }
                }
            }
        }

        /****************************************************************************
        * drawPlayCancelButton
        *
        * Description:
        *   Draws the 'Cancel' button in the corner of the window.  This is shown
        *   when the player is selecting a location to place a tile
        *
        ****************************************************************************/
        private void drawPlayCancelButton()
        {
            blitSurface(0, 
                        0,
                        Coords.IN_GAME_CANCEL_ACTION_BUTTON_MIN_X, 
                        Coords.IN_GAME_ACTION_CANCEL_BUTTON_MIN_Y, 
                        Coords.IN_GAME_CANCEL_ACTION_BUTTON_MAX_X - Coords.IN_GAME_CANCEL_ACTION_BUTTON_MIN_X, 
                        Coords.IN_GAME_ACTION_CANCEL_BUTTON_MAX_Y - Coords.IN_GAME_ACTION_CANCEL_BUTTON_MIN_Y, 
                        cancelPlayTileButton);
        }


        /****************************************************************************
        * displayDialog
        *
        * Description:
        *   Draws a dialog box on the screen.  This method is subject to change --
        *   dialogs will probably become their own objects.  
        *
        ****************************************************************************/
        private void displayDialog(Texture2D dialog, int x, int y, int width, int height)
        {
            blitSurface(0, 0, x, y, width, height, dialog);
        }

        /****************************************************************************
        * drawGameStateSpecificDialogs
        *
        * Description:
        *   Causes dialogs to be displayed on screen depending on the current game
        *   (not render) state.  
        *
        ****************************************************************************/
        private void drawGameStateSpecificDialogs()
        {
            int counter;
            int skipPlayTile = TrackInsanity.TRUE;

            if (gameInstance.gameStateChanged == TrackInsanity.TRUE)
            {
                /* Perform one time state change processing */
                foreach (Dialog d in dialogList)
                {
                    d.processHide();
                }
                switch (gameInstance.gameState)
                {
                    case (int)(Game.GameState.NEXT_PLAYER_TURN):
                        if (gameInstance.players[gameInstance.curPlayer].controlledBy == Player.HUMAN && gameInstance.numPlayers > 2)
                        {
                            playerTurn[gameInstance.curPlayer].processShow();
                        }
                        break;
                    case (int)(Game.GameState.SELECT_ACTION):
                      /* Display play option (if required) */
                        if (gameInstance.players[gameInstance.curPlayer].currentTileId != Tile.NONE ||
                            gameInstance.players[gameInstance.curPlayer].reserveTileId != Tile.NONE)
                        {
                            if (gameInstance.players[gameInstance.curPlayer].currentTileId != Tile.NONE && 
                                gameInstance.board.markLegalMoves(gameInstance.tilepool.getTile(gameInstance.players[gameInstance.curPlayer].currentTileId)) > 0)
                            {
                                skipPlayTile = TrackInsanity.FALSE;
                            }
                            if (gameInstance.players[gameInstance.curPlayer].reserveTileId != Tile.NONE && 
                                gameInstance.board.markLegalMoves(gameInstance.tilepool.getTile(gameInstance.players[gameInstance.curPlayer].reserveTileId)) > 0)
                            {
                                skipPlayTile = TrackInsanity.FALSE;
                            }
    
                            if (skipPlayTile == TrackInsanity.FALSE)
                            {
                                selectAction.setElementVisibility("selectPlayActive", true);
                                gameInstance.playIsValid = TrackInsanity.TRUE;
                            }
                        }
                        else
                        {
                            selectAction.setElementVisibility("selectPlayActive", false);
                            gameInstance.playIsValid = TrackInsanity.FALSE;
                        }

                        /* Display draw option (if required) */
                        if (gameInstance.playerHasDrawn == TrackInsanity.FALSE && 
                            gameInstance.tilepool.numUnplayedTiles > 0)
                        {
                            selectAction.setElementVisibility("selectDrawActive", true);
                            gameInstance.drawIsValid = TrackInsanity.TRUE;
                        }
                        else
                        {
                            selectAction.setElementVisibility("selectDrawActive", false);
                            gameInstance.drawIsValid = TrackInsanity.FALSE;
                        }

                        /* Display discard option (if required) */
                        if (gameInstance.players[gameInstance.curPlayer].currentTileId != Tile.NONE ||
                            gameInstance.players[gameInstance.curPlayer].reserveTileId != Tile.NONE)
                        {
                            selectAction.setElementVisibility("selectDiscardActive", true);
                            gameInstance.discardIsValid = TrackInsanity.TRUE;
                        }
                        else
                        {
                            selectAction.setElementVisibility("selectDiscardActive", false);
                            gameInstance.discardIsValid = TrackInsanity.FALSE;
                        }

                        /* Display the pass option */
                        selectAction.setElementVisibility("selectPassActive", true);
                        gameInstance.passIsValid = TrackInsanity.TRUE;
                        /* Start the display */
                        selectAction.processShow();
                        break;
                    case (int)(Game.GameState.TILE_DRAW):
                        /* If fewer tiles remain than are needed to fill the dialog, disable the extras */
                        for (counter = 0; counter < NUM_DRAW_TILES_TO_DISPLAY; counter++)
                        {
                            if (counter < gameInstance.tilepool.numUnplayedTiles)
                            {
                                selectTile.setElementVisibility(string.Concat("tile", counter),
                                                                true);
                            }
                            else
                            {
                                selectTile.setElementVisibility(string.Concat("tile", counter),
                                                                false);
                            }
                        }
                        selectTile.processShow();
                        break;
                    case (int)(Game.GameState.DISCARD):
                        chooseDiscard.processShow();
                        break;
                    case (int)(Game.GameState.COMPUTER_MOVE):
                        computerThinking.processShow();
                        break;
                    case (int)(Game.GameState.GAME_FINISHED):
                        gameFinished.processShow();
                        break;
                    case (int)(Game.GameState.SHOW_BOARD):
                        showingBoard.processShow();
                        break;
                    case (int)(Game.GameState.CONFIRM_EXIT):
                        confirmExit.processShow();
                        break;
                }
                gameInstance.gameStateChanged = TrackInsanity.FALSE;
            }

            /* Perform every frame state global state change processing */
            foreach (Dialog d in dialogList)
            {
                d.draw(this);
            }

            /* Perform every frame state specific processing */
            switch (gameInstance.gameState)
            {
                case (int)(Game.GameState.TILE_DRAW):
                    /* Draw the highlight over the currently selected tile */
                    blitSurface(0,
                                0,
                                Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[drawTileHighlighted, 0] + Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[0] - Coords.LARGE_TILE_HIGHLIGHT_OFFSET,
                                Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[drawTileHighlighted, 1] + Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[1] - Coords.LARGE_TILE_HIGHLIGHT_OFFSET,
                                Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[drawTileHighlighted, 2] + Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[0] + (2 * Coords.LARGE_TILE_HIGHLIGHT_OFFSET),
                                Coords.IN_GAME_DRAW_TILE_DIALOG_TILE_OFFSETS[drawTileHighlighted, 3] + Coords.IN_GAME_DRAW_TILE_DIALOG_OFFSET[1] + (2 * Coords.LARGE_TILE_HIGHLIGHT_OFFSET),
                                drawTileHighlightLarge);
                    break;
                case (int)(Game.GameState.TILE_SELECT):
                    drawDarkenMasks();
                    drawPlayCancelButton();
                    break;
                default:
                    break;
            }

        }

        /****************************************************************************
        * drawResultsScoreDigits
        *
        * Description:
        *   Draws a score on the screen in one of six places ('rank').  
        *
        ****************************************************************************/
        private void drawResultsScoreDigits(int rank, int score, int screenOffset)
        {
            int blitX, blitY;
            int numDigits, digit;
            int counter, offset;
            int tmpScore;

            /* Start with the most significant digit, and work toward the least */
            blitX = Coords.END_OF_GAME_RESULTS_SCORE_OFFSETS[rank, 0] + screenOffset;
            blitY = Coords.END_OF_GAME_RESULTS_SCORE_OFFSETS[rank, 1];

            tmpScore = score;

            /* The score should never be this high. If it is, just don't draw it */
            if (tmpScore > 9999)
            {
                return;
            }
            else if (tmpScore > 999 && score <= 9999)
            {
                numDigits = 4;
            }
            else if (tmpScore > 99 && score <= 999)
            {
                numDigits = 3;
            }
            else if (tmpScore > 9 && score <= 99)
            {
                numDigits = 2;
            }
            else
            {
                numDigits = 1;
            }

            for (counter = numDigits - 1; counter >= 0; counter--)
            {
                digit = (int)((float)tmpScore / Math.Pow(10.0f, (float)counter));
                tmpScore = tmpScore - (int)((float)digit * Math.Pow(10.0f, (float)counter));
                if (rank == 0)
                {
                    offset = digit * Coords.GAME_RESULTS_LARGE_DIGIT_OFFSET;
                    blitSurface(offset, 
                                0, 
                                blitX, 
                                blitY, 
                                Coords.GAME_RESULTS_LARGE_DIGIT_WIDTHS[digit], 
                                Coords.GAME_RESULTS_LARGE_DIGIT_HEIGHT,
                                resultsLargeDigits);
                    blitX += Coords.GAME_RESULTS_LARGE_DIGIT_WIDTHS[digit] + Coords.GAME_RESULTS_LARGE_DIGIT_SPACING;
                }
                else
                {
                    offset = digit * Coords.GAME_RESULTS_SMALL_DIGIT_OFFSET;
                    blitSurface(offset, 
                                0, 
                                blitX, 
                                blitY, 
                                Coords.GAME_RESULTS_SMALL_DIGIT_WIDTHS[digit], 
                                Coords.GAME_RESULTS_SMALL_DIGIT_HEIGHT, 
                                resultsSmallDigits);
                    blitX += Coords.GAME_RESULTS_SMALL_DIGIT_WIDTHS[digit] + Coords.GAME_RESULTS_SMALL_DIGIT_SPACING;
                }
            }
        }

        /****************************************************************************
        * drawResultsScores
        *
        * Description:
        *   Draw the player names and scores for all players on the results screen.
        *
        ****************************************************************************/
        private void drawResultsScores(int offset)
        {
            int counter, counter2, temp;
            int[] playerRanks = new int[TrackInsanity.MAX_PLAYERS];

            /* Use the scores of each player to rank them */
            for (counter = 0; counter < gameInstance.numPlayers; counter++)
            {
                playerRanks[counter] = counter;
            }
            for (counter = 0; counter < gameInstance.numPlayers - 1; counter++)
            {
                for (counter2 = counter + 1; counter2 < gameInstance.numPlayers; counter2++)
                {
                    if (gameInstance.players[playerRanks[counter2]].score > gameInstance.players[playerRanks[counter]].score)
                    {
                        temp = playerRanks[counter2];
                        playerRanks[counter2] = playerRanks[counter];
                        playerRanks[counter] = temp;
                    }
                }
            }
            
            /* Display the highest ranking player at the top */
            blitSurface(Coords.GAME_RESULTS_LARGE_DIGIT_OFFSET,
                        0, 
                        Coords.END_OF_GAME_RESULTS_RANK_OFFSETS[0, 0] + offset, 
                        Coords.END_OF_GAME_RESULTS_RANK_OFFSETS[0, 1], 
                        Coords.GAME_RESULTS_LARGE_DIGIT_WIDTHS[1], 
                        Coords.GAME_RESULTS_LARGE_DIGIT_HEIGHT, 
                        resultsLargeDigits);
            blitSurface(0, 
                        playerRanks[0] * Coords.GAME_RESULTS_LARGE_PLAYER_HEIGHT, 
                        Coords.END_OF_GAME_RESULTS_PLAYER_NAME_OFFSETS[0, 0] + offset, 
                        Coords.END_OF_GAME_RESULTS_PLAYER_NAME_OFFSETS[0, 1], 
                        Coords.GAME_RESULTS_LARGE_PLAYER_WIDTH, 
                        Coords.GAME_RESULTS_LARGE_PLAYER_HEIGHT, 
                        resultsLargePlayers);
            drawResultsScoreDigits(0, gameInstance.players[playerRanks[0]].score, offset);

            /* Display the remaining players in a smaller font below the top player */
            for (counter = 1; counter < gameInstance.numPlayers; counter++)
            {
                blitSurface((counter + 1) * Coords.GAME_RESULTS_SMALL_DIGIT_OFFSET, 
                            0, 
                            Coords.END_OF_GAME_RESULTS_RANK_OFFSETS[counter, 0] + offset,
                            Coords.END_OF_GAME_RESULTS_RANK_OFFSETS[counter, 1], 
                            Coords.GAME_RESULTS_SMALL_DIGIT_WIDTHS[(counter + 1)], 
                            Coords.GAME_RESULTS_SMALL_DIGIT_HEIGHT, 
                            resultsSmallDigits);
                blitSurface(0, 
                            playerRanks[counter] * Coords.GAME_RESULTS_SMALL_PLAYER_HEIGHT, 
                            Coords.END_OF_GAME_RESULTS_PLAYER_NAME_OFFSETS[counter, 0] + offset, 
                            Coords.END_OF_GAME_RESULTS_PLAYER_NAME_OFFSETS[counter, 1], 
                            Coords.GAME_RESULTS_SMALL_PLAYER_WIDTH, 
                            Coords.GAME_RESULTS_SMALL_PLAYER_HEIGHT, 
                            resultsSmallPlayers);
                drawResultsScoreDigits(counter, gameInstance.players[playerRanks[counter]].score, offset);
            }
        }

        /****************************************************************************
        * drawResultsScreen
        *
        * Description:
        *   Draws the entire contents of the results screen.
        *
        ****************************************************************************/
        private void drawResultsScreen(int offset)
        {
            blitSurface(0,
                        0,
                        offset,
                        0,
                        resultsBackground.Width,
                        resultsBackground.Height,
                        resultsBackground);
            drawResultsScores(offset);
        }

        /****************************************************************************
        * processInGameEvents
        *
        * Description:
        *   Checks to see if important areas of the screen have been clicked, and
        *   take the required actions.  This method only applies to areas within the
        *   game itself.  Other areas (title screen, option screen) are handled 
        *   in another method. 
        *
        ****************************************************************************/
        private void processInGameEvents(int xPos, int yPos)
        {
            int counter, counter2, tileX, tileY;
            /* Check to see if the 'X' in the upper right hand corner is displayed */
            if (gameInstance.gameState != (int)Game.GameState.CONFIRM_EXIT &&
                    isWithinBoundary(xPos, 
                                     yPos, 
                                     Coords.IN_GAME_END_GAME_BUTTON_MIN_X, 
                                     Coords.IN_GAME_END_GAME_BUTTON_MIN_Y, 
                                     Coords.IN_GAME_END_GAME_BUTTON_MAX_X, 
                                     Coords.IN_GAME_END_GAME_BUTTON_MAX_Y) == true)
            {
                gameInstance.changeState((int)Game.GameState.CONFIRM_EXIT);
            }

            /* State specific */
            switch (gameInstance.gameState)
            {
                case (int)(Game.GameState.NEXT_PLAYER_TURN):
                    if (gameInstance.numPlayers == 2 || gameInstance.players[gameInstance.curPlayer].controlledBy == Player.COMPUTER)
                    {
                        gameInstance.changeState((int)(Game.GameState.SELECT_ACTION));
                    }
                    else if (playerTurn[gameInstance.curPlayer].isClicked(String.Concat("P", gameInstance.curPlayer, "Turn"), xPos, yPos) == true)
                    {
                        gameInstance.changeState((int)(Game.GameState.SELECT_ACTION));
                    }
                    break;
                case (int)(Game.GameState.SELECT_ACTION):
                    if(selectAction.isClicked("selectDrawActive", xPos, yPos))
                    {
                        if (gameInstance.drawIsValid == TrackInsanity.TRUE)
                        {
                            gameInstance.changeState((int)(Game.GameState.TILE_DRAW));
                        }
                    }
                    if(selectAction.isClicked("selectPlayActive", xPos, yPos))
                    {
                        if (gameInstance.playIsValid == TrackInsanity.TRUE)
                        {
                            gameInstance.changeState((int)(Game.GameState.TILE_SELECT));
                        }
                    }
                    /* Check if 'discard tile' was selected */
                    if(selectAction.isClicked("selectDiscardActive", xPos, yPos))
                    {
                        if(gameInstance.discardIsValid == TrackInsanity.TRUE)
                        {
                            gameInstance.changeState((int)(Game.GameState.DISCARD));
                        }
                    }

                    if(selectAction.isClicked("selectPassActive", xPos, yPos))
                    {
                        if(gameInstance.passIsValid == TrackInsanity.TRUE)
                        {
                            gameInstance.changeState((int)(Game.GameState.END_TURN));
                        }
                    }
                    break;
                case (int)(Game.GameState.TILE_DRAW):
                    /* Check each valid tile coordinate to see if the mouse has been clicked.  If so, update the highlighted tile. */
                    if (gameInstance.tilepool.numUnplayedTiles < NUM_DRAW_TILES_TO_DISPLAY)
                    {
                        counter2 = gameInstance.tilepool.numUnplayedTiles;
                    }
                    else
                    {
                        counter2 = NUM_DRAW_TILES_TO_DISPLAY;
                    }
                    for(counter=0;counter<counter2;counter++)
                    {
                        if(selectTile.isClicked(string.Concat("tile", counter),
                                                xPos,
                                                yPos) == true)
                        {
                            System.Diagnostics.Debug.WriteLine("Clicked on tile {0}", counter);
                            prevDrawTileHighlighted = drawTileHighlighted;
                            drawTileHighlighted = counter;
                        }
                    }
                    /* Check to see if 'OK' was selected */
                    if(selectTile.isClicked("ok", 
                                            xPos,
                                            yPos) == true)
                    {
                        if(gameInstance.players[gameInstance.curPlayer].currentTileId == Tile.NONE)
                        {
                            gameInstance.players[gameInstance.curPlayer].currentTileId = gameInstance.tilepool.drawRandomTile();
                            gameInstance.selectedMoveIsReserveTile = TrackInsanity.FALSE;
                            gameInstance.selectedMoveTileId = gameInstance.players[gameInstance.curPlayer].currentTileId;
                        }
                        else
                        {
                            gameInstance.players[gameInstance.curPlayer].reserveTileId = gameInstance.tilepool.drawRandomTile();
                            gameInstance.selectedMoveIsReserveTile = TrackInsanity.TRUE;
                            gameInstance.selectedMoveTileId = gameInstance.players[gameInstance.curPlayer].reserveTileId;
                        }
                        drawTileHighlighted = 0;
                        prevDrawTileHighlighted = 0;
                        gameInstance.playerHasDrawn = TrackInsanity.TRUE;
                        gameInstance.changeState((int)(Game.GameState.SELECT_ACTION));
                    }
                    /* Check to see if 'Cancel' was selected */
                    if(selectTile.isClicked("cancel", 
                                            xPos, 
                                            yPos) == true)
                    {
                        drawTileHighlighted = 0;
                        prevDrawTileHighlighted = 0;
                        gameInstance.changeState((int)(Game.GameState.SELECT_ACTION));
                    }
                    break;
                case (int)(Game.GameState.TILE_SELECT):
                    /* Check to see if one of the player's tiles has been pressed.  If so, and a tile exists, make it the active tile */
                    if(isWithinBoundary(xPos, 
                                        yPos, 
                                        Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 0],                                        
                                        Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 1],                                        
                                        Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 0] + Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 2],                                       
                                        Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 1] + Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 3]) == true)
                    {
                        if(gameInstance.selectedMoveIsReserveTile == TrackInsanity.TRUE && gameInstance.players[gameInstance.curPlayer].currentTileId != Tile.NONE)
                        {
                            gameInstance.selectedMoveIsReserveTile = TrackInsanity.FALSE;
                            gameInstance.selectedMoveTileId = gameInstance.players[gameInstance.curPlayer].currentTileId;
                            gameInstance.board.markLegalMoves(gameInstance.tilepool.getTile(gameInstance.selectedMoveTileId));
                        }
                    }
                    if(isWithinBoundary(xPos, 
                                        yPos, 
                                        Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 0],                                        
                                        Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 1],                                        
                                        Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 0] + Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 2],                                         
                                        Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 1] + Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 3]) == true)
                    {
                        gameInstance.selectedMoveIsReserveTile = TrackInsanity.TRUE;
                        gameInstance.selectedMoveTileId = gameInstance.players[gameInstance.curPlayer].reserveTileId;
                        gameInstance.board.markLegalMoves(gameInstance.tilepool.getTile(gameInstance.selectedMoveTileId));
                    }
                    if(isWithinBoundary(xPos, 
                                        yPos, 
                                        Coords.IN_GAME_BOARD_TILE_AREA_X,
                                        Coords.IN_GAME_BOARD_TILE_AREA_Y,
                                        Coords.IN_GAME_BOARD_TILE_AREA_MAX_X, 
                                        Coords.IN_GAME_BOARD_TILE_AREA_MAX_Y) == true)
                    {
                        tileX = ((xPos - Coords.IN_GAME_BOARD_TILE_AREA_X) / Coords.SMALL_TILE_WIDTH) + 1;
                        tileY = ((yPos - Coords.IN_GAME_BOARD_TILE_AREA_Y) / Coords.SMALL_TILE_WIDTH) + 1;
                        if(gameInstance.board.isLegalMove(tileX, tileY) == TrackInsanity.TRUE)
                        {
                            /* A move has been made. Prepare to switch states. */
                            gameInstance.selectedMoveTileX = tileX;
                            gameInstance.selectedMoveTileY = tileY;
                            gameInstance.players[gameInstance.curPlayer].lastMoveX = gameInstance.selectedMoveTileX;
                            gameInstance.players[gameInstance.curPlayer].lastMoveY = gameInstance.selectedMoveTileY;
                            /* Take the tile out of the player's inventory */
                            if(gameInstance.selectedMoveIsReserveTile == TrackInsanity.TRUE)
                            {
                                gameInstance.players[gameInstance.curPlayer].reserveTileId = Tile.NONE;
                            }
                            else
                            {
                                if(gameInstance.players[gameInstance.curPlayer].reserveTileId != Tile.NONE)
                                {
                                    gameInstance.players[gameInstance.curPlayer].currentTileId = gameInstance.players[gameInstance.curPlayer].reserveTileId;
                                    gameInstance.players[gameInstance.curPlayer].reserveTileId = Tile.NONE;
                                }
                                else
                                {
                                    gameInstance.players[gameInstance.curPlayer].currentTileId = Tile.NONE;
                                }
                            }
                            gameInstance.changeState((int)(Game.GameState.TILE_PLAY));
                        }
                        else
                        {
                            /* Do something for an illegal move? */
                        }
                    }
                    /* Check to see if the cancel button was pressed */
                    if(isWithinBoundary(xPos, 
                                        yPos, 
                                        Coords.IN_GAME_CANCEL_ACTION_BUTTON_MIN_X, 
                                        Coords.IN_GAME_ACTION_CANCEL_BUTTON_MIN_Y, 
                                        Coords.IN_GAME_CANCEL_ACTION_BUTTON_MAX_X, 
                                        Coords.IN_GAME_ACTION_CANCEL_BUTTON_MAX_Y) == true)
                    {
                        gameInstance.changeState((int)(Game.GameState.SELECT_ACTION));
                    }
                    break;
                case (int)(Game.GameState.DISCARD):
                    /* Check to see if one of the player's tiles has been selected */
                    if (isWithinBoundary(xPos, 
                                         yPos, 
                                         Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 0],                                         
                                         Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 1],                                         
                                         Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 0] + Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 2],                                         
                                         Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 1] + Coords.IN_GAME_PLAYER_TILE1_OFFSETS[gameInstance.curPlayer, 3]) == true)
                    {
                        if (gameInstance.selectedMoveIsReserveTile == TrackInsanity.TRUE && gameInstance.players[gameInstance.curPlayer].currentTileId != Tile.NONE)
                        {
                            gameInstance.selectedMoveIsReserveTile = TrackInsanity.FALSE;
                            gameInstance.selectedMoveTileId = gameInstance.players[gameInstance.curPlayer].currentTileId;
                            gameInstance.board.markLegalMoves(gameInstance.tilepool.getTile(gameInstance.selectedMoveTileId));
                        }
                    }
                    if (isWithinBoundary(xPos, 
                                         yPos, 
                                         Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 0],                                         
                                         Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 1],                                         
                                         Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 0] + Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 2],                                         
                                         Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 1] + Coords.IN_GAME_PLAYER_TILE2_OFFSETS[gameInstance.curPlayer, 3]) == true)
                    {
                        if (gameInstance.selectedMoveIsReserveTile == TrackInsanity.FALSE && gameInstance.players[gameInstance.curPlayer].reserveTileId != Tile.NONE)
                        {
                            gameInstance.selectedMoveIsReserveTile = TrackInsanity.TRUE;
                            gameInstance.selectedMoveTileId = gameInstance.players[gameInstance.curPlayer].reserveTileId;
                            gameInstance.board.markLegalMoves(gameInstance.tilepool.getTile(gameInstance.selectedMoveTileId));
                        }
                    }
                    /* Check to see if 'OK' was selected */
                    if(chooseDiscard.isClicked("ok", xPos, yPos) == true)
                    {
                        /* Discard the tile to the tile pool */
                        gameInstance.discardTile();
                        gameInstance.changeState((int)(Game.GameState.END_TURN));
                    }
                    /* Check to see if 'Cancel' was selected */
                    if(chooseDiscard.isClicked("cancel", xPos, yPos) == true)
                    {
                        gameInstance.changeState((int)(Game.GameState.SELECT_ACTION));
                    }
                    break;
                case (int)(Game.GameState.GAME_FINISHED):
                    if (gameFinished.isClicked("showBoard", xPos, yPos) == true)
                    {
                        gameInstance.changeState((int)(Game.GameState.SHOW_BOARD));
                    }
                    if(gameFinished.isClicked("showScores", xPos, yPos) == true)
                    {
                        /* Go to the game results screen */
                        titleScreenMovingX = SharedData.xRes;
                        titleScreenMovingSpeed = 2.0f;
                        inGameMovingToFinished = true;
                    }
                    break;
                case (int)(Game.GameState.SHOW_BOARD):
                    if (showingBoard.isClicked("ok", xPos, yPos) == true)
                    {
                        /* Go to the game results screen */
                        titleScreenMovingX = SharedData.xRes;
                        titleScreenMovingSpeed = 2.0f;
                        inGameMovingToFinished = true;
                    }
                    break;
                case (int)(Game.GameState.CONFIRM_EXIT):
                    /* Check to see if 'yes' was selected */
                    if (confirmExit.isClicked("yes", xPos, yPos))
                    {
                        confirmExit.setVisible(false);
                        topTrainX = Coords.TRAIN_INITIAL_X;
                        topTrainDirection = TRAIN_DIR_RIGHT;
                        bottomTrainX = Coords.TRAIN_FINAL_X;
                        bottomTrainDirection = TRAIN_DIR_LEFT;
                        inGameMovingToTitle = true;
                        titleScreenBackgroundMovingX = -SharedData.xRes;
                        titleScreenMovingX = -SharedData.xRes;
                        titleScreenMovingSpeed = 2.0f;
                    }
                    if(confirmExit.isClicked("no", xPos, yPos))
                    {
                        gameInstance.changeState(gameInstance.previousGameState);
                    }
                    break;
                default:
                    break;
            }                                                          
        }

        /****************************************************************************
        * drawInGamePlayerScore
        *
        * Description:
        *   Draw the current score for the specified player.  
        *
        ****************************************************************************/
        private void drawInGamePlayerScore(int player)
        {
            int blitX;
            int blitY;
            int score;
            int numDigits;
            int digit;
            int counter,offset;

            /* Start with the most significant digit and work toward the least */
            blitX = Coords.IN_GAME_PLAYER_SCORE_OFFSETS[player, 0];
            blitY = Coords.IN_GAME_PLAYER_SCORE_OFFSETS[player, 1];

            score = gameInstance.players[player].score;

            /* The score should never be this high. If it is, just don't draw it */
            if(score > 9999)
            {
                return;
            }
            else if(score > 999 && score <= 9999)
            {
                numDigits = 4;
            }
            else if(score > 99 && score <= 999)
            {
                numDigits = 3;
            }
            else if(score > 9 && score <= 99)
            {
                numDigits = 2;
            }
            else
            {
                numDigits = 1;
            }

            for(counter=numDigits-1;counter>=0;counter--)
            {
                digit = (int)((float)score / Math.Pow(10.0f, (float)counter));
                score = score - (int)((float)digit * Math.Pow(10.0f, (float)counter));
                offset = digit * Coords.IN_GAME_DIGIT_OFFSET;
                blitSurface(offset,
                            0, 
                            blitX, 
                            blitY, 
                            Coords.OPTION_SCREEN_DIGIT_WIDTHS[digit], 
                            Coords.IN_GAME_DIGIT_HEIGHT, 
                            digits);
                blitX += (Coords.OPTION_SCREEN_DIGIT_WIDTHS[digit] + Coords.IN_GAME_LETTER_SPACING);
            }
        }

        public void drawTitleInfo(int offset)
        {
            blitSurface(0,
                        0,
                        Coords.TITLE_COPYRIGHT_TEXT_X + offset,
                        Coords.TITLE_COPYRIGHT_TEXT_Y,
                        copyrightText.Width,
                        copyrightText.Height,
                        copyrightText);
            blitSurface(0,
                        0,
                        Coords.TRACK_WORD_FINAL_X + offset,
                        Coords.TRACK_WORD_Y,
                        trackWord.Width,
                        trackWord.Height,
                        trackWord);
            blitSurface(0,
                        0,
                        Coords.INSANITY_WORD_FINAL_X + offset,
                        Coords.TRACK_WORD_Y,
                        insanityWord.Width,
                        insanityWord.Height,
                        insanityWord);
        }

        /****************************************************************************
        * updateScreen
        *
        * Description:
        *   This is the function that gets called once per iteration of the game
        *   loop, and is responsible for making sure the display is correct
        *   regardless of the current render state.
        *
        ****************************************************************************/
        public void updateScreen(GraphicsDevice d)
        {
            switch (SharedData.renderState)
            {
                case (int)RenderState.COMPANY_LOGO:
                    d.Clear(Color.White);
                    if (startLogoFadeIn == true)
                    {
                        blitAlphaSurface(0, 0, 0, 0, SharedData.xRes, SharedData.yRes, titleAlpha, logoScreenBG);
                        if (titleAlpha <= 100)
                        {
                            titleAlpha = titleAlpha + LOGO_FADE_IN_SPEED;
                        }
                    }
                    if (startTitleDropDown == true)
                    {
                        blitAlphaSurface(0, 0, 0, (int)titleDropDownY, SharedData.xRes, SharedData.yRes, 100, titleScreenBlankBG);
                        if (titleDropDownY < -titleDropDownSpeed)
                        {
                            titleDropDownY = titleDropDownY + (int)titleDropDownSpeed;
                            titleDropDownSpeed = titleDropDownSpeed * 1.2f;
                        }
                        else
                        {
                            titleDropDownY = 0;
                            if(moveTrackWord == false)
                            {
                                moveWordTicks = SharedData.curTicks;
                            }
                            moveTrackWord = true;
                        }
                    }
                    if (moveTrackWord == true)
                    {
                        blitAlphaSurface(0, 0, (int)moveTrackWordX, Coords.TRACK_WORD_Y, trackWord.Width, trackWord.Height, 100, trackWord);
                        if(moveTrackWordX < (Coords.TRACK_WORD_FINAL_X - moveTrackWordSpeed))
                        {
                            moveTrackWordX = moveTrackWordX + moveTrackWordSpeed;
                            moveTrackWordSpeed = moveTrackWordSpeed * 1.2f;
                        }
                        else
                        {
                            moveTrackWordX = Coords.TRACK_WORD_FINAL_X;
                        }
                    }
                    if(moveInsanityWord == true)
                    {
                        blitAlphaSurface(0, 0, (int)moveInsanityWordX, Coords.TRACK_WORD_Y, insanityWord.Width, insanityWord.Height, 100, insanityWord);
                        System.Diagnostics.Debug.WriteLine("X (first) = {0}, speed = {1}, final = {2}", (int)moveInsanityWordX, (int)moveInsanityWordSpeed, Coords.INSANITY_WORD_FINAL_X);
                        if((int)(moveInsanityWordX + moveInsanityWordSpeed) > Coords.INSANITY_WORD_FINAL_X)
                        {
                            moveInsanityWordX = moveInsanityWordX + moveInsanityWordSpeed;
                            moveInsanityWordSpeed = moveInsanityWordSpeed * 1.2f;
                            System.Diagnostics.Debug.WriteLine("X = {0}", moveInsanityWordX);
                        }
                        else
                        {
                            moveInsanityWordX = Coords.INSANITY_WORD_FINAL_X;
                        }
                    }
                    break;
                case (int)RenderState.TITLE_SCREEN:
                    renderTitleScreenDrawBase();
                    drawTitleInfo(0);
                    if (SharedData.curTicks - titleScreenPauseFlashTicks > TAP_SCREEN_PRE_FLASH_DELAY)
                    {
                        /* If the current frame count falls within the correct range, draw the 'Tap screen to play' message */
                        if ((SharedData.numFrames % FRAME_RATE) >= TAP_MSG_FRAME_SHOWN_TIME &&
                            (SharedData.numFrames % FRAME_RATE) < TAP_MSG_FRAME_HIDDEN_TIME)
                        {
                            blitSurface(0,
                                        0,
                                        Coords.TITLE_SCREEN_TAP_TO_PLAY_X,
                                        Coords.TITLE_SCREEN_TAP_TO_PLAY_Y,
                                        Coords.TITLE_SCREEN_TAP_TO_PLAY_WIDTH,
                                        Coords.TITLE_SCREEN_TAP_TO_PLAY_HEIGHT,
                                        tapScreenMsg);
                        }
                    }
                    else
                    {
                         blitSurface(0,
                                   0,
                                   Coords.TITLE_SCREEN_TAP_TO_PLAY_X,
                                   Coords.TITLE_SCREEN_TAP_TO_PLAY_Y,
                                   Coords.TITLE_SCREEN_TAP_TO_PLAY_WIDTH,
                                   Coords.TITLE_SCREEN_TAP_TO_PLAY_HEIGHT,
                                   tapScreenMsg);
                    }
                    break;
                case (int)(RenderState.TITLE_MENU_SCREEN):
                    /* Draw the stuff common to the title screen */
                    renderTitleScreenDrawBase();
                    drawTitleInfo(0);
                    /* Draw the menu screen */
                    blitAlphaSurface(0, 
                                0,
                                Coords.TITLE_SCREEN_MENU_X, 
                                Coords.TITLE_SCREEN_MENU_Y, 
                                Coords.TITLE_SCREEN_MENU_WIDTH,
                                Coords.TITLE_SCREEN_MENU_HEIGHT, 
                                titleMenuAlpha,
                                titleMenu);
                    if (titleMenuAlpha < 100)
                    {
                        titleMenuAlpha += 5;
                    }
                    break;
                case (int)(RenderState.NEW_GAME_SCREEN):
                    /* Draw the stuff common to the title screens */
                    /* Draw everything else */
                    if (titleMovingToNewGame == true)
                    {
                        if (titleScreenMovingX >= (-SharedData.xRes))
                        {
                            titleScreenMovingX = titleScreenMovingX - titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX - (int)(titleScreenMovingSpeed / 2);
                            newGameScreenMovingX = newGameScreenMovingX - titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.2f;
                            if (newGameScreenMovingX < 0)
                            {
                                newGameScreenMovingX = 0;
                            }
                        }
                        else
                        {
                            titleScreenMovingX = -SharedData.xRes;
                            newGameScreenMovingX = 0;
                            titleMovingToNewGame = false;
                            titleScreenMovingSpeed = 0;
                        }
                        renderTitleScreenDrawBase();
                        drawTitleInfo((int)titleScreenMovingX);
                        /* Draw the menu screen */
                        blitSurface(0,
                                    0,
                                    Coords.TITLE_SCREEN_MENU_X + (int)titleScreenMovingX,
                                    Coords.TITLE_SCREEN_MENU_Y,
                                    Coords.TITLE_SCREEN_MENU_WIDTH,
                                    Coords.TITLE_SCREEN_MENU_HEIGHT,
                                    titleMenu);
                        drawNewGameScreenExtras((int)newGameScreenMovingX);
                    }
                    else if (newGameMovingToTitle == true)
                    {
                        if (titleScreenMovingX + titleScreenMovingSpeed <= 0)
                        {
                            titleScreenMovingX = titleScreenMovingX + titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX + (int)(titleScreenMovingSpeed / 2);
                            newGameScreenMovingX = newGameScreenMovingX + titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.2f;
                        }
                        else
                        {
                            titleScreenMovingX = 0;
                            newGameScreenMovingX = SharedData.xRes;
                            newGameMovingToTitle = false;
                            titleScreenMovingSpeed = 0;
                            changeState((int)RenderState.TITLE_MENU_SCREEN, SharedData.renderState);
                        }
                        renderTitleScreenDrawBase();
                        drawTitleInfo((int)titleScreenMovingX);
                        drawNewGameScreenExtras((int)newGameScreenMovingX);
                    }
                    else if (newGameMovingToInGame == true)
                    {
                        if (newGameScreenMovingX - titleScreenMovingSpeed >= -SharedData.xRes)
                        {
                            if (titleScreenBackgroundMovingX > 0)
                            {
                                titleScreenBackgroundMovingX -= titleScreenBlankBG.Width;
                            }
                            newGameScreenMovingX = newGameScreenMovingX - titleScreenMovingSpeed;
                            titleScreenMovingX = titleScreenMovingX - titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX - (int)titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.1f;
                        }
                        else
                        {
                            newGameScreenMovingX = -SharedData.xRes;
                            newGameMovingToInGame = false;
                            titleScreenMovingSpeed = 0;
                            /* Perform one time state change processing */
                            changeState((int)RenderState.IN_GAME, SharedData.renderState);
                        }
                        /* Draw the menu screen */
                        drawInGameScreen();
                        blitSurface(0,
                                    0,
                                    (int)titleScreenBackgroundMovingX,
                                    0,
                                    titleScreenBlankBG.Width,
                                    titleScreenBlankBG.Height,
                                    titleScreenBlankBG);
                        blitSurface(0,
                                    0,
                                    (int)titleScreenBackgroundMovingX + titleScreenBlankBG.Width,
                                    0,
                                    (int)newGameScreenMovingX - titleScreenBackgroundMovingX,
                                    titleScreenBlankBG.Height,
                                    titleScreenBlankBG);
                        drawNewGameScreenExtras((int)newGameScreenMovingX);
                    }
                    else
                    {
                        /* Not moving, just draw the single screen */
                        renderTitleScreenDrawBase();
                        drawNewGameScreenExtras((int)newGameScreenMovingX);
                    }
                    break;
                case (int)(RenderState.OPTIONS_SCREEN):
                    /* Draw the stuff common to the title screens */
                    if (titleMovingToOptions == true)
                    {
                        if (titleScreenMovingX >= (-SharedData.xRes))
                        {
                            titleScreenMovingX = titleScreenMovingX - titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX - (int)(titleScreenMovingSpeed / 2);
                            optionScreenMovingX = optionScreenMovingX - titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.2f;
                            if (optionScreenMovingX < 0)
                            {
                                optionScreenMovingX = 0;
                            }
                        }
                        else
                        {
                            titleScreenMovingX = -SharedData.xRes;
                            optionScreenMovingX = 0;
                            titleMovingToOptions = false;
                            titleScreenMovingSpeed = 0;
                        }
                        renderTitleScreenDrawBase();
                        drawTitleInfo((int)titleScreenMovingX);
                        /* Draw the menu screen */
                        blitSurface(0,
                                    0,
                                    Coords.TITLE_SCREEN_MENU_X + (int)titleScreenMovingX,
                                    Coords.TITLE_SCREEN_MENU_Y,
                                    Coords.TITLE_SCREEN_MENU_WIDTH,
                                    Coords.TITLE_SCREEN_MENU_HEIGHT,
                                    titleMenu);
                        drawOptionsScreenExtras((int)optionScreenMovingX);
                    }
                    else if (optionsCancelMovingToTitle == true)
                    {
                        if (titleScreenMovingX + titleScreenMovingSpeed <=0)
                        {
                            titleScreenMovingX = titleScreenMovingX + titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX + (int)(titleScreenMovingSpeed / 2);
                            optionScreenMovingX = optionScreenMovingX + titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.2f;
                        }
                        else
                        {
                            titleScreenMovingX = 0;
                            optionScreenMovingX = SharedData.xRes;
                            optionsCancelMovingToTitle = false;
                            titleScreenMovingSpeed = 0;
                            changeState((int)RenderState.TITLE_MENU_SCREEN, SharedData.renderState);
                        }
                        renderTitleScreenDrawBase();
                        drawTitleInfo((int)titleScreenMovingX);
                        drawOptionsScreenExtras((int)optionScreenMovingX);
                    }
                    else if (optionsOkMovingToTitle == true)
                    {
                        if (titleScreenMovingX - titleScreenMovingSpeed >= 0)
                        {
                            titleScreenMovingX = titleScreenMovingX - titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX - (int)(titleScreenMovingSpeed / 2);
                            optionScreenMovingX = optionScreenMovingX - titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.2f;
                        }
                        else
                        {
                            titleScreenMovingX = 0;
                            optionScreenMovingX = -SharedData.xRes;
                            optionsOkMovingToTitle = false;
                            titleScreenMovingSpeed = 0;
                            changeState((int)RenderState.TITLE_MENU_SCREEN, SharedData.renderState);
                        }
                        renderTitleScreenDrawBase();
                        drawTitleInfo((int)titleScreenMovingX);
                        drawOptionsScreenExtras((int)optionScreenMovingX);
                    }
                    else
                    {
                        renderTitleScreenDrawBase();
                        /* Draw everything else */
                        drawOptionsScreenExtras(0);
                    }
                    break;
                case (int)(RenderState.IN_GAME):
                    drawInGameScreen();
                    drawGameStateSpecificDialogs();
                    if (inGameMovingToTitle == true)
                    {
                        if (titleScreenMovingX + titleScreenMovingSpeed <= 0)
                        {
                            titleScreenMovingX = titleScreenMovingX + titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.1f;
                        }
                        else
                        {
                            titleScreenMovingX = 0;
                            inGameMovingToTitle = false;
                            titleScreenMovingSpeed = 0;
                            SharedData.reset();
                            gameInstance.reset();
                            changeState((int)(RenderState.TITLE_MENU_SCREEN), SharedData.renderState);
                        }
                        blitSurface(0,
                                    0,
                                    (int)titleScreenMovingX,
                                    0,
                                    titleScreenBlankBG.Width,
                                    titleScreenBlankBG.Height,
                                    titleScreenBlankBG);
                        drawTitleInfo((int)titleScreenMovingX);
                        /* Draw the menu screen */
                    }
                    if (inGameMovingToFinished == true)
                    {
                        if (titleScreenMovingX - titleScreenMovingSpeed >= 0)
                        {
                            titleScreenMovingX = titleScreenMovingX - titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.1f;
                        }
                        else
                        {
                            titleScreenMovingX = 0;
                            inGameMovingToFinished = false;
                            titleScreenMovingSpeed = 0;
                            changeState((int)(RenderState.GAME_RESULTS_SCREEN), SharedData.renderState);
                        }
                        blitSurface(0,
                                    0,
                                    (int)titleScreenMovingX,
                                    0,
                                    titleScreenBlankBG.Width,
                                    titleScreenBlankBG.Height,
                                    titleScreenBlankBG);
                        drawResultsScreen((int)titleScreenMovingX);
                    }
                    break;
                case (int)(RenderState.GAME_RESULTS_SCREEN):
                    if (finishedMovingToTitle == true)
                    {
                        if (finishedMovingX - titleScreenMovingSpeed >= (-SharedData.xRes))
                        {
                            titleScreenMovingX = titleScreenMovingX - titleScreenMovingSpeed;
                            titleScreenBackgroundMovingX = titleScreenBackgroundMovingX - (int)(titleScreenMovingSpeed / 2);
                            finishedMovingX = finishedMovingX - titleScreenMovingSpeed;
                            titleScreenMovingSpeed = titleScreenMovingSpeed * 1.2f;
                        }
                        else
                        {
                            titleScreenMovingX = 0;                            
                            finishedMovingToTitle = false;
                            titleScreenMovingSpeed = 0;
                            /* Initialize everything (yes, this is a stupid place to do this!) */
                            SharedData.reset();
                            gameInstance.reset();
                            changeState((int)(RenderState.TITLE_MENU_SCREEN), SharedData.renderState);
                        }
                        renderTitleScreenDrawBase();
                        drawResultsScreen((int)finishedMovingX);
                        drawTitleInfo((int)titleScreenMovingX);
                        drawAnimatedTrains();
                    }
                    else
                    {
                        renderTitleScreenDrawBase();
                        drawResultsScreen(0);
                        drawAnimatedTrains();
                    }                    
                    break;
                case (int)(RenderState.END_GAME):
                default:
                    break;
            }
        }

        /****************************************************************************
        * updateLogic
        *
        * Description:
        *   Update any dynamic elements that rely on a particular render state. 
        *
        ****************************************************************************/
        public void updateLogic()
        {
            switch (SharedData.renderState)
            {
                case (int)(RenderState.COMPANY_LOGO):
                    break;
                case (int)(RenderState.TITLE_SCREEN):
                case (int)(RenderState.TITLE_MENU_SCREEN):
                case (int)(RenderState.NEW_GAME_SCREEN):
                case (int)(RenderState.GAME_RESULTS_SCREEN):
                case (int)(RenderState.OPTIONS_SCREEN):
                    if (topTrainDirection == TRAIN_DIR_LEFT)
                    {
                        topTrainX -= TRAIN_BANNER_SPEED;
                        if (topTrainX <= Coords.TRAIN_INITIAL_X)
                        {
                            topTrainDirection = TRAIN_DIR_RIGHT;
                        }
                    }
                    else
                    {
                        topTrainX += TRAIN_BANNER_SPEED;
                        if (topTrainX >= Coords.TRAIN_FINAL_X)
                        {
                            topTrainDirection = TRAIN_DIR_LEFT;
                        }
                    }
                    if (bottomTrainDirection == TRAIN_DIR_LEFT)
                    {
                        bottomTrainX -= TRAIN_BANNER_SPEED;
                        if (bottomTrainX <= Coords.TRAIN_INITIAL_X)
                        {
                            bottomTrainDirection = TRAIN_DIR_RIGHT;
                        }
                    }
                    else
                    {
                        bottomTrainX += TRAIN_BANNER_SPEED;
                        if (bottomTrainX >= Coords.TRAIN_FINAL_X)
                        {
                            bottomTrainDirection = TRAIN_DIR_LEFT;
                        }
                    }
                    break;
                case (int)(RenderState.IN_GAME):
                    switch (gameInstance.gameState)
                    {
                        case (int)(Game.GameState.COMPUTER_MOVE):
                            /* If we're in the middle of a CPU 'thinking' delay, don't proceed with the move */
                            if (SharedData.cpuMoveAllowed == false)
                            {
                                break;
                            }
                            SharedData.currentMove = ComputerAI.determineNextMove(gameInstance, null);
                            while (SharedData.currentMove.moveType != (int)(ComputerAI.Action.END_TURN))
                            {
                                System.Diagnostics.Debug.WriteLine("Processing AI move");
                                gameInstance.applyComputerMove();
                                SharedData.previousMove = SharedData.currentMove;
                                System.Diagnostics.Debug.WriteLine("Determining next move");
                                SharedData.currentMove = ComputerAI.determineNextMove(gameInstance, SharedData.previousMove);
                            }
                            gameInstance.changeState((int)(Game.GameState.END_TURN));
                            break;
                        default:
                            break;
                    }
                    break;
                case (int)(RenderState.END_GAME):
                default:
                    break;
            }

        }

        /****************************************************************************
        * processInput
        *
        * Description:
        *   Checks to see if important areas of the screen have been pressed, and
        *   take appropriate action accordingly.  
        *
        ****************************************************************************/
#if WINDOWS
        public void processInput()
#else
        public void processInput(TouchPanelCapabilities tcaps)
#endif
        {
            int xPos, yPos, counter;

            /* Only check controls if the touch screen is available */
#if WINDOWS

            MouseState m;

            m = Mouse.GetState();
            xPos = m.X;
            yPos = m.Y;

            if(acceptInput == true && m.LeftButton == ButtonState.Pressed)
#else
            if (tcaps.IsConnected)
            {
                /* Get the current touch state */
                TouchCollection t = TouchPanel.GetState();
                
                if (t.Count > 0 )
                {                    
                    /* Only use the first touch event (no multi-touch support) */
                    Vector2 translatedTouchLocation = new Vector2(t[0].Position.Y, (SharedData.yRes - t[0].Position.X));
                    xPos = (int)(translatedTouchLocation.X);
                    yPos = (int)(translatedTouchLocation.Y);

                    if (acceptInput == true && t[0].State == TouchLocationState.Pressed)
#endif

                    {
                        switch (SharedData.renderState)
                        {
                            /* A press automatically forces the company logo screen to change to the title screen */
                            case (int)(RenderState.COMPANY_LOGO):
                                changeState((int)RenderState.TITLE_SCREEN, SharedData.renderState);
                                break;
                            /* A press on the title screen brings up the menu. */
                            case (int)(RenderState.TITLE_SCREEN):
                                changeState((int)RenderState.TITLE_MENU_SCREEN, SharedData.renderState);
                                break;
                            case (int)(RenderState.TITLE_MENU_SCREEN):
                                /* New game */
                                if(isWithinBoundary(xPos, 
                                                    yPos, 
                                                    Coords.NEW_GAME_MIN_X, 
                                                    Coords.NEW_GAME_MIN_Y, 
                                                    Coords.NEW_GAME_MAX_X, 
                                                    Coords.NEW_GAME_MAX_Y) == true)
                                {
                                    changeState((int)RenderState.NEW_GAME_SCREEN, SharedData.renderState);
                                }
                                if(isWithinBoundary(xPos, 
                                                    yPos, 
                                                    Coords.OPTIONS_MIN_X, 
                                                    Coords.OPTIONS_MIN_Y, 
                                                    Coords.OPTIONS_MAX_X, 
                                                    Coords.OPTIONS_MAX_Y) == true)
                                {
                                    changeState((int)RenderState.OPTIONS_SCREEN, SharedData.renderState);
                                }
                                if(isWithinBoundary(xPos, 
                                                    yPos, 
                                                    Coords.EXIT_MIN_X, 
                                                    Coords.EXIT_MIN_Y, 
                                                    Coords.EXIT_MAX_X, 
                                                    Coords.EXIT_MAX_Y) == true)
                                {
                                    changeState((int)RenderState.END_GAME, SharedData.renderState);
                                }
                                break;
                            case (int)(RenderState.NEW_GAME_SCREEN):
                                /* The 'Start game' button */
                                if(isWithinBoundary(xPos, 
                                                    yPos, 
                                                    Coords.NEW_GAME_START_GAME_MIN_X, 
                                                    Coords.NEW_GAME_START_GAME_MIN_Y, 
                                                    Coords.NEW_GAME_START_GAME_MAX_X, 
                                                    Coords.NEW_GAME_START_GAME_MAX_Y) == true)
                                {
                                    optionScreenMovingX = 0;
                                    titleScreenMovingSpeed = 2.0f;
                                    newGameMovingToInGame = true;
                                    newGameMovingToTitle = false;
                                    gameInstance.initPlayersFromUi();
                                    if (titleScreenBackgroundMovingX < -SharedData.xRes)
                                    {
                                        titleScreenBackgroundMovingX += SharedData.xRes;
                                    }
                                }
                                if(isWithinBoundary(xPos, 
                                                    yPos,  
                                                    Coords.NEW_GAME_BACK_TO_TITLE_MIN_X,  
                                                    Coords.NEW_GAME_BACK_TO_TITLE_MIN_Y,  
                                                    Coords.NEW_GAME_BACK_TO_TITLE_MAX_X,  
                                                    Coords.NEW_GAME_BACK_TO_TITLE_MAX_Y) == true)
                                {
                                    titleScreenMovingX = -SharedData.xRes;
                                    newGameScreenMovingX = 0;
                                    titleScreenMovingSpeed = 2.0f;
                                    newGameMovingToTitle = true;
                                }
                                /* Check the 'number of players' option */
                                for (counter = 0; counter < (TrackInsanity.MAX_PLAYERS - 1); counter++)
                                {
                                    if(isWithinBoundary(xPos,
                                                        yPos,
                                                        Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 0], Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 1],                                                             
                                                        (Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 0] + Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 2]) + 1,                                                            
                                                        (Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 1] + Coords.NEW_GAME_NUMBER_OF_PLAYERS_OFFSETS[counter, 3]) + 1 ) == true)
                                    {
                                        SharedData.selectedPlayers = counter + 2;
                                    }
                                }
                                /* Check the 'human' versus 'CPU' player options for each player */
                                for (counter = 0; counter < SharedData.selectedPlayers; counter++)
                                {
                                    if (isWithinBoundary(xPos, 
                                                         yPos,
                                                         Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 0], Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 1],
                                                         (Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 0] + Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 2]) + 1,
                                                         (Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 1] + Coords.NEW_GAME_HUMAN_PLAYER_OFFSETS[counter, 3]) + 1) == true)
                                    {
                                        SharedData.playerState[counter] = Player.HUMAN;
                                    }
                                }
                                for (counter = 1; counter < SharedData.selectedPlayers; counter++)
                                {
                                    if (isWithinBoundary(xPos, 
                                                         yPos,
                                                         Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 0], Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 1],                                                             
                                                         (Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 0] + Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 2]) + 1,                                                           
                                                         (Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 1] + Coords.NEW_GAME_CPU_PLAYER_OFFSETS[counter, 3]) + 1) == true)
                                    {
                                        SharedData.playerState[counter] = Player.COMPUTER;
                                    }
                                }
                                break;
                            case (int)(RenderState.OPTIONS_SCREEN):
                                /* Was 'Back to title' pressed? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.NEW_GAME_BACK_TO_TITLE_MIN_X, 
                                                     Coords.NEW_GAME_BACK_TO_TITLE_MIN_Y, 
                                                     Coords.NEW_GAME_BACK_TO_TITLE_MAX_X, 
                                                     Coords.NEW_GAME_BACK_TO_TITLE_MAX_Y) == true)
                                {
                                    /* Restore the original 'good' settings to the temp values */
                                    gameInstance.tmpHighlightTracks = gameInstance.highlightTracks;
                                    gameInstance.tmpHighlightLegalMoves = gameInstance.highlightLegalMoves;
                                    gameInstance.tmpShowLastMove = gameInstance.showLastMove;
                                    gameInstance.tmpMusicVolume = gameInstance.musicVolume;
                                    gameInstance.tmpEffectsVolume = gameInstance.effectsVolume;
                                    gameInstance.tmpDefaultAILevel = gameInstance.defaultAILevel;
                                    titleScreenMovingX = -SharedData.xRes;
                                    optionScreenMovingX = 0;
                                    titleScreenMovingSpeed = 2.0f;
                                    optionsCancelMovingToTitle = true;
                                    optionsOkMovingToTitle = false;
                                    titleMovingToOptions = false;
                                }
                                
                                /* Was OK pressed?  If so, save the settings */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_OK_MIN_X,
                                                     Coords.OPTIONS_SCREEN_OK_MIN_Y, 
                                                     Coords.OPTIONS_SCREEN_OK_MAX_X, 
                                                     Coords.OPTIONS_SCREEN_OK_MAX_Y) == true)
                                {
                                    gameInstance.highlightTracks = gameInstance.tmpHighlightTracks;
                                    gameInstance.highlightLegalMoves = gameInstance.tmpHighlightLegalMoves;
                                    gameInstance.showLastMove = gameInstance.tmpShowLastMove;
                                    gameInstance.musicVolume = gameInstance.tmpMusicVolume;
                                    gameInstance.effectsVolume = gameInstance.tmpEffectsVolume;
                                    gameInstance.defaultAILevel = gameInstance.tmpDefaultAILevel;
                                    titleScreenMovingX = SharedData.xRes;
                                    optionScreenMovingX = 0;
                                    titleScreenMovingSpeed = 2.0f;
                                    optionsOkMovingToTitle = true;
                                    optionsCancelMovingToTitle = false;
                                    titleMovingToOptions = false;
                                }
                                
                                /* Was 'No' chosen for the 'Highlight Tracks' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 1],                                                         
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 2] + 1,                                                      
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[0, 3] + 1) == true)
                                {
                                    gameInstance.tmpHighlightTracks = Game.OPTION_NO;
                                }
                                
                                /* Was 'Yes' chosen for the 'Highlight Tracks' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos,
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 1],                                                          
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 2] + 1,                                                         
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_TRACKS_OFFSETS[1, 3] + 1) == true)
                                {
                                    gameInstance.tmpHighlightTracks = Game.OPTION_YES;
                                }

                                /* Was 'No' chosen for the 'Highlight legal moves' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 1],                                                          
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 2] + 1,                                                         
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[0, 3] + 1) == true)
                                {
                                    gameInstance.tmpHighlightLegalMoves = Game.OPTION_NO;
                                }

                                /* Was 'Yes' chosen for the 'Highlight Legal Moves' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 1],                                                         
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 2] + 1,                                                         
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LEGAL_OFFSETS[1, 3] + 1) == true)
                                {
                                    gameInstance.tmpHighlightLegalMoves = Game.OPTION_YES;
                                }

                                /* was 'No' chosen for the 'Highlight last move' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 1],                                                        
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 2] + 1,                                                        
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[0, 3] + 1) == true)
                                {
                                    gameInstance.tmpShowLastMove = Game.OPTION_NO;
                                }

                                /* Was 'Last player' chosen for the 'Highlight last move' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 1],                                                         
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 2] + 1,                                                        
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[1, 3] + 1) == true)
                                {
                                    gameInstance.tmpShowLastMove = Game.OPTION_LAST_PLAYER;
                                }

                                /* Was 'All players' chosen for the 'Highlight last move' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 0], 
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 1],                                                    
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 0] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 2] + 1,                                                     
                                                     Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 1] + Coords.OPTIONS_SCREEN_HIGHLIGHT_LAST_MOVE_OFFSETS[2, 3] + 1) == true)
                                {
                                    gameInstance.tmpShowLastMove = Game.OPTION_ALL_PLAYERS;
                                }

                                /* Was 'Easy' chosen for the 'Computer AI' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos,
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 0], 
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 1],                                                         
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 0] + Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 2] + 1,                                                        
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 1] + Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[0, 3] + 1) == true)
                                {
                                    gameInstance.tmpDefaultAILevel = Game.OPTION_AI_EASY;
                                }

                                /* Was 'normal' chosen for the 'Computer AI' option? */
                                if (isWithinBoundary(xPos, 
                                                     yPos,
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 0], 
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 1],                                                         
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 0] + Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 2] + 1,                                                       
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 1] + Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[1, 3] + 1) == true)
                                {
                                    gameInstance.tmpDefaultAILevel = Game.OPTION_AI_MEDIUM;
                                }

                                /* Was 'hard' chosen for the 'Computer AI' option */
                                if (isWithinBoundary(xPos, 
                                                     yPos,
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 0], 
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 1],                                                          
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 0] + Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 2] + 1,                                                         
                                                     Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 1] + Coords.OPTIONS_SCREEN_AI_LEVEL_OFFSETS[2, 3] + 1) == true)
                                {
                                    gameInstance.tmpDefaultAILevel = Game.OPTION_AI_HARD;
                                }

                                for (counter = 0; counter < 10; counter++)
                                {
                                    if (isWithinBoundary(xPos, 
                                                         yPos, 
                                                         Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[counter, 0],
                                                         Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[counter, 1],                                                              
                                                         Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[counter, 0] + Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[counter, 2] + 1,                                                            
                                                         Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[counter, 1] + Coords.OPTIONS_SCREEN_MUSIC_VOLUME_OFFSETS[counter, 3] + 1) == true)
                                    {
                                        gameInstance.tmpMusicVolume = counter;
                                    }

                                    if (isWithinBoundary(xPos, 
                                                         yPos, 
                                                         Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[counter, 0], 
                                                         Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[counter, 1],
                                                         Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[counter, 0] + Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[counter, 2] + 1,
                                                         Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[counter, 1] + Coords.OPTIONS_SCREEN_SFX_VOLUME_OFFSETS[counter, 3] + 1) == true)
                                    {
                                        gameInstance.tmpEffectsVolume = counter;
                                    }
                                }                                
                                break;
                            case (int)(RenderState.IN_GAME):
                                processInGameEvents(xPos, yPos);
                                break;
                            case (int)(RenderState.GAME_RESULTS_SCREEN):
                                if (isWithinBoundary(xPos, 
                                                     yPos, 
                                                     Coords.END_GAME_BACK_TO_TITLE_MIN_X, 
                                                     Coords.END_GAME_BACK_TO_TITLE_MIN_Y,                                                         
                                                     Coords.END_GAME_BACK_TO_TITLE_MAX_X,
                                                     Coords.END_GAME_BACK_TO_TITLE_MAX_Y) == true)
                                {
                                    finishedMovingToTitle = true;
                                    titleScreenMovingSpeed = 2.0f;
                                    titleScreenMovingX = SharedData.xRes;
                                    finishedMovingX = 0;
                                }
                                break;
                            default:
                                break;
                        }
                        acceptInput = false;
                    }
                    else if (acceptInput == false)
                    {                        
#if WINDOWS
                        if(m.LeftButton == ButtonState.Released)
#else
                        if (t[0].State == TouchLocationState.Released)
#endif
                        {
                            acceptInput = true;
                        }
                    }
#if WINDOWS
#else
                }
            }
#endif
        }

        /****************************************************************************
        * isWithinBoundary
        *
        * Description:
        *   Checks to see if a specified point falls within the specified boundary.   
        *
        ****************************************************************************/
        private static bool isWithinBoundary(int xPos, int yPos, int minX, int minY, int maxX, int maxY)
        {
            if (xPos >= minX && xPos <= maxX && yPos >= minY && yPos <= maxY)
            {
                return true;
            }
            return false;
        }

    }
}
