#ifndef _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_
#define _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_

#include <iostream>

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

private:
	size_t _size;
	bool   _ordered;
};

#endif