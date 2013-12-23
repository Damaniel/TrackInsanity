#! /usr/bin/env python

#============================================================================
# TIComputerAI.py
#
# Contains a set of classes:  
#   - ComputerAIPacket - information about a computer player's move,
#                        (draw/pass/play), along with location information
#   - AIMoveEval - information about the results of playing a particular tile
#                  in a given location on the board.
#   - ComputerAI - the driver class for a computer AI player - based on the
#                  AI difficulty, evaluates all legal moves for a player,
#                  ranks them based on relative value, and picks a move
#                  to make.  The ComputerAI can make more than one move per
#                  round (like draw, then play), if desired.
#
# ** This implementation contains all necessary functions from the C#/XNA 
# version of the game.  Not all functions are debugged though.
#=============================================================================
import copy
from TIGame import *
from TITile import *
from TIBoard import *

class ComputerAIPacket:
	def __init__(self):
		self.moveType = None
		self.moveX = None
		self.moveY = None
		self.heldTile = None
		
class AIMoveEval:
	def __init__(self):
		self.tileType = None
		self.tileX = None
		self.tileY = None
		self.value = 0
	
	def __init__(self, tt, tx, ty, tv):
		self.tileType = tt
		self.tileX = tx
		self.tileY = ty
		self.value = tv
			
