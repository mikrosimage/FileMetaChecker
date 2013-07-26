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


void Report::addBasicElement( std::shared_ptr< be::Element > element )
{
	_basicElementTree.add( toKey( element->getUniqueId() ), element );
}

std::shared_ptr< be::Element > Report::getBasicElement( const size_t uniqueId )
{
	return _basicElementTree.get_child( toKey( uniqueId ) ).data();
}

std::string Report::toKey( size_t id )
{
	std::stringstream sstr;
	sstr << id;
	return sstr.str();
}


}
