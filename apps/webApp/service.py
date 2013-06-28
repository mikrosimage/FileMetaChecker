#!/usr/bin/python
import sys
import os
import datetime
current_dir = os.path.dirname(os.path.abspath(__file__))
print current_dir

import cherrypy
from cherrypy import tools

from subprocess import *

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.MIMEBase import MIMEBase
from email import Encoders

sys.path.append( os.environ['WRITER_ROOT'] + '/pythonToHtml' )
sys.path.append( os.environ['REPORT_ROOT'] )
from xmlToHtml import *

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
		print inputFile
		cmdQc = "mikqc " + filename
		processQc = Popen( cmd, stdout=PIPE, stderr=PIPE, env=env, shell=True )
		mikqcReport = inputFile + ".xml"
		return mikqcReport


	def loudnessAnalyse( self, inputFile, loudnessStandard ):
		print inputFile
		print loudnessStandard

		loudness = ""
		if type( loudnessStandard ) == str :
			loudness += " --standard=" + loudnessStandard + " "
		if len( loudnessStandard ) != 0 :
			for standard in loudnessStandard :
				loudness += " --standard=" + standard + " "

		loudnessCmd = os.environ['LOUDNESS_PATH'] + " "  + loudness + inputFile
		call( loudnessCmd, shell=True )
		loudnessReport = inputFile + "_PLoud.xml"

		return loudnessReport


	def generateReport( self, inputFile, pdf=False ):
		# print inputFile
		stream = ""
		parser = XmlParser()
		parser.parseXml( inputFile, "html" )
		if not pdf :
			xth = XmlToHtml()
			xth.convertToStream( parser )
			stream   = xth.getHtmlStream()
		return stream;

	def sendReport( self, email, date ):
		print "emitter  : " + emitter
		print "receiver : " + email
		if email.count( "@mikrosimage.eu", len( email )-15, len( email ) ) != 1 :
			raise RuntimeError( "Forbidden e-mail address !" )

		receiver = email
		msg = MIMEMultipart('alternative')
		msg['Subject'] = "Quality Check report"
		msg['From'] = emitter
		msg['To'] = receiver
		text  = "Hi there !\n\n"
		text += "Here is your QC report !\n"
		text += "QC started on : " + date + ".\n\n"
		text += "Keep in touch ! ;)\n\n"
		text += "- The QC Team"

		part1 = MIMEText(text, 'plain')
		msg.attach(part1)

		server = smtplib.SMTP('localhost')
		server.sendmail( emitter, receiver, msg.as_string() )
		server.quit()

	@cherrypy.expose
	def execute( self, filename, date=datetime.datetime.now(), display=True, loudness=False ):
		env = os.environ
		try :
			print filename
			print date
			# self.reports.append( self.fileAnalyse( filename ) )
			# if loudness :
			# 	self.reports.append( self.loudnessAnalyse( filename ) )
			reportPage  = pageHeader
			reportPage += "<div id='date'>" + str(date) + "</div>"
			reportPage += self.generateReport( filename )
			reportPage += pageFooter
			return reportPage;

		except Exception, e:
			error = "Error : " + str( e )
			print error;


cherrypy.quickstart( Root(), '/', 'config' )
