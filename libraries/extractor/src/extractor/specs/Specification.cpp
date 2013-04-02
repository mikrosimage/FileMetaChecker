#include "Specification.hpp"

#include <common/global.hpp>

#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

const std::string kStandard  = "standard";
const std::string kLabel     = "label";
const std::string kId        = "id";
const std::string kType      = "type";
const std::string kExtension = "extension";
const std::string kHeader    = "header";

const std::string kFooter    = "footer";

Specification::Specification()
{
	
}

void Specification::setSpecTree( const Spec& spec )
{
	_spec = spec;
}

std::string Specification::getId( )
{
	return _spec.get< std::string >( kStandard + "." + kId );
}

std::string Specification::getLabel( )
{
	return _spec.get< std::string >( kStandard + "." + kLabel );
}

std::string Specification::getType( )
{
	return _spec.get< std::string >( kStandard + "." + kType );
}

std::vector< std::string > Specification::getsupportedExtensions( )
{
	std::vector< std::string > list;
	BOOST_FOREACH( Node &node, _spec.get_child( kStandard + "." + kExtension ) )
	{
		list.push_back( node.second.data() );
	}
	return list;
}

SpecIt Specification::getHeader( )
{
	return _spec.find( kHeader );
}
