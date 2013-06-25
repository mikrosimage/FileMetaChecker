#!/usr/bin/python
from xmlParser import *
from htmlwriter import *

class XmlToHtml():
	def __init__( self ):
		self.hw      = HtmlWriter()

	def setReportDocumentBeginning( self ):
		self.hw.addDocumentHeader()
		self.hw.addOpeningTag( "html", "lang=\"fr\"" )
		self.hw.addOpeningTag( "head" )
		self.hw.addOpeningTag( "meta", "charset=\"utf-8\"", True )
		self.hw.addTitle( "QUALITY CHECK")
		self.hw.addLinkHeader( "stylesheet", "style.css" )
		self.hw.addScriptHeader( "https://www.google.com/jsapi", "text/javascript" )
		self.hw.addClosingTag( "head" )
		self.hw.addOpeningTag( "body" )
	
	def setReportDocumentTitle( self, filename, version, logoPath ):
		self.hw.addOpeningTag( "div", "id=\"header\"" )
		self.hw.addImage( logoPath, "id=\"logo-qc\"" )
		self.hw.addHeader( "Quality Check", 1 )
		self.hw.addText( "Version " + str(version), "italic" )
		self.hw.addHeader( filename, 2 )
		self.hw.addClosingTag( "div" )

	def setReportDocumentEnding( self ):
		self.hw.addClosingTag( "body" )
		self.hw.addClosingTag( "html" )

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
						dataArray[i][j] = "<font color=\"#f00\">" + str( dataArray[i][j] ) + "</font>"
				elif type( dataArray[i][j] ) is list :
					for k in range( 0, len(dataArray[i][j]) ):
						if statusArray[i] != "valid" and statusArray[i] != "" :
							dataArray[i][j] = "<font color=\"#f00\">" + str( dataArray[i][j] ) + "</font>"
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
					array[i][j] = transformSpecialCharacters( array[i][j] )
					if checkStrings :
						array[i][j] = checkString( array[i][j], 50 )
				elif type( array[i][j] ) is list and len( array[i][j] ) > 0 :
					array[i][j] = self.checkSpecialDataCharacters( array[i][j] )
		return array;

	def addFileSystemInfoSection( self, section ):
		section.setFieldsValues()
		section.data = self.checkSpecialDataCharacters( section.data )
		
		self.hw.addOpeningTag( "div", "class=\"section\" id=\"file-system\"" )
		fsiTable = HtmlTable()
		fsiTable.setContent( section.data )
		self.hw.addTable( fsiTable, "file-system-table" )
		self.hw.addClosingTag( "div" )

		# print "\nFileSystemInfoSection : " + section.title
		# print section.data
		# print section.dataStatus


	def addSummarySectionLine( self, section ):
		section.setFieldsValues()

		sumLine = []
		sumLine.append( section.title )
		sumLine.append( section.date  )
		if section.status == "valid" :
			sumLine.append( "<font color=\"#0f0\">" + section.status + "</font>" )
		else :
			sumLine.append( "<font color=\"#f00\">" + section.status + "</font>" )
		return sumLine;

		# print section.title
		# print section.data
		# print section.dataStatus

	def getSummarySection( self, stream ):
		summaryData = []
		for description in stream.descriptions :
			if description.title == "fileValidator" :
				summaryData.append( ["<strong>Specification</strong>", "<strong>Date</strong>", "<strong>Status</strong>"] )
				for section in description.sections :
					if isinstance( section, SpecificationSection ):
						summaryData.append( self.addSummarySectionLine( section ) )
			
			if description.title == "loudness" :
				summaryData.append( [ "<strong>- Loudness -</strong>", "", "" ] )
				for section in description.sections :
					if isinstance( section, LoudnessSection ):
						summaryData.append( self.addSummarySectionLine( section ) )

		return summaryData;

	def addQualityCheckDetailSection( self, section ):
		self.hw.addOpeningTag( "div", "class=\"section\" id=\"detail\"" )
		detailTable = HtmlTable()
		detailTableData = []
		detailTableData.append( [ "<strong>" + section.title + "</strong><br><font color=\"#777\">" + section.date + "</font>", "" ] )

		for pair in section.data :
			detailTableData.append( pair )
		for s in range( 0, len( section.group ) ) :
			section.group[s].setFieldsValues()
			detailTableData.append( [ section.group[s].title + ": <i>" + section.group[s].status + "</i>", "" ] )
			section.group[s].data = self.checkSpecialDataCharacters( section.group[s].data, True )
			for pair in section.group[s].data :
				detailTableData.append( pair )
			# print section.group[s].data
			# print section.group[s].dataStatus
		color = "#0f0" if section.status == "valid" or section.status == "not illegal" else "#f00"
		status = "<font color=\"" + color + "\">" + section.status + "</font>"

		detailTableData.append( ["<strong>Status : " + status + "</strong>"] )

		detailTable.setContent( detailTableData )
		self.hw.addTable( detailTable, "detail-table" )
		self.hw.addClosingTag( "div" )

		# print "\nSpecificationSection : " + section.title
		# print section.data
		# print detailTableData

	def getPlotsValues( self, plotsList ) :
		data = []

		size = len( plotsList[0][1] )
		# print size
		for i in range( 1, len(plotsList) ) :
			if len(plotsList[i][1]) != size :
				# print plotsList[i][1]
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
					if plot[1][j-1] == "-inf" :
						plot[1][j-1] = "-100"
					line.append( float( plot[1][j-1] ) )
			data.append( line )

		return data;


	def addLoudnessSection( self, section, loudnessnum ):
		if len( section.data ) == 0 :
			section.setFieldsValues()
		
		self.hw.addOpeningTag( "div", "class=\"section\" id=\"loudness-summary\"" )
		loudnessTable = HtmlTable()
		loudnessTableData = []
		loudnessTableData.append( [ "<strong>" + section.title + "</strong><br><font color=\"#777\">" + section.date + "</font>", "" ] )
		for pair in self.dataStatusColor( section.data, section.dataStatus ) :
			loudnessTableData.append( pair )

		loudnessTable.setContent( loudnessTableData )
		self.hw.addTable( loudnessTable, "loudness-table" )
		self.hw.addClosingTag( "div" )

		graphScript = ""
		graphScript += "function drawChart_" + str(loudnessnum) + "() {"
		graphScript += "var data = google.visualization.arrayToDataTable("
		graphScript += str( self.getPlotsValues( section.plots ) )
		graphScript += """);
				var options = {
					chartArea: {width: "80%", height: "80%"},
					areaOpacity: 0.3,
					title: 'Loudness',
					titleTextStyle: {color: 'white', fontSize: 20},
					legend : {position: 'bottom', textStyle: {color: 'white'}},
					backgroundColor: '#222222',
					vAxis: {
						title: 'Level [dB]', 
						textStyle: {color: 'white'}, 
						titleTextStyle: {color: 'white'},
						gridlines: {count: 8},
						viewWindow:{ max:   0, min: -70 },
						viewWindowMode:'explicit'
					},
					hAxis: {title: 'Time [s]', textStyle: {color: 'white'}, titleTextStyle: {color: 'white'}}
				};
				var chart = new google.visualization.AreaChart(document.getElementById('loudness-graph-"""
		graphScript += str(loudnessnum)
		graphScript += """'));
				chart.draw(data, options);
			}
			"""
		self.hw.addScript( graphScript, "text/javascript" )
		self.hw.addOpeningTag( "div", 'class="chart_div" id="loudness-graph-' + str(loudnessnum) + '"' )
		self.hw.addClosingTag( "div" )
		
		# print "\nLoudnessSection : " + section.title
		# print section.data
		# print section.dataStatus
		# print section.plots


	def convertToFile( self, xmlParser ):
		######     XML     ######
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")
		
		if type( os.getenv( "QC_RESOURCES" ) ) is not str :
			raise ValueError( "You must define the 'QC_RESOURCES' macro" )

		logoPath = os.getenv( "QC_RESOURCES" ) + "imageCheckOK256.png"

		filename = xmlParser.xmlFilename.replace( "-merged.xml", "" )
		if filename == xmlParser.xmlFilename :
			filename = xmlParser.xmlFilename.replace( "-audio.xml", "" )

		self.setReportDocumentBeginning()
		self.setReportDocumentTitle( filename, "1.0", logoPath )
		
		self.hw.addOpeningTag( "section", "class=\"accordion\"" )

		######   System Information  ######
		self.hw.addOpeningTag( "div" )
		self.hw.addOpeningTag( "input", "id=\"system-title\" type=\"checkbox\" checked", True )
		self.hw.addOpeningTag( "label", "for=\"system-title\"")
		self.hw.addText( "System Information" )
		self.hw.addClosingTag( "label" )

		self.hw.addOpeningTag( "article", "class=\"system-content\"")
		for section in xmlParser.sections :
			if isinstance( section, FileSystemInfoSection ):
				self.addFileSystemInfoSection( section )
		self.hw.addClosingTag( "article" )
		self.hw.addClosingTag( "div" )

		######   SpecificationSection  ######
		# ========== Summary ==========
		self.hw.addOpeningTag( "div" )
		self.hw.addOpeningTag( "input", "id=\"summary-title\" type=\"checkbox\" checked", True )
		self.hw.addOpeningTag( "label", "for=\"summary-title\"")
		self.hw.addText( "Quality Check - Summary" )
		self.hw.addClosingTag( "label" )

		self.hw.addOpeningTag( "article", "class=\"summary-content\"")
		for stream in xmlParser.streams :
			self.hw.addOpeningTag( "div", "class=\"section\" id=\"summary\"" )
			summaryTable = HtmlTable()
			summaryTable.setContent( self.getSummarySection( stream ) )
			summaryTable.setTitle( stream.title )
			self.hw.addTable( summaryTable, "summary-table" )
			self.hw.addClosingTag( "div" )

		self.hw.addClosingTag( "article" )
		self.hw.addClosingTag( "div" )

		# ========== Detail ==========
		loudnessnum = 0

		for i, stream in enumerate( xmlParser.streams ) :
			self.hw.addOpeningTag( "div" )
			self.hw.addOpeningTag( "input", "id=\"detail-" + str(i) + "-title\" type=\"checkbox\"", True )
			self.hw.addOpeningTag( "label", "for=\"detail-" + str(i) + "-title\"")
			self.hw.addText( "Quality Check - " + stream.title )
			self.hw.addClosingTag( "label" )

			self.hw.addOpeningTag( "article", "class=\"detail-content\"")
			for description in stream.descriptions :
				if description.title == "fileValidator" :
					self.hw.addHeader( "Detail :", 3 )
				if description.title == "loudness" :
					self.hw.addHeader( "Loudness :", 3 )

				for section in description.sections :
					if isinstance( section, SpecificationSection ):
						self.addQualityCheckDetailSection( section )
					if isinstance( section, LoudnessSection ):
						self.addLoudnessSection( section, loudnessnum )
						loudnessnum += 1
		
			self.hw.addClosingTag( "article" )
			self.hw.addClosingTag( "div" )

		# ========== Draw charts ========
		displayScript = ""
		displayScript += "function displayCharts() {\n"
		for i in range( 0, loudnessnum ) :
			displayScript += "google.setOnLoadCallback( drawChart_" + str(i) + "() )\n"
		displayScript += "}"

		self.hw.addScript( displayScript, "text/javascript" )
		self.hw.addOpeningTag( "div", "id=\"fake\" onclick=displayCharts()" )
		self.hw.addClosingTag( "div" )

		self.hw.addClosingTag( "section" )

		self.setReportDocumentEnding()

	def convertToStream( self, xmlParser ):
		######     XML     ######
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")
		
		self.hw.addOpeningTag( "section", "class=\"accordion\"" )

		######   System Information  ######
		self.hw.addOpeningTag( "div" )
		self.hw.addOpeningTag( "input", "id=\"system-title\" type=\"checkbox\" checked", True )
		self.hw.addOpeningTag( "label", "for=\"system-title\"")
		self.hw.addText( "System Information" )
		self.hw.addClosingTag( "label" )

		self.hw.addOpeningTag( "article", "class=\"system-content\"")
		for section in xmlParser.sections :
			if isinstance( section, FileSystemInfoSection ):
				self.addFileSystemInfoSection( section )
		self.hw.addClosingTag( "article" )
		self.hw.addClosingTag( "div" )

		######   SpecificationSection  ######
		# ========== Summary ==========
		self.hw.addOpeningTag( "div" )
		self.hw.addOpeningTag( "input", "id=\"summary-title\" type=\"checkbox\" checked", True )
		self.hw.addOpeningTag( "label", "for=\"summary-title\"")
		self.hw.addText( "Quality Check - Summary" )
		self.hw.addClosingTag( "label" )

		self.hw.addOpeningTag( "article", "class=\"summary-content\"")
		for stream in xmlParser.streams :
			self.hw.addOpeningTag( "div", "class=\"section\" id=\"summary\"" )
			summaryTable = HtmlTable()
			summaryTable.setContent( self.getSummarySection( stream ) )
			summaryTable.setTitle( stream.title )
			self.hw.addTable( summaryTable, "summary-table" )
			self.hw.addClosingTag( "div" )

		self.hw.addClosingTag( "article" )
		self.hw.addClosingTag( "div" )

		# ========== Detail ==========
		loudnessnum = 0

		for i, stream in enumerate( xmlParser.streams ) :
			self.hw.addOpeningTag( "div" )
			self.hw.addOpeningTag( "input", "id=\"detail-" + str(i) + "-title\" type=\"checkbox\"", True )
			self.hw.addOpeningTag( "label", "for=\"detail-" + str(i) + "-title\"")
			self.hw.addText( "Quality Check - " + stream.title )
			self.hw.addClosingTag( "label" )

			self.hw.addOpeningTag( "article", "class=\"detail-content\"")
			for description in stream.descriptions :
				if description.title == "fileValidator" :
					self.hw.addHeader( "Detail :", 3 )
				if description.title == "loudness" :
					self.hw.addHeader( "Loudness :", 3 )

				for section in description.sections :
					if isinstance( section, SpecificationSection ):
						self.addQualityCheckDetailSection( section )
					if isinstance( section, LoudnessSection ):
						self.addLoudnessSection( section, loudnessnum )
						loudnessnum += 1
		
			self.hw.addClosingTag( "article" )
			self.hw.addClosingTag( "div" )

		# ========== Draw charts ========
		displayScript = ""
		displayScript += "function displayCharts() {\n"
		for i in range( 0, loudnessnum ) :
			displayScript += "google.setOnLoadCallback( drawChart_" + str(i) + "() )\n"
		displayScript += "}"

		self.hw.addScript( displayScript, "text/javascript" )
		self.hw.addOpeningTag( "div", "id=\"fake\" onclick=displayCharts()" )
		self.hw.addClosingTag( "div" )

		self.hw.addClosingTag( "section" )

	def getHtmlStream( self ):
			return self.hw.getStream()
