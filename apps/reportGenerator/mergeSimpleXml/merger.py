from xml.etree import ElementTree

def mergeXml( xmlFilesList, finalXmlFilename ):
	finalXml = ElementTree.ElementTree()	
	xmlCombinator = ElementTree.Element('root')

	i = 0
	for filename in xmlFilesList:
		root = ElementTree.parse(filename).getroot()
		xmlCombinator.insert(i, root)
		i+=1

	if len(list(xmlCombinator)) != 0:
		finalXml._setroot( xmlCombinator )
		finalXml.write( finalXmlFilename )