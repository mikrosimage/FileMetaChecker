#include "Data.hpp"
#include <SpecReader/SpecNode.hpp>

namespace basic_element
{

Data::Data( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous )
 : Element( node, previous )
{
}

void Data::check()
{
	std::cout << _prop.uId << "  check data" << std::endl;
	
	if( _prop.uId == 7 )
	{
		_prop.status = eStatusInvalidButOptional;
		return;
	}
	
	if( _prop.uId == 16 )
	{
		_prop.status = eStatusInvalidButSkip;
		return;
	}
	
	_prop.status = eStatusValid;
}

}
