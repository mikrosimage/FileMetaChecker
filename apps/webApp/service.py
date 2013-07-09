#!/usr/bin/python
import os
import datetime

import cherrypy
from cherrypy import tools
from cherrypy.lib.static import *

from tools import *

current_dir = os.path.dirname(os.path.abspath(__file__))
# print current_dir

f = open( "pages/header.html", "r" )
pageHeader = f.read()
f.close()
f = open( "pages/form_content.html", "r" )
pageForm = f.read()
f.close()
f = open( "pages/footer.html", "r" )
pageFooter = f.read()
f.close()

class Root(object):
	def __init__( self ):
		self.downloadFilepath = current_dir + "/downloads/fileToDownload.pdf"
		self.reportGen = ReportGenerator( current_dir )

	@cherrypy.expose
	def index( self ):
		self.reportGen.clearTempFiles()
		htmlFile  = pageHeader
		htmlFile += pageForm
		htmlFile += pageFooter
		return htmlFile;

	@cherrypy.expose
	@cherrypy.tools.json_out()
	def ls( self, indir= "/" ):
		list = []
		root, inname = os.path.split( indir )
		if not os.path.isdir( indir ):
			if os.path.exists( indir ):
				return indir
			else:
				if os.path.exists( os.path.dirname( indir ) ):
					indir = os.path.dirname( indir )
				else:
					return ""

		for elem in os.listdir( indir ):
			if elem.startswith( inname ) :
				list.append( os.path.join( indir, elem ) )
		list.sort()
		return list

	@cherrypy.expose
	def execute(
		self, 
		filename,
		pdf=False,
		loudness=False, 
		ebu=False, 
		cst=False, 
		atsc=False
		):

		try :
			date = datetime.datetime.now()
			root.download = Download( filename )
			print ">>> Execute : " + filename + ", at " + str(date)

			loudnessStandard = []
			if ebu :
				loudnessStandard.append(  "ebu" )
			if cst :
				loudnessStandard.append(  "cst" )
			if atsc :
				loudnessStandard.append( "atsc" )

			self.reportGen.reports.append( fileAnalyse( filename ) )
			if loudness :
				self.reportGen.reports.append( loudnessAnalyse( filename, loudnessStandard ) )
			
			reportPage  = pageHeader
			reportPage += "<div id='main-header'>"
			reportPage += "<div id='date'>Launched at : " + str(date) + "</div>"
			if pdf :
				reportPage += "<div id='download-button'><a href='/download/?filepath=" + self.downloadFilepath + "'>Download PDF Version</a><br/></div>"
			reportPage += "</div>"
			reportPage += self.reportGen.generateReport( pdf )
			reportPage += pageFooter
			
			# CLEAR
			loudnessStandard = []
			self.reportGen.reports = []

			return reportPage;

		except Exception, e:
			error = "Error : " + str( e )
			print error;


if __name__ == '__main__' :
	root = Root()
	cherrypy.quickstart( root, '/', 'config' )
