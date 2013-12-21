#! /usr/bin/env python

#=============================================================================
#
# TIBoard.py
#
# Contains a single instance of a game board, including the board itself,
# station information, and other miscellaneous things.  The tile pool used to
# be here too, but it's not really a part of the board, and is now tracked
# in the Game class.  
#
# This implementation contains all functions from the C#/XNA version of the 
# game, and all are believed to work properly.
#=============================================================================
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
		if self.type == self.TILE:
			self.tileIndex = index
			self.type = self.PLAYED_TILE
			return True
		else:
			return False
		
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
	
		#self.tp = TilePool()
		
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
	
	#=======================================================================================
	# populateStations()
	#
	# Place the correct player trains in the starting stations, depending on how many
	# players are playing.
	#=======================================================================================		
	def populateStations(self, numPlayers):
		if numPlayers < 2 or numPlayers > 6:
			return False
		for i in range(0, self.NUM_STATIONS):
			(x, y, exit) = self.getStationInfo(i)
			self.b[x][y].trainPresent = self.BOARD_STATION_DATA[numPlayers][i]
		return True
		
	#=======================================================================================
	# findNextTrackSection()
	#
	# Given a position on the board and an exit point from that position, determine the
	# corresponding board position an entry point that the exit position leads to.
	# Returns a tuple containing the new position, entry point, and tile type contained 
	# there.
	#=======================================================================================		
	def findNextTrackSection(self, x, y, exit):
		newX = None
		newY = None
		enter = None
		
		if exit == 0:
			newX = x
			newY = y - 1
			enter = 5
		elif exit == 1:
			newX = x
			newY = y - 1
			enter = 4
		elif exit == 2:
			newX = x + 1
			newY = y
			enter = 7
		elif exit == 3:
			newX = x + 1
			newY = y
			enter = 6
		elif exit == 4:
			newX = x
			newY = y + 1
			enter = 1
		elif exit == 5:
			newX = x
			newY = y + 1
			enter = 0
		elif exit == 6:
			newX = x - 1
			newY = y
			enter = 3
		elif exit == 7:
			newX = x - 1
			newY = y
			enter = 2
		else:
			newX = x
			newY = y
			enter = exit
			
		return (newX, newY, enter, self.b[newX][newY].type)
	
	#=======================================================================================
	# markLegalMoves
	#
	# Given a tile, determines which moves on the board are legal positions for the tile.
	#
	# The basic rules determining legality are:
	#   - Tiles can only be placed in designated 'tile' locations, not on stations, and
	#     not in the corners between the groups of stations
	#   - Tiles must be placed adjacent to an outer station, or adjacent to an already
	#     played tile.
	#   - A tile must not be played that would immediately cause a train to go from the
	#     station to the same station with no intermediate tiles (so no placements that
	#     score only one point).  This also applies to pieces that move a train from
	#     a corner station to the adjacent corner station with a single piece of track
	#     (a move which would also score only one point).
	#   - If there are no tiles left to be drawn (that is, all tiles are either on the
	#     board already or in a player's hand), then the previous 2 rules do not apply,
	#     and a player may play the tile anywhere on the board where a tile could 
	#     legally go (so no playing on stations or in the corners between them!
	#=======================================================================================	
	def markLegalMoves(self, t, poolCount):
		# Start by marking all moves as legal
		for i in range(0, self.WIDTH):
			for j in range(0, self.HEIGHT):
				self.legalMove[i][j] = self.LEGAL_MOVE
				
		# Then, mark the 'obvious' illegal squares as such.  These include
		# the corners and stations, which can never hold a tile.
		for i in range(0, self.WIDTH):
			self.legalMove[i][0] = self.ILLEGAL_MOVE
			self.legalMove[i][self.HEIGHT-1] = self.ILLEGAL_MOVE
			
		for i in range(0, self.HEIGHT):
			self.legalMove[0][i] = self.ILLEGAL_MOVE
			self.legalMove[self.WIDTH-1][i] = self.ILLEGAL_MOVE
			
		self.legalMove[4][4] = self.ILLEGAL_MOVE
		self.legalMove[4][5] = self.ILLEGAL_MOVE
		self.legalMove[5][4] = self.ILLEGAL_MOVE
		self.legalMove[5][5] = self.ILLEGAL_MOVE
		
		# Mark all already-played squares as illegal.
		for i in range(1, self.WIDTH-1):
			for j in range(1, self.HEIGHT-1):
				if self.b[i][j].type == BoardSquare.PLAYED_TILE:
					self.legalMove[i][j] = self.ILLEGAL_MOVE
					
		# Check the inner station squares for adjacent tiles.  For a square not adjacent
		# to one of the outer stations, playing there is only legal if an adjacent square
		# has already been played on.  If squares in all four of the cardinal directions
		# are empty (or central stations), then the square should be marked as illegal.
		for i in range(2, self.WIDTH-2):
			for j in range(2, self.HEIGHT-2):
				if self.b[i+1][j].type != BoardSquare.PLAYED_TILE and \
				   self.b[i-1][j].type != BoardSquare.PLAYED_TILE and \
				   self.b[i][j-1].type != BoardSquare.PLAYED_TILE and \
				   self.b[i][j+1].type != BoardSquare.PLAYED_TILE:
					self.legalMove[i][j] = self.ILLEGAL_MOVE
		
		# Check the outer station squares for track lengths of 1.  This check will be
		# performed with 8 individual sets of checks (4 for the sides, 4 for the corners).
		# (This check is done because, unless there are no other options, a player cannot
		# place a tile adjacent to an outer station that would cause the train at that
		# station to score only one point (by making the destination station the same
		# as the source station with no intermediate tiles.)
		for i in range(0, self.WIDTH-1):
			if t.findExit(0) == 1:
				self.legalMove[i][1] = self.ILLEGAL_MOVE
			if t.findExit(5) == 4:
				self.legalMove[i][self.HEIGHT-2] = self.ILLEGAL_MOVE
				
		for i in range(0, self.HEIGHT-1):
			if t.findExit(3) == 2:
				self.legalMove[self.WIDTH-2][i] = self.ILLEGAL_MOVE
			if t.findExit(7) == 6:
				self.legalMove[1][i] = self.ILLEGAL_MOVE
				
		# Check the top left corner
		if t.findExit(6) == 1 or t.findExit(0) == 7:
			self.legalMove[1][1] = self.ILLEGAL_MOVE
			
		# Check the top right corner
		if t.findExit(2) == 1 or t.findExit(3) == 0:
			self.legalMove[self.WIDTH-2][1] = self.ILLEGAL_MOVE
			
		# Check the bottom right corner
		if t.findExit(4) == 3 or t.findExit(5) == 2:
			self.legalMove[self.WIDTH-2][self.HEIGHT-2] = self.ILLEGAL_MOVE
			
		# Check the bottom left corner
		if t.findExit(5) == 6 or t.findExit(4) == 7:
			self.legalMove[1][self.HEIGHT-2] = self.ILLEGAL_MOVE
			
		# Count the legal moves on the board
		legalMoves = 0
		for i in range(0, self.WIDTH):
			for j in range(0, self.HEIGHT):
				if self.legalMove[i][j] == self.LEGAL_MOVE:
					legalMoves = legalMoves + 1
		
		# At this point, if there are no legal moves, then check the poolCount.
		# If there are no unplayed cards left in the pool (that is, all tiles 
		# are either on the board or in player's hands), then all spaces where
		# a tile could normally be played but which have been marked illegal 
		# due to other rules will become marked legal (the rules state a tile
		# can be played anywhere if there are no otherwise legal moves and
		# there are no tiles left to be drawn).  
		if legalMoves == 0 and poolCount == 0:
			for i in range(1, self.WIDTH-1):
				for j in range(1, self.HEIGHT-1):
					if self.b[i][j].type == BoardSquare.TILE:
						self.legalMove[i][j] = self.LEGAL_MOVE
		else:
			return legalMoves
		
		# Finally, count the legal moves again and get an updated total.
		legalMoves = 0
		for i in range(0, self.WIDTH):
			for j in range(0, self.HEIGHT):
				if self.legalMove[i][j] == self.LEGAL_MOVE:
					legalMoves = legalMoves + 1
			
		return legalMoves
		
	#=======================================================================================
	# getStationInfo
	#
	# Given a station ID (0-31), returns the x and y position (in tiles) on the board,
	# as well as the train's exit position from the station (0-8).
	#=======================================================================================			
	def getStationInfo(self, station):
		# The top stations (0-7)
		if station >=0 and station < 8:
			x = 1 + station
			y = 0
			exit = 5
		# The right side stations (8-15)
		elif station >=8 and station < 16:
			x = 9
			y = 1 + (station - 8)
			exit = 7
		# The bottom stations (16-23)
		elif station >=16 and station < 24:
			x = 8 - (station - 16)
			y = 9
			exit = 1
		# The left stations (24-31)
		elif station >=24 and station < 32:
			x = 0
			y = 8 - (station - 24)
			exit = 3
		else:
			x = 0
			y = 0
			exit = 0
			
		return (x, y, exit)

	#=======================================================================================
	# getStationNumber
	#
	# Returns the station number of the station situated at the specified x and y position.
	# Returns -1 if no station is found there (or if the station is a central station). 
	#=======================================================================================		
	def getStationNumber(self, x, y):
		if x == 0:
			if y>=1 and y < (self.HEIGHT - 1):
				station = 32 - y
			else:
				station = -1
		elif x == (self.WIDTH - 1):
			if y>=1 and y < (self.HEIGHT - 1):
				station = 7 + y
			else:
				station = -1
		elif y == 0:
			if x>=1 and x < (self.WIDTH - 1):
				station = x - 1
			else:
				station = -1
		elif y == (self.HEIGHT - 1):
			if x>=1 and x < (self.WIDTH - 1):
				station = 24 - x
			else:
				station = -1
		else:
			station = -1
			
		return station
		
	#=======================================================================================
	# placeTile()
	#
	# If a move (placement if a specified tile index at a specified x, y location) is legal,
	# then make the move.
	#=======================================================================================
	def placeTile(self, x, y, index):
		if x < 0 or y < 0 or x >= self.WIDTH or y >= self.HEIGHT:
			return self.ILLEGAL_MOVE
			
		if self.b[x][y].type != BoardSquare.TILE:
			return self.ILLEGAL_MOVE
			
		if self.isLegalMove(x, y) == False:
			return self.ILLEGAL_MOVE
			
		if self.b[x][y].tileIndex != Tile.NONE:
			return self.ILLEGAL_MOVE
			
		result = self.b[x][y].setTileIndex(index)
		if result == True:
			return self.LEGAL_MOVE
		else:
			return self.ILLEGAL_MOVE
			
	#=======================================================================================
	# isLegalMove()
	#
	# Determines if a move is legal or not.
	#=======================================================================================
	def isLegalMove(self, x, y):
		if self.legalMove[x][y] == self.LEGAL_MOVE:
			return True
		else:
			return False
	
	#=======================================================================================
	# getTrackStatus()
	#
	# getter function for track status (for a specified station)
	#=======================================================================================	
	def getTrackStatus(self, index):
		if index < 0 or index > self.NUM_STATIONS:
			return None
		
		return self.trackStatus[index]
		
	#=======================================================================================
	# setTrackStatus()
	#
	# setter function for track status (for a specified station and status value)
	#=======================================================================================			
	def setTrackStatus(self, index, status):
		if index >=0 and index < self.NUM_STATIONS:
			self.trackStatus[index] = status
			
	#=======================================================================================
	# removeTile()
	#
	# removes a tile from the board at the specified location.
	#=======================================================================================				
	def removeTile(self, x, y):
		if x < 0 or y < 0 or x >= self.WIDTH or y >= self.HEIGHT:
			return False
		
		if self.b[x][y].type != BoardSquare.PLAYED_TILE:
			return False
			
		if self.b[x][y].tileIndex == Tile.NONE:
			return False
			
		self.b[x][y].tileIndex = Tile.NONE
		self.b[x][y].type = BoardSquare.TILE
		return True
		
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
		
	#=======================================================================================
	# printLegalMoves()
	#=======================================================================================		
	def printLegalMoves(self, t):
		self.markLegalMoves(t, 60)
		print "------------------- (legal moves)"
		for i in range(0, self.HEIGHT):
			for j in range(0, self.WIDTH):
				print self.legalMove[j][i],
			print ""
		print "-------------------"

	#=======================================================================================
	# printStationInfo()
	#=======================================================================================		
	def printStationInfo(self):
		for i in range(0, self.HEIGHT):
			for j in range(0, self.WIDTH):
				print str(self.getStationNumber(j, i)), 
			print ""
			