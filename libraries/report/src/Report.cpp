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
	//_elementMap.insert( std::make_pair( element->getUniqueId(), element ) );
	//_basicElementTree.add( element->getId(), element->getUniqueId() );
}

std::shared_ptr< be::Element > Report::getBasicElement( const size_t uniqueId )
{
	return _elementMap.at( uniqueId );
}

}
