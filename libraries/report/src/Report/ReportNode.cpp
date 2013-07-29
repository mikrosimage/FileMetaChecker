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

ReportNode ReportNode::next()
{
	bpt::ptree::const_iterator node = _node;
	size_t index = _index;

	if( _index >= _indexTotal - 1 )
		return ReportNode( node, _indexTotal, _indexTotal );

	return ReportNode( ++node, ++index, _indexTotal );
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
