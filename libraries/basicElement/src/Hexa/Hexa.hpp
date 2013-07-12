#ifndef _BASIC_ELEMENT_HEXA_HEXA_HPP_
#define _BASIC_ELEMENT_HEXA_HEXA_HPP_

#include "Tools/Map.hpp"
#include "Tools/Range.hpp"

namespace basic_element
{
namespace hexa_element
{

class Hexa : public Element
{
public:
	Hexa();

	Range< std::string >& getRange();
	Map  < std::string >& getMap();

	void setData( const char* data, const size_t& size );
	std::string getData() const;
	EStatus checkData();

private:
	std::string          _data;
	Range< std::string > _range;
	Map  < std::string > _map;
};

}
}

#include "Hexa.tcc"

#endif
