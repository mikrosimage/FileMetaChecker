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
		unsigned short i;
		char c[2];
	} binInt = { 0x0001 };

	return binInt.c[0];
}

void Translator::getOrderedData( char* buffer )
{
	if( ! _ref->_isBigEndian )
		std::reverse_copy( &_ref->_data[0], &_ref->_data[0] + _ref->_data.size(), buffer );
	else
		std::memcpy( buffer, &_ref->_data[0], _ref->_data.size() );

	if( ! isSystemLittleEndian() )
		return;

	switch( _ref->_type )
	{
		case eTypeInt16 :
		case eTypeUInt16 :
		case eTypeInt32 :
		case eTypeUInt32 :
		case eTypeInt64 :
		case eTypeUInt64 :
		case eTypeFloat :
		case eTypeDouble :
		case eTypeIeeeExtended :
		{
			std::vector<char> temp;
			temp.resize( _ref->_data.size() );

			std::memcpy( &temp[0], buffer, _ref->_data.size() );
			std::reverse_copy( &temp[0], &temp[0] + _ref->_data.size(), buffer ); // swap if system little endian
			break;
		}
		default:
			break;
	}
}

}
