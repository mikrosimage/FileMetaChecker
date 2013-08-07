#ifndef _BASIC_ELEMENT_NUMBER_NUMBER_HPP_
#define _BASIC_ELEMENT_NUMBER_NUMBER_HPP_

#include "Tools/Map.hpp"
#include "Tools/Range.hpp"
#include "Element.hpp"

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
	~Number();

	void                 setData ( const char* data, const size_t& size );
	void                 getData ( char* buffer ) const;
	size_t               getSize () const;
	NumberType           getValue() const;
	Map  < NumberType >& getMap  ();
	void                 addRange( const NumberType& min, const NumberType& max );
	std::vector< Range< NumberType > >& getRange();

	std::string toString() const;

	EStatus     checkData();
	std::vector< std::pair< std::string, std::string > > getElementInfo();

	std::string getStringFromType();

private:
	void setSize   ();
	void translate ( const char* data );
	void setSubType();

private:
	std::vector< Range< NumberType > > _ranges;
	Map  < NumberType >                _map;
	NumberData                         _numData;
};


}
}

#include "Number.tcc"

#endif
