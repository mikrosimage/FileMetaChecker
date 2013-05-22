#ifndef _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_
#define _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_

#include <common/global.hpp>

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

	void addRepetitionMapElement( std::string id, std::vector<size_t> repetition, bool optional )
	{
		_repetitionMap.insert( std::pair< std::string, std::vector<size_t> >( id, repetition ) );
		_iterationMap.insert ( std::pair< std::string, size_t >             ( id, 0          ) );
		_optionalMap.insert  ( std::pair< std::string, bool >               ( id, optional   ) );
	}

	std::vector<size_t> getRepetitionMapElement( std::string id )
	{
		return _repetitionMap[ id ];
	}

	void iterateIterationMapElement( std::string id )
	{
		_iterationMap[ id ] += 1;
	}

	bool isIterationValid( std::string id )
	{
		bool iterationIsValid = true;
		if( _iterationMap.count( id ) && _repetitionMap.count( id ) && _optionalMap.count( id ) )
		{	
			if( !_optionalMap.at( id ) && ( _iterationMap.at( id ) < _repetitionMap[ id ].at( 0 ) ) )
			{
				iterationIsValid = false;
				LOG_ERROR( "This element does not appear (or is not valid) enough times : " << id 
				           << " ( " << _iterationMap.at( id ) << "/" << _repetitionMap.at( id ).at( 0 ) << " )" );
			}
			if( _iterationMap.at( id ) > _repetitionMap.at( id ).at( 1 ) )
			{
				iterationIsValid = false;
				LOG_ERROR( "This element appears too much times : " << id << " ( " << _iterationMap.at(id) 
					       << "/" << _repetitionMap.at(id).at(1) << " )" );
			}
		}
		else
		{
			throw std::runtime_error( "Unknown element : " + id );
		}
		return iterationIsValid;
	}
	
private:
	size_t _size;
	bool   _ordered;
	std::map< std::string, std::vector<size_t> > _repetitionMap;
	std::map< std::string, size_t >              _iterationMap;
	std::map< std::string, bool >                _optionalMap;
};

#endif