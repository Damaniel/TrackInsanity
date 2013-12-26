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
	playerAvatars = None
	selectedOption = None
	
	# AI move packets
	currentMove = None
	previousMove = None
	
	cpuDelayTicks = None
	
	def __init__(self):
		self.reset()
		
	def reset(self):
		SharedData.selectedPlayers = MIN_PLAYERS
		SharedData.playerState = []
		SharedData.playerState.append(Player.HUMAN)
		for i in range(0, MAX_PLAYERS-1):
			SharedData.playerState.append(Player.COMPUTER)
			
		# These are the selected avatars for each player.
		SharedData.playerAvatars = [0, 1, 2, 3, 4, 5]
		
		# These are the selected options from the user, or will be when they get
		# updated by the UI.
		SharedData.selectedOption = [0, 0, 1, 1, 9, 9]
		
		SharedData.exitGame = False
		
		