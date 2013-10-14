#include "Data.hpp"
#include <SpecReader/SpecNode.hpp>

namespace basic_element
{

Data::Data( const spec_reader::SpecNode* node, const Element* previous )
 : Element( node, previous )
{
}

void Data::check()
{
	std::cout << _index << "  check data" << std::endl;
	
	if( _index == 7 )
	{
		_status = eStatusInvalidButOptional;
		return;
	}
	
	if( _index == 16 )
	{
		_status = eStatusInvalidButSkip;
		return;
	}
	
	_status = eStatusValid;
}

}
