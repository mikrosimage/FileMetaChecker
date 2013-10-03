import os

from xml.dom.ext import StripXml
from xml.dom.ext.reader import Sax2

from section import *
from stream import *
from utils import *

class XmlParser():
	def __init__( self ):
		self.root = None
		self.sections = []
		self.streams  = []
		self.forbidden  = []
		self.xmlFilename = ""

	def parseXml( self, xmlFile, outputFormat ):
		file = open( xmlFile )
		self.xmlFilename = os.path.basename( xmlFile )
		xmlStream = file.read()
		file.close()

		reader = Sax2.Reader()
		docTree = reader.fromString( xmlStream )
		StripXml( docTree )
		self.root = docTree.documentElement

		if self.root.childNodes is None :
			return;

		# get xml sections
		for node in self.root.childNodes :
			print node.tagName
			if node.tagName == fileSystemInfo :
				section = FileSystemInfoSection( outputFormat, node.getAttribute( labelAttr ) )
				section.status = node.getAttribute( statusAttr )
				self.sections.append( section )
				section.setAvailableFields( node )
				if node.childNodes is not None :
					print node.childNodes
					for element in node.childNodes:
						print element
						section.fields.append( element )

			if node.tagName == "stream" :
				stream = Stream( node.getAttribute( "type" ), node.getAttribute( "index" ), node.getAttribute( labelAttr ) )
				self.streams.append( stream )
				# print "stream : " + section.type + " " + section.index

				for child in node.childNodes:
					description = Description( child.tagName )
					stream.descriptions.append( description )
					if child.tagName == "fileValidator" :
						for subChild in child.childNodes:
							if subChild.tagName == specification :
								section = SpecificationSection( subChild.getAttribute( labelAttr ) )
								section.status = subChild.getAttribute( statusAttr )
								section.date   = subChild.getAttribute( "date" )

							else :
								section = Section( subChild.tagName )
								section.status = subChild.getAttribute( statusAttr )
			
							description.sections.append( section )
							section.setAvailableFields( subChild )
							if subChild.childNodes is not None :
								for element in subChild.childNodes:
									section.fields.append( element )
					
					elif child.tagName == "loudness" :
						for subChild in child.childNodes:
							section = LoudnessSection( subChild.getAttribute( "standard" ) )
							section.status = subChild.getAttribute( statusAttr )
							section.date   = subChild.getAttribute( "date" )
							
							description.sections.append( section )
							section.setAvailableFields( subChild )
							if subChild.childNodes is not None :
								for element in subChild.childNodes:
									section.fields.append( element )

					else :
						for subChild in child.childNodes:
							section = Section( subChild.getAttribute( labelAttr ) )
							section.status = subChild.getAttribute( statusAttr )
							
							description.sections.append( section )
							section.setAvailableFields( subChild )
							if subChild.childNodes is not None :
								for element in subChild.childNodes:
									section.fields.append( element )

	def displaySections( self ):
		for section in self.sections:
			if self.forbidden.count( section ) != 0 :
				continue;
			section.displayFields( section.fields )
		for stream in self.streams:
			if self.forbidden.count( stream ) != 0 :
				continue;
			stream.displayFields( )
	
	def removeField( self, fieldTagName ):
		for section in self.sections:
			for field in section.availableFields :
				if field.tagName == fieldTagName :
					section.forbiddenFields.append( field )

		for stream in self.streams:
			if stream.index == fieldTagName :
				self.forbidden.append( stream )

			for description in stream.descriptions :
				if description.title == fieldTagName :
					stream.forbiddenDescriptions.append( description )

				for section in description.sections :
					if section.title == fieldTagName :
						description.forbiddenSections.append( section )

					for field in section.availableFields :
						if field.tagName == fieldTagName :
							section.forbiddenFields.append( field )
