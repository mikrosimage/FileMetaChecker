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

class Element
{
	struct Properties
	{
		std::string   id;
		std::string   label;
		std::string   error;
		std::string   warning;
		
		size_t        uId;
		size_t        size;
		size_t        iteration;
		
		std::string   countExpr;
		std::string   groupSizeExpr;
		std::string   requiredExpr;
		
		std::vector< std::pair< std::string, std::string > > repetExpr;

		EType         type;
		ESubType      subType;
		EDisplayType  displayType;
		
		EStatus       status;
		
		char*         data;
		
		bool          bigEndianData;
		bool          isOptional;
		bool          isOrdered;
		bool          hasGroup;
	};

public:
	Element( const spec_reader::SpecNode* node, const std::shared_ptr< Element > previous = std::shared_ptr< Element >() );
	
	const spec_reader::SpecNode* next( );
	
	std::string getId()        const { return _prop.id; }
	EType       getType()      const { return _prop.type; }
	EStatus     getStatus()          { return _prop.status; }
	size_t      getIndex()     const { return _prop.uId; }
	size_t      getIteration() const { return _prop.iteration; }
	
	std::string getStringStatus() const;
	
	std::weak_ptr< Element > getParent() const { return _parent; }
	size_t getChildrenNumber() const;

	void getEndianOrderedData( char* buffer, const char* data ) const;

	char* get() const;
	
	virtual void check() = 0;

protected:
	std::weak_ptr< Element >                  _parent;
	const std::weak_ptr< Element >            _previous;
	const spec_reader::SpecNode*              _specNode;
	std::vector< std::shared_ptr< Element > > _children;
	
	Properties _prop;
	bool       _checkedGroup;
	
};

}

#endif
