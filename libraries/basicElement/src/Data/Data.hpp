#ifndef _BASIC_ELEMENT_DATA_DATA_HPP_
#define _BASIC_ELEMENT_DATA_DATA_HPP_

#include "Element.hpp"

namespace basic_element
{
namespace data_element
{

class Data : public Element
{
public:
	Data( const std::string& id, const ESubType& subType = eSubTypeRaw, const EDisplayType& dispType = eDisplayTypeDefault );
	~Data();

	void        setData ( const char* data, const size_t& size );
	void        getData ( char* buffer ) const;
	size_t      getSize () const;
	std::string getAscii() const;
	std::string getHexa () const;
	std::vector< unsigned int > toIntVector();
	
	void setSpecData( const std::string& specValue );
	void setSpecData( const std::vector< std::string >& specValues );

	Element::EStatus checkData();
	
//	void setDisplayType( const std::string& displayType );
	std::vector< std::pair< std::string, std::string > > getElementInfo();

	Data& operator=( const Data& other );

protected:
	std::string getStringFromType();
	std::string getStringFromData();
	std::string getRawDataString();
	char* getDataPtr (  ) const;
	
private:
	std::string                _specValue;
	std::vector< std::string > _specValues;
	char*                      _data;
};

}
}

#endif
