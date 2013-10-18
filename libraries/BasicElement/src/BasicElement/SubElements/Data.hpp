#ifndef _BASIC_ELEMENT_SUBELEMENTS_DATA_HPP_
#define _BASIC_ELEMENT_SUBELEMENTS_DATA_HPP_

#include <iostream>

#include <Common/Element.hpp>

namespace basic_element
{

class Data : public Element
{
	
public:
	Data( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous = std::shared_ptr< Element >() );	
	void check();
};

}

#endif
