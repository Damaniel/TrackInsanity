#! /usr/bin/env python

import pygame, sys
from TIConsts import *
from TIRender import Render

from pygame.locals import *

class Input:

	def processInputs(self, renderInstance):	
		
		for event in pygame.event.get():		
			if renderInstance.currentRenderState == RENDER_STATE_COMPANY_LOGO:
				if event.type == QUIT:
					pygame.quit()
					sys.exit()
				elif event.type == KEYDOWN:
					renderInstance.changeRenderState(RENDER_STATE_TITLE_SCREEN)
					return
			else:
				if event.type == QUIT:
					pygame.quit()
					sys.exit()
				elif event.type == KEYDOWN:
					if event.key in (K_ESCAPE, K_RETURN):
						pygame.event.post(pygame.event.Event(QUIT))
						return
						