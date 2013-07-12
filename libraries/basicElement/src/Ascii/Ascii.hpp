#ifndef _BASIC_ELEMENT_ASCII_ASCII_HPP_
#define _BASIC_ELEMENT_ASCII_ASCII_HPP_

#include "Element.hpp"

namespace basic_element
{
namespace ascii_element
{

class Ascii : public Element
{
public:
	Ascii();

	void setData( const char* data, const size_t& size );
	std::string getData() const;
	EStatus checkData();

private:
	std::string _data;
};

}
}

#endif