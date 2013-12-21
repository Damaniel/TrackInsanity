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
#=============================================================================
class ComputerAIPacket:
	