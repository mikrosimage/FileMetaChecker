from utils import *

class Section():
	def __init__( self, title="" ):
		self.title           = title
		self.fields          = []
		self.forbiddenFields  = []
		self.availableFields = []
		self.data            = []
		self.status          = ""
		self.dataStatus      = []

	def displayFields( self ):
		print self.title + " :"
		for field in self.fields:
			if self.forbiddenFields.count( field ) == 0 :
				print "\t" + field.tagName
				if field.childNodes != None and len( field.childNodes ) > 1 :
					for child in field.childNodes :
						if child.nodeType == 1 and self.forbiddenFields.count( child ) == 0 :
							print "\t\t" + child.tagName
							if child.childNodes != None and len( child.childNodes ) > 1 :
								for subchild in child.childNodes :
									if subchild.nodeType == 1 and self.forbiddenFields.count( subchild ) == 0 :
										print "\t\t\t" + subchild.tagName

	def getAvailableFields( self, root ):
		if root.childNodes != None and len( root.childNodes ) > 1 :
			for child in root.childNodes :
				if child.nodeType == 1 :
					self.availableFields.append( child )
					if child.childNodes != None and len( child.childNodes ) > 1 :
						for subchild in root.childNodes :
							if subchild.nodeType == 1 :
								self.availableFields.append( subchild )
								if subchild.childNodes != None and len( subchild.childNodes ) > 1 :
									for subsubchild in subchild.childNodes :
										if subsubchild.nodeType == 1 :
											self.availableFields.append( subsubchild )
											if subsubchild.childNodes != None and len( subsubchild.childNodes ) > 1 :
												for subsubsubchild in subsubchild.childNodes :
													if subsubsubchild.nodeType == 1 :
														self.availableFields.append( subsubsubchild )

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes != None and self.forbiddenFields.count( rootChild ) == 0 :
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
							if subChild.childNodes != None and self.forbiddenFields.count( subChild ) == 0 :
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
		if self.forbiddenFields.count( rootChild ) == 0 :
			if rootChild.tagName == permissions :
				data.append( rootChild.getAttribute( labelAttr ) )
				self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 

				perms = []
				for child in rootChild.childNodes:
					if self.forbiddenFields.count( child ) == 0 :
						if len( child.childNodes ) > 0 :
							rights = []
							rights.append( child.getAttribute( labelAttr ) + " : " )
							for subChild in child.childNodes:
								right = ""
								if self.forbiddenFields.count( subChild ) == 0 :
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

			elif rootChild.childNodes != None :
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
								if subChild.childNodes != None and self.forbiddenFields.count( subChild ) == 0 :
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
				# print "CASE 1 : " + rootChild.getAttribute( labelAttr )
				data.append( checkString( rootChild.getAttribute( labelAttr ), stringMaxLength ) )
				self.dataStatus.append( rootChild.getAttribute( statusAttr ) ) 
				data.append( checkString( rootChild.childNodes[0].nodeValue, stringMaxLength ) )
			else :
				# print "CASE 2 : " + rootChild.getAttribute( labelAttr )
				self.groupTitle = rootChild.getAttribute( labelAttr )
				
				for child in rootChild.childNodes:
					if child.nodeType != 3 and self.forbiddenFields.count( child ) == 0 :
						section = Section( child.getAttribute( labelAttr ) + " ( " + child.childNodes[0].nodeValue + " )" )
						section.status = child.getAttribute( statusAttr )
						
						if child.childNodes :
							for subChild in child.childNodes:
								# print str(child.tagName) + " : " + str(subChild.nodeType)
								if  self.forbiddenFields.count( subChild ) == 0 :
									if subChild.nodeType == 3 and subChild.childNodes :
										# print "    CASE 2.1 : " + str( subChild )
										section.title += " (" + checkString( subChild.nodeValue, stringMaxLength ) + ")"
									# elif subChild.childNodes :
									elif subChild.nodeType == 1 :
										# print "    CASE 2.2 : " + str( subChild )
										section.fields.append( subChild )
						
						self.group.append( section )
				# data.append( " --- " )
				
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
		if rootChild.childNodes != None and self.forbiddenFields.count( rootChild ) == 0 :
			if len( rootChild.childNodes ) == 1 :
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
	