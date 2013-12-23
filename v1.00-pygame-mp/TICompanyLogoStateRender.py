#! /usr/bin/env python

import pygame
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender

class CompanyLogoStateRender(StateRender):

	# Surface objects for game graphics
	goatLogo = None
	emptyTitleBG = None
	trackWord = None
	insanityWord = None
	copyrightText = None

	# Colors (mostly used for text)
	whiteColor = None

	# Game fonts
	baseFont = None
	
	# variables used to track various logo/title screen animation states
	goatLogoAlpha = 0
	emptyTitleBGYPos = 0 - GAME_YRES
	emptyTitleBGYVelocity = 2.0
	emptyTitleBGYAccel = 1.25
	trackWordXPos = RENDER_TRACK_WORD_INITIAL_X
	trackWordSpeed = 2.0
	trackWordAccel = 1.25
	insanityWordXPos = RENDER_INSANITY_WORD_INITIAL_X
	insanityWordSpeed = -2.0
	insanityWordAccel = 1.25
	
	waitingToStartLogoFadeIn = True
	droppingTitleBackground = False
	titleBackgroundDropped = False
	waitingToScrollWords = False
	scrollingTrackWord = False
	scrollingInsanityWord = False
	waitingToChangeToTitleScreenState = False
	
	waitingToStartLogoFadeInTime = 0
	waitingToDropBackgroundTime = 0
	waitingToScrollTrackWordTime = 0
	waitingToChangeToTitleScreenStateTime = 0
	
	def __init__(self, w):
		self.windowSurfaceObj = w
		self.dirtyRects = []
		self.targetState = None
		self.whiteColor = pygame.Color(255, 255, 255)
		
		self.loadAssets()
		self.initRenderState()
		
	def loadAssets(self):
		# Company logo and title screen assets
		self.goatLogo = pygame.image.load('res/title/holygoat.png').convert()
		self.emptyTitleBG = pygame.image.load('res/title/titleblank.png').convert()
		self.trackWord = pygame.image.load('res/title/trackword.png').convert_alpha()
		self.insanityWord = pygame.image.load('res/title/insanityword.png').convert_alpha()
		self.copyrightText = pygame.image.load('res/title/copyright.png').convert_alpha()
		
	def initRenderState(self):
		self.waitingToStartLogoFadeIn = True
		self.droppingTitleBackground = False
		self.titleBackgroundDropped = False
		self.waitingToScrollWords = False
		self.scrollingTrackWord = False
		self.scrollingInsanityWord = False
		self.waitingToChangeToTitleScreenState = False
		self.goatLogoAlpha = 0
		self.emptyTitleBGYPos = 0 - GAME_YRES
		self.emptyTitleBGYVelocity = 2.0
		self.emptyTitleBGYAccel = 1.2
		self.trackWordXPos = RENDER_TRACK_WORD_INITIAL_X
		self.trackWordSpeed = 2.0
		self.trackWordAccel = 1.15
		self.insanityWordXPos = RENDER_INSANITY_WORD_INITIAL_X
		self.insanityWordSpeed = -2.0
		self.insanityWordAccel = 1.15			
		self.waitingToStartLogoFadeInTime = pygame.time.get_ticks()
		self.waitingToDropBackgroundTime = 0
		self.waitingToScrollTrackWordTime = 0
		self.waitingToChangeToTitleScreenStateTime = 0			
		return
	
	def updateLogic(self):
		return				

	def checkTimingConditions(self, ticks):
		#
		# Checks per-frame timing conditions for various render states.  These include:
		#
		# Company logo:
		#  - Wait 1 second before starting to fade the logo in from white
		#  - While fading, fade in at a rate that will cause full fade in in approximately 1 second
		#  - After fading in, wait 4 seconds before bringing in the title screen background
		#  - Drop the title screen (at an accelerating rate).  After doing so, wait 1 second.
		#  - Scroll the words 'Track' and 'Insanity' in.  
		#  - Wait 0.5 seconds
		#  - Finally, transition to the 'Title Screen' render state.
		#
		# If we're waiting to start showing the logo, check to see if it's time
		if self.waitingToStartLogoFadeIn == True:
			if (ticks - self.waitingToStartLogoFadeInTime) >= RENDER_LOGO_CLEAR_BEFORE_FADE_IN_TIME:
				self.waitingToStartLogoFadeIn = False
				return
		# If we're not waiting to start showing the logo, then adjust the alpha.  If already at
		# maximum alpha, then start waiting to drop the background.
		if self.waitingToStartLogoFadeIn == False and self.droppingTitleBackground == False:
			if self.goatLogoAlpha < 255:
				self.goatLogoAlpha += RENDER_LOGO_FADE_IN_SPEED
				if self.goatLogoAlpha >= 255:
					self.waitingToDropBackgroundTime = ticks   # Prepare to wait 4 seconds.  
			else:
				self.goatLogoAlpha = 255
				if (ticks - self.waitingToDropBackgroundTime) >= RENDER_LOGO_WAIT_BEFORE_DROP_BG_TIME:
					self.droppingTitleBackground = True
			return
		# If dropping the background and not done yet, then figure out where it should be drawn on
		# the next frame.
		if self.droppingTitleBackground == True and self.titleBackgroundDropped == False:
			self.emptyTitleBGYPos += self.emptyTitleBGYVelocity
			self.emptyTitleBGYVelocity *= self.emptyTitleBGYAccel
			if self.emptyTitleBGYPos >= 0:
				self.emptyTitleBGYPos = 0
				self.titleBackgroundDropped = True
				waitingToScrollTrackWordTime = ticks
			return
		# If the background is dropped, then check to see if it's time to start moving the words yet
		if self.titleBackgroundDropped == True and self.scrollingTrackWord == False:
			if (ticks - self.waitingToScrollTrackWordTime) >= RENDER_LOGO_WAIT_BEFORE_SCROLLING_TRACK_WORD_TIME:
				self.scrollingTrackWord = True
			return
		# If we need to move the 'track' word, do so.
		if self.scrollingTrackWord == True and self.scrollingInsanityWord == False:
			self.trackWordXPos += self.trackWordSpeed
			self.trackWordSpeed *= self.trackWordAccel
			if self.trackWordXPos >= RENDER_TRACK_WORD_FINAL_X:
				self.scrollingInsanityWord = True
				self.trackWordXPos = RENDER_TRACK_WORD_FINAL_X
			return
		# If we need to move the 'insanity' word, do so.
		if self.scrollingInsanityWord == True and self.waitingToChangeToTitleScreenState == False:
			self.insanityWordXPos += self.insanityWordSpeed
			self.insanityWordSpeed *= self.insanityWordAccel
			if self.insanityWordXPos <= RENDER_INSANITY_WORD_FINAL_X:
				self.waitingToChangeToTitleScreenState = True
				self.waitingToChangeToTitleScreenStateTime = ticks
				self.insanityWordXPos = RENDER_INSANITY_WORD_FINAL_X
			return
		# Finally, if the words are in place, wait briefly before changing state.
		if self.waitingToChangeToTitleScreenState == True:
			if (ticks - self.waitingToChangeToTitleScreenStateTime) >= RENDER_LOGO_WAIT_BEFORE_CHANGING_TO_TITLE_SCREEN_TIME:
				self.targetState = RENDER_STATE_TITLE_SCREEN
			return
	
	def renderScreen(self):
		r = self.windowSurfaceObj.fill(self.whiteColor)
		self.dirtyRects.append(r)
		# Show the logo as long as part of it is still on screen.
		if self.waitingToStartLogoFadeIn == False and self.titleBackgroundDropped == False:
			self.goatLogo.set_alpha(self.goatLogoAlpha)
			self.windowSurfaceObj.blit(self.goatLogo, (0,0))
		# Overlay the background bitmap if it's being dropped.  If it's already dropped,
		# display it fully on screen.
		if self.droppingTitleBackground == True and self.titleBackgroundDropped == False:
			self.windowSurfaceObj.blit(self.emptyTitleBG, (0, self.emptyTitleBGYPos))
		elif self.titleBackgroundDropped == True:
			self.windowSurfaceObj.blit(self.emptyTitleBG, (0, 0))
		# Draw the 'track' and 'insanity' words on screen
		if self.scrollingTrackWord == True:
			self.windowSurfaceObj.blit(self.trackWord, (self.trackWordXPos, RENDER_WORDS_Y))
		if self.scrollingInsanityWord == True:
			self.windowSurfaceObj.blit(self.insanityWord, (self.insanityWordXPos, RENDER_WORDS_Y))
		return
			
	def processInputs(self):			
		for event in pygame.event.get():		
			if event.type == QUIT:
				pygame.quit()
				sys.exit()
			elif event.type == KEYDOWN:
				self.targetState = RENDER_STATE_TITLE_SCREEN
				return