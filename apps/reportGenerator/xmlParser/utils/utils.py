fileSystemInfo = "systemInfo"
permissions    = "permissions"
permTrue       = "True"
permFalse      = "False"

specification  = "specification"
idAttr         = "id"
labelAttr      = "label"
optionalAttr   = "optional"
typeAttr       = "type"
statusAttr     = "status"
modeAttr       = "display_mode"
levelAttr      = "level"

colors = [ "blue", "red", "green", "orange", "black" ]

stringMaxLength = 40

suffixes = {
	1000: ['KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
	1024: ['KiB', 'MiB', 'GiB', 'TiB', 'PiB', 'EiB', 'ZiB', 'YiB']
	}

def getReadableSize( sizeString, a_kilobyte_is_1024_bytes=True ):
	size = int( sizeString )
	if size < 0:
		raise ValueError('number must be non-negative')
	multiple = 1024 if a_kilobyte_is_1024_bytes else 1000
	for suffix in suffixes[multiple]:
		size /= multiple
		if size < multiple:
			readableSize = '{0:.1f} {1}'.format(size, suffix) + " (" + sizeString + " bytes)"
			return readableSize
	raise ValueError('number too large');


def checkPathLength( path, maxLength ):
	substr = ""
	substrList = []

	if len(path) <= maxLength :
		return path;

	folderList = path.split( "/" )
	for i in range( 0, len( folderList ) ) :
		if i != len( folderList )-1 :
			if len( substr + folderList[i] + "/" ) < maxLength :
				substr += folderList[i] + "/"
			else :
				substrList.append( substr )
				substr = ""
				substr += folderList[i] + "/"
		else :
			substrList.append( substr )

	path =  "\\pbox{0.7\\textwidth}{" + "... \\\\ ".join( substrList ) + "}";
	return path;

def checkString( string, maxLength ):
	if string.count( "\pbox" ) > 0 :
		return string;

	linesString = ""
	linesString = checkStringLineBreak( string, maxLength )

	if linesString == string :
		linesString = checkStringLength( string, maxLength )

	if linesString != string :  
		string =  "\\pbox{0.7\\textwidth}{" + linesString + " }";

	return string;


def checkStringLineBreak( string, maxLength ):
	if string.count( "\n" ) == 0 :
		return string;

	lineList = string.split( "\n" )
	for i in range( 0, len(lineList) ) :
		if len( lineList[i] ) > 0 :
			lineList[i] = checkStringLength( lineList[i], maxLength )

	for j in range( len(lineList), 0, -1 ) :
		if len(lineList[j-1]) == 0 :
			lineList.pop( j-1 )
	string = " \\\\ ".join( lineList );
	# print " --- > Line Break : " + string
	return string;

def splitString( string, separator, maxLength ):
	substrList = []
	wordList      = string.split( separator )
	previousWords = ""
	for i in range( 0, len( wordList ) ) :
		wordList[i] = checkStringLength( wordList[i], maxLength )
		if i != len( wordList )-1 :
			if len( previousWords + wordList[i] + separator ) < maxLength :
				previousWords += wordList[i] + separator
			else :
				substrList.append( previousWords )
				previousWords = ""
				previousWords += wordList[i] + separator
		else:
			substrList.append( previousWords )
			substrList.append( wordList[i] )
	for j in range( len(substrList), 0, -1 ) :
		if len(substrList[j-1]) == 0 :
			substrList.pop( j-1 )
	string = " \\\\ ".join( substrList );
	print " --- > Length & '" + separator + "' : " + string

	return string;


def checkStringLength( string, maxLength ):
	if len( string ) <= maxLength :
		return string;

	if string.count( " " ) > 0 :							# string counts space character(s)
		string = splitString( string, " ", maxLength )

	elif string.count( "," ) > 0 :							# string counts comma character(s)
		string = splitString( string, ",", maxLength )

	elif string.count( "-" ) > 0 :							# string counts dash character(s)
		string = splitString( string, "-", maxLength )

	elif string.count( "/" ) > 0 :							# string counts slash character(s)
		string = splitString( string, "/", maxLength )

	elif string.count( "_" ) > 0 :							# string counts underscore character(s)
		string = splitString( string, "_", maxLength )

	else :
		substrList = []
		strPartStart = 0
		strPart      = string
		while len( strPart ) >  maxLength - 3 :
			strPart = string[ strPartStart:int(maxLength - 3) ]
			substrList.append( strPart + "..." )
			strPartStart += maxLength - 3
		substrList.append( string[strPartStart:] )
		string = " \\\\ ".join( substrList );
		# print " --- > Length only : " + string

	return string;
