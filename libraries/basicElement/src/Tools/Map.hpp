#ifndef _BASIC_ELEMENT_TOOLS_MAP_HPP_
#define _BASIC_ELEMENT_TOOLS_MAP_HPP_

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
		LOG_INFO( " => Map: \tSET MAP " );
		_map = map;
	}

	void addPair( const KeyType& key, const std::string& label )
	{
		LOG_INFO( " => Map: \tADD PAIR " );
		_map.insert( std::pair< KeyType, std::string >( key, label ) );
	}

	std::string getLabel( const KeyType& key )
	{
		LOG_INFO( " => Map: \tGET LABEL " );
		std::string label = _map[ key ];
		if( label == "" )
			_map.erase( key );
		return label;
	}

	size_t getSize()
	{
		LOG_INFO( " => Map: \tGET SIZE " );
		return _map.size();
	}


private:
	std::map< KeyType, std::string > _map;
};

}

#endif
