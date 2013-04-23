#ifndef _EXTRACTOR_INPUT_FILE_TRANSLATOR_HPP_
#define _EXTRACTOR_INPUT_FILE_TRANSLATOR_HPP_

#include "types.hpp"
#include "common/global.hpp"

#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdlib>

namespace utils
{
	union FloatConverter
	{
		float f;
		unsigned char c[0];
	};

	union DoubleConverter
	{
		double f;
		unsigned char c[0];
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
};

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

template<>
int16 Translator<int16>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 1;
	const size_t pos1 = bigEndian ? 1 : 0;
	
	return ( (size_t)(unsigned char) data[ pos0 ] << 8 ) +
		   (size_t)(unsigned char) data[ pos1 ];
}

template<>
uint16 Translator<uint16>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 1;
	const size_t pos1 = bigEndian ? 1 : 0;
	return ( (size_t)(unsigned char) data[ pos0 ] << 8 ) +
		   (size_t)(unsigned char) data[ pos1 ];
}

template<>
int32 Translator<int32>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 3;
	const size_t pos1 = bigEndian ? 1 : 2;
	const size_t pos2 = bigEndian ? 2 : 1;
	const size_t pos3 = bigEndian ? 3 : 0;
	
	return  ( ( (size_t)(unsigned char)data[ pos0 ] ) << 24 ) +
			( ( (size_t)(unsigned char)data[ pos1 ] ) << 16 ) +
			( ( (size_t)(unsigned char)data[ pos2 ] ) << 8  ) +
			(size_t)(unsigned int) data[ pos3 ];
}

template<>
uint32 Translator<uint32>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 3;
	const size_t pos1 = bigEndian ? 1 : 2;
	const size_t pos2 = bigEndian ? 2 : 1;
	const size_t pos3 = bigEndian ? 3 : 0;

	/*std::ostringstream os;
	for( size_t i=0; i<4; ++i )
		os << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)data[ bigEndian ? i : 3 - i ];
	COMMON_COUT_VAR( os.str() );*/

	return ( ( (size_t)(unsigned char)data[ pos0 ] ) << 24 ) |
		   ( ( (size_t)(unsigned char)data[ pos1 ] ) << 16 ) |
		   ( ( (size_t)(unsigned char)data[ pos2 ] ) << 8 ) |
		   (size_t)(unsigned char)data[ pos3 ];
}

template<>
float Translator<float>::translate( const char* data, const size_t, const bool bigEndian )
{
	utils::FloatConverter floatValue;

	if( bigEndian )
	{
		std::memcpy( floatValue.c, data, 4);
	}
	else
	{
		for( size_t i = 0; i < 4; i++ )
		{
			floatValue.c[ i ] = data[ 3 - i ];
		}
	}

	return floatValue.f;
}

template<>
double Translator<double>::translate( const char* data, const size_t, const bool bigEndian )
{
	utils::DoubleConverter doubleValue;

	if( bigEndian )
	{
		std::memcpy( doubleValue.c, data, 8);
	}
	else
	{
		for( size_t i = 0; i < 8; i++ )
		{
			doubleValue.c[ i ] = data[ 7 - i ];
		}
	}
	
	return doubleValue.f;
}

#endif
