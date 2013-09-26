#ifndef _BASIC_ELEMENT_DATA_DATA_HPP_
#define _BASIC_ELEMENT_DATA_DATA_HPP_

#include <basicElement/Element.hpp>

namespace basic_element
{
namespace data_element
{

class Data : public Element
{
public:
	Data( const std::string& id, const ESubType& subType = eSubTypeRaw, const EDisplayType& dispType = eDisplayTypeDefault );
	~Data();

	void        set ( const char* data, const size_t& size );
	
	template< EDisplayType DisplayType, typename OutputType >
	OutputType get() const;
	
	void setSpecData( const std::string& specValue );
	void setSpecData( const std::vector< std::string >& specValues );

	Element::EStatus checkData();
	
	std::vector< std::pair< std::string, std::string > > getElementInfo();

	Data& operator=( const Data& other );

private:
	template< typename NumberType >
	std::vector< NumberType > convertToVector() const;
	
private:
	std::string                _specValue;
	std::vector< std::string > _specValues;
	char*                      _data;
};

}
}

#include "Data.tcc"

#endif
