#! /usr/bin/env python

import pygame, sys
from pygame.locals import *

pygame.init()
fpsClock = pygame.time.Clock()

pygame.mouse.set_visible(False)

windowSurfaceObj = pygame.display.set_mode((320,240))
pygame.display.set_caption('Stuff')

redColor = pygame.Color(255, 0, 0)
mockup = pygame.image.load('res/mockup.png').convert()

while True:
	windowSurfaceObj.blit(mockup, (0,0))
	
	for event in pygame.event.get():
		if event.type == QUIT:
			pygame.quit()
			sys.exit()
		elif event.type == KEYDOWN:
			if event.key in (K_ESCAPE, K_RETURN):
				pygame.event.post(pygame.event.Event(QUIT))
				
	pygame.display.update()
	fpsClock.tick(30)
	
			