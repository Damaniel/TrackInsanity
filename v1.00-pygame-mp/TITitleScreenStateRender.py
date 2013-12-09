#! /usr/bin/env python

import pygame
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender

class TitleScreenStateRender(StateRender):
	
	# Variables controlling the train positionss.
	topTrainX = 0
	topTrainDirection = 0
	bottomTrainX = 0
	bottomTrainDirection = 0

	emptyTitleBG = None
	trackWord = None
	insanityWord = None
	copyrightText = None
	trainBannerTop = None
	trainBannerBottom = None
	pressStartToPlay = None
	
	frameCount = 0
	
	def __init__(self, w):
		self.windowSurfaceObj = w
		self.dirtyRects = []
		self.targetState = None
		
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
		self.renderTitleScreenDrawBase()
		self.drawTitleInfo(0)
		if (self.frameCount % 30) >= RENDER_PRESS_START_FRAME_SHOWN_TIME and (self.frameCount % 30) < RENDER_PRESS_START_FRAME_HIDDEN_TIME:
			self.windowSurfaceObj.blit(self.pressStartToPlay, (RENDER_TITLE_PRESS_START_X, RENDER_TITLE_PRESS_START_Y))
		return