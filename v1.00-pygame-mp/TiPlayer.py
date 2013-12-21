#! /usr/bin/env python

#=============================================================================
# TIPlayer.py
#
# A class defining a player.  The player can be human or AI, and keeps track
# of the last move made (so the UI can highlight it).
#
# ** This implementation contains all functions from the C#/XNA version of the 
# game.  Not all functions are debugged though.
#=============================================================================

from TITile import *

class Player:
	# The type of player this is (human or computer controlled)
	NOBODY = 0
	HUMAN = 1
	COMPUTER = 2
	
	# The three AI levels - dumb, dumber, and dumbest (listed in reverse order below) :)
	AI_DEFAULT = 0
	AI_SMARTER = 1
	AI_SMARTEST = 2
	
	# A placeholder for the initial value of lastMoveX and lastMoveY.
	INVALID_LAST_MOVE = -1
	
	def __init__(self, control, initialScore, computerAI):
		# Check to make sure the 'controlled by' option passed in is valid.
		if control == self.NOBODY or control == self.HUMAN or control == self.COMPUTER:
			self.controlledBy = control
		else:
			self.controlledBy = self.NOBODY
			
		# Make sure the initial score passed in is valid.
		if initialScore >= 0:
			self.score = initialScore
		else:
			score = 0
		
		# Blank out the player's tiles.
		self.currentTileId = Tile.NONE
		self.reserveTileId = Tile.NONE
		
		self.numStationsComplete = 0
		
		# Make sure the passed in AI level is valid.
		if computerAI == self.AI_DEFAULT or computerAI == self.AI_SMARTER or computerAI == self.AI_SMARTEST:
			self.computerAiLevel = computerAI
		else:
			self.computerAiLevel == self.AI_DEFAULT
			
		# Clear the 'last move' positions.
		self.lastMoveX = self.INVALID_LAST_MOVE
		self.lastMoveY = self.INVALID_LAST_MOVE