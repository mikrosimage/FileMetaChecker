#ifndef _REPORT_GENERATOR_REPORT_HPP_
#define _REPORT_GENERATOR_REPORT_HPP_

#include <iomanip>

namespace basic_element
{
	class Element;
}

namespace report_generator
{

class Report
{
public:
	Report()
	{
	}
	
	void addElement( const basic_element::Element* e );
	
};

}

#endif
