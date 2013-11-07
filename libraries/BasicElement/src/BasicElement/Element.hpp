#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <Common/common.hpp>

#include <set>
#include <string>
#include <vector>
#include <memory>

namespace spec_reader
{
	class SpecNode;
}

namespace basic_element
{

typedef std::vector< std::pair< std::string, std::string > > ExpressionList;

typedef          char       int8;
typedef unsigned char      uint8;
typedef          short      int16;
typedef unsigned short     uint16;
typedef          int        int32;
typedef unsigned int       uint32;
typedef   signed long long  int64;
typedef unsigned long long uint64;

typedef long double  ieeeExtended;    // 80 bits IEEE Standard 754 floating point

class Element
{

public:
	Element( const std::shared_ptr< spec_reader::SpecNode > node, 
		     const std::shared_ptr< Element > previous = std::shared_ptr< Element >(),
		     const std::shared_ptr< Element > parent = nullptr );
	
	std::shared_ptr< spec_reader::SpecNode > next( );

	void  set( const char* data, const size_t& size );

	std::shared_ptr< Element >               getParent()   const { return _parent.lock(); }
	std::shared_ptr< Element >               getPrevious() const { return _previous.lock(); }
	std::shared_ptr< spec_reader::SpecNode > getSpecNode()       { return _specNode; }

protected:
	void initSize();

protected:
	std::weak_ptr< Element > _parent;
	std::weak_ptr< Element > _previous;
	std::shared_ptr< spec_reader::SpecNode >  _specNode;
	std::vector< std::shared_ptr< Element > > _children;

public:
	std::string   _id;
	std::string   _label;
	
	size_t        _uId;
	size_t        _size;
	size_t        _iteration;
	
	std::string   _countExpr;
	std::string   _requiredExpr;
	std::string   _groupSizeExpr;
	
	std::vector< std::string >           _values;
	std::map< std::string, std::string > _map;

	std::string   _mapValue;
	std::string   _dispValue;
	
	ExpressionList _rangeExpr;
	ExpressionList _repetExpr;

	EType         _type;
	EDisplayType  _displayType;
	
	EStatus       _status;
	
	std::string   _error;
	std::string   _warning;
	
	bool          _isGroup;
	bool          _isOrdered;
	bool          _isOptional;
	bool          _isBigEndian;
	bool          _checkedGroup;

	char*         _data;
		
};

}

#endif
