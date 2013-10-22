#include <Common/log.hpp> 	// @todelete !
#include <iomanip>

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
	std::memcpy( num.data, _ref->getData(), _ref->getSize() );
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
std::vector< NumberType > Translator::convertToVector() const
{
	if( _ref->getSize() % sizeof( NumberType ) != 0 )
		throw std::runtime_error( "invalid data size" );
	std::vector< NumberType > vector;
	for( size_t i = 0; i < _ref->getSize(); i += sizeof( NumberType ) )
	{
		NumberData< NumberType > num;
		std::memcpy( num.data, &_ref->getData()[i], _ref->getSize() );
		vector.push_back( num.value );
	}
	return vector;
}

}
