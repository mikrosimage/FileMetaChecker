#!/usr/bin/python
import os
from xmlParser import *
import xml.etree.ElementTree as ET
from htmlwriter import *

class XmlToHtml():
	def __init__( self ):
		self.hw = HtmlWriter()
		self.page = ET.Element('html')
		self.page.set( "lang", "fr" )
		self.head = ET.SubElement( self.page, "head" )
		self.body = ET.SubElement( self.page, "body" )
		self.plotNum = 0
		self.plots = []

		if type( os.getenv( "QC_RESOURCES" ) ) is not str :
			raise ValueError( "You must define the 'QC_RESOURCES' macro" )
		self.resourcePath = os.getenv( "QC_RESOURCES" )

	def getHtml( self ):
		doctype = "<!DOCTYPE html>"
		content = ET.tostring( self.page, method="html" )
		return doctype + content

	def setHeader( self ):
		meta = ET.SubElement( self.head, "meta" )
		meta.set( "charset", "utf-8" )
		title = ET.SubElement( self.head, "title" )
		title.text = "QUALITY CHECK"
		css = ET.SubElement( self.head, "link" )
		css.set( "rel", "stylesheet" )
		css.set( "href", self.resourcePath + "css/style.css" )
		script1 = ET.SubElement( self.head, "script" )
		script1.set( "type", "text/javascript" )
		script1.set( "src", self.resourcePath + "js/jquery-1.10.1.js" )
		script2 = ET.SubElement( self.head, "script" )
		script2.set( "type", "text/javascript" )
		script2.set( "src", self.resourcePath + "js/jquery-ui-1.10.3.js" )
		script3 = ET.SubElement( self.head, "script" )
		script3.set( "type", "text/javascript" )
		script3.set( "src", "https://www.google.com/jsapi" )
		script4 = ET.SubElement( self.head, "script" )
		script4.set( "type", "text/javascript" )
		script4.set( "src", 'https://www.google.com/jsapi?autoload={"modules":[{"name":"visualization","version":"1","packages":["corechart","table"]}]}' )

	def setPageHeader( self ):
		header = ET.SubElement( self.body, "div" )
		header.set( "id", "header" )
		headerContent = ET.SubElement( header, "div" )
		headerContent.set( "id", "header-content" )
		title = ET.SubElement( headerContent, "div" )
		title.set( "id", "title" )
		logo = ET.SubElement( title, "img" )
		logo.set( "id", "logo-qc" )
		logo.set( "src", self.resourcePath + "img/logo_qc.png" )
		titleText = ET.SubElement( title, "div" )
		titleText.set( "id", "title-text" )
		titleText.set( "title", "Quality Check" )
		titleText.text = "Quality Check"
		logoMikros = ET.SubElement( header, "img" )
		logoMikros.set( "id", "logo-mikros" )
		logoMikros.set( "src", self.resourcePath + "img/logo_mikros.png" )

	def setPageFooter( self ):
		js = ET.SubElement( self.page, "script" )
		js.set( "type", "text/javascript" )
		js.set( "src", self.resourcePath + "js/script.js" )

	def getPlots( self, element ) :
		for child in list( element ) :
			if child.get( "type" ) == "plot" :
				self.plots.append( child )
			else :
				self.getPlots( child )
				continue
	
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

	def setPlots( self, parent, elementTag, parent_map ) :
		# print self.plots
		for i, plot in enumerate( self.plots ) :
			childPlot = []
			# print str(plot) + " : " + str(parent_map[ plot ])
			values = []
			values.append( plot.tag )
			values.extend( plot.text.split( ", " ) )
			childPlot.append( values )
			for j, otherPlot in enumerate( self.plots ) :
				if i != j and parent_map[ plot ] == parent_map[ otherPlot ] :
					# print "+ " + str(otherPlot) + " : " + str(parent_map[ otherPlot ])
					values = []
					values.append( otherPlot.tag )
					values.extend( otherPlot.text.split( ", " ) )
					childPlot.append( values )
			# print len( childPlot )
			# print childPlot
			self.plots.remove( plot )

			graphScript = ""
			graphScript += "function drawChart_" + str( self.plotNum ) + "() {"
			graphScript += "var data = google.visualization.arrayToDataTable("
			graphScript += str( self.sortPlotsValues( childPlot ) )
			graphScript += """);
					var options = {
						chartArea: {width: "80%", height: "80%"},
						areaOpacity: 0.3,
						title: '"""
			graphScript += parent_map[ plot ].get( "label" )
			graphScript += """',
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
					var chart = new google.visualization.AreaChart(document.getElementById('"""
			graphScript += elementTag + "-graph-" + str( self.plotNum )
			graphScript += """'));
					chart.draw(data, options);
				}
				"""

			script = ET.SubElement( parent, "script" )
			script.set( "type", "text/javascript" )
			script.text = graphScript
			
			graph = ET.SubElement( parent, "div" )
			graph.set( "class", "chart_div" )
			graph.set( "id", elementTag + "-graph-" + str( self.plotNum ) )

			self.plotNum += 1

	def setPageContent( self, root ):
		main = ET.SubElement( self.body, "div" )
		main.set( "id", "main" )

		section = ET.SubElement( main, "section" )
		section.set( "class", "accordion" )
		
		for child in list( root ):
			self.plots = []
			# print child.tag
			div = ET.SubElement( section, "div" )

			index = ""
			if child.get( "index" ) :
				index = child.get( "index" )
			childTag = child.tag + index

			input = ET.SubElement( div, "input" )
			input.set( "id", childTag + "-title" )
			input.set( "type", "checkbox" )

			label = ET.SubElement( div, "label" )
			label.set( "for", childTag + "-title" )
			label.text = child.get( "label" )

			article = ET.SubElement( div, "article" )
			# article.set( "class", childTag + index + "-content" )

			content = ET.SubElement( article, "div" )
			content.set( "class", "section" )
			content.set( "id", childTag + index )

			table = HtmlTable()
			parent_map = dict( (c, p) for p in root.getiterator() for c in p )
			# print parent_map
			# file = open( "parent_map.txt", "w+" )
			# file.write( str(parent_map) )
			# file.close()

			if list( child )[0].get( "label" ) :
				# print "CASE CHILD : " + child.tag
				table.getTableElement( content, child )
				self.getPlots( child )
			else :
				for node in list( child ) :
					# print "CASE NODE : " + node.tag
					table.getTableElement( content, node )
					self.getPlots( node )
			
			self.setPlots( content, childTag, parent_map )
			

		# ========== Draw charts ========
		displayText = ""
		displayText += "function displayCharts() {\n"
		for i in range( 0, self.plotNum ) :
			displayText += "google.setOnLoadCallback( drawChart_" + str(i) + "() )\n"
		displayText += "}"

		displayScript = ET.SubElement( main, "script" )
		displayScript.set( "type", "text/javascript" )
		displayScript.text = displayText


	def convertToFile( self, xmlParser ) :
		if not isinstance( xmlParser, XmlParser ):
			raise ValueError("request a 'XmlParser' object")
		
		logoPath = self.resourcePath + "imageCheckOK256.png"
		root = xmlParser.getRoot()

		self.setHeader()
		self.setPageHeader()
		self.setPageContent( root )
		self.setPageFooter()
