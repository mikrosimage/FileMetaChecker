#!/usr/bin/python
import sys
import os
import datetime
current_dir = os.path.dirname(os.path.abspath(__file__))
print current_dir

import cherrypy
from cherrypy import tools

from subprocess import *

import xml.etree.ElementTree as ET

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.MIMEBase import MIMEBase
from email import Encoders

sys.path.append( os.environ['WRITER_ROOT'] + '/pythonToHtml' )
sys.path.append( os.environ['REPORT_ROOT'] )
sys.path.append( os.environ['REPORT_ROOT'] + '/mergeSimpleXml' )

from xmlToHtml import *
from merger import *

emitter = "noreply@domain"

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
		self.reports = []

	@cherrypy.expose
	def index( self ):
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


	def fileAnalyse( self, inputFile ):
		print "*** CHECK ! ***"
		print inputFile
		cmdQc = "mikqc " + inputFile
		processQc = Popen( cmdQc, stdout=PIPE, stderr=PIPE, env=os.environ, shell=True )
		mikqcReport = inputFile + ".xml"
		print "*** DONE ! (CHECK) ***"
		return mikqcReport


	def loudnessAnalyse( self, inputFile, loudnessStandard=["ebu"] ):
		# print "*** LOUDNESS ! ***"
		# print inputFile
		# print loudnessStandard

		loudness = ""
		for standard in loudnessStandard :
			# print standard
			loudness += " --standard=" + standard + " "
		loudnessCmd = os.environ['LOUDNESS_PATH'] + " "  + loudness + inputFile
		# print loudnessCmd
		call( loudnessCmd, shell=True )

		loudnessReport = inputFile.replace( ".wav", "_PLoud.xml" )

		# print "*** DONE ! (LOUDNESS) ***"
		return loudnessReport


	def generateReport( self, pdf=False ):
		# print "*** REPORT ! ***"
		print self.reports

		# print " >>> MERGE <<< "
		xmlStream = mergeXml( self.reports )

		stream = ""		
		parser = XmlParser()
		parser.setXmlStream( xmlStream )
		root = parser.getRoot()
		# print root
		if not pdf :
			xth = XmlToHtml()
			main = ET.Element( "div" )
			main.set( "id", "main" )
			xth.setPageContent( root, main )
			# print list( main )
			for child in list( main ) :
				stream += ET.tostring( child )
		# print "*** DONE ! (REPORT) ***"

		return stream;

	# def sendReport( self, email, date ):
	# 	print "emitter  : " + emitter
	# 	print "receiver : " + email
	# 	if email.count( "@mikrosimage.eu", len( email )-15, len( email ) ) != 1 :
	# 		raise RuntimeError( "Forbidden e-mail address !" )

	# 	receiver = email
	# 	msg = MIMEMultipart('alternative')
	# 	msg['Subject'] = "Quality Check report"
	# 	msg['From'] = emitter
	# 	msg['To'] = receiver
	# 	text  = "Hi there !\n\n"
	# 	text += "Here is your QC report !\n"
	# 	text += "QC started on : " + date + ".\n\n"
	# 	text += "Keep in touch ! ;)\n\n"
	# 	text += "- The QC Team"

	# 	part1 = MIMEText(text, 'plain')
	# 	msg.attach(part1)

	# 	server = smtplib.SMTP('localhost')
	# 	server.sendmail( emitter, receiver, msg.as_string() )
	# 	server.quit()

	def clearTempFiles( self ):
		for filepath in self.reports :
			cmdClear = "rm " + filepath
			call( cmdClear, shell=True )

	@cherrypy.expose
	def execute(
		self, 
		filename, 
		loudness, 
		ebu=False, 
		cst=False, 
		atsc=False 
		):

		try :
			date = datetime.datetime.now()
			print ">>> Execute : " + filename + ", at " + str(date)

			loudnessStandard = []
			if ebu :
				loudnessStandard.append(  "ebu" )
			if cst :
				loudnessStandard.append(  "cst" )
			if atsc :
				loudnessStandard.append( "atsc" )

			self.reports.append( self.fileAnalyse( filename ) )
			if loudness :
				self.reports.append( self.loudnessAnalyse( filename, loudnessStandard ) )
			

			reportPage  = pageHeader
			reportPage += "<div id='date'>" + str(date) + "</div>"
			reportPage += self.generateReport()
			reportPage += pageFooter
			
			# print reportPage
			# file = open( "reportPage.html", "w+" )
			# file.write( reportPage )
			# file.close()

			# CLEAR
			self.clearTempFiles()
			loudnessStandard = []
			self.reports = []

			return reportPage;

		except Exception, e:
			error = "Error : " + str( e )
			print error;


cherrypy.quickstart( Root(), '/', 'config' )
