#ifndef _REPORT_REPORT_NODE_HPP_
#define _REPORT_REPORT_NODE_HPP_

#include <boost/property_tree/ptree.hpp>

#include <Element.hpp>

static const std::string kReport    = "report";

namespace be  = basic_element;
namespace bpt = boost::property_tree;

namespace report_generator
{

typedef bpt::basic_ptree< std::string, std::shared_ptr< be::Element > >                 ReportTree;
typedef bpt::basic_ptree< std::string, std::shared_ptr< be::Element > >::const_iterator ReportIterator;

class ReportNode
{
public:
	ReportNode() {}
	ReportNode( const ReportIterator node, const size_t& index, ReportTree* tree );
	ReportNode( const ReportIterator node, const size_t& index, ReportTree* tree, ReportNode* parent );
	~ReportNode();

	ReportNode appendNext ( std::shared_ptr< be::Element > element );
	ReportNode appendChild( std::shared_ptr< be::Element > element );
	ReportNode* parent();
	ReportTree* getSecond();
	size_t getIndex();

private:
	ReportIterator _nodeIt;
	ReportNode*    _parent;
	ReportTree*    _tree;
	size_t         _index;
};

}

#endif
