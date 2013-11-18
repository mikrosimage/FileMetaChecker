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
	
	void init( const std::vector< std::shared_ptr< basic_element::Element > >& elementList );
	void print();
	void print( const std::shared_ptr< basic_element::Element > element, const std::string& dispColor );
	void writeXml( const std::string& filename );
	
private:
	std::vector< std::shared_ptr< basic_element::Element > > _elementList;

};

}

#endif
