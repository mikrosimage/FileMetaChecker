#ifndef _QICHE_REPORT_HPP_
#define _QICHE_REPORT_HPP_

#include "element.hpp"

#include <iomanip>

class Report
{
public:
	Report()
	{
	}
	
	void addElement( const Element* e )
	{
		size_t count = 0;
		
		const Element* elem = e;
		
		while( ( elem = elem->getParent() ) != NULL )
		{
			count++;
		}

		std::cout << std::setw( 5*count ) << " " << "-----------------------------" << std::endl;
		std::cout << std::setw( 5*count ) << " " << "| " << e->getIndex() << "\t" << e->getId() << " (" << e->getStringStatus() << ") - " << e->getIteration()  << std::endl;
		std::cout << std::setw( 5*count ) << " " << "-----------------------------" << std::endl;
	}
	
};

#endif
