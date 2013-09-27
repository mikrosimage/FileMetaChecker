#include <cstdlib>

namespace basic_element
{
namespace number_element
{

template< typename NumberType >
Number< NumberType >::Number( const std::string& id, const ESubType& subType, const EDisplayType& dispType  )
	: Element( id, eTypeNumber, subType, dispType )
{
	setSubType();
	setSize();
	LOG_ERROR( _size );
}

template< typename NumberType >
Number< NumberType >::~Number()
{
}

template< typename NumberType >
void Number< NumberType >::translate( const char* data )
{
	char* buffer = new char[_size];
	getEndianOrderedData( buffer, data );
	reverseEndianness( _numData.data, buffer ); // @todo swap system if big endian (little endian here)
	
	delete[] buffer;
	//LOG_TRACE( " Number: \t_numData.value: " << _numData.value );
}

template<>
void Number< int8 >::translate( const char* data )
{
	std::memcpy( _numData.data, data, _size );
	//LOG_TRACE( " Number: \t_numData.value: " << (int) _numData.value );
}

template<>
void Number< uint8 >::translate( const char* data )
{
	std::memcpy( _numData.data, data, _size );
	//LOG_TRACE( " Number: \t_numData.value: " << (int) _numData.value );
}

template< typename NumberType >
NumberType Number< NumberType >::fromString( const std::string& value )
{
	try
	{
		NumberType ret;
		std::stringstream sstr( value );
		sstr >> ret;
		//LOG_TRACE( "fromString: " << ret );
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
		//LOG_TRACE( "fromString: " << ret );
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
		LOG_TRACE( "fromString: " << ret );
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
void Number< NumberType >::set( const char* data, const size_t& size )
{
	//LOG_TRACE( " Number: \tSET DATA " << " @ " << &data << " to @ " << &_numData.data );
	if( size != _size )
		throw std::runtime_error( "Invalid data size." );
	translate( data );
}

template< typename NumberType >
template< EDisplayType DisplayType, typename OutputType >
OutputType Number< NumberType >::get() const
{
	return _numData.value;
}

template< >
template< >
std::string Number< int8 >::get< eDisplayTypeAscii, std::string >() const
{
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}

template< >
template< >
std::string Number< uint8 >::get< eDisplayTypeAscii, std::string >() const
{
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}

#define GET_NUMBER( x ) \
template< > \
template< > \
std::string Number< x >::get< eDisplayTypeAscii, std::string >() const \
{ \
	std::stringstream sstr; \
	sstr << _numData.value; \
	return sstr.str(); \
}

GET_NUMBER( int16  )
GET_NUMBER( uint16 )
GET_NUMBER( int32  )
GET_NUMBER( uint32 )
GET_NUMBER( int64  )
GET_NUMBER( uint64 )
GET_NUMBER( float  )
GET_NUMBER( double )
GET_NUMBER( ieeeExtended )

template< typename NumberType >
void Number< NumberType >::addRange( const NumberType& min, const NumberType& max )
{
	Range< NumberType > range;
	range.setRange( min, max );
	_ranges.addRange( range );
}

template< typename NumberType >
void Number< NumberType >::setRanges( const std::vector< std::pair< std::string, std::string > >& ranges )
{
	for( std::pair< std::string, std::string > range : ranges )
		addRange( fromString( range.first ), fromString( range.second ) );
}

template< typename NumberType >
Ranges< NumberType >& Number< NumberType >::getRanges()
{
	return _ranges;
}

template< typename NumberType >
Map< NumberType >& Number< NumberType >::getMap()
{
	return _map;
}

template< typename NumberType >
void Number< NumberType >::setMap( const std::map< std::string, std::string >& map )
{
	for( const std::pair< std::string, std::string > mapPair : map )
		_map.addPair( fromString( mapPair.first ), mapPair.second );
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
Element::EStatus Number< NumberType >::checkData()
{
	_status = eStatusInvalid;

	if( _ranges.empty() )
	{
		_status = eStatusPassOver;
		return eStatusPassOver;
	}
	
	if( _ranges.isInRanges( _numData.value ) )
		_status = eStatusValid;
	
	return _status;
}

template< typename NumberType >
std::vector< std::pair< std::string, std::string > > Number< NumberType >::getElementInfo()
{
	std::vector< std::pair< std::string, std::string > > elemInfo;
	std::vector< std::pair< std::string, std::string > > commonInfo = getCommonElementInfo();
	
	elemInfo.insert( elemInfo.end(), commonInfo.begin(), commonInfo.end() );

	elemInfo.push_back( std::make_pair( kType, getStringFromType() ) );

	if( _map.getSize() == 0 )
	{
		elemInfo.push_back( std::make_pair( kValue, get< eDisplayTypeAscii, std::string >() ) );
	}
	else
	{
		std::string value = _map.getLabel( _numData.value );
		
		if( ! value.empty() )
			value.append( " (" + get< eDisplayTypeAscii, std::string >() + ")" );
		else
			value = "----- (" + get< eDisplayTypeAscii, std::string >() + ")";
		
		elemInfo.push_back( std::make_pair( kValue, value ) );
	}

	return elemInfo;
}

#define GET_STRING_FROM_TYPE( x, y ) \
template< > \
std::string Number< x >::getStringFromType() { return y; }

GET_STRING_FROM_TYPE( int8,   kInt8   )
GET_STRING_FROM_TYPE( uint8,  kUInt8  )
GET_STRING_FROM_TYPE( int16,  kInt16  )
GET_STRING_FROM_TYPE( uint16, kUInt16 )
GET_STRING_FROM_TYPE( int32,  kInt32  )
GET_STRING_FROM_TYPE( uint32, kUInt32 )
GET_STRING_FROM_TYPE( int64,  kInt64  )
GET_STRING_FROM_TYPE( uint64, kUInt64 )
GET_STRING_FROM_TYPE( float,  kFloat  )
GET_STRING_FROM_TYPE( double, kDouble )
GET_STRING_FROM_TYPE( ieeeExtended, kIeeeExtended )

#define SET_SUB_TYPE( x, y ) \
template< > \
void Number< x >::setSubType() { _subType = y; }

SET_SUB_TYPE( int8,   eSubTypeInt8   )
SET_SUB_TYPE( uint8,  eSubTypeUInt8  )
SET_SUB_TYPE( int16,  eSubTypeInt16  )
SET_SUB_TYPE( uint16, eSubTypeUInt16 )
SET_SUB_TYPE( int32,  eSubTypeInt32  )
SET_SUB_TYPE( uint32, eSubTypeUInt32 )
SET_SUB_TYPE( int64,  eSubTypeInt64  )
SET_SUB_TYPE( uint64, eSubTypeUInt64 )
SET_SUB_TYPE( float,  eSubTypeFloat  )
SET_SUB_TYPE( double, eSubTypeDouble )
SET_SUB_TYPE( ieeeExtended, eSubTypeIeeeExtended )

}
}
