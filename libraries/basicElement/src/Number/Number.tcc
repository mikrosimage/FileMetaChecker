#include <cstdlib>

namespace basic_element
{
namespace number_element
{

template< typename NumberType >
Number< NumberType >::Number()
	: Element( eTypeNumber )
{
	setSubType();
	setSize();
}

template< typename NumberType >
Number< NumberType >::~Number()
{
}

template< typename NumberType >
void Number< NumberType >::translate( const char* data )
{
	char* buffer = new char [_size];
	getEndianOrderedData( buffer, data );
	reverseEndianness( _numData.data, buffer ); // @todo swap system if big endian (little endian here)
	delete[] buffer;
	//BE_LOG_TRACE( " Number: \t_numData.value: " << _numData.value );
}

template<>
void Number< int8 >::translate( const char* data )
{
	std::memcpy( _numData.data, data, _size );
	//BE_LOG_TRACE( " Number: \t_numData.value: " << (int) _numData.value );
}

template<>
void Number< uint8 >::translate( const char* data )
{
	std::memcpy( _numData.data, data, _size );
	//BE_LOG_TRACE( " Number: \t_numData.value: " << (int) _numData.value );
}



template< typename NumberType >
std::string Number< NumberType >::toString() const
{
	std::stringstream sstr;
	sstr << _numData.value;
	return sstr.str();
}

template< >
std::string Number< int8 >::toString( ) const
{
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}

template< >
std::string Number< uint8 >::toString( ) const
{
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}

template< typename NumberType >
NumberType Number< NumberType >::fromString( const std::string& value )
{
	try
	{
		NumberType ret;
		std::stringstream sstr( value );
		sstr >> ret;
		LOG_INFO( "fromString: " << ret );
		return ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
int8 Number< int8 >::fromString( const std::string& value )
{
	try
	{
		short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		LOG_INFO( "fromString: " << ret );
		if( ret > 127 || ret < -128 )
			throw std::range_error( "fromString: string cannot be converted to int8" );
		return (int8) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
uint8 Number< uint8 >::fromString( const std::string& value )
{
	try
	{
		unsigned short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		LOG_INFO( "fromString: " << ret );
		if( ret > 255 || ret < 0 )
			throw std::range_error( "fromString: string cannot be converted to uint8" );
		return (uint8) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}


template< typename NumberType >
void Number< NumberType >::setData( const char* data, const size_t& size )
{
	//BE_LOG_TRACE( " Number: \tSET DATA " << " @ " << &data << " to @ " << &_numData.data );
	if( size != _size )
		throw std::runtime_error( "Invalid data size." );
	translate( data );
}

template< typename NumberType >
void Number< NumberType >::getData( char* buffer ) const
{
	//BE_LOG_TRACE( " Number: \tGET DATA from @ " << &_numData.data << " to @ " << &buffer << " | size: " << _size );
	std::reverse_copy( _numData.data, _numData.data + _size, buffer );
	//BE_LOG_TRACE( " Number: \tdata  : " << (int) _numData.data[0] << "\t\t\t@ " << &_numData.data );
	//BE_LOG_TRACE( " Number: \tbuffer: " << (int)(unsigned char) buffer[0] << "\t\t\t@ " << &buffer );
}

template< typename NumberType >
NumberType Number< NumberType >::getValue() const
{
	return _numData.value;
}


template< typename NumberType >
void Number< NumberType >::addRange( const NumberType& min, const NumberType& max )
{
	_ranges.push_back( Range< NumberType >( min, max ) );
}

template< typename NumberType >
void Number< NumberType >::setRanges( const std::vector< std::pair< std::string, std::string > >& ranges )
{
	for( std::pair< std::string, std::string > range : ranges )
		addRange( fromString( range.first ), fromString( range.second ) );
}

template< typename NumberType >
std::vector< Range< NumberType > >& Number< NumberType >::getRange()
{
	return _ranges;
}

template< typename NumberType >
Map< NumberType >& Number< NumberType >::getMap()
{
	return _map;
}

template< typename NumberType >
void Number< NumberType >::setSize()
{
	_size = sizeof( NumberType );
}

template< >
void Number< ieeeExtended >::setSize()
{
	_size = 10;
}

template< typename NumberType >
size_t Number< NumberType >::getSize() const
{
	return _size;
}

template< typename NumberType >
Element::EStatus Number< NumberType >::checkData()
{
	_status = eStatusInvalid;
	
	if( _ranges.empty() )
	{
		_status = eStatusPassOver;
		return eStatusPassOver;
	}
	
	for( Range< NumberType > range : _ranges )
	{
		if( range.isInRange( _numData.value ) )
			_status = eStatusValid;
	}
	
	return _status;
}

template< typename NumberType >
std::vector< std::pair< std::string, std::string > > Number< NumberType >::getElementInfo()
{
	std::vector< std::pair< std::string, std::string > > elemInfo;
	std::vector< std::pair< std::string, std::string > > commonInfo = getCommonElementInfo();
	
	elemInfo.insert( elemInfo.end(), commonInfo.begin(), commonInfo.end() );

	elemInfo.push_back( std::make_pair( "type",   getStringFromType() ) );
	elemInfo.push_back( std::make_pair( "value",  toString()          ) );
	return elemInfo;
}

template< >
std::string Number< int8 >::getStringFromType() { return "int8"; }

template< >
std::string Number< uint8 >::getStringFromType() { return "uint8"; }

template< >
std::string Number< int16 >::getStringFromType() { return "int16"; }

template< >
std::string Number< uint16 >::getStringFromType() { return "uint16"; }

template< >
std::string Number< int32 >::getStringFromType() { return "int32"; }

template< >
std::string Number< uint32 >::getStringFromType() { return "uint32"; }

template< >
std::string Number< int64 >::getStringFromType() { return "int64"; }

template< >
std::string Number< uint64 >::getStringFromType() { return "uint64"; }

template< >
std::string Number< float >::getStringFromType() { return "float"; }

template< >
std::string Number< double >::getStringFromType() { return "double"; }

template< >
std::string Number< ieeeExtended >::getStringFromType() { return "ieeeExtended"; }


template< >
void Number<  int8  >::setSubType() { _subType = eNumberTypeInt8; }

template< >
void Number< uint8  >::setSubType() { _subType = eNumberTypeUInt8; }

template< >
void Number<  int16 >::setSubType() { _subType = eNumberTypeInt16; }

template< >
void Number< uint16 >::setSubType() { _subType = eNumberTypeUInt16; }

template< >
void Number<  int32 >::setSubType() { _subType = eNumberTypeInt32; }

template< >
void Number< uint32 >::setSubType() { _subType = eNumberTypeUInt32; }

template< >
void Number<  int64 >::setSubType() { _subType = eNumberTypeInt64; }

template< >
void Number< uint64 >::setSubType() { _subType = eNumberTypeUInt64; }

template< >
void Number<  float >::setSubType() { _subType = eNumberTypeFloat; }

template< >
void Number< double >::setSubType() { _subType = eNumberTypeDouble; }

template< >
void Number< ieeeExtended >::setSubType() { _subType = eNumberTypeIeeeExtended; }

}
}
