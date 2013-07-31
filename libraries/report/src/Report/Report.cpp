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

ReportNode Report::addRootElement( std::shared_ptr< be::Element > element )
{
	ReportTree tree;
	size_t index = getSize();

	tree.add( toKey( index ), element );
	_basicElementTree.add_child( kReport + ".", tree );
	
	ReportIterator it = _basicElementTree.get_child( kReport ).begin();
	std::advance( it, index );
	
	return ReportNode( it, index, &_basicElementTree );;
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
