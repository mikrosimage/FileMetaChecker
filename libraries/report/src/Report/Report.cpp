#include "Report.hpp"

#include <common/global.hpp>
#include <boost/foreach.hpp>

static const std::string kReport = "report";

namespace report_generator
{

Report::Report()
{
}

Report::Report( const ReportTree& reportTree )
	: _basicElementTree( reportTree )
{
}

Report::~Report()
{
}


void Report::addBasicElement( std::shared_ptr< be::Element > element )
{
	ReportTree tmpTree;
	tmpTree.add( toKey( element->getUniqueId() ), element );
	_basicElementTree.add_child( kReport + ".", tmpTree );
}

ReportNode Report::getFirstNode()
{
	try
	{
		return ReportNode( getBegin(), 0, getSize() );
	}
	catch( const std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

ReportIterator Report::getBegin()
{
	return _basicElementTree.get_child( kReport ).begin();
}

size_t Report::getSize()
{
	return _basicElementTree.get_child( kReport ).size();
}

std::string Report::toKey( size_t id )
{
	std::stringstream sstr;
	sstr << id;
	return sstr.str();
}


}
