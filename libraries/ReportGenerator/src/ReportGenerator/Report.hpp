#ifndef _REPORT_GENERATOR_REPORT_HPP_
#define _REPORT_GENERATOR_REPORT_HPP_

#include <iomanip>
#include <memory>
#include <vector>

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
	
	void addElement( const std::shared_ptr< basic_element::Element > e );
	void print( const std::shared_ptr< basic_element::Element > e );
	void print();

private:
	std::vector< std::shared_ptr< basic_element::Element > > _elements;

};

}

#endif
