#ifndef _BASIC_ELEMENT_SUBELEMENTS_NUMBER_HPP_
#define _BASIC_ELEMENT_SUBELEMENTS_NUMBER_HPP_

#include <Common/Element.hpp>

namespace basic_element
{

class Number : public Element
{
	
public:
	Number( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous = std::shared_ptr< Element >() );
	void check();
};

}

#endif