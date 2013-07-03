import xml.etree.ElementTree as ET

class XmlParser():
	def __init__( self ):
		self.xmlStream = ""
		self.hiddenFields = []

	def setXmlFile( self, filename ) :
		try :
			file = open( filename )
			buffer = file.read()
			ET.fromstring( buffer )
			self.xmlStream = buffer
			file.close()
		except TypeError as e :
			print "File path expected : '" + str(e) + "'"
			print "Trying to set input as stream..."
			self.setXmlStream( filename )
		except IOError as e :
			print "Invalid file path : trying to set input as stream..."
			self.setXmlStream( filename )
		except ET.ParseError as e :
			raise RuntimeError( "Invalid XML stream : '" + str(e) + "'" )

	def setXmlStream( self, xmlStream ) :
		if type( xmlStream ) is not str :
			raise ValueError( "xmlStream must be a string object !" )
		try :
			ET.fromstring( xmlStream )
			self.xmlStream = xmlStream
		except ET.ParseError as e :
			raise RuntimeError( "Invalid XML stream : '" + str(e) + "'" )

	def getRoot( self ):
		root = ET.fromstring( self.xmlStream )
		return root;

	def displayChildren( self, parent, level=1 ) :
		if list( parent ):
			for element in parent :
				if not self.hiddenFields.count( element.tag ) :
					print "\t"*level + str( element.tag )
					self.displayChildren( element, level + 1 )

	def display( self ):
		try : 
			root = ET.fromstring( self.xmlStream )
			print root.tag
			self.displayChildren( root )
		
		except ET.ParseError as e:
			print "Invalid XML stream : '" + str(e) + "'"

	def hideField( self, tagName ):
		if len( list( self.getRoot().iter( tagName ) ) ) == 0 :
			print "Warning : '" + tagName + "' element cannot be found."
		else :
			self.hiddenFields.append( tagName )
