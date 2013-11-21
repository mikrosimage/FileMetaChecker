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
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	
public:
	Report( const std::vector< ShPtrElement >& elementList = std::vector< ShPtrElement >() );
	
	void add( const ShPtrElement element );
	void add( const std::vector< ShPtrElement >& elementList );
	
	void print();
	void print( const ShPtrElement element, const std::string& dispColor );
	void writeXml( const std::string& filename );
	
private:
	std::vector< ShPtrElement > _elementList;

};

}

#endif
