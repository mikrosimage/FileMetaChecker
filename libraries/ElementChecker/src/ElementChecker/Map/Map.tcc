
namespace element_checker
{

template< typename KeyType >
Map< KeyType >::Map( const std::map< std::string, std::string >& map )
{
	setMap( map );
}

template< typename KeyType >
std::string Map< KeyType >::getLabel( const KeyType& key )
{
	std::string label = _map[ key ];
	if( label == "" )
		_map.erase( key );
	return label;
}

template< typename KeyType >
size_t Map< KeyType >::getSize()
{
	return _map.size();
}

template< typename KeyType >
void Map< KeyType >::setMap( const std::map< std::string, std::string >& map )
{
	try
	{
		for( std::pair< std::string, std::string > pair : map )
			_map.emplace( fromString( pair.first ), pair.second );

		// for( std::pair< KeyType, std::string > pair : _map )
		// 	LOG_TRACE( "Map::setMap: " << pair.first << " | " << pair.second );

		if( map.size() != _map.size() )
			throw std::runtime_error( "Map::setMap: cannot set the entire map" );
	}
	catch( std::runtime_error e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< typename KeyType >
KeyType Map< KeyType >::fromString( const std::string& value )
{
	try
	{
		KeyType ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "Map::fromString: " << ret );
		return ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
char Map< char >::fromString( const std::string& value )
{
	try
	{
		short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "Map::fromString: " << ret );
		if( ret > 127 || ret < -128 )
			throw std::range_error( "Map::fromString: string cannot be converted to int8" );
		return (char) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
unsigned char Map< unsigned char >::fromString( const std::string& value )
{
	try
	{
		unsigned short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "Map::fromString: " << ret );
		if( ret > 255 || ret < 0 )
			throw std::range_error( "Map::fromString: string cannot be converted to uint8" );
		return (unsigned char) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

}
