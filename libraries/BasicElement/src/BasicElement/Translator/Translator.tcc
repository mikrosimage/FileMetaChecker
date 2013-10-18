
#include <Common/common.hpp>
#include <Common/log.hpp> 	// @todelete !

#include <sstream> 
#include <vector>

template< typename OutType>
OutType Translator::get()
{
	LOG_TRACE( "Generic translator: to number type" );
	if( ref->getSize() != sizeof( OutType ) )
		throw std::runtime_error( "invalid data size" );
	NumberData< OutType > num;
	std::memcpy( num.data, ref->getData(), ref->getSize() );
	return num.value;
}

template<>
std::vector<int> Translator::get< std::vector< int > >()
{
	LOG_TRACE( "Specific translator: to vector" );
	std::vector<int> vec;
	for( size_t i=0; i < ref->getSize(); ++i )
		vec.push_back( (int) ref->getData()[i] );
	return vec;
}

template< >
std::string Translator::get< std::string >()
{
	LOG_TRACE( "Specific translator: to string" );
	return std::string { ref->getData(), ref->getSize() };
}

