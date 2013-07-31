#include "Report.hpp"

#include <common/global.hpp>
#include <boost/foreach.hpp>

namespace report_generator
{

Report::Report()
{
}

Report::~Report()
{
}

ReportNode Report::addFirstElement( std::shared_ptr< be::Element > element )
{
	if( getSize() > 0 )
	{
		LOG_WARNING( "addFirstElement: first node already exist.");
	}
	else
	{
		ReportTree tree;
		tree.add( toKey( getSize() ), element );
		_basicElementTree.add_child( kReport + ".", tree );
	}
	return getFirstNode();
}

ReportNode Report::getFirstNode()
{
	try
	{
		if( getSize() == 0 )
			throw std::runtime_error( "getFirstNode: empty tree." );
		return ReportNode( getBegin(), 0, &_basicElementTree );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

ReportIterator Report::getBegin()
{
	try
	{
		if( getSize() == 0 )
			throw std::runtime_error( "getBegin: empty tree." );
		return _basicElementTree.get_child( kReport ).begin();
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

size_t Report::getSize()
{
	if( _basicElementTree.empty() )
		return 0;
	return _basicElementTree.get_child( kReport ).size();
}

std::string Report::toKey( size_t id )
{
	std::stringstream sstr;
	sstr << id;
	return sstr.str();
}


}
