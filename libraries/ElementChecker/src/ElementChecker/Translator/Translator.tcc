#include <Common/log.hpp> 	// @todelete !
#include <iomanip>
#include <algorithm>
#include <stdexcept>

namespace element_checker
{

template< typename OutType>
OutType Translator::get()
{
	if( _ref->_data.empty() )
		throw std::runtime_error( "Undefined data" );

	// LOG_TRACE( "Generic translator: to number type" );
	if( _ref->_data.size() != sizeof( OutType ) )
		throw std::runtime_error( "invalid data size" );
	
	NumberData< OutType > num;
	char* buffer = new char [ _ref->_data.size() ];
	
	getOrderedData( buffer );
	std::memcpy( num.data, buffer, _ref->_data.size() );
	delete[] buffer;
	return num.value;
}

#define GET_VECTOR( x ) \
template< > \
std::vector< x > Translator::get< std::vector< x > >() \
{ \
	if( _ref->_data.empty() ) \
		throw std::runtime_error( "Undefined data" ); \
	return convertToVector< x >(); \
}

GET_VECTOR(          char      )
GET_VECTOR( unsigned char      )
GET_VECTOR(          short     )
GET_VECTOR( unsigned short     )
GET_VECTOR(          int       )
GET_VECTOR( unsigned int       )
GET_VECTOR(          long long )
GET_VECTOR( unsigned long long )
GET_VECTOR(          float     )
GET_VECTOR(          double    )
GET_VECTOR(     long double    )


template< >
std::string Translator::get< std::string >()	// get ascii character !
{
	if( _ref->_data.empty() )
		throw std::runtime_error( "Undefined data" );		// @todo: return "";
	// LOG_TRACE( "Specific translator: to string" );
	return std::string { &_ref->_data[0], _ref->_data.size() };
}

template< typename NumberType >
std::vector< NumberType > Translator::convertToVector()
{
	if( _ref->_data.size() % sizeof( NumberType ) != 0 )
		throw std::runtime_error( "invalid data size" );

	std::vector< NumberType > vector;
	
	for( size_t i = 0; i < _ref->_data.size(); i += sizeof( NumberType ) )
	{
		NumberData< NumberType > num;
		char* buffer = new char [ _ref->_data.size() ];

		if( _ref->_data.size() == sizeof( NumberType ) )
			getOrderedData( buffer );
		else
			std::memcpy( buffer, &_ref->_data[0], _ref->_data.size() );

		std::memcpy( num.data, &buffer[i], _ref->_data.size() );
		delete[] buffer;
		vector.push_back( num.value );
	}
	return vector;
}

bool Translator::isSystemLittleEndian()
{
	union
	{
		unsigned int i;
		char c[4];
	} binInt = { 0x01020304 };

	return ( binInt.c[0] == 4 ); 
}

void Translator::getOrderedData( char* buffer )
{
	if( ! _ref->_isBigEndian )
		std::reverse_copy( &_ref->_data[0], &_ref->_data[0] + _ref->_data.size(), buffer );
	else
		std::memcpy( buffer, &_ref->_data[0], _ref->_data.size() );

	if( ( _ref->_type == eTypeInt8
	   || _ref->_type == eTypeUInt8
	   || _ref->_type == eTypeInt16
	   || _ref->_type == eTypeUInt16
	   || _ref->_type == eTypeInt32
	   || _ref->_type == eTypeUInt32
	   || _ref->_type == eTypeInt64
	   || _ref->_type == eTypeUInt64
	   || _ref->_type == eTypeFloat
	   || _ref->_type == eTypeDouble
	   || _ref->_type == eTypeIeeeExtended )
	   && isSystemLittleEndian() )
	{
		char* temp = new char[ _ref->_data.size() ];
		std::memcpy( temp, buffer, _ref->_data.size() );
		std::reverse_copy( temp, temp + _ref->_data.size(), buffer );	// swap if system little endian
		delete[] temp;
	}
}

}
