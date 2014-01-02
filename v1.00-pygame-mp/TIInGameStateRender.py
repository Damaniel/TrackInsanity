#! /usr/bin/env python

import pygame, sys
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender
from TIGame import Game
from TIPlayer import Player
from TITile import Tile

class InGameStateRender(StateRender):	
	def __init__(self, w):
		self.windowSurfaceObj = w
		self.dirtyRects = []
		self.targetState = None
		self.gameInstance = None
		
		self.loadAssets()
		self.initRenderState()		
				
	def loadAssets(self):
		self.dialogFont = pygame.font.Font('res/fonts/Munro.ttf', 10)
		
		self.avatarTabs = pygame.image.load('res/common/avatarBoxes.png').convert()
		self.avatars    = pygame.image.load('res/common/avatars.png').convert_alpha()		
		
		self.boardBG = pygame.image.load('res/board/boardBG.png').convert()
		self.playerStationsTiles = pygame.image.load('res/board/playerStationsTiles.png').convert()
		self.tileStripIcons = pygame.image.load('res/board/tileStripIcons.png').convert()
		
		return
		
	def initRenderState(self):
		self.playerDialogChanged = False
		self.boardBGChanged = True
		
		self.gameInstance = Game()
		# Read in the values from the user
		self.gameInstance.initPlayersFromUi()
		
		# Create the initial player dialog box
		self.renderPlayerDialogBox()
		
	def updateLogic(self):
		return				

	def checkTimingConditions(self, ticks):
		return
		
	def renderScreen(self):
		if self.boardBGChanged == True:
			r = self.windowSurfaceObj.blit(self.boardBG, (0,0))
			self.dirtyRects.append(r)
			self.boardBGChanged = False
		
		if self.playerDialogChanged == True:
			r = self.windowSurfaceObj.blit(self.playerSurface, RENDER_IN_GAME_PLAYER_DIALOG_BOX_POS)
			self.dirtyRects.append(r)
			self.playerDialogChanged = False
			
	def processInputs(self):			
		for event in pygame.event.get():		
			# If the window close button was clicked (in Windows)
			if event.type == QUIT:
				pygame.quit()
				sys.exit()			
	
	# Draws a copy of the player dialog box to the global 'playerSurface' object that's been set aside to hold it.
	def renderPlayerDialogBox(self):
		self.playerSurface = pygame.Surface(RENDER_IN_GAME_PLAYER_DIALOG_BOX_SIZE, flags=SRCALPHA)
		self.playerSurface.fill((0, 0, 0))
		
		# Draw the graphical stuff first, then do the text
		for i in range(0, self.gameInstance.numPlayers):
			# Color boxes and avatars
			avatarX = 16 * (self.gameInstance.players[i].avatarId - (int(self.gameInstance.players[i].avatarId / 16) * 16))
			avatarY = 16 * int(self.gameInstance.players[i].avatarId / 16)			
			self.playerSurface.blit(self.avatarTabs, RENDER_IN_GAME_PLAYER_AVATAR_BOX_POS[i], RENDER_AVATAR_BOX_STRIP_POS[i])
			self.playerSurface.blit(self.avatars, RENDER_IN_GAME_PLAYER_AVATAR_POS[i], (avatarX, avatarY, 16, 16))
			
			# Base station dialog and complete/incomplete station information
			self.playerSurface.blit(self.playerStationsTiles, RENDER_IN_GAME_PLAYER_STATION_TILE_BOX_POS[i])
			
			# XXX render stations here
			
			
			# Render the active player's tiles, if they're the current player and human.  Otherwise, draw a generic background.
			if self.gameInstance.curPlayer == i and self.gameInstance.players[i].controlledBy == Player.HUMAN:
				# Get the primary and secondary tile for the player being processed
				primaryTile = self.gameInstance.players[i].currentTileId
				secondaryTile = self.gameInstance.players[i].reserveTileId
				if primaryTile != Tile.NONE:
					self.playerSurface.blit(self.tileStripIcons, RENDER_IN_GAME_PLAYER_HELD_PRIMARY_POS[i], (primaryTile * RENDER_IN_GAME_TILE_ICON_SIZE, 0, RENDER_IN_GAME_TILE_ICON_SIZE, RENDER_IN_GAME_TILE_ICON_SIZE))
				if secondaryTile != Tile.NONE:
					self.playerSurface.blit(self.tileStripIcons, RENDER_IN_GAME_PLAYER_HELD_SECONDARY_POS[i], (secondaryTile * RENDER_IN_GAME_TILE_ICON_SIZE, 0, RENDER_IN_GAME_TILE_ICON_SIZE, RENDER_IN_GAME_TILE_ICON_SIZE))
			else:
				self.playerSurface.blit(self.tileStripIcons, RENDER_IN_GAME_PLAYER_HELD_PRIMARY_POS[i], RENDER_IN_GAME_TILE_ICON_BG_RECT)
				self.playerSurface.blit(self.tileStripIcons, RENDER_IN_GAME_PLAYER_HELD_SECONDARY_POS[i], RENDER_IN_GAME_TILE_ICON_BG_RECT)
			
		self.playerDialogChanged = True
		
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