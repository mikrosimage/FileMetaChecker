#include "Element.hpp"
#include "common.hpp"

namespace basic_element
{

Element::Element( EType type )
	: _bigEndianData( true )
	, _type( type )
	, _status( eStatusUnknown )
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
	std::stringstream sstr, sstr2;
	for( size_t i = 0; i < _size; ++i )
	{
		sstr  << (int)(unsigned char) data[i]   << " ";
		sstr2 << (int)(unsigned char) buffer[i] << " ";
	}
	BE_LOG_TRACE( " ++>   data: "  << sstr.str() );
	BE_LOG_TRACE( " ++> buffer: " << sstr2.str() );

	BE_LOG_TRACE( " EndianessConverter: GET ENDIANESS " );
	if( !_bigEndianData )
	{
		BE_LOG_TRACE( " EndianessConverter: case 1 ( little ) " );
		std::reverse_copy( data, data + _size, buffer );
	}
	else
	{
		BE_LOG_TRACE( " EndianessConverter: case 2 ( big ) " );
		std::memcpy( buffer, data, _size );
	}

	std::stringstream sstr3, sstr4;
	for( size_t i = 0; i < _size; ++i )
	{
		sstr3  << (int)(unsigned char) data[i]   << " ";
		sstr4 << (int)(unsigned char) buffer[i] << " ";
	}
	BE_LOG_TRACE( " ==>   data: "  << sstr3.str() );
	BE_LOG_TRACE( " ==> buffer: " << sstr4.str() );
}


void Element::reverseEndianness( char* buffer, const char* data ) const
{
	std::reverse_copy( data, data + _size, buffer );
}

}
