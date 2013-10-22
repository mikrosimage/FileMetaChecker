#ifndef _ELEMENT_CHECKER_CHECKER_HPP_
#define _ELEMENT_CHECKER_CHECKER_HPP_

#include <Common/common.hpp>
#include <Common/Element.hpp>

#include <iostream>
#include <memory>

namespace element_checker
{

class Checker
{
	
public:
	Checker( const std::shared_ptr< basic_element::Element > element );

	void check();

private:
	std::shared_ptr< basic_element::Element > _element;
};

}

#endif
