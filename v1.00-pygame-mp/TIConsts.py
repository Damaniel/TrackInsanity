#! /usr/bin/env python

#=============================================================================
#
# TIConsts.py - a whole bunch of constants representing positions
# on the screen and other things.  Coordinate values and image sizes are 
# specific to the GCW Zero version of
# the game.  
#
# The following coordinates contain absolute values which may vary depending
# on platform (and should be adjusted accordingly):
# 
#  GAME_XRES
#  GAME_YRES
#  TRAIN_BANNER_WIDTH
#  TRAIN_BANNER_HEIGHT
#  TRAIN_BANNER_TOP_Y
#
#=============================================================================

# The base game resolution
GAME_XRES = 320
GAME_YRES = 240

MIN_PLAYERS = 2
MAX_PLAYERS = 6

# Render states.  Used by the renderer to decide what to draw on the screen
# at any point in time.
# All of the 'title screen' displays (the title screen, options screen,
# new game screen, etc) are substates of the RENDER_STATE_TITLE_SCREEN
# state.  This is because that state requires the ability to render 
# parts of multiple display states at a time, and having a full set of
# 'full' states for those states would require each one have a full set
# of title screen assets that loads when the state is initialized.  
RENDER_STATE_NONE = 0
RENDER_STATE_COMPANY_LOGO = 1
RENDER_STATE_TITLE_SCREEN = 2
RENDER_STATE_IN_GAME = 3
RENDER_STATE_GAME_RESULTS_SCREEN = 4
RENDER_STATE_END_GAME = 5


# The speed (represented in percentage of alpha channel to add to total alpha
# per frame update) which the Holy Meatgoat logoo will be displayed on screen.
RENDER_LOGO_FADE_IN_SPEED = 10

# The amount of time (in milliseconds) to hold in certain states)
RENDER_LOGO_CLEAR_BEFORE_FADE_IN_TIME = 1000
RENDER_LOGO_WAIT_BEFORE_DROP_BG_TIME = 4000
RENDER_LOGO_WAIT_BEFORE_SCROLLING_TRACK_WORD_TIME = 500
RENDER_LOGO_WAIT_BEFORE_CHANGING_TO_TITLE_SCREEN_TIME = 500

#----------------------------------------------------
# Coordinates and sizes related to the title screen:
#----------------------------------------------------

# Positions of the words 'Track' and 'Insanity'
RENDER_TRACK_WORD_INITIAL_X = -279
RENDER_TRACK_WORD_FINAL_X = 4
RENDER_INSANITY_WORD_INITIAL_X = (GAME_XRES + 100)
RENDER_INSANITY_WORD_FINAL_X = 137
RENDER_WORDS_Y = 31

# Given a standard 30 frame per second update rate, the 
# 'PRESS START' message should be shown from frames 0 to 21
# and the message hidden from frames 22-29.
RENDER_PRESS_START_FRAME_SHOWN_TIME = 0
RENDER_PRESS_START_FRAME_HIDDEN_TIME = 22

# Size information for the train banner on the title screen
RENDER_TRAIN_BANNER_WIDTH = 264
RENDER_TRAIN_BANNER_HEIGHT = 11

# Start and end positions for the moving trains at the title screen.
# They both start and end 72 pixels off screen in their appropriate
# directions.
RENDER_TRAIN_BANNER_INITIAL_X = 0 - (RENDER_TRAIN_BANNER_WIDTH + 72)
RENDER_TRAIN_BANNER_FINAL_X   = (GAME_XRES + 72) 
RENDER_TRAIN_BANNER_TOP_Y     = 6
RENDER_TRAIN_BANNER_BOTTOM_Y  = (GAME_YRES - RENDER_TRAIN_BANNER_HEIGHT - RENDER_TRAIN_BANNER_TOP_Y) + 1    # 224 on GCW Zero

RENDER_TRAIN_DIR_LEFT = 0
RENDER_TRAIN_DIR_RIGHT = 1
RENDER_TRAIN_BANNER_SPEED = 2

RENDER_TITLE_COPYRIGHT_TEXT_X = 19
RENDER_TITLE_COPYRIGHT_TEXT_Y = 193

RENDER_TITLE_PRESS_START_X = 43
RENDER_TITLE_PRESS_START_Y = 130

RENDER_TITLE_OPTION_NEW_GAME_X = 101
RENDER_TITLE_OPTION_NEW_GAME_Y = 100
RENDER_TITLE_OPTION_OPTIONS_X = 117
RENDER_TITLE_OPTION_OPTIONS_Y = 127
RENDER_TITLE_OPTION_QUIT_X = 138
RENDER_TITLE_OPTION_QUIT_Y = 153

RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_LEFT_X = 86
RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_RIGHT_X = 225