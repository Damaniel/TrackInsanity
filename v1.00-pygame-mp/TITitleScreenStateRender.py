#! /usr/bin/env python

import pygame, sys
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender

class TitleScreenStateRender(StateRender):
	
	# Variables controlling the train positionss.
	topTrainX = 0
	topTrainDirection = 0
	bottomTrainX = 0
	bottomTrainDirection = 0

	# The graphics components for this state.  Includes all static and moving
	# components.  newGameText, optionsText and quitText all hold 2 images - 
	# a 'standard' version in white, and a 'highlighted' version in yellow.
	# The one displayed depends on which option is currently selected.
	# highlightArrows also contain a pair - a left and a right arrow. 
	emptyTitleBG = None
	trackWord = None
	insanityWord = None
	copyrightText = None
	trainBannerTop = None
	trainBannerBottom = None
	pressStartToPlay = None
	newGameText = (None, None)
	optionsText = (None, None)
	quitText    = (None, None)
	highlightArrows = (None, None)
	
	# The option on the title menu that the user has selected.
	titleMenuOptionSelected = 0
	
	# A frame counter used to help decide when to display things on screen
	# (like some flashing text, which only displays for 22 out of every 30 frames)
	frameCount = 0
	
	#
	# Used to figure out which parts of the title screen should be shown on the
	# screen (menu options and the like)
	#
	renderSubstate = None
	prevRenderSubstate = None
	
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
	SUBSTATE_TITLE_SCREEN = 0
	SUBSTATE_TITLE_MENU_SCREEN = 1
	SUBSTATE_TITLE_NEW_GAME_SCREEN = 2
	SUBSTATE_TITLE_OPTIONS_SCREEN = 3
	SUBSTATE_TITLE_NEW_MP_GAME_SCREEN = 4
	
	def __init__(self, w):
		self.windowSurfaceObj = w
		self.dirtyRects = []
		self.targetState = None
		
		self.renderSubstate = self.SUBSTATE_TITLE_SCREEN
		self.renderSubState = None
		
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
				
			
		return
		
	def processInputs(self):	
		for event in pygame.event.get():		
			if event.type == QUIT:
				pygame.quit()
				sys.exit()
			elif event.type == KEYDOWN:			
				if self.renderSubstate == self.SUBSTATE_TITLE_SCREEN:
					if event.key == K_RETURN:
						self.prevRenderSubstate = self.SUBSTATE_TITLE_SCREEN
						self.renderSubstate = self.SUBSTATE_TITLE_MENU_SCREEN				
				elif self.renderSubstate == self.SUBSTATE_TITLE_MENU_SCREEN:
					if event.key == K_DOWN:
						self.titleMenuOptionSelected += 1
						if self.titleMenuOptionSelected >= 3:
							self.titleMenuOptionSelected = 0
					elif event.key == K_UP:
						self.titleMenuOptionSelected -= 1
						if self.titleMenuOptionSelected < 0:
							self.titleMenuOptionSelected = 2
					elif event.key == K_RETURN:
						if self.titleMenuOptionSelected == 2:
							pygame.event.post(pygame.event.Event(QUIT))
							
				if event.key == K_ESCAPE:
					pygame.event.post(pygame.event.Event(QUIT))
					return
					