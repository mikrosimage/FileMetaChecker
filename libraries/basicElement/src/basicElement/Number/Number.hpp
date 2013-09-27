#ifndef _BASIC_ELEMENT_NUMBER_NUMBER_HPP_
#define _BASIC_ELEMENT_NUMBER_NUMBER_HPP_

#include <basicElement/Tools/Map.hpp>
#include <basicElement/Tools/Ranges.hpp>
#include <basicElement/Element.hpp>

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
	Number( const std::string& id, const ESubType& subType = eSubTypeInt8, const EDisplayType& dispType = eDisplayTypeDefault );
	~Number();
	
	void set ( const char* data, const size_t& size );
	
	template< EDisplayType DisplayType = eDisplayTypeDefault, typename OutputType = NumberType >
	OutputType get() const;
	
	Map  < NumberType >& getMap  ();

	void setMap( const std::map< std::string, std::string >& map );

	void addRange( const NumberType& min, const NumberType& max );
	void setRanges( const std::vector< std::pair< std::string, std::string > >& ranges );
	Ranges< NumberType >& getRanges();

	EStatus     checkData();
	std::vector< std::pair< std::string, std::string > > getElementInfo();

protected:
	NumberType fromString( const std::string& value );
	void setSize   ();
	void translate ( const char* data );

public:
	std::string getStringFromType();

protected:
	void setSubType();

private:
	Ranges< NumberType >  _ranges;
	Map   < NumberType >  _map;
	NumberData            _numData;
};


}
}

#include "Number.tcc"

#endif
