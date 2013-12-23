#! /usr/bin/env python

#=============================================================================
# TITestSuite.py
#
# Just some tests thrown together to test parts of the non-graphics code.
# They're kind of tricky (and ported from a language with static typing),
# so everything needs more testing to ensure that the right variables are
# being passed around everywhere.
#=============================================================================

import copy, sys
from TIBoard import BoardSquare, Board
from TITilePool import TilePool
from TITile import Tile
from TISharedData import SharedData
from TIPlayer import Player
from TIGame import Game
from TIComputerAI import ComputerAI, ComputerAIPacket

g = Game()

def runBoardTests():

	# For all of these tests, the reference boards are arranged in proper
	# row/column order.  The boards as stored in the Board class are 
	# backwards (so checking b[i][j] against defaultBoard[j][i] is actually
	# correct.
	passed = True
	
	defaultBoard = [[3,0,0,0,0,0,0,0,0,3],
	                [0,2,2,2,2,2,2,2,2,0],
					[0,2,2,2,2,2,2,2,2,0],
					[0,2,2,2,2,2,2,2,2,0],
					[0,2,2,2,1,1,2,2,2,0],
					[0,2,2,2,1,1,2,2,2,0],
					[0,2,2,2,2,2,2,2,2,0],
					[0,2,2,2,2,2,2,2,2,0],
					[0,2,2,2,2,2,2,2,2,0],
					[3,0,0,0,0,0,0,0,0,3]]
					
	defaultTileIndices = [[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
						  [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]]
						  
	b = Board()
	b.populateStations(2)
	tilePool = TilePool()	
	
	# Check the board contents to make sure they match the reference board
	for i in range(0, Board.WIDTH):
		for j in range(0, Board.HEIGHT):
			if b.b[i][j].type != defaultBoard[j][i]:
				print "  - boardTest failed in defaultBoard[" + str(i) + "][" + str(j) + "]"
				passed = False
			if b.b[i][j].tileIndex != defaultTileIndices[j][i]:
				print "  - boardTest failed in defaultTileIndices[" + str(i) + "][" + str(j) + "]"			
				passed = False
	
	# Try placing a few tiles that are illegal to play
	# Place the '4 U-shaped' piece (always illegal as a first move) on the board in 2 different places
	result = testTilePlacement(copy.deepcopy(b), tilePool, 1, 1, 0, False)
	if result == False:
		passed = False
	result = testTilePlacement(copy.deepcopy(b), tilePool, 0, 0, 0, False)
	if result == False:
		passed = False
	# Try placing a tile somewhere in the middle of the board
	result = testTilePlacement(copy.deepcopy(b), tilePool, 2, 2 ,2, False)
	if result == False:
		passed = False
		
	# Try placing a few tiles that are legal to play
	bC = copy.deepcopy(b)
	result = testTilePlacement(bC, tilePool, 1, 1, 2, True)
	if result == False:
		passed = False
	result = testTilePlacement(bC, tilePool, 1, 2, 2, True)
	if result == False:
		passed = False
	result = testTilePlacement(bC, tilePool, 1, 3, 8, True)
	if result == False:
		passed = False
		
	return passed

def testTilePlacement(b, tp, x, y, tileIndex, expectedResult):
	b.markLegalMoves(tp.getTile(tileIndex), tp.numUnplayedTiles())
	result = b.placeTile(x, y, tileIndex)

	if result == Board.ILLEGAL_MOVE and expectedResult == True:
		print " - testTilePlacement failed placing illegal tile " + str(tileIndex) + " at (" + str(x) + "," + str(y) + ")"
		return False
	elif result == Board.LEGAL_MOVE and expectedResult == False:
		print " - testTilePlacement failed placing legal tile " + str(tileIndex) + " at (" + str(x) + "," + str(y) + ")"
		return False
		
	return True

def runTilePoolTests():
	tp = TilePool()
	tiles = []
	
	for i in range(0,60):
		val = tp.drawRandomTile()
		if val in tiles or val == Tile.ERROR:
			print "  - runTilePoolTests: tile drawn was already drawn before"
			return False
		tiles.append(val)

	return True

def setUpTextGameInstance():
	
	# Get number of players
	while True:
		players = raw_input("Number of players (2-6):").strip()
		try:
			players = int(players)
		except:
			players = 2
		if players >= 2 and players <= 6:
			break
			
	# Determine how many human/CPU players
	playerTypes = []
	for i in range(0, players):
		while True:
			tmpType = raw_input("Player " + str(i+1) + " - [H]uman or [C]omputer:").strip()
			if tmpType[0] == 'H' or tmpType[0] == 'h':
				playerTypes.append(Player.HUMAN)
				break
			elif tmpType[0] == 'C' or tmpType[0] == 'c':
				playerTypes.append(Player.COMPUTER)
				break
				
	# Determine the level
	while True:
		level = raw_input("AI level (1/2/3 - 1 is easy, 3 is hard):").strip()
		try:
			level = int(level)
		except:
			level = 1
		if level >=1 and level <=3:
			break
	
	# Fill the shared data structure with this information
	SharedData.selectedPlayers = players
	SharedData.playerState = []
	for i in range(0, len(playerTypes)):
		SharedData.playerState.append(playerTypes[i])
	
	if level == 1:
		g.defaultAILevel = Game.OPTION_AI_EASY
	elif level == 2:
		g.defaultAILevel = Game.OPTION_AI_MEDIUM
	elif level == 3:
		g.defaultAILevel = Game.OPTION_AI_HARD
		
	g.initPlayersFromUi()
	
	print ""
	print "Number of players: " + str(SharedData.selectedPlayers)
	print "Player types:"
	for i in range(0, SharedData.selectedPlayers):
		print "   " + str(i+1) + " - ",
		if SharedData.playerState[i] == Player.HUMAN:
			print "Human"
		else:
			print "Computer",
			if g.defaultAILevel == Game.OPTION_AI_EASY:
				print "(easy)"
			elif g.defaultAILevel == Game.OPTION_AI_MEDIUM:
				print "(medium)"
			elif g.defaultAILevel == Game.OPTION_AI_HARD:
				print "(hard)"
			else:
				print "(unknown)"
	print ""
	
# This is a bare minimum game player that supports computer players only
# human players can be selected, but they auto-pass.  It's mostly to make
# sure that the AI routines (and their support functions) don't crash.
def runGameInstance():
	g.changeState(Game.GAME_STATE_NEXT_PLAYER_TURN)
	finished = False
	
	while finished == False:
		g.board.printBoard()
		print "Player " + str(g.curPlayer +1) + "'s turn:"
		if SharedData.playerState[g.curPlayer] == Player.HUMAN:
			print "Passing..."
			g.changeState(Game.GAME_STATE_END_TURN)	
		else:
			print "Computer turn now!"
			SharedData.currentMove = ComputerAI.determineNextMove(g, None)
			while SharedData.currentMove.moveType != ComputerAI.ACTION_END_TURN:
				g.applyComputerMove()
				SharedData.previousMove = SharedData.currentMove
				SharedData.currentMove = ComputerAI.determineNextMove(g, SharedData.previousMove)
			g.changeState(Game.GAME_STATE_END_TURN)
		
		if g.gameState == Game.GAME_STATE_GAME_FINISHED:
			print "Game is done!"
			finished = True
			
	print "Final scores:"
	for i in range(0, SharedData.selectedPlayers):
		print "  Player " + str(i+1) +  " - " + str(g.players[i].score)
		
def main():
	setUpTextGameInstance()
	runGameInstance()
	
	#result = runBoardTests()
	#if result == False:
	#	print "runBoardTests: FAILED"
	#else:
	#	print "runBoardTests: PASSED"
	#result = runTilePoolTests()
	#if result == False:
	#	print "runTilePoolTests: FAILED"
	#else:
	#	print "runTilePoolTests: PASSED"
	
if __name__ == '__main__':
	main()