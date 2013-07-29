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
	Report( const ReportTree& reportTree );
	~Report();

	void addBasicElement( std::shared_ptr< be::Element > element );

	ReportNode getFirstNode();

	ReportIterator getBegin();
	size_t getSize();
	
private:
	std::string toKey( size_t id );

private:
	ReportTree _basicElementTree;
};

}

#endif
