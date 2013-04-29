#!/usr/bin/python
from pandocmarkdownwriter import *
from xmlParser import *

class XmlToMarkdown():
	def __init__( self ):
		self.pmw      = PandocMarkdownWriter()

	def dataStatusColor( self, dataArray, statusArray ):
		if type( dataArray ) is not list :
			raise ValueError( "list of lists expected" )
		if type( dataArray[0] ) is not list :
			raise ValueError( "list of lists expected" )
		if type( statusArray ) is not list :
			raise ValueError( "list expected" )
		if len( dataArray ) != len( statusArray ) :
			print len( dataArray )
			print len( statusArray )
			raise ValueError( "'dataArray' and 'statusArray' must have the same length" )

		for i in range( 0, len( dataArray ) ):
			for j in range( 0, len( dataArray[0] ) ):

				if type( dataArray[i][j] ) is unicode or type( dataArray[i][j] ) is str :
					if statusArray[i] != "valid" and statusArray[i] != "" :
						dataArray[i][j] = "{"+"\\"+"color{red}"+ str( dataArray[i][j] ) +"}"
				elif type( dataArray[i][j] ) is list :
					for k in range( 0, len(dataArray[i][j]) ):
						if statusArray[i] != "valid" and statusArray[i] != "" :
							dataArray[i][j] = "{"+"\\"+"color{red}"+ str( dataArray[i][j] ) +"}"
		return dataArray;

	def checkSpecialDataCharacters( self, array ):
		if type(array) is not list :
			raise ValueError( "list of lists expected" )
		if type(array[0]) is not list :
			raise ValueError( "list of lists expected" )

		for i in range( 0, len( array ) ):
			for j in range( 0, len( array[0] ) ):
				if type( array[i][j] ) is unicode or type( array[i][j] ) is str :
					array[i][j] = self.pmw.transformSpecialCharacters( array[i][j] )
				elif type( array[i][j] ) is list :
					for k in range( 0, len(array[i][j]) ):
						array[i][j][k] = self.pmw.transformSpecialCharacters( array[i][j][k] )
		return array;

	def convert( self, xmlParser ):
		######     XML     ######
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")

		######   Mardown   ######
		self.pmw.addTitle( xmlParser.filename.replace( ".xml", "" ) )

		######   Document  ######
		for section in xmlParser.sections :
			print section.title
			section.setFieldsValues()
			print section.data
			print section.dataStatus

			### TABLE ###
			t = PandocMarkdownTable()

			t.setTitle( section.title )

			# checks data special characters 
			section.data = self.checkSpecialDataCharacters( section.data )
			section.data = self.dataStatusColor( section.data, section.dataStatus )

			t.setContent( section.data )
			for i in range( 0, len(section.data) ):
				t.setCellStyle( i, 0, 'bold')
			t.setBorders('frame')
			t.setRowAlignment( 0, 'left')
			t.setRowAlignment( 1, 'left')
			t.setTableAlignment('center')
			self.pmw.addTable( t )

			if hasattr( section, 'group' ) :
				self.pmw.addHeader( section.groupTitle + " : _" + section.status + "_", 2 )
				for s in range( 0, len( section.group ) ) :
					section.group[s].setFieldsValues()
					print section.group[s].dataStatus

					t2 = PandocMarkdownTable()
					t2.setTitle( section.group[s].title + " : \\textit{" + section.group[s].status + "} " )

					# checks data special characters 
					section.group[s].data = self.checkSpecialDataCharacters( section.group[s].data )

					t2.setContent( section.group[s].data )
					for i in range( 0, len(section.group[s].data) ):
						t2.setCellStyle( i, 0, 'bold')
					t2.setBorders('frame')
					t2.setRowAlignment( 0, 'left')
					t2.setRowAlignment( 1, 'left')
					t2.setTableAlignment('center')
					self.pmw.addTable( t2 )

	def getMarkdownStream( self ):
			return self.pmw.getStream()
