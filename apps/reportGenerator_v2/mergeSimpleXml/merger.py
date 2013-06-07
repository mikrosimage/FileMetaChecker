#!/usr/bin/python
import sys
from xml.etree import ElementTree
from optparse import OptionParser

######  OPTIONS  ######
optionParser = OptionParser()
optionParser.add_option(	"-f", "--file",
					type="string",
					dest="fileList",
					action="append",
					help="merge XML files",
					metavar="FILE"	)

optionParser.add_option(	"-o", "--output",
					type="string",
					dest="outputFile",
					action="store",
					help="output XML filename",
					metavar="OUTPUT"	)


(options, args) = optionParser.parse_args()

if len( args ) == 0 and options.fileList == None :
	optionParser.print_help()
	sys.exit()

def mergeXml( xmlFilesList, finalXmlFilename ):
	finalXml = ElementTree.ElementTree()	
	xmlCombinator = ElementTree.Element('root')

	i = 0
	for filename in xmlFilesList:
		print filename
		root = ElementTree.parse(filename).getroot()
		xmlCombinator.insert(i, root)
		i+=1

	if len(list(xmlCombinator)) != 0:
		finalXml._setroot( xmlCombinator )
		finalXml.write( finalXmlFilename )


if options.fileList :

	if options.outputFile :
		outputFile = options.outputFile
	else:
		outputFile = options.fileList[0].replace( ".xml", "-merged.xml" )
	print ">>> Output XML filename : '" + outputFile
	
	mergeXml( options.fileList, outputFile )