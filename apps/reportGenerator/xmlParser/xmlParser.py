from xml.dom.ext import StripXml
from xml.dom.ext.reader import Sax2

from section import *
from utils import *

class XmlParser():
	def __init__( self ):
		self.root = None
		self.sections = []
		self.filename = ""

	def parseXml( self, xmlFile ):
		self.filename = xmlFile
		file = open( self.filename )
		xmlStream = file.read()
		file.close()

		reader = Sax2.Reader()
		docTree = reader.fromString( xmlStream )
		StripXml( docTree )
		self.root = docTree.documentElement

		if self.root.childNodes is not None :
			# get xml sections
			for child in self.root.childNodes :
				if child.tagName == fileSystemInfo :
					section = FileSystemInfoSection( "File System Info" )
				elif child.tagName == specification :
					section = SpecificationSection( child.getAttribute( labelAttr ) )
				else :
					section = Section( child.tagName )
				self.sections.append( section )
				# get xml elements (tagName)
				if child.childNodes is not None :
					for subChild in child.childNodes:
						section.fields.append( subChild )

	def displaySections( self ):
		for section in self.sections:
			section.displayFields()
	
	def removeField( self, fieldTagName ):
		for section in self.sections:
			for field in section.fields:
				if field.tagName == fieldTagName:
					section.fields.remove(field)
