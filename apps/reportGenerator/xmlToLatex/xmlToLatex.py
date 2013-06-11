#!/usr/bin/python
from latexwriter import *
from xmlParser import *

class XmlToLatex():
	def __init__( self ):
		self.lw      = LatexWriter()

	def setReportDocument( self, filename, QCversion, logoPath ):
		self.lw.setDocumentClass( "report", "11pt, a4paper" )
		self.lw.addSimpleLineBreak()

		self.lw.addPackage( "xcolor",            "usenames,dvipsnames,table" )
		self.lw.addPackage( "inputenc",           "utf8" )
		self.lw.addPackage( "fontenc",             "OT1" )
		self.lw.addPackage( "babel", "francais, english" )
		self.lw.addPackage(  "fancyhdr" )
		self.lw.addPackage(  "graphicx" )
		self.lw.addPackage(      "ulem" )
		self.lw.addPackage(     "array" )
		self.lw.addPackage( "longtable" )
		self.lw.addPackage(      "tikz" )
		self.lw.addPackage(  "pgfplots" )
		self.lw.addPackage(      "pbox" )
		self.lw.addPackage(      "tabu" )
		self.lw.addSimpleLineBreak()

		self.lw.addSimpleCommand( "pagestyle", "fancy" )
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

		self.lw.addNewCommand( "filename", "\\textit{" + self.lw.transformSpecialCharacters( filename  ) + "}" )
		self.lw.addNewCommand( "version",  self.lw.transformSpecialCharacters( QCversion ) )
		self.lw.addNewCommand( "QcLogo",   self.lw.transformSpecialCharacters( logoPath  ) )

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
						dataArray[i][j] = "{"+"\\"+"color{red}\\textbf{"+ str( dataArray[i][j] ) +"}}"
				elif type( dataArray[i][j] ) is list :
					for k in range( 0, len(dataArray[i][j]) ):
						if statusArray[i] != "valid" and statusArray[i] != "" :
							dataArray[i][j] = "{"+"\\"+"color{red}\\textbf{"+ str( dataArray[i][j] ) +"}}"
		return dataArray;

	def checkSpecialDataCharacters( self, array, checkStrings=False ):
		if len( array ) == 0 :
			raise ValueError( "Empty list : list of lists expected" )
		if type(array) is not list :
			raise ValueError( "list of lists expected" )
		if len( array ) > 0 and type(array[0]) is not list :
			raise ValueError( "list of lists expected" )

		for i in range( 0, len( array ) ):
			for j in range( 0, len( array[0] ) ):
				if type( array[i][j] ) is unicode or type( array[i][j] ) is str :
					array[i][j] = self.lw.transformSpecialCharacters( array[i][j] )
					if checkStrings :
						array[i][j] = checkString( array[i][j], 50 )
				elif type( array[i][j] ) is list and len( array[i][j] ) > 0 :
					array[i][j] = self.checkSpecialDataCharacters( array[i][j] )


		return array;

	def convert( self, xmlParser ):
		######     XML     ######
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")

		self.setReportDocument( xmlParser.xmlFilename.replace( "-merged.xml", "" ), "1.0", "imageCheckOK256.png" )
		
		self.lw.beginDocument()
		self.lw.addMacro( "maketitle" )
		self.lw.addSimpleLineBreak()
		
		######   FileSystemInfoSection  ######
		self.lw.addHeader( "System Information :", 3 )
		for section in xmlParser.sections :
			if isinstance( section, FileSystemInfoSection ):
				print "\nFileSystemInfoSection : " + section.title
				section.setFieldsValues()
				print section.data
				print section.dataStatus

				### TABLE ###
				fsiTable = LatexTable()
				# fsiTable.setTitle( section.title )

				# checks data special characters 
				section.data = self.checkSpecialDataCharacters( section.data )
				# section.data = self.dataStatusColor( section.data, section.dataStatus )
				
				fsiTable.setContent( section.data )
				fsiTable.setBorders('none')
				fsiTable.setRowAlignment( 0, 'left')
				fsiTable.setRowAlignment( 1, 'right', True )
				fsiTable.setTableAlignment('center')

				self.lw.addTable( fsiTable, False, "0.9\\textwidth" )


		self.lw.addSimpleCommand( "vspace", "2cm" )
		self.lw.addSimpleLineBreak()


		######   SpecificationSection  ######
		self.lw.addHeader( "Quality Check report :", 3 )

		# ========== Summary ==========
		self.lw.addHeader( "Summary :", 4 )
		recapTable = LatexTable()
		recapData = []
		recapData.append( ["\\multicolumn{1}{c}{\\textbf{Specification}}", "\\multicolumn{1}{c}{\\textbf{Status}}"] )
		
		for section in xmlParser.sections :
			if isinstance( section, SpecificationSection ):
				# print section.title
				section.setFieldsValues()
				# print section.data
				# print section.dataStatus

				### TABLE ###
				recapLine = []
				recapLine.append( section.title )
				if section.status == "valid" :
					recapLine.append( "\\color{OliveGreen}\\textbf{" + section.status + "}" )
				else :
					recapLine.append( "\\color{red}\\textbf{" + section.status + "}" )
				recapData.append( recapLine )

		recapTable.setContent( recapData )
		recapTable.setBorders('header')
		recapTable.setRowAlignment( 0, 'left')
		recapTable.setRowAlignment( 1, 'right', True )
		recapTable.setTableAlignment('center')
		self.lw.addTable( recapTable, False, "0.7\\textwidth" )

		self.lw.addMacro( "newpage" )

		# ========== Detail ==========
		self.lw.addHeader( "Detail :", 4 )

		for section in xmlParser.sections :
			if isinstance( section, SpecificationSection ):
				detailTable = LatexTable()
				detailTableData = []
				detailTableData.append( ["\\multicolumn{2}{|c|}{\\textbf{" + section.title + "}}"] )
				print "\nSpecificationSection : " + section.title
				print section.data

				for pair in section.data :
					detailTableData.append( pair )

				for s in range( 0, len( section.group ) ) :
					section.group[s].setFieldsValues()
					# print section.group[s].data
					# print section.group[s].dataStatus

					detailTableData.append( ["\\hline \\multicolumn{2}{|c|}{" + section.group[s].title + ": \\textit{" + section.group[s].status + "}}"] )
					section.group[s].data = self.checkSpecialDataCharacters( section.group[s].data, True )
					for pair in section.group[s].data :
						detailTableData.append( pair )

				if section.status == "valid" :
					status = "\\color{OliveGreen}" + section.status
				else :
					status = "\\color{red}" + section.status
				detailTableData.append( ["\\hline \\multicolumn{2}{|c|}{\\textbf{Status : " + status + "}}"] )

				detailTable.setContent( detailTableData )
				detailTable.setBorders('closedHeader')
				detailTable.setRowAlignment( 0, 'p{0.45\\textwidth}')
				detailTable.setRowAlignment( 1, 'p{0.45\\textwidth}')
				detailTable.setTableAlignment('center')
				print detailTableData
				self.lw.addTable( detailTable, True )


		self.lw.addMacro( "newpage" )
		
		# ========== Loudness ==========
		self.lw.addHeader( "Loudness :", 4 )

		for section in xmlParser.sections :
			if isinstance( section, LoudnessSection ):
				print "\nLoudnessSection : " + section.title
				section.setFieldsValues()
				print section.data
				print section.dataStatus
				# print section.plots
				
				loudnessTable = LatexTable()
				loudnessTableData = []
				loudnessTableData.append( ["\\multicolumn{2}{c}{\\textbf{" + section.title + "}}"] )
				for pair in self.dataStatusColor( section.data, section.dataStatus ) :
					loudnessTableData.append( pair )

				loudnessTable.setContent( loudnessTableData )
				loudnessTable.setBorders('header')
				loudnessTable.setRowAlignment( 0, 'left')
				loudnessTable.setRowAlignment( 1, 'right', True)
				loudnessTable.setTableAlignment('center')
				self.lw.addTable( loudnessTable, False, "0.7\\textwidth" )

				graph = LatexGraph( section.title )
				for j in range( 0, len(section.plots) ) :
					i = len(section.plots) - (j+1)
					curve = LatexPlot( section.plots[i][0], section.plots[i][1] )
					curve.setColor( colors[i] )
					curve.setCoordinates()
					curve.setFill( True )
					curve.setOpacity( 0.5 )
					graph.addCurve( curve )
				self.lw.addGraph( graph, "\\textwidth-1.4cm", "time", "level", "s", "dB" )
				self.lw.addMacro( "newpage" )


		self.lw.endDocument()

	def getLatexStream( self ):
			return self.lw.getStream()
