#! /usr/bin/env python

import pygame
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender
from TIGame import Game
from TIPlayer import Player

class InGameStateRender(StateRender):	
	def __init__(self, w):
		self.windowSurfaceObj = w
		self.dirtyRects = []
		self.targetState = None
		self.gameInstance = None
		
		self.loadAssets()
		self.initRenderState()		
				
	def loadAssets(self):
		return
		
	def initRenderState(self):
		self.gameInstance = Game()
		# Read in the values from the user
		self.gameInstance.initPlayersFromUi()
		self.printGameSettings()
						
		quit()
		
	def updateLogic(self):
		return				

	def checkTimingConditions(self, ticks):
		return
		
	def renderScreen(self):
		return
			
	def processInputs(self):			
		return
		
		
	def printGameSettings(self):
		print "Game settings:"
		print "Number of players = " + str(self.gameInstance.numPlayers)
		print "Player types:"
		for i in range(0, 6):
			if self.gameInstance.players[i].controlledBy == Player.HUMAN:
				print "Player " + str(i+1) + ": Human (avatar " + str(self.gameInstance.players[i].avatarId) + ")"
			elif self.gameInstance.players[i].controlledBy == Player.COMPUTER:
				print "Player " + str(i+1) + ": Computer (level " + str(self.gameInstance.players[i].computerAiLevel) + ", avatar " + str(self.gameInstance.players[i].avatarId) + ")"
		print "Highlight player tracks (0 = Yes, 1 = No)? " + str(self.gameInstance.highlightTracks)
		print "Highlight legal moves (0 = Yes, 1 = No)? " + str(self.gameInstance.highlightLegalMoves)
		print "Show last move? (0 = No, 1 = Last, 2 = All)? " + str(self.gameInstance.showLastMove)