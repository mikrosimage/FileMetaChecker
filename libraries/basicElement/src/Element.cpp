#include "Element.hpp"

namespace basic_element
{

Element::Element( EType type )
	: _type( type )
	, _status( eStatusUnknown )
{
}

void Element::setId( const std::string& id )
{
	LOG_INFO( " Element: SET ID " );
	_id = id;
}

void Element::setLabel( const std::string& label )
{
	LOG_INFO( " Element: SET LABEL " );
	_label = label;
}

void Element::setStatus( const EStatus status )
{
	LOG_INFO( " Element: SET STATUS " );
	_status = status;
}

}
