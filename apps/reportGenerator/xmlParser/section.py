from utils import *

class Section():
	def __init__( self, title="" ):
		self.title           = title
		self.fields          = []
		self.forbiddenFields = []
		self.availableFields = []
		self.data            = []
		self.status          = ""
		self.dataStatus      = []

	def displayFields( self, rootField, level=0 ):
		if level == 0 :
			print self.title
			for field in rootField:
				if self.forbiddenFields.count( field ) != 0 or field.nodeType == 3:
					continue;
				self.displayFields( field, level+1 )
		else :		
			print "\t"*level + rootField.tagName
			if rootField.childNodes == None or len( rootField.childNodes ) <= 1 :
				return;
			for field in rootField.childNodes :
				if field.nodeType != 1 or self.forbiddenFields.count( field ) != 0 :
					continue;
				self.displayFields( field, level+1 )	


	def setAvailableFields( self, root ):
		if root.childNodes == None and len( root.childNodes ) <= 1 :
			return;

		for child in root.childNodes :
			if child.nodeType != 1 :
				continue;
			self.availableFields.append( child )
			if child.childNodes == None and len( child.childNodes ) <= 1 :
				continue;
			self.setAvailableFields( child )


	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes == None or self.forbiddenFields.count( rootChild ) != 0 :
			return;

		data.append( checkString( rootChild.getAttribute( labelAttr ), stringMaxLength ) )
		self.dataStatus.append( rootChild.getAttribute( statusAttr ) )
		if len( rootChild.childNodes ) == 1 :
			data.append( checkString( rootChild.childNodes[0].nodeValue, stringMaxLength ) )
		else :
			multipleLines = []
			for child in rootChild.childNodes:
				if child.childNodes != None and child.nodeValue != None and self.forbiddenFields.count( child ) == 0 :
					multipleLines.append( child.nodeValue )
				else:
					for subChild in child.childNodes:
						if subChild.childNodes == None or self.forbiddenFields.count( subChild ) != 0 :
							continue;
						for values in subChild.childNodes: 
							multipleLines.append( str( subChild.tagName  ) + " : " + str( values.nodeValue ) )
			data.append( multipleLines )

		return data

	def setFieldsValues( self ):
		for field in self.fields:
			data = self.getChildValue ( field )
			if len( data ) != 0 :
				self.data.append( data ) 


class FileSystemInfoSection( Section ):
	def getChildValue( self, rootChild ):
		data = []
		if self.forbiddenFields.count( rootChild ) != 0 :
			return;

		if rootChild.tagName == permissions :
			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 
			perms = []

			for child in rootChild.childNodes:
				if self.forbiddenFields.count( child ) != 0 :
					continue;

				if len( child.childNodes ) > 0 :
					rights = []
					rights.append( child.getAttribute( labelAttr ) + " : " )

					for subChild in child.childNodes:
						right = ""
						if self.forbiddenFields.count( subChild ) != 0 :
							continue;

						if subChild.childNodes[0].nodeValue == permFalse :
							right += "\\color{Gray}" + subChild.tagName
						else :
							right += subChild.tagName
						rights.append( right )
					perms.append( rights )

				else :
					perms.append( "( " + child.data + " )" )
			data.append( perms )

		elif rootChild.tagName == "absolutePath" :
			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 
			if len( rootChild.childNodes ) == 1 :
				data.append( checkPathLength( rootChild.childNodes[0].nodeValue, 60 ) )

		elif rootChild.childNodes == None :
			return;
			
		else :
			data.append( rootChild.getAttribute( labelAttr ) )
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 

			if len( rootChild.childNodes ) == 1 :
				data.append( checkString( rootChild.childNodes[0].nodeValue, stringMaxLength ) )
			
			else :
				multipleLines = []
			
				for child in rootChild.childNodes:
					if child.childNodes != None and child.nodeValue != None and self.forbiddenFields.count( child ) == 0 :
						multipleLines.append( child.nodeValue )
					else:
						for subChild in child.childNodes:
							if subChild.childNodes == None or self.forbiddenFields.count( subChild ) != 0 :
								continue;
							for values in subChild.childNodes: 
								multipleLines.append( str( subChild.tagName  ) + " : " + str( values.nodeValue ) )
			
				data.append( multipleLines )

		return data


class SpecificationSection( Section ):
	def __init__( self, title="" ):
		self.title       = title
		self.fields      = []
		self.forbiddenFields = []
		self.availableFields = []
		self.data        = []
		self.group       = []
		self.groupTitle  = ""
		self.status      = ""
		self.dataStatus  = []

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes != None and self.forbiddenFields.count( rootChild ) == 0 :
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
			if len( rootChild.childNodes ) == 1 and rootChild.childNodes[0].nodeType == 3 :
				data.append( checkString( rootChild.getAttribute( labelAttr ), stringMaxLength ) )
				self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 
				data.append( checkString( rootChild.childNodes[0].nodeValue, stringMaxLength ) )
			else :
				self.groupTitle = rootChild.getAttribute( labelAttr )
				
				for child in rootChild.childNodes:
					if child.nodeType == 3 or self.forbiddenFields.count( child ) != 0 :
						continue;
					section = Section( child.getAttribute( labelAttr ) + " ( " + child.childNodes[0].nodeValue + " )" )
					section.status = child.getAttribute( statusAttr )
					
					if child.childNodes :
						for subChild in child.childNodes:
							if  self.forbiddenFields.count( subChild ) != 0 :
								continue;
							if subChild.nodeType == 3 and subChild.childNodes :
								section.title += " (" + checkString( subChild.nodeValue, stringMaxLength ) + ")"
							elif subChild.nodeType == 1 :
								section.fields.append( subChild )
					
					self.group.append( section )
				
		return data

class LoudnessSection( Section ):
	def __init__( self, title="" ):
		self.title      = title
		self.fields     = []
		self.forbiddenFields = []
		self.availableFields = []
		self.data       = []
		self.standard   = ""
		self.dataStatus = []
		self.plots		= []

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes == None or self.forbiddenFields.count( rootChild ) != 0 or len( rootChild.childNodes ) != 1 :
			return;

		if rootChild.tagName == "MaxShortTermValues" or rootChild.tagName == "TruePeakValues" :
			plot = []
			plot.append( rootChild.tagName )
			plot.append( rootChild.childNodes[0].nodeValue.split( ", " ) )
			self.plots.append( plot )
		else :
			self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 
			data.append( rootChild.tagName )
			data.append( checkString( rootChild.childNodes[0].nodeValue, stringMaxLength ) )

		return data
	