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

# Identifiers for the options on the option screen.
OPTION_HIGHLIGHT_PLAYER_TRACKS = 0
OPTION_HIGHLIGHT_LEGAL_MOVES = 1
OPTION_SHOW_LAST_MOVE = 2
OPTION_COMPUTER_AI_LEVEL = 3
OPTION_MUSIC_VOLUME = 4
OPTION_EFFECTS_VOLUME = 5

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
# 'PRESS START' message should be shown from frames 0 to 20
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


RENDER_OPTION_BASE_POS = (0, 0)
RENDER_OPTION_TEXT_POS = ((14, 61), (14, 84), (14, 108), (13, 133), (13, 155), (13,178))
RENDER_OPTION_HPT_POS =  ((220, 61), (280, 61))
RENDER_OPTION_HLM_POS =  ((220, 84), (280, 84))
RENDER_OPTION_SLM_POS =  ((166, 110), (220, 110), (281, 110))
RENDER_OPTION_CAL_POS =  ((158, 134), (206, 131), (274, 131))
RENDER_OPTION_MV_POS =   ((142, 155), (158, 155), (173, 155), (191, 155), (208, 155), (225, 155), (243, 155), (261, 155), (276, 155), (294, 155))
RENDER_OPTION_EV_POS =   ((142, 178), (158, 178), (173, 178), (191, 178), (208, 178), (225, 178), (243, 178), (261, 178), (276, 178), (294, 178))

RENDER_OPTION_DIGIT_STRIP_POS = ((0, 0, 11, 15), \
                                 (11, 0, 9, 15), \
							     (20, 0, 12, 15), \
								 (32, 0, 12, 15), \
								 (44, 0, 11, 15), \
								 (55, 0, 12, 15), \
								 (67, 0, 12, 15), \
								 (79, 0, 9, 15), \
								 (88, 0, 11, 15), \
								 (99, 0, 12, 15))
									   
RENDER_OPTION_OK_POS = (99, 203)
RENDER_OPTION_CANCEL_POS = (188, 203)

# Displayable options on the 'New Game' (local mode) screen
RENDER_NEW_GAME_TITLE_POS = (101, 23)
RENDER_NEW_GAME_NUM_PLAYERS_POS = (22, 54)
RENDER_NEW_GAME_PLAYERS_DIGITS_POS = ((183, 55), (207, 55), (232, 55), (256, 55), (281, 55))
RENDER_NEW_GAME_AVATAR_BOX_POS = ((18, 79), (18, 111), (18, 143), (175, 79), (175, 111), (175, 143))
RENDER_NEW_GAME_AVATAR_HI_POS = ((15, 76), (15, 108), (15, 140), (172, 76), (172, 108), (172, 140))
RENDER_NEW_GAME_HUMAN_POS = ((53, 81), (53, 113), (53, 145), (210, 81), (210, 113), (210, 145))
RENDER_NEW_GAME_CPU_POS = ((120, 81), (120, 113), (120, 145), (277, 81), (277, 113), (277, 145))
RENDER_NEW_GAME_INSTR_POS = (9, 175)
RENDER_NEW_GAME_BACK_POS = (190, 204)
RENDER_NEW_GAME_BEGIN_POS = (45, 203)

RENDER_NEW_GAME_AVATAR_BOX_STRIP_POS = ((0, 0, 18, 18), \
										(18, 0, 18, 18), \
										(36, 0, 18, 18), \
										(54, 0, 18, 18), \
										(72, 0, 18, 18), \
										(90, 0, 18, 18), \
										(108, 0, 18, 18))