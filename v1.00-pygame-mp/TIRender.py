#! /usr/bin/env python

import pygame
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender
from TICompanyLogoStateRender import CompanyLogoStateRender
from TITitleScreenStateRender import TitleScreenStateRender

class Render:

	# The current render state
	currentRenderState = None
	
	# The global 'screen' surface object
	windowSurfaceObj = None
	
	# An object that holds the render subclass for our current state.
	crs = None
	
	@staticmethod
	def initGraphics(w):
		Render.windowSurfaceObj = w

	@staticmethod
	def setInitialRenderState():
		Render.changeRenderState(RENDER_STATE_COMPANY_LOGO)

	@staticmethod
	def changeRenderState(newState):
		if newState == RENDER_STATE_COMPANY_LOGO:
			Render.currentRenderState = RENDER_STATE_COMPANY_LOGO
			Render.crs = CompanyLogoStateRender(Render.windowSurfaceObj)
		if newState == RENDER_STATE_TITLE_SCREEN:
			Render.currentRenderState = RENDER_STATE_TITLE_SCREEN
			Render.crs = TitleScreenStateRender(Render.windowSurfaceObj)
	
	@staticmethod
	def updateRenderLogic():
		Render.crs.updateLogic()
		
	@staticmethod
	def checkRenderTimingConditions(t):
		Render.crs.checkTimingConditions(t)
		
	@staticmethod
	def updateScreen():
		Render.crs.renderScreen()

	@staticmethod
	def getDirtyRects():
		return Render.crs.dirtyRects
	
	@staticmethod
	def clearDirtyRects():
		if Render.crs.dirtyRects != None:
			del Render.crs.dirtyRects[:]

	@staticmethod
	def checkTargetState():
		if Render.crs.targetState != None:
			Render.changeRenderState(Render.crs.targetState)
	