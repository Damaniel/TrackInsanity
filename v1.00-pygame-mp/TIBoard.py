#! /usr/bin/env python

#=============================================================================
#
# TIBoard.py
#
# Contains a single instance of a game board, including the board itself,
# station information, the tile pool, and other things.
#
#=============================================================================

from TITilePool import *
from TITile import *

class BoardSquare:
	# Constants used to specify the content of the board sqaure.  Every 
	# position on the board is either a station, a central station (one of the
	# four in the middle), a tile square (a location where a tile can be 
	# played), a corner square (purely cosmetic), and a played tile square, 
	# (a location where a tile can be, and has already beeen played).
	STATION = 0
	CENTRAL_STATION = 1
	TILE = 2
	CORNER = 3
	PLAYED_TILE = 4
	
	def __init__(self, t=None, tP=None, tI=None):
		# Type is either a station, central station, location where a tile
		# can be placed, a corner, or a location where a tile has bee
		# placed
		self.type = t
		
		# trainPresent is either NO_TRAIN, or an index that represents one
		# of the (up to) 6 players
		self.trainPresent = tP
		
		# If the type is a PLAYED_TILE, this is the index of tile contained
		# in it (offset into the tile pool)
		self.tileIndex = tI

	#=======================================================================================
	# setTileIndex()
	#
	# Sets the tile index of this board square (if a tile has been placed).
	#=======================================================================================		
	def setTileIndex(self, index):
		if self.type == TILE:
			self.tileIndex = index
			self.type = PLAYED_TILE
			
