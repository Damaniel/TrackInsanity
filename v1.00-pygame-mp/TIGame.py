#! /usr/bin/env python

#=============================================================================
# TIGame.py
#
# A game is a single instance of a TrackInsanity game.  It contains a board
# (and associated tilepool), all required options information (including a
# 'temporary' set that's changed by the UI to hold player selections until
# the 'OK' option is chosen from the options menu.
#
# ** This implementation contains all functions from the C#/XNA version of the 
# game.  Not all functions are debugged though.
#=============================================================================

import random
from TITilePool import *
from TITile import *
from TIBoard import *
from TIPlayer import *
from TISharedData import *
from TIConsts import *
from TIComputerAI import *

class Game:

	# A series of game states.  These tell us what portion of the game is currently
	# occuring.  Some states are changed on a per-turn basis, some only occur at
	# the end of the game, and some can occur at any time (like GAME_STATE_PAUSE)
	GAME_STATE_DEFAULT = 0
	GAME_STATE_NEXT_PLAYER_TURN = 1
	GAME_STATE_SELECT_ACTION = 2
	GAME_STATE_TILE_DRAW = 3
	GAME_STATE_TILE_SELECT = 4
	GAME_STATE_TILE_PLAY = 5
	GAME_STATE_PASS = 6
	GAME_STATE_DISCARD = 7
	GAME_STATE_END_TURN = 8
	GAME_STATE_GAME_FINISHED = 9
	GAME_STATE_CONFIRM_EXIT = 10
	GAME_STATE_SHOW_BOARD = 11
	GAME_STATE_COMPUTER_MOVE = 12
	GAME_STATE_PAUSE = 13
	
	CPU_MIN_THINK_TIME = 1200
	CPU_MAX_THINK_TIME = 2200

	OPTION_YES = 0	
	OPTION_NO = 1
	
	OPTION_HIGHLIGHT_NONE = 0
	OPTION_HIGHLIGHT_LAST_PLAYER = 1
	OPTION_HIGHLIGHT_ALL_PLAYERS = 2
	
	OPTION_AI_EASY = 0
	OPTION_AI_MEDIUM = 1
	OPTION_AI_HARD = 2
	
	def __init__(self):
		self.tilepool = None
		self.board = None
		self.players = None
		
		self.highlightTracks = self.OPTION_YES
		self.highlightLegalMoves = self.OPTION_YES
		self.showLastMove = self.OPTION_HIGHLIGHT_ALL_PLAYERS
		self.musicVolume = 4
		self.effectsVolume = 4
		
		# Creates the local variables that will be used in reset() later
		self.numPlayers = None
		self.curPlayer = None
		self.gameState = None
		self.previousGameState = None
		self.playerHasDrawn = None
		self.playIsValid = None
		self.drawIsValid = None
		self.discardIsValid = None
		self.passIsValid = None
		self.selectedMoveTileX = None
		self.selectedMoveTileY = None
		self.selectedMoveTileId = None
		self.selectedMoveIsReserveTile = None
		
		# Resets values specific to a particular game instance
		self.reset()
		
	def reset(self):
		self.tilepool = TilePool()
		self.board = Board()
		self.players = []
		
		self.numPlayers = 0
		self.curPlayer = 0
		self.gameState = self.GAME_STATE_DEFAULT
		self.previousGameState = self.GAME_STATE_DEFAULT
		self.playerHasDrawn = False
		self.playIsValid = False
		self.drawIsValid = False
		self.discardIsValid = False
		self.passIsValid = False
		self.selectedMoveTileX = -1
		self.selectedMoveTileY = -1
		self.selectedMoveTileId = Tile.NONE
		self.selectedMoveIsReserveTile = False
		
	def initPlayersFromUi(self):
		if SharedData.selectedPlayers < MIN_PLAYERS or SharedData.selectedPlayers > MAX_PLAYERS:
			print "initPlayersFromUi: Invalid number of players selected"
			return False
		
		# Set the AI level
		aiLevel = SharedData.selectedOption[OPTION_COMPUTER_AI_LEVEL]
		
		# Create new players of each kind
		for i in range(0, SharedData.selectedPlayers):
			self.players.append(Player(SharedData.playerState[i], 0, aiLevel))
			
		# Create 'nobody' place holders for all non-playing characters
		for i in range(0, MAX_PLAYERS - SharedData.selectedPlayers):
			self.players.append(Player(Player.NOBODY, 0, aiLevel))
		
		# Set each player's avatar ID
		for i in range(0, SharedData.selectedPlayers):
			self.players[i].avatarId = SharedData.playerAvatars[i]

		# Figure out the number of stations that each player receives in this game
		self.numStationsPerPlayer = int(Board.NUM_STATIONS / SharedData.selectedPlayers)
		
		# Set the number of players for this game instance, and set the current player
		# to player 1
		self.numPlayers = SharedData.selectedPlayers
		self.curPlayer = 0
		
		# Assign player trains to stations depending on the number of players in this game.
		for i in range(0, Board.NUM_STATIONS):
			(stationX, stationY, stationExit) = self.board.getStationInfo(i)
			self.board.b[stationX][stationY].trainPresent = Board.BOARD_STATION_DATA[self.numPlayers][i]
				
		# Set the rest of the options from the UI
		self.highlightTracks = SharedData.selectedOption[OPTION_HIGHLIGHT_PLAYER_TRACKS]
		self.highlightLegalMoves = SharedData.selectedOption[OPTION_HIGHLIGHT_LEGAL_MOVES]
		self.showLastMove = SharedData.selectedOption[OPTION_SHOW_LAST_MOVE]
		self.musicVolume = SharedData.selectedOption[OPTION_MUSIC_VOLUME]
		self.effectsVolume = SharedData.selectedOption[OPTION_EFFECTS_VOLUME]
		
	
	def changeState(self, state):
		if state == self.GAME_STATE_DEFAULT:
			return True
			
		self.previousGameState = self.gameState
		self.gameState = state
		self.gameStateChanged = True
		
		if state == self.GAME_STATE_NEXT_PLAYER_TURN:
			print "changeState: Changing game state to NEXT_PLAYER_TURN"
			humanPlayers = 0
			for i in range(0, self.numPlayers):
				if self.players[i].controlledBy == Player.HUMAN:
					humanPlayers = humanPlayers + 1
			if humanPlayers == 1 or self.players[self.curPlayer].controlledBy == Player.COMPUTER:
				self.changeState(self.GAME_STATE_SELECT_ACTION)
			return True
		
		if state == self.GAME_STATE_SELECT_ACTION:
			print "changeState: Changing game state to SELECT_ACTION"
			if self.players[self.curPlayer].controlledBy == Player.COMPUTER:
				self.changeState(self.GAME_STATE_COMPUTER_MOVE)
			else:
				# If a player holds 2 tiles, be sure that 'draw tile' gets disabled
				if self.players[self.curPlayer].currentTileId != Tile.NONE and \
				   self.players[self.curPlayer].reserveTileId != Tile.NONE:
					self.playerHasDrawn = True
				
				# If a reserve tile is present, select it by default
				if self.players[self.curPlayer].reserveTileId != Tile.NONE:
					self.selectedMoveIsReserveTile = True
				else:
					self.selectedMoveIsReserveTile = False
			return True
			
		if state == self.GAME_STATE_TILE_DRAW:
			print "changeState: Changing game state to TILE_DRAW"
			return True
			
		if state == self.GAME_STATE_TILE_SELECT:
			print "changeState: Changing game state to TILE_SELECT"
			# If the player holds 2 tiles, use the reserve tile as the default.
			# Otherwise, use the primary tile as the default.
			if self.players[self.curPlayer].reserveTileId != Tile.NONE:
				tileIdToUse = self.players[self.curPlayer].reserveTileId
				self.selectedMoveIsReserveTile = True
			else:
				tileIdToUse = self.players[self.curPlayer].currentTileId
				self.selectedMoveIsAReserveTile = False
				
			self.selectedMoveTileId = tileIdToUse
			
			# Mark the legal moves for the selected tile on the game board.
			self.board.markLegalMoves(self.tilepool.getTile(tileIdToUse), self.tilepool.numUnplayedTiles())
			return True
		
		if state == self.GAME_STATE_TILE_PLAY:
			print "changeState: Changing game state to TILE_PLAY"
			self.board.placeTile(self.selectedMoveTileX, self.selectedMoveTileY, self.selectedMoveTileId)
			self.checkForCompletedTracks()
			self.selectedMoveTileX = -1
			self.selectedMoveTileY = -1
			self.selectedMoveTileId = Tile.NONE
			self.changeState(self.GAME_STATE_GAME_TURN)
			return True
			
		if state == self.GAME_STATE_DISCARD:
			print "changeState: Changing game state to DISCARD"
			return True
			
		if state == self.GAME_STATE_END_TURN:
			print "changeState: Changing game state to END_TURN"
			self.playerHasDrawn = False
			self.curPlayer = self.curPlayer + 1
			
			if self.curPlayer >= self.numPlayers:
				self.curPlayer = 0
				
			if self.checkForEndOfGame() == True:
				self.changeState(self.GAME_STATE_GAME_FINISHED)
			else:
				self.changeState(self.GAME_STATE_NEXT_PLAYER_TURN)
			
			return True
			
		if state == self.GAME_STATE_CONFIRM_EXIT:
			print "changeState: Changing game state to CONFIRM_EXIT"
			return True
			
		if state == self.GAME_STATE_GAME_FINISHED:
			print "changeState: Changing game state to GAME_FINISHED"
			return True
		
		if state == self.GAME_STATE_SHOW_BOARD:
			print "changeState: Changing game state to SHOW_BOARD"
			return True
			
		if state == self.GAME_STATE_COMPUTER_MOVE:
			print "changeState: Changing game state to COMPUTER_MOVE"
			SharedData.cpuMoveAllowed = False
			SharedData.cpuDelayTicks = random.randint(self.CPU_MIN_THINK_TIME, self.CPU_MAX_THINK_TIME)
			return True
			
		return False

	def checkForEndOfGame(self):
		gameEnded = True
		
		# If there are any tiles in the tile pool, the game isn't over
		if self.tilepool.numUnplayedTiles() > 0:
			gameEnded = False
			
		# If any player is still holding a tile, then the game isn't over.
		for i in range(0, self.numPlayers):
			if self.players[i].currentTileId != Tile.NONE or self.players[i].reserveTileId != Tile.NONE:
				gameEnded = False
				
		return gameEnded
		
	def discardTile(self):
		if self.tilepool.returnTileToPool(self.selectedMoveTileId) == False:
			return False
			
		if self.selectedMoveIsReserveTile == True:
			self.players[self.curPlayer].reserveTileId = Tile.NONE
		else:
			if self.players[self.curPlayer].reserveTileId != Tile.NONE:
				self.players[self.curPlayer].currentTileId = self.players[self.curPlayer].reserveTileId
				self.players[self.curPlayer].reserveTileId = Tile.NONE
			else:
				self.players[self.curPlayer].currentTileId = Tile.NONE
				
		return True
		
	def checkForCompletedTracks(self):
		for i in range(0, Board.NUM_STATIONS):
			(score, t, destination) = self.calculateTrackScore(i, Tile.NONE)
			if destination == BoardSquare.STATION or destination == BoardSquare.CENTRAL_STATION:
				if self.board.getTrackStatus(i) != Board.TRACK_PROCESSED:
					self.board.setTrackStatus(i, Board.TRACK_COMPLETE)
					self.processCompletedTrack(i, score)
	
	def processCompletedTrack(self, station, score):
		if station < 0 or station >= Board.NUM_STATIONS:
			print "processCompletedTrack: index out of range!"
			return False
			
		if self.board.getTrackStatus(station) != Board.TRACK_COMPLETE:
			return True

		playerToAward = Board.BOARD_STATION_DATA[self.numPlayers][station]
			
		# If this station doesn't belong to anybody, nobody gets the score.
		if  playerToAward == Board.NO_TRAIN:
			return True
		
		self.players[playerToAward-1].score += score
		self.players[playerToAward-1].numStationsComplete += 1
		
		(stationX, stationY, stationExit) = self.board.getStationInfo(station)
		self.board.b[stationX][stationY].trainPresnt = Board.NO_TRAIN
		self.board.setTrackStatus(station, Board.TRACK_PROCESSED)
		
		return True
		
	#=======================================================================================
	# calculateTrackScore()
	#
	# Determines the score of a completed track given a specific station (or a partial track
	# if the player is an AI player evaluating moves).
	#=======================================================================================		
	def calculateTrackScore(self, station, passThruTileId):
	
		# passThruTileId is a variable that contains a tile ID (or NONE for human players).  The tile ID
		# is used to determine whether the score obtained as a result of this board position is a result
		# of the tile that has been specified in passThruTileId.  For AI players, knowing whether the 
		# move itself is responsible for the score is important for deciding move value.  
		# If the tile was involved in the score, then passThruTile is set to True and returned, otherwise
		# it is set to false.  
		#
		# For human players, these values are ignored (scoring is only done for human players when a
		# track section is completed.  AI players evaluate all legal board positions for value, hence
		# the need to treat their case separately.
		#
		# loopLimit is used to prevent the track traversal code from getting into an infinite loop.  The
		# code should never do this anyway; this is merely a failsafe that would allow me to debug
		# issues with determining legal board moves (legal moves never generate infinite loops).
		
		passThruTile = None
		loopLimit = 255
		
		if passThruTileId != Tile.NONE:
			passThruTile = False
			
		(stationX, stationY, stationExit) = self.board.getStationInfo(station)
		if self.board.b[stationX][stationY].type != BoardSquare.STATION:
			print "calculateTrackScore: starting point isn't station!"
			return (None, None, None)
			
		# No train is present, so there's no score.
		if self.board.b[stationX][stationY].trainPresent == Board.NO_TRAIN:
			return (None, None, None)
			
		(newX, newY, newExit, newType) = self.board.findNextTrackSection(stationX, stationY, stationExit)
		
		# Traverse the track while lengths of track still exist.  Keep track of the number of segments
		# seen.  If we've seen more than 255, assume we've hit some kind of infinite loop and exit the
		# while loop.
		loopCatcher = 0
		score = 0
		while newType == BoardSquare.PLAYED_TILE and loopCatcher < loopLimit:
			if passThruTileId != Tile.NONE and self.board.b[newX][newY].tileIndex == passThruTileId:
				passThruTile = True
			score = score + 1
			loopCatcher = loopCatcher + 1
			(oldX, oldY, oldExit) = (newX, newY, self.tilepool.getTile(self.board.b[newX][newY].tileIndex).findExit(newExit))
			(newX, newY, newExit, newType) = self.board.findNextTrackSection(oldX, oldY, oldExit)
			
		if loopCatcher >= loopLimit:
			print "calculateTrackScore: infinite loop caught"
			return (None, None, None)
		
		destination = self.board.b[newX][newY].type
		
		# If the destination isn't a station and we're calculating score for a human player, that's
		# bad.  Note: the 'checkCompletedStations' function can get here just fine.
		if passThruTileId == Tile.NONE and \
		   destination != BoardSquare.STATION and \
		   destination != BoardSquare.CENTRAL_STATION:
			return (None, None, None)
			
		# If the destination was a central station, double the score
		if destination == BoardSquare.CENTRAL_STATION:
			score = score * 2
		
		return (score, passThruTile, destination)
		
	def applyComputerMove(self):
		cur = SharedData.currentMove
		
		if cur.moveType == ComputerAI.ACTION_DRAW:
			if self.players[self.curPlayer].currentTileId == Tile.NONE:
				self.players[self.curPlayer].currentTileId = self.tilepool.drawRandomTile()
				self.selectedMoveIsReserveTile = False
				self.selectedMoveTileId = self.players[self.curPlayer].currentTileId
			else:
				self.players[self.curPlayer].reserveTileId = self.tilepool.drawRandomTile()
				self.selectedMoveIsReserveTile = True
				self.selectedMoveTileId = self.players[self.curPlayer].reserveTileId
			return True
		elif cur.moveType == ComputerAI.ACTION_PLAY:
			self.selectedMoveTileX = cur.moveX
			self.selectedMoveTileY = cur.moveY
			if cur.heldTile == ComputerAI.HELD_TILE_PRIMARY:
				self.selectedMoveTileId = self.players[self.curPlayer].currentTileId
				self.selectedMoveIsReserveTile = False
				if self.players[self.curPlayer].reserveTileId != Tile.NONE:
					self.players[self.curPlayer].currentTileId = self.players[self.curPlayer].reserveTileId
					self.players[self.curPlayer].reserveTileId = Tile.NONE
				else:
					self.players[self.curPlayer].currentTileId = Tile.NONE
			else:
				self.selectedMoveTileId = self.players[self.curPlayer].reserveTileId
				self.selectedMoveIsReserveTile = True
				self.players[self.curPlayer].reserveTileId = Tile.NONE
				
			# Do all the things normally done by a human player playing (mark legal moves, place the tile, highlight the space)
			self.board.markLegalMoves(self.tilepool.getTile(self.selectedMoveTileId), self.tilepool.numUnplayedTiles())
			self.board.placeTile(self.selectedMoveTileX, self.selectedMoveTileY, self.selectedMoveTileId)
			self.players[self.curPlayer].lastMoveX = self.selectedMoveTileX
			self.players[self.curPlayer].lastMoveY = self.selectedMoveTileY
			self.checkForCompletedTracks()
			return True
		elif cur.moveType == ComputerAI.ACTION_DISCARD:
			if cur.heldTile == ComputerAI.HELD_TILE_PRIMARY:
				self.selectedMoveTileId = self.players[self.curPlayer].currentTileId
				self.selectedMoveIsReserveTile = False
			else:
				self.selectedMoveTileId = self.players[self.curPlayer].reserveTileId
				self.selectedMoveIsReserveTile = True
			self.discardTile()
			return True
		elif cur.moveType == ComputerAI.ACTION_END_TURN:
			# Do nothing, since the computer doesn't have a proper 'end turn' action defined.  
			return True
		else:
			return False
		
		