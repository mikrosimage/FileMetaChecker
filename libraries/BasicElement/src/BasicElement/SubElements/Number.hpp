#ifndef _BASIC_ELEMENT_SUBELEMENTS_NUMBER_HPP_
#define _BASIC_ELEMENT_SUBELEMENTS_NUMBER_HPP_

#include <BasicElement/Element.hpp>

namespace basic_element
{

class Number : public Element
{
	
public:
	Number( const spec_reader::SpecNode* node, const Element* previous );
	void check();
};

}

#endif