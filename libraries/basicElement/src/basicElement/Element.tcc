
namespace basic_element
{

size_t Element::_lastUniqueId = 0;

Element::Element( const std::string& id, EType type, ESubType subType, EDisplayType dispType )
	: _id            ( id )
	, _label         ( "" )
	, _error         ( "" )
	, _warning       ( "" )
	, _uniqueId      ( _lastUniqueId++ )
	, _size          ( 0 )
	, _type          ( type )
	, _subType       ( subType )
	, _displayType   ( dispType )
	, _status        ( eStatusUnknown )
	, _bigEndianData ( true )
{
}

void Element::setId( const std::string& id )
{
	_id = id;
}

void Element::setLabel( const std::string& label )
{
	_label = label;
}

void Element::setDisplayType( const std::string& displayType )
{
	_displayType = eDisplayTypeDefault;
	
	if( displayType == kAscii )
		_displayType = eDisplayTypeAscii;
	else if( displayType == kHexa )
		_displayType = eDisplayTypeHexa;
	else if( displayType != "" )
		LOG_WARNING( _id << ": Unknown display type!" );
}

void Element::setStatus( const EStatus status )
{
	_status = status;
}

void Element::setBigEndianness( bool isBigEndian )
{
	_bigEndianData = isBigEndian;
}

template< >
ESubType Element::getSubType< ESubType >() const
{
	return _subType;
}

template< >
std::string Element::getSubType< std::string >() const
{
	switch( _subType )
	{
		case eSubTypeUnknown: return "";
		case eSubTypeInt8:    return kInt8;
		case eSubTypeUInt8:   return kUInt8;
		case eSubTypeInt16:   return kInt16;
		case eSubTypeUInt16:  return kUInt16;
		case eSubTypeInt32:   return kInt32;
		case eSubTypeUInt32:  return kUInt32;
		case eSubTypeInt64:   return kInt64;
		case eSubTypeUInt64:  return kUInt64;
		case eSubTypeFloat:   return kFloat; 
		case eSubTypeDouble:  return kDouble;
		case eSubTypeIeeeExtended: return kIeeeExtended;
		case eSubTypeAscii:   return kAscii;
		case eSubTypeHexa:    return kHexa;
		case eSubTypeRaw :    return kRaw;
	}
	return "";
}

void Element::getEndianOrderedData( char* buffer, const char* data ) const
{
	if( !_bigEndianData )
	{
		//LOG_TRACE( " EndianessConverter: case 1 ( little ) " );
		std::reverse_copy( data, data + _size, buffer );
	}
	else
	{
		//LOG_TRACE( " EndianessConverter: case 2 ( big ) " );
		std::memcpy( buffer, data, _size );
	}
}

void Element::reverseEndianness( char* buffer, const char* data ) const
{
	std::reverse_copy( data, data + _size, buffer );
}


void Element::addErrorLabel( const std::string& error )
{
	_status = Element::eStatusInvalid;
	_error += error;
}

void Element::addWarningLabel( const std::string& warning )
{
	_warning += warning;
}

std::string Element::getErrorLabel()
{
	return _error;
}

std::string Element::getWarningLabel()
{
	return _warning;
}

std::vector< std::pair< std::string, std::string > > Element::getCommonElementInfo()
{
	std::vector< std::pair< std::string, std::string > > commonInfo;
	commonInfo.push_back( std::make_pair( kId,     getId()           ) );
	commonInfo.push_back( std::make_pair( kLabel,  getLabel()        ) );
	commonInfo.push_back( std::make_pair( kStatus, getStatusString() ) );
	
	if( ! _error.empty() )
		commonInfo.push_back( std::make_pair( kError,   _error ) );
	if( ! _warning.empty() )
		commonInfo.push_back( std::make_pair( kWarning, _warning ) );

	return commonInfo;
}

std::string Element::getStatusString()
{
	switch( getStatus() )
	{
		case Element::eStatusUnknown  : return kUnknown;
		case Element::eStatusValid    : return kValid;
		case Element::eStatusInvalid  : return kInvalid;
		case Element::eStatusPassOver : return kPassOver;
	}
	return kUnknown;
}


}
