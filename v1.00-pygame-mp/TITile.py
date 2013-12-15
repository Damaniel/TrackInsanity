#! /usr/bin/env python

#=============================================================================
#
# TITile.py
#
#=============================================================================

class Tile:
	# The number of exitable positions on a tile.  TrackInsanity tiles have
	# 8 exits.
	NUM_EXITS = 8
	
	# Assorted error states
	OK      =  0
	INVALID = -1
	ERROR   = -2
	
	# A location with no tile is set to this value (Tile.NONE)
	NONE    = -1
	
	def __init__(self):
		# Exits are described further in the __init__ method of TilePool,
		# since the master list of tile types is stored there.
		self.exits = []
		self.tileStripOffset = None		
		
		# Initialize the tile (give it 8 exits)
		for i in range(0, self.NUM_EXITS):
			self.exits.append(None)
	
	#=======================================================================================
	# findExit
	#
	# Returns the exit position for a given entry position.
	#=======================================================================================	
	def findExit(self, entry):
		if entry < 0 or entry >= self.NUM_EXITS:
			return None
		
		return self.exits[entry]
	
	#=======================================================================================
	# setExits
	#
	# Initializes a full set of exits for the tile.  values must be a list with at least
	# NUM_EXITS elements.
	#=======================================================================================		
	def setExits(self, values):
		for i in range(0, self.NUM_EXITS):
			self.setExit(i, values[i])

	#=======================================================================================
	# setExit
	#
	# Sets the exit position for a single tile entry position.
	#=======================================================================================			
	def setExit(self, index, value):
		self.exits[index] = value
		