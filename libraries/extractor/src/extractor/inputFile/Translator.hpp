#ifndef _EXTRACTOR_INPUT_FILE_TRANSLATOR_HPP_
#define _EXTRACTOR_INPUT_FILE_TRANSLATOR_HPP_

#include "types.hpp"
#include "common/global.hpp"

#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

namespace utils
{
	template< typename Type >
	union EndiannessConverter
	{
		Type value;
		unsigned char data[0];
	};
}



template< typename DataType >
class Translator
{
public:
	Translator()
	{
		
	}
	
	DataType translate( const char* data, const size_t size = 0, const bool bigEndian = true );
	std::string translate( const DataType& data );
};


template< typename DataType >
DataType Translator<DataType>::translate( const char* data, const size_t size, const bool bigEndian )
{
	utils::EndiannessConverter<DataType> endianConv;
	
	if( bigEndian )
	{
		std::reverse_copy( data, data + sizeof( DataType ), endianConv.data );
	}
	else
	{
		std::memcpy( endianConv.data, data, sizeof( DataType ) );
	}
	return endianConv.value;
}

template<>
Hexa Translator<Hexa>::translate( const char* data, const size_t size, const bool )
{
	Hexa ret;
	std::ostringstream os;
	for( size_t i = 0; i < size; ++i )
		os << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)data[i];
	ret.value = os.str();
	return ret;
}

template<>
Ascii Translator<Ascii>::translate( const char* data, const size_t size, const bool )
{
	Ascii ret;
	std::ostringstream os;
	for( size_t index = 0; index < size; ++index )
		os << (unsigned char)data[ index ];
	ret.value = os.str();
	return ret;
}

template<>
int8 Translator<int8>::translate( const char* data, const size_t, const bool )
{
	return data[0];
}

template<>
sint8 Translator<sint8>::translate( const char* data, const size_t, const bool )
{
	return data[0];
}

template<>
uint8 Translator<uint8>::translate( const char* data, const size_t, const bool )
{
	return data[0];
}


template< typename DataType >
std::string Translator<DataType>::translate( const DataType & data)
{
	std::stringstream str;
	str << data;
	return str.str();
}

template<>
std::string Translator<int8>::translate( const int8 & data)
{
	std::stringstream str;
	str << (short)data;
	return str.str();
}

template<>
std::string Translator<uint8>::translate( const uint8 & data)
{
	std::stringstream str;
	str << (unsigned short)data;
	return str.str();
}

#endif
