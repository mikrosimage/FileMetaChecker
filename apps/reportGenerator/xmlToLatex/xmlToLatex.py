#!/usr/bin/python
import os
from latexwriter import *
from xmlParser import *

class XmlToLatex():
	def __init__( self ):
		self.lw = LatexWriter()
		self.plots = []
		self.colors = [ "blue", "red", "green", "orange", "black" ]

	def setReportDocument( self, filename, QCversion, logoPath ):
		self.lw.setDocumentClass( "report", "11pt, a4paper" )
		self.lw.addSimpleLineBreak()

		self.lw.addPackage( "xcolor",   "usenames,dvipsnames,table" )
		self.lw.addPackage( "inputenc", "utf8" )
		self.lw.addPackage( "fontenc",  "OT1" )
		self.lw.addPackage( "babel",    "francais, english" )
		self.lw.addPackage( "fancyhdr" )
		self.lw.addPackage( "graphicx" )
		self.lw.addPackage( "ulem" )
		self.lw.addPackage( "array" )
		self.lw.addPackage( "longtable" )
		self.lw.addPackage( "tikz" )
		self.lw.addPackage( "pgfplots" )
		self.lw.addPackage( "pbox" )
		self.lw.addPackage( "tabu" )
		self.lw.addSimpleLineBreak()

		self.lw.addSimpleCommand( "pagestyle",      "fancy" )
		self.lw.addSimpleCommand( "usetikzlibrary", "pgfplots.units" )
		self.lw.addSimpleLineBreak()

		self.lw.addMacroSetting( "hoffset",        0, "cm" )
		self.lw.addMacroSetting( "voffset",        0, "cm" )
		self.lw.addMacroSetting( "oddsidemargin",  0, "cm" )
		self.lw.addMacroSetting( "evensidemargin", 0, "cm" )
		self.lw.addMacroSetting( "topmargin",      0, "cm" )
		self.lw.addMacroSetting( "headwidth",     16, "cm" )
		self.lw.addMacroSetting( "headheight",     1, "cm" )
		self.lw.addMacroSetting( "headsep",        1, "cm" )
		self.lw.addMacroSetting( "textheight",    22, "cm" )
		self.lw.addMacroSetting( "textwidth",     16, "cm" )
		self.lw.addMacroSetting( "marginparsep",   0, "cm" )
		self.lw.addMacroSetting( "footskip",     1.5, "cm" )
		self.lw.addSimpleLineBreak()

		self.lw.addRawLatex( "\\renewcommand*\\familydefault{\\sfdefault}" )

		self.lw.addNewCommand( "filename", "\\textit{" + transformSpecialCharacters( filename  ) + "}" )
		self.lw.addNewCommand( "version",  transformSpecialCharacters( QCversion ) )
		self.lw.addNewCommand( "QcLogo",   transformSpecialCharacters( logoPath  ) )

		self.lw.addNewCommand( "QcHeader",      "Quality Check : \\filename" )
		self.lw.addNewCommand( "QcFile",      "File: \\filename" )
		self.lw.addNewCommand( "QcVersion", "Version: \\version" )
		self.lw.addSimpleLineBreak()

		self.lw.addSimpleCommand( "title", "Quality Check" )
		self.lw.addSimpleLineBreak()


		maketitle = ""
		maketitle += "\\renewcommand{\\maketitle}{\n"
		maketitle += "\\begin{titlepage}\n"
		maketitle += "\\begin{center}\n"
		maketitle += "\\includegraphics[width=0.3\\textwidth]{\\QcLogo}~\\\\[1cm]\n"
		maketitle += "\\color{black} \\rule{\\linewidth}{1pt} \\\\[0.6cm]\n"
		maketitle += "{\\fontsize{50}{50} \\usefont{OT1}{phv}{b}{n} \\color{black} \\selectfont \\@title}\\\\[0.4cm]\n"
		maketitle += "\\color{black} \\rule{\\linewidth}{1pt} \\\\[0.8cm]\n"
		maketitle += "{\\large \\QcFile}\\\\[0.4cm]\n"
		maketitle += "{\\large \\QcVersion}\\\\ \n"
		maketitle += "\\vfill\n"
		maketitle += "{\\large \\today}\n"
		maketitle += "\\end{center}\n"
		maketitle += "\\end{titlepage}\n"
		maketitle += "\\par}"
		
		self.lw.addMacro( "makeatletter" )
		self.lw.addRawLatex( maketitle )
		self.lw.addMacro( "makeatother" )
		self.lw.addSimpleLineBreak()

		self.lw.addSimpleCommand( "fancyhead" )
		self.lw.addSimpleCommand( "fancyfoot" )
		self.lw.addSimpleCommand( "lhead", "\\QcHeader" )
		self.lw.addSimpleCommand( "rhead", "\\today" )
		self.lw.addRawLatex( "\\renewcommand{\\headrulewidth}{1pt}" )
		self.lw.addSimpleCommand( "cfoot", "\\thepage" )
		self.lw.addSimpleLineBreak()

	def getChildData( self, element ) :
		data = []
		for child in list( element ) :
			if not child.get( "label" ) and not child.get( "status" ) :
				if list( child ) :
					data.extend( self.getChildData( child ) )
				continue

			pair = []
			# print child
			if child.get( "date" ) or child.get( "index" ) :
				string  = "\\hline \\multicolumn{2}{|c|}{ \\textbf{" + child.get( "label" ) + "}"
				string += ": \\textit{" + child.get( "status" ) + "}" if child.get( "status" ) else ""
				string += "}"
				pair.append( string )
				data.append( pair )
				
			else :
				if child.get( "label" ) :
					# print ">>> LABEL"
					pair.append( child.get( "label" ) )
					if child.text :
						# print "\t>>> TEXT" + child.text
						if child.get( "status" ) == "not valid" or child.get( "status" ) == "illegal" : 
							pair.append( "{"+"\\"+"color{red}\\textbf{"+ child.text +"}}")
						else :
							pair.append( child.text )
							
					elif child.get( "type" ) == "data" :
						# print "\t>>> TYPE DATA"
						pair.append( "\\textit{- DATA -}")

					elif child.get( "status" ) and len( list( child ) ) == 0 :
						# print "\t>>> STATUS"
						if child.get( "status" ) == "valid" :
							pair.append( "{"+"\\"+"color{OliveGreen}\\textbf{valid}}")
						if child.get( "status" ) == "not valid" or child.get( "status" ) == "illegal" :
							pair.append( "{"+"\\"+"color{red}\\textbf{"+ child.get( "status" ) +"}}")

					elif list( child ) :
						# print child
						if not list( child.itertext() ) :
							table = []
							for subchild in list( child ) :
								line = self.getChildData( subchild )
								line.insert( 0, str( subchild.get( "label" ) ) + " :" )
								table.append( line )
								# print "table " + subchild.tag + " : " + str(table)
							pair.append( table )
								# continue
							data.append( pair )
							continue
						else :
							pair = [ "\\hline \\multicolumn{2}{|c|}{" + child.get( "label" ) + "}" ]
							data.append( pair )
							data.extend( self.getChildData( child ) )
							continue
					
					data.append( pair )
						
				else :
					# print ">>> NO LABEL"
					if child.get( "status" ) == "false" :
						data.append( "\\"+"color{Gray}"+ child.tag )
					else :
						data.append( child.tag )

			if list( child ) :
				data.extend( self.getChildData( child ) )
		return data

	def addFileSystemInfoSection( self, element ):
		fsiTable = LatexTable()
		fsiTable.setContent( self.getChildData( element ) )
		fsiTable.setBorders('none')
		fsiTable.setRowAlignment( 0, 'left')
		fsiTable.setRowAlignment( 1, 'right', True )
		fsiTable.setTableAlignment('center')

		self.lw.addTable( fsiTable, False, "0.9\\textwidth" )
		self.lw.addSimpleCommand( "vspace", "2cm" )
		self.lw.addSimpleLineBreak()

	def addSummarySection( self, element ):
		summaryTable = LatexTable()
		summaryTable.setContent( self.getChildData( element ) )
		# summaryTable.setBorders( 'header' )
		summaryTable.setRowAlignment( 0, 'left', True )
		summaryTable.setRowAlignment( 1, 'right' )
		summaryTable.setTableAlignment('center')
		self.lw.addTable( summaryTable, False, "0.7\\textwidth" )
		self.lw.addSimpleLineBreak()

	def addQualityCheckDetailSection( self, element ):
		detailTable = LatexTable()
		detailTableData = []
		detailTableData.append( ["\\multicolumn{2}{|c|}{ \\begin{tabular}{c} \\textbf{" + element.get( "label" ) + "} \\\\ \\footnotesize \\color{gray} " + element.get( "date" ) + "\\end{tabular} }"] )
		detailTableData.extend( self.getChildData( element ) )
		detailTableData.append( ["\\hline \\multicolumn{2}{|c|}{\\textbf{Status : " + element.get( "status" ) + "}}"] )

		detailTable.setContent( detailTableData )
		detailTable.setBorders('closedHeader')
		detailTable.setRowAlignment( 0, 'p{0.45\\textwidth}')
		detailTable.setRowAlignment( 1, 'p{0.45\\textwidth}')
		detailTable.setTableAlignment('center')
		self.lw.addTable( detailTable, True )

	def getPlots( self, element ) :
		plots = []
		for child in list( element ) :
			if len( list( child.attrib ) ) != 0 :
				plots.extend( self.getPlots( child ) )
				continue
			
			if child.text :
				# print "  >> " + str(child)
				plots.append( child )
		return plots

	def sortPlotsValues( self, plotsList ) :
		data = []

		size = len( plotsList[0] )
		# print size
		for i in range( 1, len(plotsList) ) :
			if len(plotsList[i]) != size :
				raise ValueError( "Plots must have the same data length." )

		for j in range( 0, size ):
			line = []
			if j == 0 :
				line.append( "Second" )
				for plot in plotsList :
					line.append( str( plot[0] ) )
			else :
				line.append( j-1 )
				for plot in plotsList :
					if plot[j] == "-inf" :
						plot[j] = "-100"
					line.append( float( plot[j] ) )
			data.append( line )

		return data;

	def addLoudnessSection( self, element ):
		loudnessTable = LatexTable()
		loudnessTableData = []
		loudnessTableData.append( ["\\multicolumn{2}{c}{ \\begin{tabular}{c} \\textbf{" + element.get( "label" ) + "} \\\\ \\footnotesize \\color{gray} " + element.get( "date" ) + "\\end{tabular} }"] )
		loudnessTableData.extend( self.getChildData( element ) )

		loudnessTable.setContent( loudnessTableData )
		loudnessTable.setBorders('header')
		loudnessTable.setRowAlignment( 0, 'left')
		loudnessTable.setRowAlignment( 1, 'right', True)
		loudnessTable.setTableAlignment('center')
		self.lw.addTable( loudnessTable, False, "0.7\\textwidth" )

		childPlots = []
		plots = self.getPlots( element )
		# print plots
		for plot in plots :
			values = []
			values.append( plot.tag )
			values.append( plot.text.split( ", " ) )
			childPlots.append( values )
		# print childPlots
		graph = LatexGraph( element.get( "label" ) )
		for j in range( 0, len( childPlots ) ) :
			i = len( childPlots ) - (j+1)
			curve = LatexPlot( childPlots[i][0], childPlots[i][1] )
			curve.setColor( self.colors[i] )
			curve.setCoordinates()
			curve.setFill( True )
			curve.setOpacity( 0.5 )
			graph.addCurve( curve )
		self.lw.addGraph( graph, "\\textwidth-1.4cm", "time", "level", "s", "dB" )


	def convert( self, xmlParser ):
		######     XML     ######
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")

		if type( os.getenv( "QC_RESOURCES" ) ) is not str :
			raise ValueError( "You must define the 'QC_RESOURCES' macro" )

		logoPath = os.getenv( "QC_RESOURCES" ) + "imageCheckOK256.png"
		
		root = xmlParser.getRoot()
		title = list( root.iter( "absolutePath" ) )[0].text
		parent_map = dict( (c, p) for p in root.getiterator() for c in p )

		self.setReportDocument( title, "1.0", logoPath )
		
		self.lw.beginDocument()
		self.lw.addMacro( "maketitle" )
		self.lw.addSimpleLineBreak()
		
		self.lw.addHeader( "System Information :", 3 )
		if root.find( "systemInfo" ) is not None :
			self.addFileSystemInfoSection( root.find( "systemInfo" ) )

		######   SpecificationSection  ######
		self.lw.addHeader( "Quality Check report :", 3 )
		
		# ========== Summary ==========
		self.lw.addHeader( "Summary :", 4 )
		if root.find( "status" ) is not None :
			self.addSummarySection( root.find( "status" ) )

		self.lw.addMacro( "newpage" )

		# ========== Detail ==========
		if root.findall( "stream" ) is not None :
			for stream in root.findall( "stream" ) :
				self.lw.addHeader( stream.get( "label" ) + " :", 4 )
				if stream.iter( "specification" ) :
					self.lw.addHeader( "Detail :", 5 )
					for spec in list( stream.iter( "specification" ) ) :
						self.addQualityCheckDetailSection( spec )

				if stream.find( "loudness" ) is not None :
					self.lw.addHeader( "Loudness :", 5 )
					for program in stream.find( "loudness" ) :
						self.addLoudnessSection( program )
						self.lw.addMacro( "newpage" )

		self.lw.endDocument()

	def getLatexStream( self ):
			return self.lw.getStream()


	# def checkSpecialDataCharacters( self, array, checkStrings=False ):
	# 	if len( array ) == 0 :
	# 		raise ValueError( "Empty list : list of lists expected" )
	# 	if type(array) is not list :
	# 		raise ValueError( "list of lists expected" )
	# 	if len( array ) > 0 and type(array[0]) is not list :
	# 		raise ValueError( "list of lists expected" )

	# 	for i in range( 0, len( array ) ):
	# 		for j in range( 0, len( array[0] ) ):
	# 			if type( array[i][j] ) is unicode or type( array[i][j] ) is str :
	# 				array[i][j] = transformSpecialCharacters( array[i][j] )
	# 				if checkStrings :
	# 					array[i][j] = checkString( array[i][j], 50 )
	# 			elif type( array[i][j] ) is list and len( array[i][j] ) > 0 :
	# 				array[i][j] = self.checkSpecialDataCharacters( array[i][j] )
	# 	return array;