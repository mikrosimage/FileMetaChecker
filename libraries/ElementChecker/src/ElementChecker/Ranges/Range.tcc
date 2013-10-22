#include <Common/log.hpp>

#include <limits>
#include <sstream>

namespace element_checker
{

template< typename ValueType >
Range< ValueType >::Range()
	: _min( std::numeric_limits< ValueType >::min() )
	, _max( std::numeric_limits< ValueType >::max() )
{
}

template< >
Range< float >::Range()
	: _min( -std::numeric_limits< float >::max() )
	, _max(  std::numeric_limits< float >::max() )
{
}

template< >
Range< double >::Range()
	: _min( -std::numeric_limits< double >::max() )
	, _max(  std::numeric_limits< double >::max() )
{
}

template< >
Range< long double >::Range()
	: _min( -std::numeric_limits< long double >::max() )
	, _max(  std::numeric_limits< long double >::max() )
{
}

template< typename ValueType >
void Range< ValueType >::setMinimum( const ValueType& min )
{
	if( min > _max )
		throw std::range_error( "The 'min' value must be less than the 'max' value" );
	_min = min;
}

template< typename ValueType >
void Range< ValueType >::setMaximum( const ValueType& max )
{
	if( max < _min )
		throw std::range_error( "The 'min' value must be less than the 'max' value" );
	_max = max;
}

template< typename ValueType >
void Range< ValueType >::setRange( const ValueType& min, const ValueType& max )
{
	if( min > max )
		throw std::range_error( "The 'min' value must be less than the 'max' value" );
	_min = min;
	_max = max;
}

template< typename ValueType >
void Range< ValueType >::setRange( const std::string& min, const std::string& max )
{
	ValueType minValue = fromString( min );
	ValueType maxValue = fromString( max );
	if( minValue > maxValue )
		throw std::range_error( "The 'min' value must be less than the 'max' value" );
	_min = minValue;
	_max = maxValue;
}

template< typename ValueType >
bool Range< ValueType >::isInRange( const ValueType& value )
{
	try
	{
		if( value < _min || value > _max )
			return false;
		return true;
	}
	catch( std::exception& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
	catch( ... )
	{
		LOG_ERROR( "Unknown error" );
		throw;
	}
}

template< typename ValueType >
ValueType Range< ValueType >::fromString( const std::string& value )
{
	try
	{
		ValueType ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "NumberTranslator::fromString: " << ret );
		return ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
char Range< char >::fromString( const std::string& value )
{
	try
	{
		short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "NumberTranslator::fromString: " << ret );
		if( ret > 127 || ret < -128 )
			throw std::range_error( "fromString: string cannot be converted to int8" );
		return (char) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
unsigned char Range< unsigned char >::fromString( const std::string& value )
{
	try
	{
		unsigned short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "NumberTranslator::fromString: " << ret );
		if( ret > 255 || ret < 0 )
			throw std::range_error( "fromString: string cannot be converted to uint8" );
		return (unsigned char) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

}
