
#include "Checker.hpp"

namespace element_checker
{

Checker::Checker( const std::shared_ptr< basic_element::Element > element )
	: _element( element )
{
}

void Checker::check()
{
	std::cout << _element->getType() << std::cout;
}

}

