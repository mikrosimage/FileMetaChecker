#ifndef _REPORT_REPORT_HPP_
#define _REPORT_REPORT_HPP_

#include <boost/property_tree/ptree.hpp>

#include "ReportNode.hpp"

namespace bpt = boost::property_tree;

namespace basic_element{
	class Element;
}

namespace report_generator
{

class Report
{
public:
	Report();
	~Report();

	ReportNode addRootElement( std::shared_ptr< basic_element::Element > element );
	size_t getSize();

	ReportTree getReportTree() const;
	
private:
	std::string toKey( size_t id );

private:
	ReportTree _basicElementTree;
};

}

#include "Report.tcc"

#endif
