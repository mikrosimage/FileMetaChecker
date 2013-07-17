#include "Element.hpp"

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
	LOG_INFO( " Element: SET ID " );
	_id = id;
}

void Element::setLabel( const std::string& label )
{
	LOG_INFO( " Element: SET LABEL " );
	_label = label;
}

void Element::setStatus( const EStatus status )
{
	LOG_INFO( " Element: SET STATUS " );
	_status = status;
}

void Element::setBigEndianness( bool isBigEndian )
{
	LOG_INFO( " Element: SET BIG ENDIANESS " );
	_isBigEndian = isBigEndian; 
}

bool Element::getBigEndianness() const
{
	LOG_INFO( " Element: GET BIG ENDIANESS " );
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
	LOG_INFO( " ++>   data: "  << sstr.str() );
	LOG_INFO( " ++> buffer: " << sstr2.str() );

	LOG_INFO( " EndianessConverter: GET ENDIANESS " );
	if( !_isBigEndian )
	{
		LOG_INFO( " EndianessConverter: case 1 ( little ) " );
		std::reverse_copy( data, data + _size, buffer );
	}
	else
	{
		LOG_INFO( " EndianessConverter: case 2 ( big ) " );
		std::memcpy( buffer, data, _size );
	}

	std::stringstream sstr3, sstr4;
	for( size_t i = 0; i < _size; ++i )
	{
		sstr3  << (int)(unsigned char) data[i]   << " ";
		sstr4 << (int)(unsigned char) buffer[i] << " ";
	}
	LOG_INFO( " ==>   data: "  << sstr3.str() );
	LOG_INFO( " ==> buffer: " << sstr4.str() );
}


void Element::reverseEndianness( char* buffer, const char* data ) const
{
	std::reverse_copy( data, data + _size, buffer );
}

}