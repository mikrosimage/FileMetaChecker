import os
import datetime

import cherrypy
from cherrypy import tools
from cherrypy.lib.static import *

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import Encoders

emitter = "noreply@domain"

def sendReport( email, date ):
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