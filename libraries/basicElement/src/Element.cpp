#include "Element.hpp"
#include "common.hpp"

namespace basic_element
{

size_t Element::_lastUniqueId = 0;

Element::Element( EType type )
	: _type          ( type )
	, _uniqueId      ( _lastUniqueId++ )
	, _status        ( eStatusUnknown )
	, _subType       ( 0 )
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

Element::ENumberType Element::getNumberSubType()
{
	if( _type != eTypeNumber )
		return eNumberTypeUnknown;
	return ( ENumberType ) _subType;
}

Element::EDataType Element::getDataSubType()
{
	if( _type != eTypeData )
		return eDataTypeUnknown;
	return ( EDataType ) _subType;
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

}
