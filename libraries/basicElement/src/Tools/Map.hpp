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
		_map = map;
	}

	void addPair( const KeyType& key, const std::string& label )
	{
		_map.insert( std::pair< KeyType, std::string >( key, label ) );
	}

	void insertPair( const std::pair< KeyType, std::string >& pair )
	{
		_map.insert( pair );
	}

	std::string getLabel( const KeyType& key )
	{
		std::string label = _map[ key ];
		if( label == "" )
			_map.erase( key );
		return label;
	}

	size_t getSize()
	{
		return _map.size();
	}


private:
	std::map< KeyType, std::string > _map;
};

}

#endif
