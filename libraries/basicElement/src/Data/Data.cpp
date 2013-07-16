#include "Data.hpp"

#include <iostream>

namespace basic_element
{
namespace data_element
{

Data::Data()
	: Element( Element::eTypeData )
	, _data( NULL )
	, _size( 0 )
	, _isBigEndian( true )
{
}

Data::~Data()
{
	if( _data != NULL )
	{
		delete[] _data;
		_data = NULL;
	}
}

void Data::setData( const char* data, const size_t& size )
{
	LOG_INFO( " Data: \tSET DATA " );
	_data = new char [size];
	_size = size;
	if( !_isBigEndian )
	{
		std::reverse_copy( data, data + _size, _data );
		// LOG_INFO( " Data: \t_data[0] : " << (unsigned int)_data[0] << "\t\t\t@ " << &_data );
	}
	else
	{
		std::memcpy( _data, data, _size );
		// LOG_INFO( " Data: \t_data[0] : " << (unsigned int)_data[0] << "\t\t\t@ " << &_data );
	}
}

void Data::getData( char* buffer ) const
{
	LOG_INFO( " Data: \tGET DATA from @ " << &_data << " to \t@ " << &buffer );
	std::memcpy( buffer, _data, _size );
	// LOG_INFO( " Data:\t _data : " << (unsigned int) _data[0] << "\t\t\t@ " << &_data );
	// LOG_INFO( " Data:\t buffer: " << (unsigned int)buffer[0] << "\t\t\t@ " << &buffer );
}

size_t Data::getSize() const
{
	LOG_INFO( " Data: \tGET SIZE " );
	return _size;
}

void Data::setBigEndianess( bool isBigEndian )
{
	_isBigEndian = isBigEndian; 
}

bool Data::getBigEndianess() const
{
	return _isBigEndian; 
}

std::string Data::getAscii() const
{
	std::stringstream sstr;
	for (size_t i = 0; i < _size; ++i)
		sstr << _data[i];
	LOG_INFO( " Data: \tTO STRING (Ascii): " << sstr.str() );
	return sstr.str();
}

std::string Data::getHexa() const
{
	std::stringstream sstr;
	char* buffer = new char [ _size ];
	std::memcpy( buffer, _data, _size );
	for (size_t i = 0; i < _size; ++i)
	{
		sstr << std::hex << std::setfill('0') << std::setw(2) << (unsigned short)buffer[i];
	}
	delete[] buffer;
	LOG_INFO( " Data: \tTO STRING (Hexa): " << sstr.str() );
	return sstr.str();
}

std::vector< unsigned int > Data::toIntVector()
{
	std::vector< unsigned int > vector;
	for ( size_t i = 0; i < _size; ++i )
	{
		vector.push_back( ( unsigned char ) _data[i] );
		LOG_INFO( " Data: \tTO INT VECTOR : " << vector.at( i ) );
	}
	return vector;
}

Element::EStatus Data::checkData()
{
	LOG_INFO( " Data: \tCHECK DATA " );
	return eStatusUnknown;
}

Data& Data::operator=( const Data& other )
{
	if( this != &other )
	{
		size_t size = other.getSize();
		char* buffer = new char [ size ];
		other.getData( buffer );
		this->setData( buffer, size );
		delete[] buffer;
	}
	return *this;
}


}
}
