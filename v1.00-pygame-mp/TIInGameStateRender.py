#! /usr/bin/env python

import pygame, sys
from pygame.locals import *
from TIConsts import *
from TIStateRender import StateRender
from TIGame import Game
from TIPlayer import Player
from TITile import Tile

class InGameStateRender(StateRender):	

	LOCAL_HUMAN_NAME_COLOR = (255, 255, 255)
	MULTI_HUMAN_NAME_COLOR = (205, 142, 163)
	CPU_NAME_COLOR = (65, 208, 210)
	SCORE_TEXT_COLOR = (255, 255, 255)
	LEADER_SCORE_TEXT_COLOR = (255, 255, 0)
	
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
		self.playerStationStatus = pygame.image.load('res/board/playerStationStatus.png').convert()
		
		return
		
	def initRenderState(self):
		self.playerDialogChanged = False
		self.boardBGChanged = True
		self.boardContentChanged = False
		
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
		# If the board background has changed, draw it.  This is done whenever requested, regardless of the state.
		if self.boardBGChanged == True:
			r = self.windowSurfaceObj.blit(self.boardBG, (0,0))
			self.dirtyRects.append(r)
			self.boardBGChanged = False
		
		# If the player dialog window has changed, draw it.  This is done whenever requested, regardless of the state.
		if self.playerDialogChanged == True:
			r = self.windowSurfaceObj.blit(self.playerSurface, RENDER_IN_GAME_PLAYER_DIALOG_BOX_POS)
			self.dirtyRects.append(r)
			self.playerDialogChanged = False
		
		# If the board content (station overlays, trains, tiles) has changed, draw it.  This is done whenever requested, regardless of the state.
		if self.boardContentChanged == True:
			r = self.windowSurfaceObj.blit(self.boardContent, RENDER_IN_GAME_BOARD_CONTENT_POS)
			self.dirtyRects.append(r)
			self.boardContentChanged = False
			
		# Draw any state-appropriate dialogs.  
		
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
		
		hiScore = -1
		hiPlayer = []
		# Figure out which player has the high score (so we can highlight it).  Only do this is the score is actually
		# greater than zero.
		for i in range(0, self.gameInstance.numPlayers):
			if self.gameInstance.players[i].score >= hiScore and self.gameInstance.players[i].score > 0:
				# Clear the list if this isn't a tie for highest score
				if self.gameInstance.players[i].score > hiScore:
					hiPlayer = []
				hiPlayer.append(i)
				hiScore = self.gameInstance.players[i].score
			
		# Draw the graphical stuff first, then do the text
		for i in range(0, self.gameInstance.numPlayers):
			# Color boxes and avatars
			avatarX = 16 * (self.gameInstance.players[i].avatarId - (int(self.gameInstance.players[i].avatarId / 16) * 16))
			avatarY = 16 * int(self.gameInstance.players[i].avatarId / 16)			
			self.playerSurface.blit(self.avatarTabs, RENDER_IN_GAME_PLAYER_AVATAR_BOX_POS[i], RENDER_AVATAR_BOX_STRIP_POS[i])
			self.playerSurface.blit(self.avatars, RENDER_IN_GAME_PLAYER_AVATAR_POS[i], (avatarX, avatarY, 16, 16))
			
			# Base station dialog and complete/incomplete station information
			self.playerSurface.blit(self.playerStationsTiles, RENDER_IN_GAME_PLAYER_STATION_TILE_BOX_POS[i])
			
			# Render station status
			for j in range(0, self.gameInstance.numStationsPerPlayer):
				if j >= RENDER_NUM_STATIONS_PER_ROW:
					drawX = ((j - RENDER_NUM_STATIONS_PER_ROW) * 4) + RENDER_IN_GAME_PLAYER_STATION_TILE_BOX_POS[i][0] + 1
					drawY = RENDER_IN_GAME_PLAYER_STATION_TILE_BOX_POS[i][1] + 7  # 7 is a vertical offset of 2 + the height of a box + the 1 pixel border
				else:
					drawX = (j * 4) + RENDER_IN_GAME_PLAYER_STATION_TILE_BOX_POS[i][0] + 1
					drawY = RENDER_IN_GAME_PLAYER_STATION_TILE_BOX_POS[i][1] + 2  # 2 is the vertical offset from the top of the box to the drawing region
					
				if j < self.gameInstance.players[i].numStationsComplete:
					srcRect = RENDER_IN_GAME_STATION_STATUS_STRIP_POS[1]
				else:
					srcRect = RENDER_IN_GAME_STATION_STATUS_STRIP_POS[2]
					
				self.playerSurface.blit(self.playerStationStatus, (drawX, drawY), srcRect)
				
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
						
			# Now the text components.  They're individually rendered to a temporary surface and blitted to the playerSurface.  
			if self.gameInstance.players[i].controlledBy == Player.HUMAN:
				nameCol = self.LOCAL_HUMAN_NAME_COLOR
			else:
				nameCol = self.CPU_NAME_COLOR				
			tmpSurface = self.dialogFont.render(self.gameInstance.players[i].playerName, False, nameCol)
			self.playerSurface.blit(tmpSurface, RENDER_IN_GAME_PLAYER_NAME_POS[i])
			
			tmpSurface = self.dialogFont.render('Score:', False, self.SCORE_TEXT_COLOR)
			self.playerSurface.blit(tmpSurface, RENDER_IN_GAME_PLAYER_SCORE_TEXT_POS[i])
			
			if i in hiPlayer:
				scoreCol = self.LEADER_SCORE_TEXT_COLOR
			else:
				scoreCol = self.SCORE_TEXT_COLOR
			tmpSurface = self.dialogFont.render(str(self.gameInstance.players[i].score), False, scoreCol)
			self.playerSurface.blit(tmpSurface, RENDER_IN_GAME_PLAYER_SCORE_POS[i])
						
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