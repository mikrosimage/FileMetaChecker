#ifndef _ELEMENT_CHECKER_CHECKER_HPP_
#define _ELEMENT_CHECKER_CHECKER_HPP_

#include <Common/common.hpp>
#include <BasicElement/Element.hpp>

#include <iostream>
#include <memory>

namespace element_checker
{

class Checker
{
	
public:
	Checker();

	void check( const std::shared_ptr< basic_element::Element > element );
	std::vector< std::shared_ptr< basic_element::Element > > getElementList() { return _elementList; }
private:
	bool isIterationValid( const std::shared_ptr< basic_element::Element > element, std::string& errorMessage );

private:
	std::vector< std::shared_ptr< basic_element::Element > > _elementList;
};

}

#endif
