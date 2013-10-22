#include <Common/log.hpp> 	// @todelete !
#include <iomanip>
#include <algorithm>

namespace element_checker
{

template< typename OutType>
OutType Translator::get()
{
	if( _ref->getData() == nullptr )
		throw std::runtime_error( "Undefined data" );

	// LOG_TRACE( "Generic translator: to number type" );
	if( _ref->getSize() != sizeof( OutType ) )
		throw std::runtime_error( "invalid data size" );
	
	NumberData< OutType > num;
	char* buffer = new char [ _ref->getSize() ];
	
	getOrderedData( buffer );
	std::memcpy( num.data, buffer, _ref->getSize() );
	delete[] buffer;
	return num.value;
}

#define GET_VECTOR( x ) \
template< > \
std::vector< x > Translator::get< std::vector< x > >() \
{ \
	if( _ref->getData() == nullptr ) \
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
std::string Translator::get< std::string >()
{
	if( _ref->getData() == nullptr )
		throw std::runtime_error( "Undefined data" );
	// LOG_TRACE( "Specific translator: to string" );
	return std::string { _ref->getData(), _ref->getSize() };
}

template< typename NumberType >
std::vector< NumberType > Translator::convertToVector()
{
	if( _ref->getSize() % sizeof( NumberType ) != 0 )
		throw std::runtime_error( "invalid data size" );

	std::vector< NumberType > vector;
	
	for( size_t i = 0; i < _ref->getSize(); i += sizeof( NumberType ) )
	{
		NumberData< NumberType > num;
		char* buffer = new char [ _ref->getSize() ];

		if( _ref->getSize() == sizeof( NumberType ) )
			getOrderedData( buffer );
		else
			std::memcpy( buffer, _ref->getData(), _ref->getSize() );

		std::memcpy( num.data, &buffer[i], _ref->getSize() );
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
	} binInt = { 0x00000001 };

	return ( binInt.c[0] == 1 ); 
}

void Translator::getOrderedData( char* buffer )
{
	if( ! _ref->isBigEndian() )
		std::reverse_copy( _ref->getData(), _ref->getData() + _ref->getSize(), buffer );
	else
		std::memcpy( buffer, _ref->getData(), _ref->getSize() );

	if( _ref->getType() == eTypeNumber && isSystemLittleEndian() )
	{
		char* temp = new char[ _ref->getSize() ];
		std::memcpy( temp, buffer, _ref->getSize() );
		std::reverse_copy( temp, temp + _ref->getSize(), buffer );	// swap if system little endian
		delete[] temp;
	}
}

}
