import os

from xml.dom.ext import StripXml
from xml.dom.ext.reader import Sax2

from section import *
from utils import *

class XmlParser():
	def __init__( self ):
		self.root = None
		self.sections = []
		self.xmlFilename = ""

	def parseXml( self, xmlFile ):
		file = open( xmlFile )
		self.xmlFilename = os.path.basename( xmlFile )
		xmlStream = file.read()
		file.close()

		reader = Sax2.Reader()
		docTree = reader.fromString( xmlStream )
		StripXml( docTree )
		self.root = docTree.documentElement

		if self.root.childNodes is not None :
			# get xml sections
			for child in self.root.childNodes :
				if child.tagName == "fileValidator" :
					for subChild in child.childNodes:
						if subChild.tagName == fileSystemInfo :
							section = FileSystemInfoSection( subChild.getAttribute( labelAttr ) )
							section.status = subChild.getAttribute( statusAttr )
						elif subChild.tagName == specification :
							section = SpecificationSection( subChild.getAttribute( labelAttr ) )
							section.status = subChild.getAttribute( statusAttr )
						else :
							section = Section( subChild.tagName )
							section.status = subChild.getAttribute( statusAttr )
						self.sections.append( section )
						# get xml elements (tagName)
						section.getAvailableFields( subChild )
						if subChild.childNodes is not None :
							for element in subChild.childNodes:
								section.fields.append( element )
				
				if child.tagName == "loudness" :
					for subChild in child.childNodes:
						section = LoudnessSection( subChild.getAttribute( "standard" ) )
						section.status = subChild.getAttribute( statusAttr )
						self.sections.append( section )
						# get xml elements (tagName)
						if subChild.childNodes is not None :
							for element in subChild.childNodes:
								section.fields.append( element )
						

	def displaySections( self ):
		for section in self.sections:
			section.displayFields()
	
	def removeField( self, fieldTagName ):
		for section in self.sections:
			for field in section.availableFields:
				if field.tagName == fieldTagName:
					section.forbiddenFields.append( field )
