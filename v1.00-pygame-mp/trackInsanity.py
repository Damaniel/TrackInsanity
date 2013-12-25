#! /usr/bin/env python

import pygame, sys
from TIConsts import *
from TIRender import Render
from TIInput import Input
from pygame.locals import *

def main():

	pygame.init()
	pygame.mixer.init()
	fpsClock = pygame.time.Clock()
	pygame.mouse.set_visible(False)
	wso = pygame.display.set_mode((GAME_XRES, GAME_YRES))
	pygame.display.set_caption('TrackInsanity v1.00 (Pygame)')

	r = Render()
	
	Render.initGraphics(wso)
	
	# Kick off the renderer state machine
	Render.setInitialRenderState()
	
	#i = 0
	
	while True:
		Render.updateRenderLogic()
		Render.processInputs()
		Render.checkRenderTimingConditions(pygame.time.get_ticks())
		Render.updateScreen()

		pygame.display.update(Render.getDirtyRects())
		fpsClock.tick(30)
		
		# The GCW Zero seems to have a very inaccurate timer - 
		# the non busy loop version of tick(30) generates a frame rate
		# of 25, while the busy loop version is very close to 30.  
		# At tick(60), the values are closer to 40/55 respectively.
		#
		# It's not really a big deal, as long as sounds aren't synchronized
		# to the update rate.  
		#fpsClock.tick_busy_loop(30)
		
		#i += 1
		#if (i % 25) == 0:
		#	print "FPS = " + str(fpsClock.get_fps())
			
		Render.clearDirtyRects()
		Render.checkTargetState()
		
	
if __name__ == '__main__':
	main()