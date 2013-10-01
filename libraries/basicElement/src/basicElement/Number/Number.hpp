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

class Number : public Element
{
	
	template< typename NumberType >
	class NumberTranslator
	{
		union NumberData
		{
			char       data[0];
			NumberType value;
		};

	public:
		NumberTranslator( const Number* parent, const char* data );

		NumberType getValue();
		NumberType fromString( const std::string& value );
		void setData( const char* data );
		bool isValueInRanges( const std::vector< std::pair< std::string, std::string > >& rawRanges );
		std::string getMapLabel( const std::map< std::string, std::string >& map );

	private:
		const Number*         _parent;
		NumberData            _numData;
		Ranges< NumberType >  _ranges;
		Map   < NumberType >  _map;
		size_t                _size;
	};

public:
	Number( const std::string& id, const ESubType& subType = eSubTypeInt8, const EDisplayType& dispType = eDisplayTypeDefault );
	~Number();
	
	void set ( const char* data, const size_t& size );
	
	template< typename OutputType, EDisplayType DisplayType = eDisplayTypeNumbers >
	OutputType get() const;
	
	void setRanges( const std::vector< std::pair< std::string, std::string > >& ranges );
	void setMap( const std::map< std::string, std::string >& map );

	EStatus checkData();
	std::vector< std::pair< std::string, std::string > > getElementInfo();

protected:
	bool isNumberInRange();
	void setSize();
	bool isSizeValid( const size_t& size );
private:
	char* _data;
	std::map< std::string, std::string >                 _rawMap;
	std::vector< std::pair< std::string, std::string > > _rawRanges;
};

}
}

#include "Number.tcc"

#endif
