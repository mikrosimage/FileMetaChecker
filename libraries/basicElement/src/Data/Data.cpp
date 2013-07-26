#include "Data.hpp"

#include <iostream>

namespace basic_element
{
namespace data_element
{

Data::Data()
	: Element( Element::eTypeData )
	, _data( NULL )
{
	_subType = Element::eDataTypeRaw;
	_size = 0;
}

Data::Data( const Element::EDataType& subType )
	: Element( Element::eTypeData )
	, _data( NULL )
{
	_subType = subType;
	_size = 0;
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
	BE_LOG_TRACE( " Data: \tSET DATA " );
	_data = new char [size];
	_size = size;
	
	getEndianOrderedData( _data, data );
}

void Data::getData( char* buffer ) const
{
	BE_LOG_TRACE( " Data: \tGET DATA from @ " << &_data << " to @ " << &buffer );
	std::memcpy( buffer, _data, _size );
	// std::reverse_copy( _data, _data + _size, buffer );
	// BE_LOG_TRACE( " Data:\t _data : " << (unsigned int) _data[0] << "\t\t\t@ " << &_data );
	// BE_LOG_TRACE( " Data:\t buffer: " << (unsigned int)buffer[0] << "\t\t\t@ " << &buffer );
}

size_t Data::getSize() const
{
	BE_LOG_TRACE( " Data: \tGET SIZE " );
	return _size;
}

std::string Data::getAscii() const
{
	BE_LOG_TRACE( " Data: \tGET ASCII " );
	std::stringstream sstr;
	for (size_t i = 0; i < _size; ++i)
		sstr << _data[i];
	BE_LOG_TRACE( " Data: \tTO STRING (Ascii): " << sstr.str() );
	return sstr.str();
}

std::string Data::getHexa() const
{
	BE_LOG_TRACE( " Data: \tGET HEXA " );
	std::stringstream sstr;
	char* buffer = new char [ _size ];
	std::memcpy( buffer, _data, _size );
	for (size_t i = 0; i < _size; ++i)
	{
		sstr << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)buffer[i];
	}
	delete[] buffer;
	BE_LOG_TRACE( " Data: \tTO STRING (Hexa): " << sstr.str() );
	return sstr.str();
}

std::vector< unsigned int > Data::toIntVector()
{
	std::vector< unsigned int > vector;
	for ( size_t i = 0; i < _size; ++i )
	{
		vector.push_back( ( unsigned char ) _data[i] );
		BE_LOG_TRACE( " Data: \tTO INT VECTOR : " << vector.at( i ) );
	}
	return vector;
}

void Data::setSpecData( const std::string& specValue )
{
	BE_LOG_TRACE( " Data: \tSET SPEC DATA " );
	_specValue = specValue;
}

Element::EStatus Data::checkData()
{
	BE_LOG_TRACE( " Data: \tCHECK DATA " );
	if( _specValue.empty() )
	{
		setStatus( eStatusPassOver );
		return eStatusPassOver;
	}

	Element::EStatus status = eStatusInvalid;
	switch( getDataSubType() )
	{
		case eDataTypeUnknown :
		{
			status = eStatusUnknown;
		} break;

		case eDataTypeAscii :
		{
			if( _specValue == getAscii() )
				status = eStatusValid;
		} break;

		case eDataTypeHexa :
		{
			if( _specValue == getHexa()  )
				status = eStatusValid;
		} break;

		case eDataTypeRaw :
		{
			status = eStatusPassOver;
		} break;
	}
	
	setStatus( status );
	return status;
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
