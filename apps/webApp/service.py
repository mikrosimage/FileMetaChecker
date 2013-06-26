#!/usr/bin/python
import sys
import os
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

class Root(object):

	@cherrypy.expose
	def index( self ):
		f = open( "index.html", "r" )
		htmlFile = f.read()
		f.close()
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


	# @cherrypy.expose
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
	def execute( self, filename, email, date ):
		env = os.environ
		reportString = ""
		try :
			reportString = self.generateReport( filename )
			print date
			if email != "" :
				self.sendReport( email, date )
			return reportString;

		except Exception, e:
			reportString = "Error : " + str( e )
			print reportString


cherrypy.quickstart( Root(), '/', 'config' )



	# @cherrypy.expose
	# #@tools.json_out()
	# def execute( self, filename ):
	# 	error = ""
	# 	result = ""
	# 	env = os.environ
	# 	try :
	# 		# cmdQc = "/datas/van/dev/BroadcastFilseQualityChecker/.dist/devpc16/clang-4.6/release /apps/mikqc " + filename
	# 		# env["LD_LIBRARY_PATH"] = "/datas/van/dev/lib/boost/stage/lib/"
	# 		# env["QC_SPECS_DIR"] = "/datas/van/dev/BroadcastFileQualityChecker/fileSpecification"
	# 		# processQc = Popen( cmd, stdout=PIPE, stderr=PIPE, env=env, shell=True )
	# 		# resultQc = processQc.stdout
	# 		# errorQc = processQc.stderr

	# 		env["QC_RESOURCES"] = "/datas/van/dev/BroadcastFileQualityChecker/apps/reportGenerator/"
	# 		env["PYTHONPATH"] = "/datas/van/dev/python-html-writer"
	# 		cmdReport = "/datas/van/dev/BroadcastFileQualityChecker/apps/reportGenerator/qcReportGeneratorHtml.py -f html -i " + filename
	# 		processReport = Popen( cmdReport, stdout=PIPE, stderr=PIPE, env=env, shell=True )
	# 		# print "processReport : "
	# 		# print processReport
	# 		resultReport = processReport.stdout
	# 		errorReport = processReport.stderr

	# 		print "resultReport : "
	# 		# print resultReport
	# 		print resultReport.read()
	# 		# print "errorReport : "
	# 		# print errorReport
	# 		# print errorReport.read() 
	# 	except :
	# 		return result + error;
	# 	print "RETURN"
	# 	return resultReport.read();