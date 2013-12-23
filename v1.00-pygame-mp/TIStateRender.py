#! /usr/bin/env python

from abc import ABCMeta, abstractmethod

# Each render state must implement a class that derives from StateRender
# A method to load assets, update logic, check timing conditions, and 
# render the screen to the global window instance are required.
class StateRender:
	__metaclass__ = ABCMeta
	
	windowSurfaceObj = None
	
	# Dirty rects for the draw object
	dirtyRects = None
	
	# If the target state should change this frame, this will be
	# set to a non-None value.  The Render instance will look for this
	# and change state accordingly.
	targetState = None
	
	@abstractmethod
	def __init__(self, w):
		pass
		
	@abstractmethod
	def loadAssets(self):
		pass
		
	@abstractmethod
	def updateLogic(self):
		pass
		
	@abstractmethod
	def checkTimingConditions(self, t):
		pass
		
	@abstractmethod
	def renderScreen(self):
		pass
		
	@abstractmethod
	def processInputs(self):
		pass