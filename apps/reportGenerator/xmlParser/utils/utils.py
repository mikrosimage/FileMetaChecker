fileSystemInfo = "systemInfo"
permissions    = "permissions"
permTrue       = "True"

specification  = "specification"
idAttr         = "id"
labelAttr      = "label"
optionalAttr   = "optional"
typeAttr       = "type"
statusAttr     = "status"
modeAttr       = "display_mode"
levelAttr      = "level"


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


def checkStringLength( string, maxLength ):
	if len(string) > maxLength :
		string = string[:int(maxLength/2)-5] + " ... " + string[( len(string)-int(maxLength/2) ):];
	return string;