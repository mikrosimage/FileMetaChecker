#include "Number.hpp"
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace basic_element
{

Number::Number( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous )
 : Element( node, previous )
{
}

void Number::check()
{
	std::cout << _prop.uId << "  check number" << std::endl;
	_prop.status = eStatusPassOverData;
}

}
