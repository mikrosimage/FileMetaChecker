#ifndef _REPORT_REPORT_HPP_
#define _REPORT_REPORT_HPP_

#include <boost/property_tree/ptree.hpp>

#include <Element.hpp>

namespace bpt = boost::property_tree;
namespace be  = basic_element;

namespace report_generator
{

class ReportNode
{
public:
	
	ReportNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal );
	~ReportNode();

	// void appendNext ( std::shared_ptr< be::Element > element );	// @todo
	// void appendChild( std::shared_ptr< be::Element > element );	// @todo

	// ReportNode next();		// @todo
	// ReportNode firstChild();	// @todo
	// ReportNode* parent();	// @todo

	size_t getIndex();
	size_t getIndexTotal();

private:
	// ReportNode* _parent;		// @todo
	bpt::ptree::const_iterator _node;
	size_t _index;
	size_t _indexTotal;
};

}

#endif