#ifndef _REPORT_REPORT_HPP_
#define _REPORT_REPORT_HPP_

#include <boost/property_tree/ptree.hpp>
#include <Element.hpp>

#include "ReportNode.hpp"

namespace bpt = boost::property_tree;
namespace be  = basic_element;

namespace report_generator
{

class Report
{
public:
	Report();
	~Report();

	void addBasicElement( std::shared_ptr< be::Element > element );

	std::shared_ptr< be::Element > getBasicElement( const size_t uniqueId );
	
private:
	std::string toKey( size_t id );

private:
	bpt::basic_ptree< std::string, std::shared_ptr< be::Element > > _basicElementTree;
};

}

#endif
