import sys

from xml.dom.ext import StripXml
from xml.dom.ext.reader import Sax2

SUFFIXES = {1000: ['KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
            1024: ['KiB', 'MiB', 'GiB', 'TiB', 'PiB', 'EiB', 'ZiB', 'YiB']}

def getReadableSize( sizeString, a_kilobyte_is_1024_bytes=True ):
	size = int( sizeString )
	if size < 0:
		raise ValueError('number must be non-negative')
	multiple = 1024 if a_kilobyte_is_1024_bytes else 1000
	for suffix in SUFFIXES[multiple]:
		size /= multiple
		if size < multiple:
			readableSize = '{0:.1f} {1}'.format(size, suffix) + " (" + sizeString + " bytes)"
			return readableSize
	raise ValueError('number too large');

def getSectionsTitles( root ):
	sections = []
	if root.childNodes is not None :
		for child in root.childNodes :
			childSection = []
			childSection.append( child.tagName )
			# if child.getAttribute('label') is not None :
			# 	childSection.append( child.getAttribute('label') )
			sections.append( childSection )
	return sections;

def checkStringLength( string ):
	if len(string) > 70 :
		string = string[:15] + " ... " + string[( len(string)-50 ):];
	return string;


class Section():
	def __init__( self, sectionTitles=["",""] ):
		self.items = []
		self.title  = sectionTitles[0]
		self.tag = sectionTitles[0]
		self.data   = []
		self.table  = None
		self.fields = []

	def setTitle( self, title ):
		self.title  = title

	def setData( self, data ):
		self.data  = data

	def getChildrenInfo( self, parent ):
		value = ""
		if len( parent.childNodes ) == 1 :
			if parent.getAttribute('type') == "hexa" :
				value = "0x" + parent.childNodes[0].data
			else:
				value = parent.childNodes[0].data
		return value;

	def setSectionData( self, root ):
		if len(root.getElementsByTagName( self.tag )) != 0 :
			nodeList = root.getElementsByTagName( self.tag )[0]
			# print nodeList
			for child in nodeList.childNodes:
				# fieldRemoved = True
				fieldRemoved = False
				subSection = []

				for field in self.fields:
					if field == child.nodeName :
						fieldRemoved = False

				# if fieldRemoved :
				# 	print child.nodeName + " is removed from list !"
				
				if not fieldRemoved :
					subSection.append( child.nodeName )
					subSection.append( self.getChildrenInfo( child ) )
					self.data.append( subSection )


	def setSectionFields( self, root ):
		if len(root.getElementsByTagName( self.tag )) != 0 :
			nodeList = root.getElementsByTagName( self.tag )[0]
			# print nodeList
			for child in nodeList.childNodes:
				field = ""
				field = child.nodeName
				self.fields.append( field )

	def displaySectionFields( self ):
		if len(self.fields) > 0:
			print self.title + " :"
			for field in self.fields:
				print "\t" + field
		else :
			print "setSectionFields before !"
			sys.exit()


	def removeSectionField( self, root, tagName ):
		if len(self.fields) > 0:
			if self.fields.count( tagName ) > 0:
				self.fields.remove( tagName )
				# print tagName + " removed !"
			# else :
			# 	print "No such a field in the list : " + tagName
		else :
			print "setSectionFields before !"
			sys.exit()




class FileSystemInfoSection( Section ):
	def getPermissions( self, parent ):
		rights = ""
		i = 0
		for child in parent.childNodes :
			user = ""
			user += child.tagName + " : "
			for subChild in child.childNodes :
				if subChild.childNodes[0].data == "True":
					user += subChild.tagName + " "
					# value += subChild.tagName + " "
			rights += user
			if i != 2:
				rights += " | "
			i += 1
		return rights;

	def getChildrenInfo( self, parent ):
		value = ""
		if parent.tagName == "size":
			value = getReadableSize( parent.childNodes[0].data)
		else:
			if len( parent.childNodes ) == 1 :
				if parent.getAttribute('type') == "hexa" :
					value = "0x" + parent.childNodes[0].data
				else:
					value = parent.childNodes[0].data
		return value;

	def setSectionData( self, root ):
		if len(root.getElementsByTagName( self.tag )) != 0 :
			nodeList = root.getElementsByTagName( self.tag )[0]
			
			for child in nodeList.childNodes:
				# fieldRemoved = True
				fieldRemoved = False
				subSection = []

				for field in self.fields:
					if field == child.nodeName :
						fieldRemoved = False

				# if fieldRemoved :
				# 	print child.nodeName + " is removed from list !"
				
				if not fieldRemoved :
					subSection.append( child.nodeName )
					if child.tagName == "permissions" :
						subSection.append( checkStringLength( self.getPermissions( child ) ) )
					else:
						subSection.append( checkStringLength( self.getChildrenInfo( child ) ) )
					self.data.append( subSection )





