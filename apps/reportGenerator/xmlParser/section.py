from utils import *

class Section():
	def __init__( self, title="" ):
		self.title      = title
		self.fields     = []
		self.data       = []
		self.status     = ""
		self.dataStatus = []

	def displayFields( self ):
		print self.title + " :"
		for field in self.fields:
			print "\t" + field.tagName

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes != None :
			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) )

			if len( rootChild.childNodes ) == 1 :
				data.append( rootChild.childNodes[0].nodeValue )
			else :
				multipleLines = []
				for child in rootChild.childNodes:
					if child.childNodes != None and child.nodeValue != None :
						multipleLines.append( child.nodeValue )
					else:
						for subChild in child.childNodes:
							if subChild.childNodes != None :
								for values in subChild.childNodes: 
									multipleLines.append( str( subChild.tagName  ) + " : " + str( values.nodeValue ) )
				data.append( multipleLines )
		return data

	def setFieldsValues( self ):
		for field in self.fields:
			data = self.getChildValue( field )
			self.data.append( data )




class FileSystemInfoSection( Section ):
	def getChildValue( self, rootChild ):
		data = []
		if rootChild.tagName == permissions :
			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 

			perms = []
			for child in rootChild.childNodes:
				if len( child.childNodes ) > 0 :
					rights = ""
					for subChild in child.childNodes:
						if subChild.childNodes[0].nodeValue == permTrue :
							rights += subChild.tagName + " "
					perms.append( child.tagName + " : " + rights )
				else :
					perms.append( "( " + child.data + " )" )
			data.append( perms )

		elif rootChild.childNodes != None :
			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 
			

			if len( rootChild.childNodes ) == 1 :
				data.append( checkStringLength( rootChild.childNodes[0].nodeValue, 70 ) )
			else :
				multipleLines = []
				for child in rootChild.childNodes:
					if child.childNodes != None and child.nodeValue != None :
						multipleLines.append( child.nodeValue )
					else:
						for subChild in child.childNodes:
							if subChild.childNodes != None :
								for values in subChild.childNodes: 
									multipleLines.append( str( subChild.tagName  ) + " : " + str( values.nodeValue ) )
				data.append( multipleLines )
		return data


class SpecificationSection( Section ):
	def __init__( self, title="" ):
		self.title       = title
		self.fields      = []
		self.data        = []
		self.group       = []
		self.groupTitle  = ""
		self.status      = ""
		self.dataStatus  = []

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes != None :
			### Node Types :
			# ELEMENT_NODE                =  1, 
			# ATTRIBUTE_NODE              =  2, 
			# TEXT_NODE                   =  3, 
			# CDATA_SECTION_NODE          =  4, 
			# ENTITY_NODE                 =  5, 
			# PROCESSING_INSTRUCTION_NODE =  6, 
			# COMMENT_NODE                =  7, 
			# DOCUMENT_NODE               =  8, 
			# DOCUMENT_TYPE_NODE          =  9, 
			# NOTATION_NODE               = 10

			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 

			if len( rootChild.childNodes ) == 1 and rootChild.childNodes[0].nodeType == 3 :
				# print "CASE 1"
				data.append( checkStringLength( rootChild.childNodes[0].nodeValue, 70 ) )
			else :
				# print "CASE 2"
				self.groupTitle = rootChild.getAttribute( labelAttr )
				
				for child in rootChild.childNodes:
					section = Section( child.getAttribute( labelAttr ) )
					section.status = child.getAttribute( statusAttr )
					
					if child.childNodes is not None :
						for subChild in child.childNodes:
							if subChild.nodeType == 3 :
								# print "    CASE 2.1 : " + str( subChild )
								section.title += " (" + checkStringLength( subChild.nodeValue, 70 ) + ")"
							else :
								# print "    CASE 2.2 : " + str( subChild )
								section.fields.append( subChild )
					
					self.group.append( section )
				data.append( " --- " )
				
		return data