class Board:

	# The board width and height (in tiles) - though the outermost rows aren't really (directly) used
	# in game for placement of tiles.  
	WIDTH = 10
	HEIGHT = 10
	NUM_STATIONS = 32
	
	# Used to mark whether the track attached to each of the starting stations is currently incomplete
	# (not connected to a terminal station), complete (connected to a terminal station), or
	# processed (completed AND scored).  
	TRACK_NOT_COMPLETE = 0
	TRACK_COMPLETE = 1
	TRACK_PROCESSED = 2
	
	# Used to mark positions on the board as legal or illegal for a particular player.
	ILLEGAL_MOVE = 0
	LEGAL_MOVE = 1
	
	# The overall station ownership array for each number of possible players.  This set of arrangements is 
	# fixed for all possible boards, so is defined as a member of the class itself.
	BOARD_STATION_DATA = {}
	
	# Train ownership flags.  Used to mark whether a train sits at a particular
	# station square or not.
	NO_TRAIN = 0
	P1_TRAIN = 1
	P2_TRAIN = 2
	P3_TRAIN = 3
	P4_TRAIN = 4
	P5_TRAIN = 5
	P6_TRAIN = 6
	
	def __init__(self):
		# Population BOARD_STATION_DATA with the station information for each possible number of players.  Between 2 and 6
		# players (the valid range), the 32 stations are subdivided among the players equally.  In the case of 3, 5, and 6
		# players, 2 stations are left empty.  The index values in these lists represent the player number that owns a 
		# particular station (or 0 if it is owned by no player at all)
		self.BOARD_STATION_DATA[2] = [2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1]
		self.BOARD_STATION_DATA[3] = [2, 1, 3, 2, 3, 2, 1, 3, 1, 3, 2, 1, 3, 1, 2, 0, 0, 3, 1, 2, 3, 1, 2, 3, 2, 3, 1, 2, 1, 3, 2, 1]
		self.BOARD_STATION_DATA[4] = [3, 4, 1, 2, 4, 3, 2, 1, 4, 3, 2, 1, 4, 3, 1, 2, 4, 3, 1, 2, 3, 4, 2, 1, 3, 4, 2, 1, 4, 3, 1, 2]
		self.BOARD_STATION_DATA[5] = [2, 4, 3, 5, 2, 1, 3, 5, 4, 2, 5, 1, 4, 2, 3, 0, 0, 1, 3, 5, 4, 2, 1, 5, 3, 4, 1, 2, 3, 4, 5, 1]
		self.BOARD_STATION_DATA[6] = [2, 1, 5, 3, 2, 4, 6, 3, 5, 2, 1, 6, 5, 3, 4, 0, 0, 1, 2, 4, 3, 6, 5, 4, 1, 3, 2, 6, 1, 5, 4, 6]
		
		# b is the actual BoardSquare representation of the board
		self.b = []
		self.legalMove = []
		self.trackStatus = []
	
		self.tp = TilePool()
		
		self.initBoardSquares()
		self.initOtherArrays()
	
	#=======================================================================================
	# initBoardSquares()
	#
	# Place all the default values into the BoardSquare structures that make up the board.
	#=======================================================================================
	def initBoardSquares(self):
		for i in range(0, self.HEIGHT):
			self.b.append([])
			for j in range(0, self.WIDTH):
				self.b[i].append(BoardSquare())
		
		# Clear the 'tile present' status for every square on the board
		for i in range(0, self.HEIGHT):
			for j in range(0, self.WIDTH):
				self.b[i][j].tileIndex = Tile.NONE
				self.b[i][j].trainPresent = self.NO_TRAIN
				
		# Set the status of the outer station squares
		for i in range(1, self.WIDTH - 1):
			self.b[i][0].type = BoardSquare.STATION
			self.b[i][self.HEIGHT-1].type = BoardSquare.STATION
			
		for i in range(1, self.HEIGHT - 1):
			self.b[0][i].type = BoardSquare.STATION
			self.b[self.WIDTH-1][i].type = BoardSquare.STATION
			
		# Set the type of the squares that hold tiles
		for i in range(1, self.WIDTH - 1):
			for j in range(1, self.HEIGHT - 1):
				self.b[i][j].type = BoardSquare.TILE
				
		# Overwrite the center squares with the CENTRAL_STATION type
		self.b[4][4].type = BoardSquare.CENTRAL_STATION
		self.b[4][5].type = BoardSquare.CENTRAL_STATION
		self.b[5][4].type = BoardSquare.CENTRAL_STATION
		self.b[5][5].type = BoardSquare.CENTRAL_STATION
				
		# Set the corner square types
		self.b[0][0].type = BoardSquare.CORNER
		self.b[0][self.HEIGHT-1].type = BoardSquare.CORNER
		self.b[self.WIDTH-1][0].type = BoardSquare.CORNER
		self.b[self.WIDTH-1][self.HEIGHT-1].type = BoardSquare.CORNER
		
	#=======================================================================================
	# initOtherArrays()
	#
	# Place all the default values into the non-BoardSquare components of the board.
	#=======================================================================================		
	def initOtherArrays(self):
		# Mark all stations as 'track incomplete'
		for i in range(0, self.NUM_STATIONS):
			self.trackStatus.append(self.TRACK_NOT_COMPLETE)
			
		# Mark all positions on the board as 'illegal move'.  They'll be calculated
		# as needed for each player
		for i in range(0, self.HEIGHT):
			self.legalMove.append([])
			for j in range(0, self.WIDTH):
				self.legalMove[i].append(self.ILLEGAL_MOVE)
	
	# === Debug printing functions are below: ============================================
	
	#=======================================================================================
	# printBoard()
	#=======================================================================================		
	def printBoard(self):
		print "------------------- (square type)"	
		for i in range(0, self.HEIGHT):
			for j in range(0, self.WIDTH):
				print str(self.b[j][i].type),
			print ""
		print "------------------- (tile index)"
		for i in range(0, self.HEIGHT):
			for j in range(0, self.WIDTH):
				print str(self.b[j][i].tileIndex),
			print ""		
		print "-------------------"
		
# This is testing code!
def main():
	b = Board()
	
	# Let's print something.
	b.printBoard()
	b.tp.printTilePool()
	
if __name__ == '__main__':
	main()