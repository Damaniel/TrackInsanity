#! /usr/bin/env python

import pygame, sys
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender

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
		self.selectedOption = [0, 0, 1, 1, 5, 5]
		self.defaultOption  = [0, 0, 1, 1, 5, 5]
		
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
		
	def drawTitleInfo(self, offset):
		r = self.windowSurfaceObj.blit(self.trackWord, (RENDER_TRACK_WORD_FINAL_X + offset, RENDER_WORDS_Y))
		self.dirtyRects.append(r)
		r = self.windowSurfaceObj.blit(self.insanityWord, (RENDER_INSANITY_WORD_FINAL_X + offset, RENDER_WORDS_Y))
		self.dirtyRects.append(r)
		r = self.windowSurfaceObj.blit(self.copyrightText, (RENDER_TITLE_COPYRIGHT_TEXT_X + offset, RENDER_TITLE_COPYRIGHT_TEXT_Y))
		self.dirtyRects.append(r)
		
	def drawOptionScreen(self):
		# Draw the base object
		self.windowSurfaceObj.blit(self.optionBase, (RENDER_OPTION_BASE_X, RENDER_OPTION_BASE_Y))
		
		#
		# Check to see which option is highlighted, and draw the highlight over that option.
		# The first NUM_OPTIONS choices control highlights for the menu options.  The last value
		# (NUM_OPTIONS) is special, and allows the control of the 'OK/Cancel' buttons.
		#
		if self.optionMenuOptionSelected >=0 and self.optionMenuOptionSelected < self.NUM_OPTIONS:
			self.windowSurfaceObj.blit(self.optionNames[self.optionMenuOptionSelected], RENDER_OPTION_TEXT_POSITIONS[self.optionMenuOptionSelected])
		elif self.optionMenuOptionSelected == self.NUM_OPTIONS:
			if self.optionMenuOkSelected == True:
				self.windowSurfaceObj.blit(self.okHighlight, (RENDER_OPTION_OK_X, RENDER_OPTION_OK_Y))
			else:
				self.windowSurfaceObj.blit(self.cancelHighlight, (RENDER_OPTION_CANCEL_X, RENDER_OPTION_CANCEL_Y))
				
		# For each of the options, check to see which value is highlighted, and highlight only that value. 
		self.windowSurfaceObj.blit(self.optionHighlights[0][self.selectedOption[0]], RENDER_OPTION_HPT_POSITIONS[self.selectedOption[0]])
		self.windowSurfaceObj.blit(self.optionHighlights[1][self.selectedOption[1]], RENDER_OPTION_HLM_POSITIONS[self.selectedOption[1]])
		self.windowSurfaceObj.blit(self.optionHighlights[2][self.selectedOption[2]], RENDER_OPTION_SLM_POSITIONS[self.selectedOption[2]])
		self.windowSurfaceObj.blit(self.optionHighlights[3][self.selectedOption[3]], RENDER_OPTION_CAL_POSITIONS[self.selectedOption[3]])
		
		# Do the same for the last two options (which need to do a lookup into a couple of offset tables since the
		# digits are all stored together
		self.windowSurfaceObj.blit(self.digitsHighlight, RENDER_OPTION_MV_POSITIONS[self.selectedOption[4]], RENDER_OPTION_DIGIT_STRIP_POSITIONS[self.selectedOption[4]])
		self.windowSurfaceObj.blit(self.digitsHighlight, RENDER_OPTION_EV_POSITIONS[self.selectedOption[5]], RENDER_OPTION_DIGIT_STRIP_POSITIONS[self.selectedOption[5]])
		
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
		self.digitsHighlight = pygame.image.load('res/title/options/digitsHighlight.png').convert_alpha()
	
		self.optionHighlights = ((self.yesHighlight, self.noHighlight), \
								 (self.yesHighlight, self.noHighlight), \
								 (self.noHighlight, self.oneHighlight, self.allHighlight), \
								 (self.easyHighlight, self.mediumHighlight, self.hardHighlight))
		
		self.okHighlight = pygame.image.load('res/title/options/okHighlight.png').convert_alpha()
		self.cancelHighlight = pygame.image.load('res/title/options/cancelHighlight.png').convert_alpha()
		
	def initRenderState(self):
		self.topTrainX = RENDER_TRAIN_BANNER_INITIAL_X
		self.topTrainDirection = RENDER_TRAIN_DIR_RIGHT
		self.bottomTrainX = RENDER_TRAIN_BANNER_FINAL_X
		self.bottomTrainY = RENDER_TRAIN_DIR_LEFT
		self.frameCount = 0
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
			self.drawTitleInfo(0)
		
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
					if event.key == K_RETURN:
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
					elif event.key == K_RETURN:
						if self.titleMenuOptionSelected == self.TITLE_MENU_OPTION_OPTIONS:
							self.prevRenderSubstate = self.renderSubstate
							self.renderSubstate = self.SUBSTATE_TITLE_OPTIONS_SCREEN
						elif self.titleMenuOptionSelected == self.TITLE_MENU_OPTION_QUIT:
							pygame.event.post(pygame.event.Event(QUIT))
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
					elif event.key == K_RETURN:
						# If the cursor is on OK or Cancel, determine which one, and either apply the current
						# settings to the game, or revert to the previous ones.
						if self.optionMenuOptionSelected == self.NUM_OPTIONS:
							if self.optionMenuOkSelected == True:
								self.prevRenderSubstate = self.renderSubstate
								self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN
							else:
								self.prevRenderSubstate = self.renderSubstate
								self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN
								
				if event.key == K_ESCAPE:
					pygame.event.post(pygame.event.Event(QUIT))
					return
					