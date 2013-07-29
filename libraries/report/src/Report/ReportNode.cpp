#include "ReportNode.hpp"

static const std::string kGroup = "group";

namespace report_generator
{

ReportNode::ReportNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal )
	: _node( node )
	, _parent( NULL )
	, _index( index )
	, _indexTotal( indexTotal )
{
}

ReportNode::ReportNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal, ReportNode* parent )
	: _node( node )
	, _parent( parent )
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

ReportNode ReportNode::firstChild()
{
	try
	{
		if( !hasGroup() )
			throw std::runtime_error( "firstChild: This node has no child." );
		return ReportNode( _node->second.get_child( kGroup ).begin(), 0, _node->second.get_child( kGroup ).size(), this );
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

ReportNode* ReportNode::parent()
{
	try
	{
		if( _parent == NULL )
			throw std::runtime_error( "parent: This node has no parent." );
		return _parent;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}


bool ReportNode::hasGroup()
{
	return _node->second.get_child_optional( kGroup );
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
