#!/usr/bin/python
from latexwriter import *
from xmlToLatex import *

from optparse import OptionParser

######  OPTIONS  ######
optionParser = OptionParser()
optionParser.add_option(	"-f", "--file",
					type="string",
					dest="filename",
					action="append",
					help="generate Latex from XML QC report file",
					metavar="FILE"	)

optionParser.add_option(	"-l", "--list",
					type="string",
					dest="fieldsfile",
					action="append",
					help="display all available fields",
					metavar="FILE"	)

optionParser.add_option(	"-c", "--choice",
					type="string",
					dest="fieldsToRemove",
					action="append",
					help="list of fields to remove from the PDF output file",
					metavar="FIELDS"	)

optionParser.add_option(	"-o", "--output",
					type="string",
					dest="outputFile",
					action="store",
					help="output Latex filename",
					metavar="OUTPUT"	)

(options, args) = optionParser.parse_args()
# print options
# print args
parser = XmlParser()

######  List of Fields  ######
if options.fieldsfile :
	for arg in args:
		options.fieldsfile.append( arg )
	for fieldsfile in options.fieldsfile :
		parser.parseXml( fieldsfile )
		if options.fieldsToRemove :
			for arg in args:
				options.fieldsfile.append( arg )
			for field in options.fieldsToRemove :
				parser.removeField( field )
		parser.displaySections()


######  Display Help  ######
if len( args ) == 0 and options.filename == None and options.fieldsfile == None:
	optionParser.print_help()
	sys.exit()


######  Generate Report  ######
if options.filename:
	for inputFile in options.filename :

		if options.outputFile :
			outputFile = options.outputFile
		else:
			outputFile = inputFile.replace( "-merged.xml", ".tex" )
		print ">>> Output PDF filename : '" + outputFile

		parser.parseXml( inputFile )
		if options.fieldsToRemove :
			for arg in args:
				options.fieldsfile.append( arg )
			for field in options.fieldsToRemove :
				parser.removeField( field )
		xtl = XmlToLatex()
		xtl.convert( parser )

		# ### OUTPUT ###
		file = open( outputFile, "w+" )
		file.write( xtl.getLatexStream() )
		file.close()