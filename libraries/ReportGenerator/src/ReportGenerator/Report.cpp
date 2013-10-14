#include "Report.hpp"

#include <iostream>

#include <BasicElement/Element.hpp>

namespace report_generator 
{

void Report::addElement( const basic_element::Element* e )
{
	size_t count = 0;
	
	const basic_element::Element* elem = e;
	
	while( ( elem = elem->getParent() ) != NULL )
	{
		count++;
	}

	std::cout << std::setw( 5*count ) << " " << "-----------------------------" << std::endl;
	std::cout << std::setw( 5*count ) << " " << "| " << e->getIndex() << "\t" << e->getId() << " (" << e->getStringStatus() << ") - " << e->getIteration()  << std::endl;
	std::cout << std::setw( 5*count ) << " " << "-----------------------------" << std::endl;
}

}
