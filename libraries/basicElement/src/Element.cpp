#include "Element.hpp"
#include "common.hpp"

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

bool Element::getBigEndianness() const
{
	return _bigEndianData;
}

void Element::getEndianOrderedData( char* buffer, const char* data ) const
{
	if( !_bigEndianData )
	{
		//BE_LOG_TRACE( " EndianessConverter: case 1 ( little ) " );
		std::reverse_copy( data, data + _size, buffer );
	}
	else
	{
		//BE_LOG_TRACE( " EndianessConverter: case 2 ( big ) " );
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
