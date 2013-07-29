#ifndef _REPORT_REPORT_NODE_HPP_
#define _REPORT_REPORT_NODE_HPP_

#include <boost/property_tree/ptree.hpp>

#include <Element.hpp>

namespace be  = basic_element;
namespace bpt = boost::property_tree;

namespace report_generator
{

typedef bpt::basic_ptree< std::string, std::shared_ptr< be::Element > > ReportTree;
typedef bpt::basic_ptree< std::string, std::shared_ptr< be::Element > >::const_iterator ReportIterator;

class ReportNode
{
public:
	
	ReportNode( const ReportIterator node, const size_t& index, const size_t& indexTotal );
	ReportNode( const ReportIterator node, const size_t& index, const size_t& indexTotal, ReportNode* parent );
	~ReportNode();

	// void appendNext( std::shared_ptr< be::Element > element );	// @todo
	// void appendChild( std::shared_ptr< be::Element > element );	// @todo

	ReportNode next();
	ReportNode firstChild();
	ReportNode* parent();

	std::shared_ptr< be::Element > getElementPointer();

	bool hasGroup();

	size_t getIndex();
	size_t getIndexTotal();

private:
	ReportIterator _node;
	ReportNode* _parent;
	size_t _index;
	size_t _indexTotal;
};

}

#endif