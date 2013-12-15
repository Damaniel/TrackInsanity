#! /usr/bin/env python

#=============================================================================
#
# TITilePool.py
#
# A TilePool is just that - a collection of tiles used in a single game.
# Each tile in the pool has one of a number of different track arrangements,
# and can either be 'played' or 'unplayed' depending on whether it's on the
# board or not.
#
#=============================================================================

from TITile import *

class TilePool:

	# The number of tiles in the tile pool
	NUM_TILES = 60
	
	# Flags used to determine whether a particular tile is played or not.
	PLAYED = 0
	UNPLAYED = 1
	
	# The raw 'exit info' for each of the 24 tile types.  Each exit takes
	# a value from 0-7, which are arranged two per side of the tile,
	# in clockwise order from the top left.  Each tile has 9 offsets.  The
	# first 8 represent each of these 8 positions.  The value assigned to
	# it is the 'exit' position from that tile.  Therefore, if index 0 has
	# a value of 1, then this means that the top left entrance exits on 
	# the top right (making a U shape on the tile itself.
	#
	# The ninth value is the tile index offset - that is, how many tiles
	# away from the leftmost tile in the (graphical) tilestrp the 
	# physical represntation of this tile is.  This is used to render
	# the correct tiles in the player UI and on the board.
	#
	TileData = [[ 1, 0, 3, 2, 5, 4, 7, 6, 1],
				[ 1, 0, 3, 2, 5, 4, 7, 6, 1],
				[ 5, 4, 7, 6, 1, 0, 3, 2, 2],
				[ 5, 4, 7, 6, 1, 0, 3, 2, 2],
				[ 5, 4, 7, 6, 1, 0, 3, 2, 2],
				[ 5, 4, 7, 6, 1, 0, 3, 2, 2],
				[ 1, 0, 7, 6, 5, 4, 3, 2, 3],
				[ 1, 0, 7, 6, 5, 4, 3, 2, 3],
				[ 7, 2, 1, 4, 3, 6, 5, 0, 4],
				[ 7, 2, 1, 4, 3, 6, 5, 0, 4],
				[ 7, 2, 1, 6, 5, 4, 3, 0, 5],
				[ 7, 2, 1, 6, 5, 4, 3, 0, 5],
				[ 7, 4, 3, 2, 1, 6, 5, 0, 6],
				[ 7, 4, 3, 2, 1, 6, 5, 0, 6],
				[ 5, 4, 3, 2, 1, 0, 7, 6, 7],
				[ 5, 4, 3, 2, 1, 0, 7, 6, 7],
				[ 5, 2, 1, 4, 3, 0, 7, 6, 8],
				[ 5, 2, 1, 4, 3, 0, 7, 6, 8],
				[ 1, 0, 7, 4, 3, 6, 5, 2, 9],
				[ 1, 0, 7, 4, 3, 6, 5, 2, 9],
				[ 7, 6, 5, 4, 3, 2, 1, 0, 10],
				[ 7, 6, 5, 4, 3, 2, 1, 0, 10],
				[ 7, 6, 5, 4, 3, 2, 1, 0, 10],
				[ 3, 2, 1, 0, 5, 4, 7, 6, 11],
				[ 3, 2, 1, 0, 5, 4, 7, 6, 11],
				[ 1, 0, 5, 4, 3, 2, 7, 6, 12],
				[ 1, 0, 5, 4, 3, 2, 7, 6, 12],
				[ 7, 6, 3, 2, 5, 4, 1, 0, 13],
				[ 7, 6, 3, 2, 5, 4, 1, 0, 13],
				[ 1, 0, 3, 2, 7, 6, 5, 4, 14],
				[ 1, 0, 3, 2, 7, 6, 5, 4, 14],
				[ 3, 6, 5, 0, 7, 2, 1, 4, 15],
				[ 3, 6, 5, 0, 7, 2, 1, 4, 15],
				[ 3, 6, 7, 0, 5, 4, 1, 2, 16],
				[ 3, 6, 7, 0, 5, 4, 1, 2, 16],
				[ 1, 0, 5, 6, 7, 2, 3, 4, 17],
				[ 1, 0, 5, 6, 7, 2, 3, 4, 17],
				[ 5, 2, 1, 6, 7, 0, 3, 4, 18],
				[ 5, 2, 1, 6, 7, 0, 3, 4, 18],
				[ 5, 2, 1, 6, 7, 0, 3, 4, 18],
				[ 5, 2, 1, 6, 7, 0, 3, 4, 18],
				[ 3, 4, 7, 0, 1, 6, 5, 2, 19],
				[ 3, 4, 7, 0, 1, 6, 5, 2, 19],
				[ 3, 4, 7, 0, 1, 6, 5, 2, 19],
				[ 3, 4, 7, 0, 1, 6, 5, 2, 19],
				[ 7, 4, 5, 6, 1, 2, 3, 0, 20],
				[ 7, 4, 5, 6, 1, 2, 3, 0, 20],
				[ 7, 4, 5, 6, 1, 2, 3, 0, 20],
				[ 7, 4, 5, 6, 1, 2, 3, 0, 20],
				[ 3, 4, 5, 0, 1, 2, 7, 6, 21],
				[ 3, 4, 5, 0, 1, 2, 7, 6, 21],
				[ 5, 6, 7, 4, 3, 0, 1, 2, 22],
				[ 5, 6, 7, 4, 3, 0, 1, 2, 22],
				[ 5, 6, 7, 4, 3, 0, 1, 2, 22],
				[ 5, 6, 7, 4, 3, 0, 1, 2, 22],
				[ 3, 2, 1, 0, 7, 6, 5, 4, 23],
				[ 3, 2, 1, 0, 7, 6, 5, 4, 23],
				[ 3, 2, 1, 0, 7, 6, 5, 4, 23],
				[ 5, 6, 3, 2, 7, 0, 1, 4, 24],
				[ 5, 6, 3, 2, 7, 0, 1, 4, 24]]
	
	def __init__(self):
		# The list of individual tiles in the tile pool
		self.t = []
	
		# A list of tile IDs that represent played and unplayed tiles
		self.playedTiles = []
		self.unplayedTiles = []
	
		# A list that contains the played/unplayed status of each tile in the pool
		# (this is an alternative representation of playedTiles/unplayedTiles)
		self.tileStatus = []	
		
		# Create the new tiles and mark them as unplayed
		for i in range(0, self.NUM_TILES):
			self.t.append(Tile())
			self.tileStatus.append(self.UNPLAYED)
			
		# Set the entry/exit values of each tile in the pool 
		for i in range(0, self.NUM_TILES):
			self.t[i].setExits(self.TileData[i])
			self.t[i].tileStripOffset = self.TileData[i][8]
		
		# Fill the lists of played and unplayed tile indices
		self.calculatePlayedTiles()
		self.calculateUnplayedTiles()
			
	#=======================================================================================
	# getTile()
	#
	# Get a specific tile from the pool
	#=======================================================================================
	def getTile(self, index):
		if index < 0 or index >= self.NUM_TILES:
			return None
		else:
			return self.t[index]
	
	#=======================================================================================
	# getTileIndexForTileId()
	#
	# Determine where in the tile (graphics) strip the current tile id is.
	#=======================================================================================
	def getTileIndexForTileId(self, id):
		if id < 0 or id >= self.NUM_TILES:
			return Tile.NONE
		else:
			return self.t[id].tileStripOffset
			
	#=======================================================================================
	# drawRandomTile()
	#
	# Returns a random unplayed tile from the pool.
	#=======================================================================================			
	def drawRandomTile(self):
		return 0
		
	#=======================================================================================
	# returnTileToPool()
	#
	# Returns a tile from somewhere (the player's hand) to the tile pool.
	#=======================================================================================			
	def returnTileToPool(self, index):
		if self.numPlayedTiles() == 0:
			return Tile.ERROR
		if self.tileStatus[index] == self.UNPLAYED:
			return Tile.ERROR
			
		self.tileStatus[index] = self.UNPLAYED
		self.calculatePlayedTiles()
		self.calculateUnplayedTiles()		
		return Tile.OK
		
	#=======================================================================================
	# calculatePlayedTiles()
	#
	# Determines which tiles have been played and add them to the played tiles list.
	#=======================================================================================			
	def calculatePlayedTiles(self):
		self.playedTiles = []
		for i in range(0, self.NUM_TILES):
			if self.tileStatus[i] == self.PLAYED:
				self.playedTiles.append(i)
			
	#=======================================================================================
	# calculateUnplayedTiles()
	#
	# Determines which tiles have not been played and add them to the unplayed tiles list.
	#=======================================================================================	
	def calculateUnplayedTiles(self):
		self.unplayedTiles = []
		for i in range(0, self.NUM_TILES):
			if self.tileStatus[i] == self.UNPLAYED:
				self.unplayedTiles.append(i)
	
	#=======================================================================================
	# numPlayedTiles
	#
	# Returns the number of played tiles in the pool
	#=======================================================================================	
	def numPlayedTiles(self):
		return len(self.playedTiles)

	#=======================================================================================
	# numUnplayedTiles
	#
	# Returns the number of unplayed tiles in the pool
	#=======================================================================================			
	def numUnplayedTiles(self):
		return len(self.unplayedTiles)
		
	#=======================================================================================
	# printTilePool
	#
	# Prints a verbose listing of each tile in the pool.
	#=======================================================================================			
	def printTilePool(self):
		for i in range(0, self.NUM_TILES):			
			print "Tile " + str(i+1) + ":"
			print "   Exits: ",
			for j in range(0, 8):
				print self.t[i].exits[j],
			print ""
			print "   Tile strip offset: " + str(self.t[i].tileStripOffset)
			print "   Played? ",
			if self.tileStatus[i] == self.PLAYED:
				print "Yes"
			else:
				print "No"
			
		