#! /usr/bin/env python

#=============================================================================
# TISharedData
#
# Contains a few miscellaneous pieces of data needed by more than one class,
# and which don't really fit anywhere else.  
#
# ** This implementation contains all functions from the C#/XNA version of the 
# game.  Not all functions are debugged though.
#=============================================================================

from TIConsts import *
from TIPlayer import *

class SharedData:
	cpuMoveAllowed = None
	exitGame = None
	selectedPlayers = None
	playerState = None
	
	# AI move packets
	currentMove = None
	previousMove = None
	
	cpuDelayTicks = None
	
	def __init__(self):
		self.reset()
		
	def reset(self):
		self.selectedPlayers = MIN_PLAYERS
		self.playerState = []
		self.playerState.append(Player.HUMAN)
		for i in range(0, MAX_PLAYERS-1):
			self.playerState.append(Player.COMPUTER)
		self.exitGame = False
		
		