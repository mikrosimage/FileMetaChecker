#!/usr/bin/python
from xml.etree import ElementTree as ET

def mergeXml( inXmls, node="root" ):
	outXmlTree = ET.ElementTree()
	mainXml = ET.Element( node )

	for i, filename in enumerate( inXmls ):
		try:
			print filename
			root = ET.parse( filename ).getroot()
			if i == 0 :
				mainXml = root
			else :
				for child in mainXml :
					if child.tag == root.tag and child.get( "label") == root.get( "label") and child.get( "index" ) :
						for element in list( root ) :
							child.append( element )

					if child.tag == "status" :
						for subchild in child :
							if subchild.tag != root.tag or subchild.get( "label") != root.get( "label") or not subchild.get( "index" ) :
								continue
							for element in root :
								for subelement in element :
									status = ET.Element( element.tag )
									status.set( "standard", subelement.get( "standard" ) )
									status.set( "label",    subelement.get( "label"    ) )
									status.set( "status",   subelement.get( "status"   ) )
									subchild.append( status )

		except Exception as e:
			print str(e) + " - Unable to merge : " + filename

	# print type( mainXml )

	return ET.tostring( mainXml )
