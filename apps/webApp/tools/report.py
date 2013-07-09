import os
import sys

sys.path.append( os.environ['REPORT_ROOT'] )
sys.path.append( os.environ['REPORT_ROOT'] + '/mergeSimpleXml' )
sys.path.append( os.environ['WRITER_ROOT'] + '/pythonToHtml' )
sys.path.append( os.environ['WRITER_ROOT'] + '/pythonToLatex' )

from subprocess import *

import xml.etree.ElementTree as ET

from xmlToHtml import *
from xmlToLatex import *
from merger import *

	
class ReportGenerator():
	def __init__( self, currentDir ) :
		self.reports = []
		self.currentDir = currentDir

	def clearTempFiles( self ):
		print "Clear..."
		for filepath in self.reports :
			cmdClear = "rm " + filepath
			call( cmdClear, shell=True )
		call( "rm " + self.currentDir + "/downloads/*", shell=True)

	def generateReport( self, pdf ):
		print "*** REPORT ! ***"
		print self.reports

		print " >>> MERGE <<< "
		xmlStream = mergeXml( self.reports )

		stream = ""		
		parser = XmlParser()
		parser.setXmlStream( xmlStream )
		root = parser.getRoot()

		print " >>> HTML <<< "
		xth = XmlToHtml()
		main = ET.Element( "div" )
		main.set( "id", "main" )
		xth.setPageContent( root, main )
		for child in list( main ) :
			stream += ET.tostring( child )

		if pdf :
			print " >>> PDF <<< "
			xtl = XmlToLatex()
			xtl.convert( parser )
			latexFilePath = self.currentDir + "/downloads/fileToDownload.tex"
			file = open( latexFilePath, "w+" )
			file.write( xtl.getLatexStream() )
			file.close()
			cmdPdf = 'pdflatex -output-directory=' + self.currentDir + '/downloads/ "' + latexFilePath + '"'
			call( cmdPdf, shell=True )

		print "*** DONE ! (REPORT) ***"

		return stream;