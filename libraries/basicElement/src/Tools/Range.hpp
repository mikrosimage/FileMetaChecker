#ifndef _BASIC_ELEMENT_TOOLS_RANGE_HPP_
#define _BASIC_ELEMENT_TOOLS_RANGE_HPP_

#include <common.hpp>

namespace basic_element
{

template< typename ValueType >
class Range
{
	struct Bound
	{
		ValueType value;
		bool      isSet;
	};
public:
	Range()
	{
		_min.isSet = false;
		_max.isSet = false;
	}

	Range( const ValueType& min, const ValueType& max )
	{
		_min.isSet = false;
		_max.isSet = false;
		setRange( min, max );
	}

	~Range()
	{
	}

	void setMinimum( const ValueType& min )
	{
		BE_LOG_TRACE( " => Range: \tSET MINIMUM " );
		if( _max.isSet && min > _max.value )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
		_min.value = min;
		_min.isSet = true;
	}

	void setMaximum( const ValueType& max )
	{
		BE_LOG_TRACE( " => Range: \tSET MAXIMUM " );
		if( _min.isSet && max < _min.value )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
		_max.value = max;
		_max.isSet = true;
	}

	void setRange( const ValueType& min, const ValueType& max )
	{
		BE_LOG_TRACE( " => Range: \tSET RANGE " );
		if( min > max )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
		_min.value = min;
		_min.isSet = true;
		_max.value = max;
		_max.isSet = true;
	}

	bool isInRange( const ValueType& value )
	{
		BE_LOG_TRACE( " => Range: \tIS IN RANGE " );
		try
		{
			if( !isSet() )
				throw std::range_error( "Range not set" );

			bool inRange = true;
			if( _min.isSet && _min.value >  value )
				inRange = false;
			if( _max.isSet && _max.value <  value )
				inRange = false;
			return inRange;
		}
		catch( std::range_error& e )
		{
			LOG_ERROR( e.what() );
			throw;
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

	bool isSet()
	{
		BE_LOG_TRACE( " => Range: \tIS SET " );
		if( _min.isSet || _max.isSet )
			return true;
		return false;
	}

private:
	size_t hexaToUint( const std::string& hexa )
	{
		size_t value;
		std::stringstream sstr( hexa );
		sstr >> std::hex >> value;
		BE_LOG_TRACE( " \t hexa   : " << hexa << " ==> value: " << value );
		return value;
	}

private:
	Bound _min;
	Bound _max;
};

// for Hexa values :
template<>
void Range< std::string >::setRange( const std::string& min, const std::string& max )
{
	BE_LOG_TRACE( " => Range: \tSET RANGE (string) " );
	if( hexaToUint( min ) > hexaToUint( max ) )
		throw std::range_error( "The 'min' value must be less than the 'max' value" );
	_min.value = min;
	_min.isSet = true;
	_max.value = max;
	_max.isSet = true;
}

template<>
bool Range< std::string >::isInRange( const std::string& value )
{
	BE_LOG_TRACE( " => Range: \tIS IN RANGE (string) " );
	try
	{
		if( !isSet() )
			throw std::range_error( "Range not set" );

		size_t intValue = hexaToUint( value );
		bool inRange = true;

		if( _min.isSet )
		{
			size_t minValue = hexaToUint( _min.value );
			if( minValue > intValue )
				inRange = false;
		}
		if( _max.isSet )
		{
			size_t maxValue = hexaToUint( _max.value );
			if( maxValue < intValue )
				inRange = false;
		}

		return inRange;
	}
	catch( std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
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


}

#endif
