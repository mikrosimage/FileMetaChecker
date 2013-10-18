#include "Data.hpp"
#include <SpecReader/SpecNode.hpp>
#include <BasicElement/Translator/Translator.hpp>

namespace basic_element
{

Data::Data( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous )
 : Element( node, previous )
{
}

void Data::set( const char* data, const size_t& size )
{
	_prop.data = new char [size];
	_prop.size = size;
	
	getEndianOrderedData( _prop.data, data );
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
