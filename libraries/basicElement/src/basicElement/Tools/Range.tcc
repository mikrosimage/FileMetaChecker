
namespace basic_element
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

}
