#ifndef _BASIC_ELEMENT_TOOLS_MAP_HPP_
#define _BASIC_ELEMENT_TOOLS_MAP_HPP_

#include <common.hpp>


namespace basic_element
{

template< typename KeyType >
class Map
{
public:
	Map()
	{
	}

	Map( const std::map< KeyType, std::string >& map )
		: _map( map )
	{
	}

	~Map()
	{
	}

	void setMap( const std::map< KeyType, std::string >& map )
	{
		BE_LOG_TRACE( " => Map: \tSET MAP " );
		_map = map;
	}

	void addPair( const KeyType& key, const std::string& label )
	{
		BE_LOG_TRACE( " => Map: \tADD PAIR " );
		_map.insert( std::pair< KeyType, std::string >( key, label ) );
	}

	void insertPair( const std::pair< KeyType, std::string >& pair )
	{
		BE_LOG_TRACE( " => Map: \tINSERT PAIR " );
		_map.insert( pair );
	}

	std::string getLabel( const KeyType& key )
	{
		BE_LOG_TRACE( " => Map: \tGET LABEL " );
		std::string label = _map[ key ];
		if( label == "" )
			_map.erase( key );
		return label;
	}

	size_t getSize()
	{
		BE_LOG_TRACE( " => Map: \tGET SIZE " );
		return _map.size();
	}


private:
	std::map< KeyType, std::string > _map;
};

}

#endif
