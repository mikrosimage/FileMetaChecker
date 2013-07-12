namespace basic_element
{
namespace number_element
{

template< typename NumberType >
Number< NumberType >::Number()
	: Element( Element::eTypeNumber )
	, _isBigEndian( true )
{
}


template< typename NumberType >
Range< NumberType >& Number< NumberType >::getRange()
{
	LOG_INFO( " Number: \tGET RANGE " );
	return _range;
}

template< typename NumberType >
Map< NumberType >& Number< NumberType >::getMap()
{
	LOG_INFO( " Number: \tGET MAP " );
	return _map;
}


template< typename NumberType >
void Number< NumberType >::setData( const char* data )
{
	LOG_INFO( " Number: \tSET DATA " );
	_data = translate( data );
}

template< typename NumberType >
NumberType Number< NumberType >::getData() const
{
	LOG_INFO( " Number: \tGET DATA " );
	LOG_INFO( " \t=> _data: " << _data );
	return _data;
}

template< typename NumberType >
Element::EStatus Number< NumberType >::checkData()
{
	LOG_INFO( " Number: \tCHECK DATA " );
	// bool isValidNumber;
	// if( _range.size() == 2 )
	// {
	// 	isValidNumber = isInRange( _data );
	// }
	return eStatusUnknown;
}


template< typename NumberType >
void Number< NumberType >::setEndianess( bool isBigEndian )
{
	LOG_INFO( " Number: \tSET ENDIANESS " );
	_isBigEndian = isBigEndian; 
}


namespace utils
{
	template< typename Type >
	union EndiannessConverter
	{
		Type value;
		unsigned char data[0];
	};
}

template< typename NumberType >
NumberType Number< NumberType >::translate( const char* data )
{
	LOG_INFO( " Number: \tTRANSLATE " );
	utils::EndiannessConverter< NumberType > endianConv;
	_size = sizeof( NumberType );
	
	if( _isBigEndian )
	{
		std::reverse_copy( data, data + _size, endianConv.data );
	}
	else
	{
		std::memcpy( endianConv.data, data, _size );
	}
	return endianConv.value;
}

template<>
int8 Number<int8>::translate( const char* data )
{
	return data[0];
}

template<>
sint8 Number<sint8>::translate( const char* data )
{
	return data[0];
}

template<>
uint8 Number<uint8>::translate( const char* data )
{
	return data[0];
}

}
}
