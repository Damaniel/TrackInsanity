#! /usr/bin/env python

import pygame, sys
from TIConsts import *
from TIRender import Render
from TIInput import Input
from pygame.locals import *

def main():

	pygame.init()
	fpsClock = pygame.time.Clock()
	pygame.mouse.set_visible(False)
	wso = pygame.display.set_mode((GAME_XRES, GAME_YRES))
	pygame.display.set_caption('TrackInsanity v1.00 (Pygame)')

	r = Render()
	
	Render.initGraphics(wso)
	
	# Kick off the renderer state machine
	Render.setInitialRenderState()
	
	while True:
		Render.updateRenderLogic()
		Render.processInputs()
		Render.checkRenderTimingConditions(pygame.time.get_ticks())
		Render.updateScreen()

		pygame.display.update(Render.getDirtyRects())
		fpsClock.tick(30)
		
		Render.clearDirtyRects()
		Render.checkTargetState()
		
if __name__ == '__main__':
	main()