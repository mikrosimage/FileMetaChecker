#!/usr/bin/python
from pandocmarkdownwriter import *
from xmlParser import *

class XmlToMarkdown():
	def __init__( self ):
		self.mdStream = ""

	def convert( self, xmlParser ):
		######     XML     ######
		if not isinstance(xmlParser, XmlParser):
			raise ValueError("request a 'XmlParser' object")

		######   Mardown   ######
		pmw = PandocMarkdownWriter()

		# header
		pmw.addTitle( xmlParser.filename.replace( ".xml", "" ) )

		######   Document  ######
		for section in xmlParser.sections :
			print section.title
			section.setFieldsValues()
			print section.data

			### TABLE ###
			t = PandocMarkdownTable()

			t.setTitle( section.title )

			# checks data special characters 
			for i in range( 0, len(section.data) ):
				for j in range( 0, len(section.data[0]) ):
					if type( section.data[i][j] ) is unicode or type( section.data[i][j] ) is str :
						section.data[i][j] = pmw.transformSpecialCharacters( section.data[i][j] )
					elif type( section.data[i][j] ) is list :
						for k in range( 0, len(section.data[i][j]) ):
							section.data[i][j][k] = pmw.transformSpecialCharacters( section.data[i][j][k] )

			t.setContent( section.data )
			for i in range( 0, len(section.data) ):
				t.setCellStyle( i, 0, 'bold')
			t.setBorders('frame')
			t.setRowAlignment( 0, 'left')
			t.setRowAlignment( 1, 'left')
			t.setTableAlignment('center')
			pmw.addTable( t )

			self.mdStream = pmw.getStream()
			# pmw.addHorizontalRule()


	def getMarkdownStream( self ):
			return self.mdStream
