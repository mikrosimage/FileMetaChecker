#ifndef _EXTRACTOR_INPUT_FILE_TRANSLATOR_HPP_
#define _EXTRACTOR_INPUT_FILE_TRANSLATOR_HPP_

#include <cstring>
#include <sstream>
#include <iomanip>

typedef          char       int8;
typedef   signed char      sint8;
typedef unsigned char      uint8;
typedef          short      int16;
typedef unsigned short     uint16;
typedef          int        int32;
typedef unsigned int       uint32;
typedef          long long  int64;
typedef   signed long long  int64;
typedef unsigned long long uint64;

struct Hexa{
	std::string value;
};
typedef struct Hexa Hexa;

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
Hexa Translator<Hexa>::translate( const char* data, const size_t, const bool )
{
	Hexa ret;
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)data[0];
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
	
	return ( (unsigned char) data[ pos0 ] << 8 ) + (unsigned char) data[ pos1 ];
}

template<>
uint16 Translator<uint16>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 1;
	const size_t pos1 = bigEndian ? 1 : 0;
	return ( (unsigned char) data[ pos0 ] << 8 ) + (unsigned char) data[ pos1 ];
}

template<>
int32 Translator<int32>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 3;
	const size_t pos1 = bigEndian ? 1 : 2;
	const size_t pos2 = bigEndian ? 2 : 1;
	const size_t pos3 = bigEndian ? 3 : 0;
	
	return  ( (unsigned int) data[ pos0 ] << 24 ) +
			( (unsigned int) data[ pos1 ] << 16 ) +
			( (unsigned int) data[ pos2 ] << 8  ) +
			(unsigned int) data[ pos3 ];
}

template<>
uint32 Translator<uint32>::translate( const char* data, const size_t, const bool bigEndian )
{
	const size_t pos0 = bigEndian ? 0 : 3;
	const size_t pos1 = bigEndian ? 1 : 2;
	const size_t pos2 = bigEndian ? 2 : 1;
	const size_t pos3 = bigEndian ? 3 : 0;
	
	return  ( (unsigned int) data[ pos0 ] << 24 ) +
			( (unsigned int) data[ pos1 ] << 16 ) +
			( (unsigned int) data[ pos2 ] << 8  ) +
			(unsigned int) data[ pos3 ];
}

#endif
