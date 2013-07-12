#include "Ascii.hpp"

namespace basic_element
{
namespace ascii_element
{

Ascii::Ascii() : Element( Element::eTypeAscii )
{
}

void Ascii::setData( const char* data, const size_t& size )
{
	LOG_INFO( " Ascii: \tSET DATA " );
	std::string str;
	for (int i = 0; i < size; ++i)
		str.push_back( data[i] );
	_data = str;
}

std::string Ascii::getData() const
{
	LOG_INFO( " Ascii: \tGET DATA " );
	return _data;
}

Element::EStatus Ascii::checkData()
{
	LOG_INFO( " Ascii: \tCHECK DATA " );
	return eStatusUnknown;
}

}
}