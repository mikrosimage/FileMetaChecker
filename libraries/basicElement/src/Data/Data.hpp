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
	Data();
	~Data();

	void setData( const char* data, const size_t& size );
	void getData( char* buffer ) const;
	size_t getSize() const;
	std::string getAscii() const;
	std::string getHexa() const;
	std::vector< unsigned int > toIntVector();
	

	EStatus checkData();

	Data& operator=( const Data& other );

private:
	char*  _data;

};

}
}

#endif
