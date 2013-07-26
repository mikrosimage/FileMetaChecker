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

template< typename ElementType >
void Report::addBasicElement( std::shared_ptr< ElementType > element )
{
}

template< >
void Report::addBasicElement( std::shared_ptr< be::Element > element )
{
	_elementMap.insert( std::make_pair( element->getUniqueId(), element ) );
	_basicElementTree.add( element->getId(), element->getUniqueId() );
}

template< >
void Report::addBasicElement( std::shared_ptr< ben::Number > element )
{
	_numberMap.insert( std::make_pair( element->getUniqueId(), element ) );
	_basicElementTree.add( element->getId(), element->getUniqueId() );
}

template< >
void Report::addBasicElement( std::shared_ptr< bed::Data > element )
{
	_dataMap.insert( std::make_pair( element->getUniqueId(), element ) );
	_basicElementTree.add( element->getId(), element->getUniqueId() );
}



std::shared_ptr< be::Element > Report::getBasicElement( const size_t uniqueId )
{
	return _elementMap.at( uniqueId );
}

// std::shared_ptr< be::Element > Report::getBasicElement( const size_t uniqueId )
// {
// 	return _elementMap.at( uniqueId );
// }

// void Report::add( const std::string& entry, const std::string& value )
// {
// 	reportTree.get_child( kRoot ).put( entry, value );
// }

// void Report::add( const bpt::ptree& tree, const std::string& rootpath )
// {
// 	reportTree.get_child( kRoot ).push_back( bpt::ptree::value_type( rootpath, tree ) );
// }

// void Report::add( const std::vector< bpt::ptree >& tree, const std::string& rootpath )
// {
// 	BOOST_FOREACH( bpt::ptree specTree, tree )
// 	{
// 		reportTree.get_child( kRoot ).push_back( bpt::ptree::value_type( rootpath, specTree ) );
// 	}
// }

// void Report::exportReport( const std::string& filename )
// {
// 	Export e;
// 	e.writeXml( reportTree, filename );
// }

}
