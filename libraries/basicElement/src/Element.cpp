#include "Element.hpp"

Element::Element( EType type )
	: _type( type )
	, _status( eStatusUnknown )
{
}

void Element::setId( const std::string& id )
{
	_id = id;
}

void Element::setLabel( const std::string& label )
{
	_label = label;
}

void Element::setStatus( const EStatus status )
{
	_status = status;
}
