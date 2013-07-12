#include "Data.hpp"

namespace basic_element
{
namespace data_element
{

Data::Data()
	: Element( Element::eTypeData )
	, _data( NULL )
	, _size( 0 )
{
}

Data::~Data()
{
	if( _data != NULL )
	{
		delete[] _data;
		_data = NULL;
	}
}


void Data::setData( const char* data, const size_t& size )
{
	LOG_INFO( " Data: \tSET DATA " );
	_data = new char [size];
	_size = size;
	std::memcpy( _data, data, _size );
}

char* Data::getData() const
{
	LOG_INFO( " Data: \tGET DATA " );
	return _data;
}

Element::EStatus Data::checkData()
{
	LOG_INFO( " Data: \tCHECK DATA " );
	return eStatusUnknown;
}

}
}