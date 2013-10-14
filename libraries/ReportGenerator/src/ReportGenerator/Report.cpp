#include "Report.hpp"

#include <iostream>

#include <BasicElement/Element.hpp>

namespace report_generator 
{

void Report::addElement( const std::shared_ptr< basic_element::Element > e )
{
	_elements.push_back( e );
}

void Report::print( const std::shared_ptr< basic_element::Element > e )
{
	size_t count = 0;
	std::shared_ptr< basic_element::Element > elem( e );
	while( 1 )
	{
		count++;
		std::weak_ptr< basic_element::Element > p = elem->getParent();
		if( p.use_count() == 0 )
			break;
		elem = p.lock();
	}

	std::cout << std::setw( 5*count ) << " " << "-------------------------------------------------" << std::endl;
	std::cout << std::setw( 5*count ) << " " << "| " << e->getIndex() << "\t" << e->getId() << " |" << e->getStringStatus() << ") - " << e->getIteration()  << std::endl;
	std::cout << std::setw( 5*count ) << " " << "-------------------------------------------------" << std::endl;
}

void Report::print()
{
	for( std::shared_ptr< basic_element::Element > e : _elements )
	{
		print( e );
	}
}

}
