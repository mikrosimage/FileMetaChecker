from utils import *

class Description():
	def __init__( self, title ):
		self.title = title
		self.sections = []
		self.forbiddenSections = []

	def displayFields( self, level=0 ):
		print "\t"*level + self.title
		for section in self.sections:
			if self.forbiddenSections.count( section ) != 0 :
				continue;
			section.displayFields( section.fields, level+1 )
	

class Stream():
	def __init__( self, streamType, index, title ):
		self.type  = streamType
		self.index = index
		self.title = title
		self.descriptions          = []
		self.forbiddenDescriptions = []

	def displayFields( self ):
		print "\n" + self.title
		for description in self.descriptions:
			if self.forbiddenDescriptions.count( description ) != 0 :
				continue;
			description.displayFields( 1 )