class ComputerAI:
	ACTION_DRAW = 0
	ACTION_PLAY = 1
	ACTION_DISCARD = 2
	ACTION_END_TURN = 3
	
	HELD_TILE_PRIMARY = 0
	HELD_TILE_RESERVE = 1
	
	COMPLETE_TRACK_WEIGHT = 1.0
	INCOMPLETE_TRACK_WEIGHT = 3.0
	COMPLETE_CENTRAL_STATION_WEIGHT = 2.0
	
	evalList = None
	
	# Determines a valid move to make next, based on the current state of the game
	# (held in 'g'), and the last move (if any).
	@staticmethod
	def determineNextMove(g, lastMove):
		# How to decide on a move (V1):
        # (Taken from my previous version of the code)
		#
        #   The following steps are used to help decide on a move.
        # 
        #   1.  If the computer has no tiles, draw one.
        #   2.  If the computer has one tile, but it has no legal moves,
        #         draw one.
        #   3.  If the computer has two tiles, but neither has legal moves,
        #       discard one
        #   4.  If the computer is to draw a tile, but no tiles remain,
        #       and any held tile cannot be played, then pass, otherwise
        #       play.
        #   5.  If the computer has made more than 2 moves in any particular
        #       round, then pass (this is to prevent an infinite move loop)
        #   6.  When the computer is to play, do the following for each held tile:
        #        - Place the tile in each legal location
        #        - For each such location:
        #           -  the value of the location starts at 0
        #           -  calculate the value of all incomplete tracks of the current
        #              player for every station that still has a train on it,
        #              *if the played tile makes up part of that track*, and add them
        #              together.  If no stations pass through it, the score will be
        #              zero at this point.  
        #           - Repeat the calculation for each of the incomplete tracks of
        #             other players that pass through that tile.  For each one that
        #             does, subtract the value of those incomplete tracks.
        #        - When finished, sort the values from high to low.  Depending on the
        #          AI level, pick a move that's either highly ranked, ranked in the
        #          middle, or lowest ranked.  If more than 5 moves exist, don't pick
        #          the absolute best or worst; pick the second best or second worst.
        #  
        #  How to store moves in order to allow them to be ranked:
        #  
        #  A structure with the following elements:
        # 
        #    int tileType (primary or secondary)
        #    int tileX    (the x position of the tile)
        #    int tileY    (the y position of the tile)
        #    int value    (the value of the this move)
        #   
        #  Create a list of this structure and add an element for each
        #  move evaluated.
		gameCopy = copy.deepcopy(g)		
		tilepool = copy.deepcopy(g.tilepool)
		
		# Create an empty list and AI packet
		ComputerAI.evalList = None
		p = ComputerAIPacket()
		
		# Determine how many tiles the AI player is holding, and how many tiles are left
		tileQuantity = 0
		holdingPrimary = False
		holdingSecondary = False
		if gameCopy.players[gameCopy.curPlayer].currentTileId != Tile.NONE:
			tileQuantity += 1
			holdingPrimary = True
		if gameCopy.players[gameCopy.curPlayer].reserveTileId != Tile.NONE:
			tileQuantity += 1
			holdingSceondary = True
		tilesInPool = tilepool.numUnplayedTiles()
		
		# The computer hasn't done anything yet
		if lastMove == None:
			# No tiles in hand?
			if tileQuantity == 0:
				if tilesInPool > 0:
					p.moveType = ComputerAI.ACTION_DRAW
					print "determineNextMove: Computer's move is DRAW"
				else:
					p.moveType = ComputerAI.ACTION_END_TURN
					print "determineNextMove: Computer's move is END_TURN (pass)"
			# One tile in hand?
			elif tileQuantity == 1:
				if holdingPrimary == True:
					legalMoves = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].currentTileId), tilesInPool)
				else:
					legalMoves = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].reserveTileId), tilesInPool)
				
				# If there are no legal moves with the tile
				if legalMoves == 0:
					if tilesInPool > 0:
						p.moveType = ComputerAI.ACTION_DRAW
						print "determineNextMove: Computer's move is DRAW"
					else:
						p.moveType = ComputerAI.ACTION_END_TURN
						print "determineNextMove: Computer's move is END_TURN (pass)"
				# This tile has legal moves, analyze them and pick one
				else:
					ComputerAI.evalList = []
					if holdingPrimary == True:
						ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].currentTileId, ComputerAI.HELD_TILE_PRIMARY)
					else:
						ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].reserveTileId, ComputerAI.HELD_TILE_RESERVE)
					ComputerAI.evalList.sort(key=lambda aieval: aieval.value, reverse=True)
					selectedMove = ComputerAI.selectListMove(gameCopy)
					p.moveType = ComputerAI.ACTION_PLAY
					p.moveX = selectedMove.tileX
					p.moveY = selectedMove.tileY
					p.heldTile = selectedMove.tileType
					print "determineNextMove: Computer's move is PLAY"
			# Two tiles in hand?
			else:
				ComputerAI.evalList = []
				legalMovesPrimary = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].currentTIleId), tilesInPool)
				if legalMovesPrimary > 0:
					ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].currentTileId, ComputerAI.HELD_TILE_PRIMARY)
					
				legalMovesSecondary = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].reserveTileId), tilesInPool)
				if legalMovesSecondary > 0:
					ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].reserveTileId, ComputerAI.HELD_TILE_SECONDARY)
				
				# If neither tile has legal moves, discard one.
				if legalMovesPrimary == 0 and legalMovesSecondary == 0:
					p.moveType = ComputerAI.ACTION_DISCARD
					p.heldTile = ComputerAI.HELD_TILE_RESERVE
					print "determineNextMove: Computer's move is DISCARD (secondary)"
				# Otherwise, pick a legal move from the list. 
				else:
					ComputerAI.evalList.sort(key=lambda aieval: aieval.value, reverse=True)
					selectedMove = ComputerAI.selectListMove(gameCopy)
					p.moveType = ComputerAI.ACTION_PLAY
					p.moveX = selectedMove.tileX
					p.moveY = selectedMove.tileY
					p.heldTile = selectedMove.tileType
					print "determineNextMove: Computer's move is PLAY"
		# The computer has taken a move this turn already
		else:
			# If the last move was a terminel move (play, pass, discard), then end turn.
			if lastMove.moveType == ComputerAI.ACTION_PLAY or \
			   lastMove.moveType == ComputerAI.ACTION_DISCARD or \
			   lastMove.moveType == ComputerAI.ACTION_END_TURN:
				p.moveType = ComputerAI.ACTION_END_TURN
				print "determineNextMove: Computer's move is END TURN"
			else:
				# The computer has no tiles (this shouldn't happen on a secondary move)
				if tileQuantity == 0:
					p.moveType = ComputerAI.ACTION_END_TURN
					print "determineNextMove: Computer's move is END TURN"
				elif tileQuantity == 1:
					if holdingPrimary == True:
						legalMoves = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].currentTileId), tilesInPool)
					else:
						legalMoves = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].reserveTileId), tilesInPool)
						
					# If there are no legal moves, end turn, otherwise analyze and pick a move
					if legalMoves == 0:
						p.moveType = ComputerAI.ACTION_END_TURN
						print "determineNextMove: Computer's move is END_TURN"
					else:
						ComputerAI.evalList = []
						if holdingPrimary == True:
							ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].currentTileId, ComputerAI.HELD_TILE_PRIMARY)
						else:
							ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].reserveTileId, ComputerAI.HELD_TILE_SECONDARY)
						ComputerAI.evalList.sort(key=lambda aieval: aieval.value, reverse=True)
						selectedMove = ComputerAI.selectListMove(gameCopy)
						p.moveType = ComputerAI.ACTION_PLAY
						p.moveX = selectedMove.tileX
						p.moveY = selectedMove.tileY
						p.heldTile = selectedMove.tileType
						print "determineNextMove: Computer's move is PLAY"
				# The computer has two tiles.  Analyze the moves (if any) and pick one.  Otherwise, discard a tile.
				else:
					ComputerAI.evalList = []
					legalMovesPrimary = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].currentTileId), tilesInPool)
					if legalMovesPrimary > 0:
						ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].currentTileId, ComputerAI.HELD_TILE_PRIMARY)
						
					legalMovesSecondary = gameCopy.board.markLegalMoves(tilepool.getTile(gameCopy.players[gameCopy.curPlayer].reserveTileId), tilesInPool)
					if legalMovesSecondary > 0:
						ComputerAI.analyzeMoves(gameCopy, gameCopy.players[gameCopy.curPlayer].currentTileId, ComputerAI.HELD_TILE_RESERVE)
					
					if legalMovesPrimary ==0 and legalMovesSecondary == 0:
						p.moveType = ComputerAI.ACTION_DISCARD
						p.heldTile = ComputerAI.HELD_TILE_RESERVE
						print "determineNextMove: Computer's move is DISCARD (secondary)"
					else:
						ComputerAI.evalList.sort(key=lambda aieval: aieval.value, reverse=True)
						selectedMove = ComputerAI.selectListMove(gameCopy)
						p.moveType = ComputerAI.ACTION_PLAY
						p.moveX = selectedMove.tileX
						p.moveY = selectedMove.tileY
						p.heldTile = selectedMove.tileType
						print "determineNextMove: Computer's move is PLAY"
						
		return p
		
	@staticmethod
	def analyzeMoves(g, tileIndex, heldTile):
		for i in range(1, Board.WIDTH - 1):
			for j in range(1, Board.HEIGHT - 1):
				if g.board.isLegalMove(i, j) == True:
					value = 0
					g.board.placeTile(i, j, tileIndex)
					for k in range(0, Board.NUM_STATIONS):
						(stationX, stationY, stationExit) = g.board.getStationInfo(k)
						if g.board.b[stationX][stationY].trainPresent != Board.NO_TRAIN:
							weight = 1.0
							(score, passThru, destination) = g.calculateTrackScore(k, tileIndex)
							if passThru == True:
								if destination == BoardSquare.TILE:
									weight = ComputerAI.INCOMPLETE_TRACK_WEIGHT
								elif destination == BoardSquare.STATION:
									weight = ComputerAI.COMPLETE_TRACK_WEIGHT
								elif destination == BoardSquare.CENTRAL_STATION:
									weight = ComputerAI.COMPLETE_CENTRAL_STATION_WEIGHT
									
								if Board.BOARD_STATION_DATA[g.numPlayers][k] == g.curPlayer + 1:
									value += int(score * weight)
								else:
									value -= int(score * weight)
					
					g.board.removeTile(i, j)
					ComputerAI.evalList.append(AIMoveEval(heldTile, i, j, value))
					
	@staticmethod 
	def selectListMove(g):
		numMoves = len(ComputerAI.evalList)
		aiLevel = g.players[g.curPlayer].computerAiLevel
		
		print "selectListMove: Evaluating moves - number of moves for player " + str(g.curPlayer) + " is " + str(numMoves) + "."

		for i in range(0, numMoves):
			print "  Move " + str(i+1) + ", score = " + str(ComputerAI.evalList[i].value)
			
		#
		# If there are three or fewer legal moves, then the smartest two AI levels take the best move,
		# while the dumbest AI takes the worst move.
		# If there are more than three moves, then the smartest AI level takes the best move, the
		# middle level AI takes a move approximately 1/3 of the way from the top of the list (or the
		# lowest ranked move with a positive score, whichever is higher), and the lowest level AI
		# takes a move approximately 2/3 of the way from the top of the list.
		#
		if numMoves < 3:
			if aiLevel == Player.AI_SMARTEST or aiLevel == Player.AI_SMARTER:
				moveIndex = 0
			else:
				moveIndex = numMoves - 1
		else:
			if aiLevel == Player.AI_SMARTEST:
				moveIndex = 0
			elif aiLevel == Player.AI_SMARTER:
				moveIndex = int(float(numMoves - 1) * 0.33)
				if ComputerAI.evalList[moveIndex].value <= 0:
					print "Shuffling move up, score is too low..."
					while moveIndex > 0 and ComputerAI.evalList[moveIndex].value <=0:
						moveIndex -= 1
			else:
				moveIndex = int(float(numMoves - 1) * 0.67)
				
		print "selectedListMove: Picking move " + str(moveIndex) + "."
		return ComputerAI.evalList[moveIndex]