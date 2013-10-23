#ifndef _ELEMENT_CHECKER_CHECKER_HPP_
#define _ELEMENT_CHECKER_CHECKER_HPP_

#include <Common/common.hpp>
#include <Common/Element.hpp>

#include <iostream>
#include <memory>

namespace basic_element
{
	class Data;
	class Number;
}

namespace element_checker
{

class Checker
{
	
public:
	Checker( const std::shared_ptr< basic_element::Element > element );

	void check();

private:
	EStatus checkData  ( std::shared_ptr< basic_element::Data   > data );
	EStatus checkNumber( std::shared_ptr< basic_element::Number > num  );

private:
	std::shared_ptr< basic_element::Element > _element;
};

}

#endif
