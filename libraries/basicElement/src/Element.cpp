#include "Element.hpp"
#include "common.hpp"

namespace basic_element
{

Element::Element( EType type )
	: _isBigEndian( true )
	, _type( type )
	, _status( eStatusUnknown )
{
}

void Element::setId( const std::string& id )
{
	BE_LOG_TRACE( " Element: SET ID " );
	_id = id;
}

void Element::setLabel( const std::string& label )
{
	BE_LOG_TRACE( " Element: SET LABEL " );
	_label = label;
}

void Element::setStatus( const EStatus status )
{
	BE_LOG_TRACE( " Element: SET STATUS " );
	_status = status;
}

void Element::setBigEndianness( bool isBigEndian )
{
	BE_LOG_TRACE( " Element: SET BIG ENDIANESS " );
	_isBigEndian = isBigEndian; 
}

bool Element::getBigEndianness() const
{
	BE_LOG_TRACE( " Element: GET BIG ENDIANESS " );
	return _isBigEndian; 
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
	if( !_isBigEndian )
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