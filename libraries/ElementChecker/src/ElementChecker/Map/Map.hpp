#ifndef _ELEMENT_CHECKER_MAP_MAP_HPP_
#define _ELEMENT_CHECKER_MAP_MAP_HPP_

namespace element_checker
{

template< typename KeyType >
class Map
{
public:
	Map( const std::map< std::string, std::string >& map );
	~Map()
	{
	}

	std::string getLabel( const KeyType& key );

	size_t getSize();

protected:
	void setMap( const std::map< std::string, std::string >& map );
	
	KeyType fromString( const std::string& value );

private:
	std::map< KeyType, std::string > _map;
};

}

#include "Map.tcc"

#endif
