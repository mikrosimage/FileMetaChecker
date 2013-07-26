#include "ReportNode.hpp"

namespace report_generator
{

ReportNode::ReportNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal )
	: _node( node )
	, _index( index )
	, _indexTotal( indexTotal )
{
}

ReportNode::~ReportNode()
{
}


size_t ReportNode::getIndex()
{
	return _index;
}

size_t ReportNode::getIndexTotal()
{
	return _indexTotal;
}


}
