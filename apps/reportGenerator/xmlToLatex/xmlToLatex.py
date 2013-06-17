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



	def addFileSystemInfoSection( self, section ):
		section.setFieldsValues()
		section.data = self.checkSpecialDataCharacters( section.data )
		
		fsiTable = LatexTable()
		# fsiTable.setTitle( section.title )
		fsiTable.setContent( section.data )
		fsiTable.setBorders('none')
		fsiTable.setRowAlignment( 0, 'left')
		fsiTable.setRowAlignment( 1, 'right', True )
		fsiTable.setTableAlignment('center')

		self.lw.addTable( fsiTable, False, "0.9\\textwidth" )
		self.lw.addSimpleCommand( "vspace", "2cm" )
		self.lw.addSimpleLineBreak()

		# print "\nFileSystemInfoSection : " + section.title
		# print section.data
		# print section.dataStatus


	def addSummarySectionLine( self, section ):
		section.setFieldsValues()

		sumLine = []
		sumLine.append( section.title )
		sumLine.append( section.date  )
		if section.status == "valid" :
			sumLine.append( "\\color{OliveGreen}\\textbf{" + section.status + "}" )
		else :
			sumLine.append( "\\color{red}\\textbf{" + section.status + "}" )
		return sumLine;

		# print section.title
		# print section.data
		# print section.dataStatus

	def getSummarySection( self, stream ):
		summaryData = []
		for description in stream.descriptions :
			if description.title == "fileValidator" :
				summaryData.append( ["\\multicolumn{1}{c}{\\textbf{Specification}}", "\\multicolumn{1}{c}{\\textbf{Date}}", "\\multicolumn{1}{c}{\\textbf{Status}}"] )
				for section in description.sections :
					if isinstance( section, SpecificationSection ):
						summaryData.append( self.addSummarySectionLine( section ) )
			
			# summaryData.append( [""] )
			if description.title == "loudness" :
				summaryData.append( ["\\multicolumn{3}{c}{\\textbf{- Loudness -}}"] )
				for section in description.sections :
					if isinstance( section, LoudnessSection ):
						summaryData.append( self.addSummarySectionLine( section ) )

		return summaryData;

	def addQualityCheckDetailSection( self, section ):
		detailTable = LatexTable()
		detailTableData = []
		detailTableData.append( ["\\multicolumn{2}{|c|}{ \\begin{tabular}{c} \\textbf{" + section.title + "} \\\\ \\footnotesize \\color{gray} " + section.date + "\\end{tabular} }"] )

		for pair in section.data :
			detailTableData.append( pair )
		for s in range( 0, len( section.group ) ) :
			section.group[s].setFieldsValues()
			detailTableData.append( ["\\hline \\multicolumn{2}{|c|}{" + section.group[s].title + ": \\textit{" + section.group[s].status + "}}"] )
			section.group[s].data = self.checkSpecialDataCharacters( section.group[s].data, True )
			for pair in section.group[s].data :
				detailTableData.append( pair )
			# print section.group[s].data
			# print section.group[s].dataStatus
		color = "OliveGreen" if section.status == "valid" or section.status == "not illegal" else "red"
		status = "\\color{" + color + "}" + section.status

		detailTableData.append( ["\\hline \\multicolumn{2}{|c|}{\\textbf{Status : " + status + "}}"] )

		detailTable.setContent( detailTableData )
		detailTable.setBorders('closedHeader')
		detailTable.setRowAlignment( 0, 'p{0.45\\textwidth}')
		detailTable.setRowAlignment( 1, 'p{0.45\\textwidth}')
		detailTable.setTableAlignment('center')
		self.lw.addTable( detailTable, True )

		# print "\nSpecificationSection : " + section.title
		# print section.data
		# print detailTableData

	def addLoudnessSection( self, section ):
		if len( section.data ) == 0 :
			section.setFieldsValues()
		
		loudnessTable = LatexTable()
		loudnessTableData = []
		loudnessTableData.append( ["\\multicolumn{2}{c}{ \\begin{tabular}{c} \\textbf{" + section.title + "} \\\\ \\footnotesize \\color{gray} " + section.date + "\\end{tabular} }"] )
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
		
		# print "\nLoudnessSection : " + section.title
		# print section.data
		# print section.dataStatus
		# print section.plots


	def convert( self, xmlParser ):
		######     XML     ######
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")

		if type( os.getenv( "QC_RESOURCES" ) ) is not str :
			raise ValueError( "You must define the 'QC_RESOURCES' macro" )

		logoPath = os.getenv( "QC_RESOURCES" ) + "imageCheckOK256.png"
		
		title = xmlParser.xmlFilename.replace( "-merged.xml", "" )
		if title == xmlParser.xmlFilename :
			title = xmlParser.xmlFilename.replace( "-audio.xml", "" )

		self.setReportDocument( title, "1.0", logoPath )
		
		self.lw.beginDocument()
		self.lw.addMacro( "maketitle" )
		self.lw.addSimpleLineBreak()
		
		self.lw.addHeader( "System Information :", 3 )
		for section in xmlParser.sections :
			if isinstance( section, FileSystemInfoSection ):
				self.addFileSystemInfoSection( section )

		######   SpecificationSection  ######
		self.lw.addHeader( "Quality Check report :", 3 )
		
		# ========== Summary ==========
		self.lw.addHeader( "Summary :", 4 )

		for stream in xmlParser.streams :
			summaryTable = LatexTable()
			summaryTable.setContent( self.getSummarySection( stream ) )
			summaryTable.setTitle( stream.title )
			summaryTable.setBorders( 'header' )
			summaryTable.setRowAlignment( 0, 'left', True )
			summaryTable.setRowAlignment( 1, 'center' )
			summaryTable.setRowAlignment( 2, 'right' )
			summaryTable.setTableAlignment('center')
			self.lw.addTable( summaryTable, False, "0.7\\textwidth" )
			self.lw.addSimpleLineBreak()

		self.lw.addMacro( "newpage" )

		# ========== Detail ==========
		for stream in xmlParser.streams :
			self.lw.addHeader( stream.title + " :", 4 )
			for description in stream.descriptions :
				if description.title == "fileValidator" :
					self.lw.addHeader( "Detail :", 5 )
				if description.title == "loudness" :
					self.lw.addHeader( "Loudness :", 5 )

				for section in description.sections :
					if isinstance( section, SpecificationSection ):
						self.addQualityCheckDetailSection( section )
					if isinstance( section, LoudnessSection ):
						self.addLoudnessSection( section )
						self.lw.addMacro( "newpage" )

		self.lw.endDocument()

	def getLatexStream( self ):
			return self.lw.getStream()
