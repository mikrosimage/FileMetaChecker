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
		if( _max.isSet && min > _max.value )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
		_min.value = min;
		_min.isSet = true;
	}

	void setMaximum( const ValueType& max )
	{
		if( _min.isSet && max < _min.value )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
		_max.value = max;
		_max.isSet = true;
	}

	void setRange( const ValueType& min, const ValueType& max )
	{
		if( min > max )
			throw std::range_error( "The 'min' value must be less than the 'max' value" );
		_min.value = min;
		_max.value = max;
		_min.isSet = true;
		_max.isSet = true;
	}

	bool isInRange( const ValueType& value )
	{
		try
		{
			if( !isSet() )
				throw std::range_error( "Range not set" );

			if( ( _min.isSet && _min.value >  value ) ||
				( _max.isSet && _max.value <  value ) )
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

	bool isSet()
	{
		return ( _min.isSet || _max.isSet );
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
	try
	{
		if( !isSet() )
			throw std::range_error( "Range not set" );

		size_t intValue = hexaToUint( value );

		if( ( _min.isSet && intValue < hexaToUint( _min.value ) ) ||
			( _max.isSet && intValue > hexaToUint( _max.value ) ) )
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


}

#endif
