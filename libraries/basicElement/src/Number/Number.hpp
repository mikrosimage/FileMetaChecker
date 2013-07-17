#ifndef _BASIC_ELEMENT_NUMBER_NUMBER_HPP_
#define _BASIC_ELEMENT_NUMBER_NUMBER_HPP_

#include "Tools/Map.hpp"
#include "Tools/Range.hpp"

namespace basic_element
{
namespace number_element
{

typedef          char       int8;
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
	union NumberData
	{
		char       data[0];
		NumberType value;
	};
public:
	Number();

	void setData( const char* data, const size_t& size );
	void getData( char* buffer ) const;
	size_t     getSize()  const;
	NumberType getValue() const;
	Range< NumberType >& getRange();
	Map  < NumberType >& getMap();

	std::string toString() const;

	EStatus checkData();

private:	
	void setSize();
	void translate( const char* data );

private:
	NumberData          _numData;
	Range< NumberType > _range;
	Map  < NumberType > _map;
};


}
}

#include "Number.tcc"

#endif
