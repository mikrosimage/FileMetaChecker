#!/usr/bin/python
from pandocmarkdownwriter import *

from mergeSimpleXml import *
from section import *

from optparse import OptionParser

from xml.dom.ext import StripXml
from xml.dom.ext.reader import Sax2


fileSystemInfo = "systemInfo"
specification = "specification"


def displayFields( filename ):
	print ">>> list of fields :" + filename

	file = open( filename )
	fileStream = file.read()
	file.close()

	reader = Sax2.Reader()
	docTree = reader.fromString( fileStream )
	StripXml( docTree )
	rootElement = docTree.documentElement

	# get QC sections from xml and display
	sectionsTitlesList = []
	sectionsTitlesList = getSectionsTitles( rootElement )

	for sectionTitles in sectionsTitlesList :
		if sectionTitles[0] == fileSystemInfo :
			section = FileSystemInfoSection( sectionTitles )
		else:
			section = Section( sectionTitles )
		section.setSectionFields( rootElement )
		section.displaySectionFields()
	return;


######  OPTIONS  ######
optionParser = OptionParser()
optionParser.add_option(	"-f", "--file",
					type="string",
					dest="filename",
					action="append",
					help="generate PDF from XML QC report file",
					metavar="FILE"	)

optionParser.add_option(	"-l", "--list",
					type="string",
					dest="fieldsfile",
					action="append",
					help="display all available fields",
					metavar="FILE"	)

optionParser.add_option(	"-c", "--choice",
					type="string",
					dest="fields",
					action="append",
					help="list of fields to remove from the PDF output file",
					metavar="FIELDS"	)

(options, args) = optionParser.parse_args()


if options.fieldsfile :
	for arg in args:
		options.fieldsfile.append( arg )
	for f in options.fieldsfile :
		displayFields( f )

# print options
# print args

# if no option nor filename :
if len( args ) == 0 and options.filename == None and options.fieldsfile == None:
	optionParser.print_help()
	sys.exit()


if options.filename:
	for inputFile in options.filename :

		outputfile = inputFile.replace( ".xml", ".md" )
		print ">>> Output PDF filename : '" + outputfile

		######   XML   ######
		file = open( inputFile )
		fileStream = file.read()
		file.close()

		reader = Sax2.Reader()
		docTree = reader.fromString( fileStream )
		StripXml( docTree )
		rootElement = docTree.documentElement

		sectionsTitlesList = []
		sectionsTitlesList = getSectionsTitles( rootElement )

		######   Mardown   ######
		file = open( outputfile, "w+" )
		pmd = PandocMarkdownWriter()

		# HEADER #
		pmd.addTitle( inputFile.replace( ".xml", "" ) )

		######   Write Document   ######
		if options.fields :
			for arg in args:
				options.fields.append( arg )

		for sectionTitles in sectionsTitlesList :
			print sectionTitles[0]

			if sectionTitles[0] == fileSystemInfo :
				section = FileSystemInfoSection( sectionTitles )
			else :
				section = Section( sectionTitles )

			section.setSectionFields( rootElement )

			if options.fields :
				for field in options.fields:
					section.removeSectionField( rootElement, field )

			section.setSectionData( rootElement )

			# ### TABLE ###
			t = PandocMarkdownTable()

			t.setTitle( sectionTitles[0] )
			t.setContent( section.data )
			for i in range( 0, len(section.data) ):
				t.setCellStyle( i, 0, 'bold')
			t.setBorders('frame')
			t.setRowAlignment( 0, 'left')
			t.setRowAlignment( 1, 'left')
			t.setTableAlignment('center')
			pmd.addTable( t )

			pmd.addHorizontalRule()


		# ### OUTPUT ###
		file.write( pmd.getStream() )
		file.close()