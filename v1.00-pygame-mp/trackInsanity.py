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
	
	# Create the new renderer and input instance.
	r = Render()	
	i = Input()
	
	r.initGraphics(wso)
	r.loadAssets()
	
	# Kick off the renderer state machine
	r.setInitialRenderState()
	
	while True:
		r.updateLogic()
		i.processInputs(r)
		r.checkTimingConditions(pygame.time.get_ticks())
		r.renderScreen()

		pygame.display.update(r.dirtyRects)
		fpsClock.tick(30)

		# Increment the frame counter (used to time some effects)
		r.frameCount = r.frameCount + 1
		
		if r.dirtyRects != None:
			del r.dirtyRects[:]
			
if __name__ == '__main__':
	main()