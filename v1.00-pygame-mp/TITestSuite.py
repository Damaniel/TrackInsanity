#! /usr/bin/env python

#=============================================================================
# TITestSuite.py
#
# Just some tests thrown together to test parts of the non-graphics code.
# They're kind of tricky (and ported from a language with static typing),
# so everything needs more testing to ensure that the right variables are
# being passed around everywhere.
#=============================================================================

import copy
from TIBoard import BoardSquare, Board
from TITilePool import TilePool
from TITile import Tile

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
	
def main():
	result = runBoardTests()
	if result == False:
		print "runBoardTests: FAILED"
	else:
		print "runBoardTests: PASSED"
	result = runTilePoolTests()
	if result == False:
		print "runTilePoolTests: FAILED"
	else:
		print "runTilePoolTests: PASSED"
	
if __name__ == '__main__':
	main()