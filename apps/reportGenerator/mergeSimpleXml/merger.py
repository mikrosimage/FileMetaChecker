#!/usr/bin/python

from xml.etree import ElementTree


def mergeXml( inXmls, node="root" ):
	outXmlTree = ElementTree.ElementTree()
	mainXml = ElementTree.Element( node )

	index = 0
	for filename in inXmls:
		try:
			root = ElementTree.parse( filename ).getroot()
			mainXml.insert( index, root )
			index += 1
		except:
			print( "unable to merge", filename )

	return outXmlTree.dump( outXmlTree )
