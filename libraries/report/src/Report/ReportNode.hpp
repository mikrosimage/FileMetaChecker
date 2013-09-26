#ifndef _REPORT_REPORT_NODE_HPP_
#define _REPORT_REPORT_NODE_HPP_

#include <boost/property_tree/ptree.hpp>

static const std::string kReport    = "report";

namespace bpt = boost::property_tree;

namespace basic_element{
	class Element;
}

namespace report_generator
{

typedef bpt::basic_ptree< std::string, std::shared_ptr< basic_element::Element > >                 ReportTree;
typedef bpt::basic_ptree< std::string, std::shared_ptr< basic_element::Element > >::const_iterator ReportIterator;

class ReportNode
{
public:
	ReportNode();
	ReportNode( const ReportIterator node, const size_t& index, ReportTree* tree, ReportNode* parent = NULL );
	~ReportNode();

	ReportNode appendNext ( std::shared_ptr< basic_element::Element > element );
	ReportNode appendChild( std::shared_ptr< basic_element::Element > element );
	ReportNode* parent();
	ReportTree* getSecond();
	size_t getIndex();

	ReportNode& operator=( const ReportNode& node )
	{
		_nodeIt = node._nodeIt;
		_parent = node._parent;
		_tree   = node._tree;
		_index  = node._index;
		return *this;
	}

private:
	ReportIterator _nodeIt;
	ReportNode*    _parent;
	ReportTree*    _tree;
	size_t         _index;
};

}

#include "ReportNode.tcc"

#endif
