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
	char* getData() const;
	EStatus checkData();

private:
	char*  _data;
	size_t _size;
};

}
}

#endif
