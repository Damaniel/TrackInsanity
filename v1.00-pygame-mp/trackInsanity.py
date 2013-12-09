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
	
	# Kick off the renderer state machine
	r.setInitialRenderState()
	
	while True:
		r.updateRenderLogic()
		i.processInputs(r)
		r.checkRenderTimingConditions(pygame.time.get_ticks())
		r.updateScreen()

		pygame.display.update(r.getDirtyRects())
		fpsClock.tick(30)
		
		r.clearDirtyRects()
		r.checkTargetState()
		
if __name__ == '__main__':
	main()