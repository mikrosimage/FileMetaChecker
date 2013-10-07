
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
	, _data          ( NULL )
	, _bigEndianData ( true )
{
}

Element::Element( const spec_reader::SpecNode& node )
	: _id            ( node.getId() )
	, _label         ( node.getLabel() )
	, _error         ( "" )
	, _warning       ( "" )
	, _uniqueId      ( _lastUniqueId++ )
	, _size          ( 0 )
	, _status        ( eStatusUnknown )
	, _data          ( NULL )
	, _bigEndianData ( node.isBigEndian() )
{
	setType( node.getType() );
	setSubType( node.getType() );
	setDisplayType( node.getDisplayType() );
}

Element::~Element()
{
	if( _data != NULL )
	{
		delete[] _data;
		_data = NULL;
	}
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

void Element::setBigEndianness( bool isBigEndian )
{
	_bigEndianData = isBigEndian;
}

void Element::setType( const std::string& type )
{
	if( type == kInt8         ) { _type = eTypeNumber;  return; }
	if( type == kUInt8        ) { _type = eTypeNumber;  return; }
	if( type == kInt16        ) { _type = eTypeNumber;  return; }
	if( type == kUInt16       ) { _type = eTypeNumber;  return; }
	if( type == kInt32        ) { _type = eTypeNumber;  return; }
	if( type == kUInt32       ) { _type = eTypeNumber;  return; }
	if( type == kInt64        ) { _type = eTypeNumber;  return; }
	if( type == kUInt64       ) { _type = eTypeNumber;  return; }
	if( type == kFloat        ) { _type = eTypeNumber;  return; }
	if( type == kDouble       ) { _type = eTypeNumber;  return; }
	if( type == kIeeeExtended ) { _type = eTypeNumber;  return; }
	if( type == kAscii        ) { _type = eTypeData;    return; }
	if( type == kHexa         ) { _type = eTypeData;    return; }
	if( type == kRaw          ) { _type = eTypeData;    return; }
	if( type == kExif         ) { _type = eTypeExif;    return; }
	if( type == kKlv          ) { _type = eTypeKlv;     return; }
	_type = eTypeUnknown;
}

void Element::setSubType( const std::string& subType )
{
	if( subType == kInt8         ) { _subType = eSubTypeInt8;        return; }
	if( subType == kUInt8        ) { _subType = eSubTypeUInt8;       return; }
	if( subType == kInt16        ) { _subType = eSubTypeInt16;       return; }
	if( subType == kUInt16       ) { _subType = eSubTypeUInt16;      return; }
	if( subType == kInt32        ) { _subType = eSubTypeInt32;       return; }
	if( subType == kUInt32       ) { _subType = eSubTypeUInt32;      return; }
	if( subType == kInt64        ) { _subType = eSubTypeInt64;       return; }
	if( subType == kUInt64       ) { _subType = eSubTypeUInt64;      return; }
	if( subType == kFloat        ) { _subType = eSubTypeFloat;       return; }
	if( subType == kDouble       ) { _subType = eSubTypeDouble;      return; }
	if( subType == kIeeeExtended ) { _subType = eSubTypeIeeeExtended;return; }
	if( subType == kAscii        ) { _subType = eSubTypeAscii;       return; }
	if( subType == kHexa         ) { _subType = eSubTypeHexa;        return; }
	if( subType == kRaw          ) { _subType = eSubTypeRaw;         return; }
	_subType = eSubTypeUnknown;
}


void Element::setStatus( const EStatus status )
{
	_status = status;
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
		case eSubTypeUnknown      : return "";
		case eSubTypeInt8         : return kInt8;
		case eSubTypeUInt8        : return kUInt8;
		case eSubTypeInt16        : return kInt16;
		case eSubTypeUInt16       : return kUInt16;
		case eSubTypeInt32        : return kInt32;
		case eSubTypeUInt32       : return kUInt32;
		case eSubTypeInt64        : return kInt64;
		case eSubTypeUInt64       : return kUInt64;
		case eSubTypeFloat        : return kFloat; 
		case eSubTypeDouble       : return kDouble;
		case eSubTypeIeeeExtended : return kIeeeExtended;
		case eSubTypeAscii        : return kAscii;
		case eSubTypeHexa         : return kHexa;
		case eSubTypeRaw          : return kRaw;
	}
	return "";
}

void Element::getEndianOrderedData( char* buffer, const char* data ) const
{
	if( !_bigEndianData )
		std::reverse_copy( data, data + _size, buffer );
	else
		std::memcpy( buffer, data, _size );
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
