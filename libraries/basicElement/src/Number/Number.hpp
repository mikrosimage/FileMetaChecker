#ifndef _BASIC_ELEMENT_NUMBER_NUMBER_HPP_
#define _BASIC_ELEMENT_NUMBER_NUMBER_HPP_

#include <vector>
#include "Tools/Map.hpp"
#include "Tools/Range.hpp"

namespace basic_element
{
namespace number_element
{

typedef          char       int8;
typedef   signed char      sint8;
typedef unsigned char      uint8;
typedef          short      int16;
typedef unsigned short     uint16;
typedef          int        int32;
typedef unsigned int       uint32;
typedef   signed long long  int64;
typedef unsigned long long uint64;

typedef long double  ieeeExtended;     // 80 bit IEEE Standard 754 floating point

template< typename NumberType >
class Number : public Element
{
public:
	Number();

	Range< NumberType >& getRange();
	Map  < NumberType >& getMap();

	void setEndianess( bool isBigEndian );

	void setData( const char* data );
	NumberType getData() const;
	
	EStatus checkData();

private:	
	NumberType translate( const char* data );

private:
	NumberType _data;
	bool       _isBigEndian;
	size_t     _size;
	Range< NumberType > _range;
	Map  < NumberType > _map;
};


}
}

#include "Number.tcc"

#endif
