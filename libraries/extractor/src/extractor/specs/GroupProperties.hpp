#ifndef _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_
#define _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_

#include <iostream>
#include <vector>
#include <map>

class GroupProperties
{
public:
	GroupProperties()
	{
		_size = 0;
		_ordered = true;
	}

	void addSize( int value )
	{
		_size += value;
	}

	size_t getSize()
	{
		return _size;
	}

	void setOrder( bool order )
	{
		_ordered = order;
	}

	bool getOrder()
	{
		return _ordered;
	}

	void addRepetitionMapElement( std::string id, std::vector<size_t> repetition )
	{
		_repetitionMap.insert( std::pair< std::string, std::vector<size_t> >( id, repetition ) );
		_iterationMap.insert ( std::pair< std::string, size_t              >( id, 0          ) );
	}

	std::vector<size_t> getRepetitionMapElement( std::string id )
	{
		return _repetitionMap[ id ];
	}

	void iterateIterationMapElement( std::string id )
	{
		_iterationMap[ id ] += 1;
	}

private:
	size_t _size;
	bool   _ordered;
	std::map< std::string, std::vector<size_t> > _repetitionMap;
	std::map< std::string, size_t >              _iterationMap;
};

#endif