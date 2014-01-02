#! /usr/bin/env python

import pygame, sys, random
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender
from TISharedData import *
from TIPlayer import Player

class TitleScreenStateRender(StateRender):
	
	def __init__(self, w):
		self.windowSurfaceObj = w
		self.dirtyRects = []
		self.targetState = None
		
		# A frame counter used to help decide when to display things on screen
		# (like some flashing text, which only displays for 22 out of every 30 frames)
		self.frameCount = 0	
		
		# Variables controlling the train positionss.
		self.topTrainX = 0
		self.topTrainDirection = 0
		self.bottomTrainX = 0
		self.bottomTrainDirection = 0		

		# The option on the title menu that the user has selected.
		self.titleMenuOptionSelected = 0
	
		# The option in the option menu that is currently highlighted
		self.optionMenuOptionSelected = 0

		# Is the OK option selected in the options menu?  This is only 
		# visible if the cursor is on the bottom row, and is decided whether
		# to highlight OK or Cancel.
		self.optionMenuOkSelected = True
		
		# The total number of options on the options screen.
		self.NUM_OPTIONS = 6	

		# The number of possible selections for each option on the options screen,
		# (from top to bottom)
		self.CHOICES_PER_OPTION = [2, 2, 3, 3, 10, 10]
		
		# The currently selected option (0 is leftmost) for each of the options
		# covered in CHOICES_PER_OPTION.  These are the absolute defaults, which
		# will be overriden (eventually) with settings from an external config file.
		# self.defaultOption holds a clean copy of the defaults, so they can be
		# restored after cancelling (selectedOption holds a live copy of the values
		# as they are being updated by the user).
		self.selectedOption = [0, 0, 1, 1, 9, 9]
		self.defaultOption  = [0, 0, 1, 1, 9, 9]
		
		# The number of players selected on the 'New Game' screen
		self.newGameSelectedPlayers = 2
		
		self.newGameOptionSelected = 0
		
		# When the last row of the new game menu is selected, this flag determines
		# which of the two options is highlighted
		self.newGameBeginSelected = True
		
		# Used to decide whether the 'Human' or 'CPU' option is selected for each player
		self.isHumanSelected = [True, False, False, False, False, False]
		
		# Default avatar indices for each player.  There are 256 total avatars (0-255)
		self.playerAvatars = [0, 1, 2, 3, 4, 5]
		
		#
		# These substates are used to determine what components of the title/option screen to show
		# or collect input for.  These are:
		#
		# Title screen: The game name, with blinking 'Press START to play'
		# Title menu screen: The game name, with 'New Game / Options / Quit' menu
		# Title new game screen: The options to start a new (local) game
		# Title options screen: A screen containing various user selectable options
		# Title new MP game screen: The multiplayer (network) game selector/lobby.
		#
		self.SUBSTATE_TITLE_SCREEN = 0
		self.SUBSTATE_TITLE_MENU_SCREEN = 1
		self.SUBSTATE_TITLE_NEW_GAME_SCREEN = 2
		self.SUBSTATE_TITLE_OPTIONS_SCREEN = 3
		self.SUBSTATE_TITLE_NEW_MP_GAME_SCREEN = 4		
		
		# Values for the options on the main menu (New game, options, quit)
		self.TITLE_MENU_OPTION_NEW_GAME = 0
		self.TITLE_MENU_OPTION_OPTIONS = 1
		self.TITLE_MENU_OPTION_QUIT = 2
		self.NUM_TITLE_MENU_OPTIONS = 3
		
		#
		# Used to figure out which parts of the title screen should be shown on the
		# screen (menu options and the like)
		#		
		self.renderSubstate = self.SUBSTATE_TITLE_SCREEN
		self.prevRenderSubstate = None
		
		self.loadAssets()
		self.initRenderState()
		
	#
	# Renders the 'common' components of the title screen - the background,
	# and the moving trains.  Eventually, this function may draw the background
	# image up to two times, depending on if the background is scrolling or not.
	#
	# Since the background image takes up the entire screen and is updated every
	# frame, the only dirty rectangle generated in this class can be found in this
	# function (and it consumes the entire screen)
	#
	def renderTitleScreenDrawBase(self):
		r = self.windowSurfaceObj.blit(self.emptyTitleBG, (0, 0))
		self.dirtyRects.append(r)
		self.drawAnimatedTrains()
		
	def drawAnimatedTrains(self):
		# Since the whole frame is being updated on the title screen, no need to include this region
		# in the dirty rect list of areas to render.
		self.windowSurfaceObj.blit(self.trainBannerTop, (self.topTrainX, RENDER_TRAIN_BANNER_TOP_Y))
		self.windowSurfaceObj.blit(self.trainBannerBottom, (self.bottomTrainX, RENDER_TRAIN_BANNER_BOTTOM_Y))
		return
		
	def drawTitleInfo(self):
		self.windowSurfaceObj.blit(self.trackWord, (RENDER_TRACK_WORD_FINAL_X, RENDER_WORDS_Y))
		self.windowSurfaceObj.blit(self.insanityWord, (RENDER_INSANITY_WORD_FINAL_X, RENDER_WORDS_Y))
		self.windowSurfaceObj.blit(self.copyrightText, (RENDER_TITLE_COPYRIGHT_TEXT_X, RENDER_TITLE_COPYRIGHT_TEXT_Y))
		
	def drawNewGameScreen(self):
		# Draw the header and instructions, since these are always fixed:
		self.windowSurfaceObj.blit(self.newGameHeader, RENDER_NEW_GAME_TITLE_POS)
		self.windowSurfaceObj.blit(self.newGameInstructions, RENDER_NEW_GAME_INSTR_POS)
		
		# Depending on whether # of players is highlighed, draw the normal or highlighted version of the 
		# text
		if self.newGameOptionSelected == 0:
			self.windowSurfaceObj.blit(self.numPlayersHighlight, RENDER_NEW_GAME_NUM_PLAYERS_POS)
		else:
			self.windowSurfaceObj.blit(self.numPlayers, RENDER_NEW_GAME_NUM_PLAYERS_POS)
			
		# Draw the options (2 - 6 players), highlighting the correct number
		for i in range(2, MAX_PLAYERS + 1):
			if self.newGameSelectedPlayers == i:
				self.windowSurfaceObj.blit(self.digitsHighlight, RENDER_NEW_GAME_PLAYERS_DIGITS_POS[i-2], RENDER_OPTION_DIGIT_STRIP_POS[i])
			else:
				self.windowSurfaceObj.blit(self.digits, RENDER_NEW_GAME_PLAYERS_DIGITS_POS[i-2], RENDER_OPTION_DIGIT_STRIP_POS[i])
		
		# Draw the correct number of avatar boxes and accompanying option text.
		# Draw the correct avatars, too.
		for i in range(0, self.newGameSelectedPlayers):
			avatarX = 16 * (self.playerAvatars[i] - (int(self.playerAvatars[i] / 16) * 16))
			avatarY = 16 * int(self.playerAvatars[i] / 16)
			self.windowSurfaceObj.blit(self.avatarBoxes, RENDER_NEW_GAME_AVATAR_BOX_POS[i], RENDER_AVATAR_BOX_STRIP_POS[i])
			self.windowSurfaceObj.blit(self.avatars, (RENDER_NEW_GAME_AVATAR_BOX_POS[i][0] + 1, RENDER_NEW_GAME_AVATAR_BOX_POS[i][1] + 1), (avatarX, avatarY, 16, 16))
			
			if self.isHumanSelected[i] == True:
				self.windowSurfaceObj.blit(self.humanHighlight, RENDER_NEW_GAME_HUMAN_POS[i])
				self.windowSurfaceObj.blit(self.cpu, RENDER_NEW_GAME_CPU_POS[i])
			else:
				self.windowSurfaceObj.blit(self.human, RENDER_NEW_GAME_HUMAN_POS[i])
				self.windowSurfaceObj.blit(self.cpuHighlight, RENDER_NEW_GAME_CPU_POS[i])
				
		# Draw a highlight around the appropriate player, if one of those is selected
		if self.newGameOptionSelected >= 1 and self.newGameOptionSelected < 1 + self.newGameSelectedPlayers:
			self.windowSurfaceObj.blit(self.avatarHighlight, RENDER_NEW_GAME_AVATAR_HI_POS[self.newGameOptionSelected - 1])
			
		# Finally, draw the back to title / begin game options (highlighted, if selected)
		if self.newGameOptionSelected == 1 + self.newGameSelectedPlayers:
			if self.newGameBeginSelected == True:
				self.windowSurfaceObj.blit(self.beginGameHighlight, RENDER_NEW_GAME_BEGIN_POS)
				self.windowSurfaceObj.blit(self.backToTitle, RENDER_NEW_GAME_BACK_POS)
			else:
				self.windowSurfaceObj.blit(self.beginGame, RENDER_NEW_GAME_BEGIN_POS)
				self.windowSurfaceObj.blit(self.backToTitleHighlight, RENDER_NEW_GAME_BACK_POS)
		else:
			self.windowSurfaceObj.blit(self.beginGame, RENDER_NEW_GAME_BEGIN_POS)
			self.windowSurfaceObj.blit(self.backToTitle, RENDER_NEW_GAME_BACK_POS)
		
	def drawOptionScreen(self):
		# Draw the base object
		self.windowSurfaceObj.blit(self.optionBase, RENDER_OPTION_BASE_POS)
		
		#
		# Check to see which option is highlighted, and draw the highlight over that option.
		# The first NUM_OPTIONS choices control highlights for the menu options.  The last value
		# (NUM_OPTIONS) is special, and allows the control of the 'OK/Cancel' buttons.
		#
		if self.optionMenuOptionSelected >=0 and self.optionMenuOptionSelected < self.NUM_OPTIONS:
			self.windowSurfaceObj.blit(self.optionNames[self.optionMenuOptionSelected], RENDER_OPTION_TEXT_POS[self.optionMenuOptionSelected])
		elif self.optionMenuOptionSelected == self.NUM_OPTIONS:
			if self.optionMenuOkSelected == True:
				self.windowSurfaceObj.blit(self.okHighlight, RENDER_OPTION_OK_POS)
			else:
				self.windowSurfaceObj.blit(self.cancelHighlight, RENDER_OPTION_CANCEL_POS)
				
		# For each of the options, check to see which value is highlighted, and highlight only that value. 
		self.windowSurfaceObj.blit(self.optionHighlights[0][self.selectedOption[0]], RENDER_OPTION_HPT_POS[self.selectedOption[0]])
		self.windowSurfaceObj.blit(self.optionHighlights[1][self.selectedOption[1]], RENDER_OPTION_HLM_POS[self.selectedOption[1]])
		self.windowSurfaceObj.blit(self.optionHighlights[2][self.selectedOption[2]], RENDER_OPTION_SLM_POS[self.selectedOption[2]])
		self.windowSurfaceObj.blit(self.optionHighlights[3][self.selectedOption[3]], RENDER_OPTION_CAL_POS[self.selectedOption[3]])
		
		# Do the same for the last two options (which need to do a lookup into a couple of offset tables since the
		# digits are all stored together
		self.windowSurfaceObj.blit(self.digitsHighlight, RENDER_OPTION_MV_POS[self.selectedOption[4]], RENDER_OPTION_DIGIT_STRIP_POS[self.selectedOption[4]])
		self.windowSurfaceObj.blit(self.digitsHighlight, RENDER_OPTION_EV_POS[self.selectedOption[5]], RENDER_OPTION_DIGIT_STRIP_POS[self.selectedOption[5]])
		
	def loadAssets(self):
		# Company logo and title screen assets
		self.emptyTitleBG = pygame.image.load('res/title/titleblank.png').convert()
		self.trackWord = pygame.image.load('res/title/trackword.png').convert_alpha()
		self.insanityWord = pygame.image.load('res/title/insanityword.png').convert_alpha()
		self.copyrightText = pygame.image.load('res/title/copyright.png').convert_alpha()
		self.trainBannerTop = pygame.image.load('res/title/trainbannertop.png').convert()
		self.trainBannerBottom = pygame.image.load('res/title/trainbannerbottom.png').convert()
		self.pressStartToPlay = pygame.image.load('res/title/pressstart.png').convert_alpha()
		
		# Title screen menu text and highlight arrows
		self.newGameText = (pygame.image.load('res/title/newgame.png').convert_alpha(), \
		                    pygame.image.load('res/title/newgameHighlight.png').convert_alpha())
		self.optionsText = (pygame.image.load('res/title/options.png').convert_alpha(), \
							pygame.image.load('res/title/optionsHighlight.png').convert_alpha())
		self.quitText    = (pygame.image.load('res/title/quit.png').convert_alpha(), \
		                    pygame.image.load('res/title/quitHighlight.png').convert_alpha())
		self.highlightArrows = (pygame.image.load('res/title/arrowLeft.png').convert_alpha(), \
								pygame.image.load('res/title/arrowRight.png').convert_alpha())

		# Digits (used in a few places)
		self.digits = pygame.image.load('res/title/digits.png').convert_alpha()		
		self.digitsHighlight = pygame.image.load('res/title/digitsHighlight.png').convert_alpha()
								
		# Option screen components
		self.optionBase = pygame.image.load('res/title/options/textBase.png').convert_alpha()
		
		# Highlighted option names
		self.optionNames = (pygame.image.load('res/title/options/oHPTHighlight.png').convert_alpha(), \
						    pygame.image.load('res/title/options/oHLMHighlight.png').convert_alpha(), \
						    pygame.image.load('res/title/options/oSLMHighlight.png').convert_alpha(), \
						    pygame.image.load('res/title/options/oCALHighlight.png').convert_alpha(), \
						    pygame.image.load('res/title/options/oMVHighlight.png').convert_alpha(), \
						    pygame.image.load('res/title/options/oEVHighlight.png').convert_alpha())
								   
		self.yesHighlight = pygame.image.load('res/title/options/yesHighlight.png').convert_alpha()
		self.noHighlight = pygame.image.load('res/title/options/noHighlight.png').convert_alpha()
		self.oneHighlight = pygame.image.load('res/title/options/oneHighlight.png').convert_alpha()
		self.allHighlight = pygame.image.load('res/title/options/allHighlight.png').convert_alpha()
		self.easyHighlight = pygame.image.load('res/title/options/easyHighlight.png').convert_alpha()
		self.mediumHighlight = pygame.image.load('res/title/options/mediumHighlight.png').convert_alpha()
		self.hardHighlight = pygame.image.load('res/title/options/hardHighlight.png').convert_alpha()
	
		self.optionHighlights = ((self.yesHighlight, self.noHighlight), \
								 (self.yesHighlight, self.noHighlight), \
								 (self.noHighlight, self.oneHighlight, self.allHighlight), \
								 (self.easyHighlight, self.mediumHighlight, self.hardHighlight))
		
		self.okHighlight = pygame.image.load('res/title/options/okHighlight.png').convert_alpha()
		self.cancelHighlight = pygame.image.load('res/title/options/cancelHighlight.png').convert_alpha()
		
		self.newGameHeader = pygame.image.load('res/title/newGame/newGameHeader.png').convert_alpha()
		self.newGameInstructions = pygame.image.load('res/title/newGame/instructions.png').convert_alpha()
		
		self.numPlayers = pygame.image.load('res/title/newGame/numPlayers.png').convert_alpha()
		self.numPlayersHighlight = pygame.image.load('res/title/newGame/numPlayersHighlight.png').convert_alpha()
		
		self.beginGame = pygame.image.load('res/title/newGame/beginGame.png').convert_alpha()
		self.beginGameHighlight = pygame.image.load('res/title/newGame/beginGameHighlight.png').convert_alpha()
		
		self.backToTitle = pygame.image.load('res/title/newGame/backToTitle.png').convert_alpha()
		self.backToTitleHighlight = pygame.image.load('res/title/newGame/backToTitleHighlight.png').convert_alpha()
		
		self.avatarBoxes = pygame.image.load('res/common/avatarBoxes.png').convert()
		self.avatarHighlight = pygame.image.load('res/common/avatarHighlight.png').convert_alpha()
		
		# The avatars themselves
		self.avatars = pygame.image.load('res/common/avatars.png').convert_alpha()
		
		self.human = pygame.image.load('res/title/newGame/human.png').convert_alpha()
		self.humanHighlight = pygame.image.load('res/title/newGame/humanHighlight.png').convert_alpha()
		self.cpu = pygame.image.load('res/title/newGame/cpu.png').convert_alpha()
		self.cpuHighlight = pygame.image.load('res/title/newGame/cpuHighlight.png').convert_alpha()
		
		# Load the music for the title screen
		#pygame.mixer.music.load('res/music/title.ogg')
		
	def initRenderState(self):
		self.topTrainX = RENDER_TRAIN_BANNER_INITIAL_X
		self.topTrainDirection = RENDER_TRAIN_DIR_RIGHT
		self.bottomTrainX = RENDER_TRAIN_BANNER_FINAL_X
		self.bottomTrainY = RENDER_TRAIN_DIR_LEFT
		self.frameCount = 0
		# Set the music volume and play the background song on loop.
		#pygame.mixer.music.set_volume(1.0)
		#pygame.mixer.music.play(-1)
		return	
		
	def updateLogic(self):
		# Move the trains this frame. 
		if self.topTrainDirection == RENDER_TRAIN_DIR_LEFT:
			self.topTrainX -= RENDER_TRAIN_BANNER_SPEED
			if self.topTrainX <= RENDER_TRAIN_BANNER_INITIAL_X:
				self.topTrainDirection = RENDER_TRAIN_DIR_RIGHT
		else:
			self.topTrainX += RENDER_TRAIN_BANNER_SPEED
			if self.topTrainX >= RENDER_TRAIN_BANNER_FINAL_X:
				self.topTrainDirection = RENDER_TRAIN_DIR_LEFT
		if self.bottomTrainDirection == RENDER_TRAIN_DIR_LEFT:
			self.bottomTrainX -= RENDER_TRAIN_BANNER_SPEED
			if self.bottomTrainX <= RENDER_TRAIN_BANNER_INITIAL_X:
				self.bottomTrainDirection = RENDER_TRAIN_DIR_RIGHT
		else:
			self.bottomTrainX += RENDER_TRAIN_BANNER_SPEED
			if self.bottomTrainX >= RENDER_TRAIN_BANNER_FINAL_X:
				self.bottomTrainDirection = RENDER_TRAIN_DIR_LEFT	

	def checkTimingConditions(self, ticks):
		return
		
	def renderScreen(self):
		self.frameCount += 1
		
		# Draw the components common to all substates.
		self.renderTitleScreenDrawBase()
		
		if self.renderSubstate == self.SUBSTATE_TITLE_SCREEN or self.renderSubstate == self.SUBSTATE_TITLE_MENU_SCREEN:
			# If on the title screen or the title menu, display the game name and copyright information.
			self.drawTitleInfo()
		
		if self.renderSubstate == self.SUBSTATE_TITLE_SCREEN:
			# On the title screen *only*, display a 'Press Start' message that flashes (by only being shown 75% of the frames)
			if (self.frameCount % 30) >= RENDER_PRESS_START_FRAME_SHOWN_TIME and (self.frameCount % 30) < RENDER_PRESS_START_FRAME_HIDDEN_TIME:
				self.windowSurfaceObj.blit(self.pressStartToPlay, (RENDER_TITLE_PRESS_START_X, RENDER_TITLE_PRESS_START_Y))
				
		# Display the title menu when in the title menu substate.  Highlight the particlar option that is currently selected
		# (as defined by titleMenuOptionSelected)
		if self.renderSubstate == self.SUBSTATE_TITLE_MENU_SCREEN:
			if self.titleMenuOptionSelected == 0:
				self.windowSurfaceObj.blit(self.newGameText[1], (RENDER_TITLE_OPTION_NEW_GAME_X, RENDER_TITLE_OPTION_NEW_GAME_Y))
				self.windowSurfaceObj.blit(self.highlightArrows[0], (RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_LEFT_X, RENDER_TITLE_OPTION_NEW_GAME_Y + 4))
				self.windowSurfaceObj.blit(self.highlightArrows[1], (RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_RIGHT_X, RENDER_TITLE_OPTION_NEW_GAME_Y + 4))				
			else:
				self.windowSurfaceObj.blit(self.newGameText[0], (RENDER_TITLE_OPTION_NEW_GAME_X, RENDER_TITLE_OPTION_NEW_GAME_Y))
			
			if self.titleMenuOptionSelected == 1:
				self.windowSurfaceObj.blit(self.optionsText[1], (RENDER_TITLE_OPTION_OPTIONS_X, RENDER_TITLE_OPTION_OPTIONS_Y))
				self.windowSurfaceObj.blit(self.highlightArrows[0], (RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_LEFT_X, RENDER_TITLE_OPTION_OPTIONS_Y + 4))
				self.windowSurfaceObj.blit(self.highlightArrows[1], (RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_RIGHT_X, RENDER_TITLE_OPTION_OPTIONS_Y + 4))						
			else:
				self.windowSurfaceObj.blit(self.optionsText[0], (RENDER_TITLE_OPTION_OPTIONS_X, RENDER_TITLE_OPTION_OPTIONS_Y))
				
			if self.titleMenuOptionSelected == 2:
				self.windowSurfaceObj.blit(self.quitText[1], (RENDER_TITLE_OPTION_QUIT_X, RENDER_TITLE_OPTION_QUIT_Y))
				self.windowSurfaceObj.blit(self.highlightArrows[0], (RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_LEFT_X, RENDER_TITLE_OPTION_QUIT_Y + 4))
				self.windowSurfaceObj.blit(self.highlightArrows[1], (RENDER_TITLE_OPTION_HIGHLIGHT_ARROW_RIGHT_X, RENDER_TITLE_OPTION_QUIT_Y + 4))						
			else:
				self.windowSurfaceObj.blit(self.quitText[0], (RENDER_TITLE_OPTION_QUIT_X, RENDER_TITLE_OPTION_QUIT_Y))
				
		if self.renderSubstate == self.SUBSTATE_TITLE_OPTIONS_SCREEN:
			self.drawOptionScreen()
		
		if self.renderSubstate == self.SUBSTATE_TITLE_NEW_GAME_SCREEN:
			self.drawNewGameScreen()
			
		return
		
	def processInputs(self):	
		for event in pygame.event.get():		
			# If the window close button was clicked (in Windows)
			if event.type == QUIT:
				pygame.quit()
				sys.exit()				
			# A key was pressed (or a joystick button was pressed):
			elif event.type == KEYDOWN:			
				# On the title screen, the following keys work:
				#    Enter, A, Start - display the title menu
				if self.renderSubstate == self.SUBSTATE_TITLE_SCREEN:					
					if event.key in (K_RETURN, K_LCTRL):
						self.prevRenderSubstate = self.SUBSTATE_TITLE_SCREEN
						self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN	
				# On the title menu screen, the following keys work:
				#    Up, Down - Move up or down the list of menu items
				#    Enter, A, Start - selects the currently highlighted item.
				#                      (either going to new game, options, or quitting)
				elif self.renderSubstate == self.SUBSTATE_TITLE_MENU_SCREEN:
					if event.key == K_DOWN:
						self.titleMenuOptionSelected += 1
						if self.titleMenuOptionSelected > self.NUM_TITLE_MENU_OPTIONS:
							self.titleMenuOptionSelected = 0
					elif event.key == K_UP:
						self.titleMenuOptionSelected -= 1
						if self.titleMenuOptionSelected < 0:
							self.titleMenuOptionSelected = self.NUM_TITLE_MENU_OPTIONS - 1
					elif event.key in (K_RETURN, K_LCTRL):
						if self.titleMenuOptionSelected == self.TITLE_MENU_OPTION_NEW_GAME:
							self.prevRenderSubstate = self.renderSubstate
							self.renderSubstate = self.SUBSTATE_TITLE_NEW_GAME_SCREEN
						elif self.titleMenuOptionSelected == self.TITLE_MENU_OPTION_OPTIONS:
							self.prevRenderSubstate = self.renderSubstate
							self.renderSubstate = self.SUBSTATE_TITLE_OPTIONS_SCREEN
						elif self.titleMenuOptionSelected == self.TITLE_MENU_OPTION_QUIT:
							pygame.event.post(pygame.event.Event(QUIT))
				# On the new game screen, the following keys work:
				#    Up, Down - Move through the (up to 6) players, or 'Cancel/Begin'
				#    Left, Right - pick CPU or Human, or 'Cancel/Begin'
				#    L, R - select the previous or next avatar for the highlighted player
				#    X - selected a random avatar for the highlighted player
				#    Enter, A, Start - Selectes Cancel or Begin, if highlighted.
				elif self.renderSubstate == self.SUBSTATE_TITLE_NEW_GAME_SCREEN:
					if event.key == K_DOWN:
						self.newGameOptionSelected += 1						
						if self.newGameOptionSelected > 1 + self.newGameSelectedPlayers:
							self.newGameOptionSelected = 0
					elif event.key == K_UP:
						self.newGameOptionSelected -= 1					
						if self.newGameOptionSelected < 0:
							self.newGameOptionSelected = 1 + self.newGameSelectedPlayers
					elif event.key == K_RIGHT:
						if self.newGameOptionSelected == 0:
							self.newGameSelectedPlayers += 1
							if self.newGameSelectedPlayers > 6:
								self.newGameSelectedPlayers = 2
						elif self.newGameOptionSelected >=1 and self.newGameOptionSelected < 1 + self.newGameSelectedPlayers:
							if self.isHumanSelected[self.newGameOptionSelected - 1] == True:
								self.isHumanSelected[self.newGameOptionSelected - 1] = False
							else:
								self.isHumanSelected[self.newGameOptionSelected - 1] = True
						elif self.newGameOptionSelected >= 1 + self.newGameSelectedPlayers:
							if self.newGameBeginSelected == True:
								self.newGameBeginSelected = False
							else:
								self.newGameBeginSelected = True
					elif event.key == K_LEFT:
						if self.newGameOptionSelected == 0:
							self.newGameSelectedPlayers -= 1
							if self.newGameSelectedPlayers < 2:
								self.newGameSelectedPlayers = 6
						elif self.newGameOptionSelected >=1 and self.newGameOptionSelected < 1 + self.newGameSelectedPlayers:
							if self.isHumanSelected[self.newGameOptionSelected - 1] == True:
								self.isHumanSelected[self.newGameOptionSelected - 1] = False
							else:
								self.isHumanSelected[self.newGameOptionSelected - 1] = True
						elif self.newGameOptionSelected >= 1 + self.newGameSelectedPlayers:
							if self.newGameBeginSelected == True:
								self.newGameBeginSelected = False
							else:
								self.newGameBeginSelected = True
					elif event.key in (K_x, K_SPACE):
						if self.newGameOptionSelected >=1 and self.newGameOptionSelected < 1 + self.newGameSelectedPlayers:
							self.playerAvatars[self.newGameOptionSelected - 1] = random.randint(0, 255)
					elif event.key in (K_l, K_TAB):
						if self.newGameOptionSelected >=1 and self.newGameOptionSelected < 1 + self.newGameSelectedPlayers:
							self.playerAvatars[self.newGameOptionSelected - 1] -= 1
							if self.playerAvatars[self.newGameOptionSelected - 1] < 0:
								self.playerAvatars[self.newGameOptionSelected - 1] = 255
					elif event.key in (K_r, K_BACKSPACE):
						if self.newGameOptionSelected >=1 and self.newGameOptionSelected < 1 + self.newGameSelectedPlayers:
							self.playerAvatars[self.newGameOptionSelected - 1] += 1
							if self.playerAvatars[self.newGameOptionSelected - 1] > 255:
								self.playerAvatars[self.newGameOptionSelected - 1] = 0				
					elif event.key in (K_RETURN, K_LCTRL):
						if self.newGameOptionSelected == 1 + self.newGameSelectedPlayers:
							if self.newGameBeginSelected == True:
								# Populate the shared data structure with values set by the user, 
								# then change the target state to 'begin game'.  The game instance
								# created in that state will set all values as necessary.
								SharedData.selectedPlayers = self.newGameSelectedPlayers
								SharedData.playerState = []
								for i in range(0, self.newGameSelectedPlayers):
									if self.isHumanSelected[i] == True:
										SharedData.playerState.append(Player.HUMAN)					
									else:
										SharedData.playerState.append(Player.COMPUTER)
								SharedData.playerAvatars = []
								for i in range(0, self.newGameSelectedPlayers):
									SharedData.playerAvatars.append(self.playerAvatars[i])
								SharedData.selectedOption = self.selectedOption								
								self.targetState = RENDER_STATE_IN_GAME
							else:
								self.prevRenderSubstate = self.renderSubstate
								self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN
				# On the options screen, the following keys work:
				#    Up, Down - Move up or down the list of options
				#    Left, Right - select between choices for the highlighted option
				#    Enter, A, Start - Selects OK or Cancel, if highlighted.  
				elif self.renderSubstate == self.SUBSTATE_TITLE_OPTIONS_SCREEN:
					if event.key == K_DOWN:
						# Move the selection down one, wrapping around if necessary.
						self.optionMenuOptionSelected += 1
						if self.optionMenuOptionSelected > self.NUM_OPTIONS:
							self.optionMenuOptionSelected = 0
					elif event.key == K_UP:
						# Move the selection up one, wrapping around if necessary
						self.optionMenuOptionSelected -= 1
						if self.optionMenuOptionSelected < 0:
							self.optionMenuOptionSelected = self.NUM_OPTIONS
					elif event.key == K_RIGHT:
						# Move the selection one to the right, wrapping around if necessary.
						# (The OK/Cancel option is a separate case, in the else clause)
						if self.optionMenuOptionSelected != self.NUM_OPTIONS:
							self.selectedOption[self.optionMenuOptionSelected] += 1
							if self.selectedOption[self.optionMenuOptionSelected] >= self.CHOICES_PER_OPTION[self.optionMenuOptionSelected]:
								self.selectedOption[self.optionMenuOptionSelected] = 0
						else:
							if self.optionMenuOkSelected == True:
								self.optionMenuOkSelected = False
							else:
								self.optionMenuOkSelected = True
					elif event.key == K_LEFT:
						# Move the selection one to the left, wrapping around if necessary.
						# (The OK/Cancel option is a separate case, in the else clause)
						if self.optionMenuOptionSelected != self.NUM_OPTIONS:
							self.selectedOption[self.optionMenuOptionSelected] -= 1
							if self.selectedOption[self.optionMenuOptionSelected] < 0:
								self.selectedOption[self.optionMenuOptionSelected] = self.CHOICES_PER_OPTION[self.optionMenuOptionSelected] - 1
						else:
							if self.optionMenuOkSelected == True:
								self.optionMenuOkSelected = False
							else:
								self.optionMenuOkSelected = True	
					elif event.key in (K_RETURN, K_LCTRL):
						# If the cursor is on OK or Cancel, determine which one, and either apply the current
						# settings to the game, or revert to the previous ones.
						if self.optionMenuOptionSelected == self.NUM_OPTIONS:
							if self.optionMenuOkSelected == True:
								# Make the selected options the default ones now.
								for i in range(0, self.NUM_OPTIONS):
									self.defaultOption[i] = self.selectedOption[i]
								self.prevRenderSubstate = self.renderSubstate
								self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN
							else:
								# Revert all settings to their default values
								for i in range(0, self.NUM_OPTIONS):
									self.selectedOption[i] = self.defaultOption[i]
								self.prevRenderSubstate = self.renderSubstate
								self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN

				if event.key == K_ESCAPE:
					pygame.event.post(pygame.event.Event(QUIT))
					return
					