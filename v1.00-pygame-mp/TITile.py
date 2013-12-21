#! /usr/bin/env python

#=============================================================================
#
# TITile.py
#
# Information about a tile (a playable piece that contains various track
# sections.  The tile type is defined by the source and destinations of the
# track sections on the piece, as defined by a set of 8 locations.  
#
# +--|--|--+
# |  0  1  |
# -7      2-
# |        |
# -6      3-
# |  5  4  |
# +--|--|--+
#
# Each track starts at one of the 8 locations and exits at another, and on
# each tile, every one of the 8 locations is part of one of the tracks.  
# In other words, there are always four track sections on a tile.  
#
# The tiles also contain a 'strip offset', which is used to assist with
# finding the correct graphical representation in the tile strip, a giant
# graphic containing all of the possible tiles. 
#
# This implementation contains all functions from the C#/XNA version of the 
# game, and all are believed to work properly.
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
		