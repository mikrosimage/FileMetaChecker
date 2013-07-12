namespace basic_element
{
namespace hexa_element
{

Hexa::Hexa() : Element( Element::eTypeHexa )
{
}

Range< std::string >& Hexa::getRange()
{
	LOG_INFO( " Hexa: \tGET RANGE " );
	return _range;
}

Map< std::string >& Hexa::getMap()
{
	LOG_INFO( " Hexa: \tGET MAP " );
	return _map;
}

void Hexa::setData( const char* data, const size_t& size )
{
	LOG_INFO( " Hexa: \tSET DATA " );
	std::string str;
	for (size_t i = 0; i < size; ++i)
		str.push_back( data[i] );
	_data = str;
}

std::string Hexa::getData() const
{
	LOG_INFO( " Hexa: \tGET DATA " );
	return _data;
}

Element::EStatus Hexa::checkData()
{
	LOG_INFO( " Hexa: \tCHECK DATA " );
	// bool isValidNumber;
	// if( _range.size() == 2 )
	// {
	// 	isValidNumber = isInRange( _data );
	// }
	return eStatusUnknown;
}

}
}
