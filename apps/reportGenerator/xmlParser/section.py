from utils import *

class Section():
	def __init__( self, title="" ):
		self.title  = title
		self.fields = []
		self.data   = []

	def displayFields( self ):
		print self.title + " :"
		for field in self.fields:
			print "\t" + field.tagName

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes != None :
			data.append( rootChild.getAttribute( labelAttr ) )

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

	def getChildValue( self, rootChild ):
		data = []
		if rootChild.childNodes != None :
			# rootLabel    = rootChild.getAttribute(    labelAttr )
			# rootType     = rootChild.getAttribute(     typeAttr )
			# rootOptional = rootChild.getAttribute( optionalAttr )
			# rootStatus   = rootChild.getAttribute(   statusAttr )
			# rootLevel    = rootChild.getAttribute(    levelAttr )
			# rootMode     = rootChild.getAttribute(     modeAttr )

			data.append( rootChild.getAttribute( labelAttr ) )

			if len( rootChild.childNodes ) == 1 :
				data.append( checkStringLength( rootChild.childNodes[0].nodeValue, 70 ) )
			else :
				multipleLines = []
				for child in rootChild.childNodes:
					print child
					if child.childNodes != None and child.nodeValue != None :
						multipleLines.append( child.nodeValue )
					else:
						multipleLines.append( child.getAttribute( labelAttr ) + " (" + child.getAttribute( statusAttr ) + ") :" )
						for subChild in child.childNodes:
							if subChild.childNodes != None :
								for values in subChild.childNodes: 
									multipleLines.append( "~ ~ " + str( subChild.getAttribute( labelAttr )  ) + " : " + str( values.nodeValue ) )
				data.append( multipleLines )
		return data