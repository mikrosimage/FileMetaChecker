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

class Element
{
	struct Properties
	{
		std::string   id;
		std::string   label;
		
		size_t        uId;
		size_t        size;
		size_t        iteration;
		
		std::string   countExpr;
		std::string   requiredExpr;
		std::string   groupSizeExpr;
		
		std::vector< std::string > values;
		ExpressionList rangeExpr;
		ExpressionList repetExpr;
		std::map< std::string, std::string > map;

		EType         type;
		ESubType      subType;
		EDisplayType  displayType;
		
		EStatus       status;
		
		char*         data;
		
		bool          isGroup;
		bool          isOrdered;
		bool          isOptional;
		bool          bigEndianData;
		
		std::string   error;
		std::string   warning;
	};

public:
	Element( const std::shared_ptr< spec_reader::SpecNode > node, 
		     const std::shared_ptr< Element > previous = std::shared_ptr< Element >(),
		     const std::shared_ptr< Element > parent = nullptr );
	
	std::shared_ptr< spec_reader::SpecNode > next( );
	// std::shared_ptr< Element > next( );
	
	std::string getId()    const { return _prop.id; }
	std::string getLabel() const { return _prop.label; }

	size_t      getUniqueId()  const { return _prop.uId; }
	size_t      getSize()      const { return _prop.size; }
	size_t      getIteration() const { return _prop.iteration; }
	
	std::string getCount()       const { return _prop.countExpr; }
	std::string getRequirement() const { return _prop.requiredExpr; }
	std::string getGroupSize()   const { return _prop.groupSizeExpr; }

	std::vector< std::string >           getValues()      const { return _prop.values; }
	ExpressionList                       getRange()       const { return _prop.rangeExpr; }
	ExpressionList                       getRepetitions() const { return _prop.repetExpr; }
	std::map< std::string, std::string > getMap()         const { return _prop.map; }

	EType        getType()         const { return _prop.type; }
	ESubType     getSubType()      const { return _prop.subType; }
	EDisplayType getDisplayType()  const { return _prop.displayType; }
	EStatus      getStatus()       const { return _prop.status; }
	std::string  getStringStatus() const;
	
	char* getData() const;
	void  set( const char* data, const size_t& size );

	bool isGroup()     const { return _prop.isGroup; }
	bool isOrdered()   const { return _prop.isOrdered; }
	bool isOptional()  const { return _prop.isOptional; }
	bool isBigEndian() const { return _prop.bigEndianData; }

	std::shared_ptr< Element > getParent() const { return _parent.lock(); }

	void addErrorLabel( const std::string& error );
	void addWarningLabel( const std::string& warning );

	std::string getErrorLabel();
	std::string getWarningLabel();

	void setStatus( const EStatus status );

	std::shared_ptr< spec_reader::SpecNode > getSpecNode()
	{
		return _specNode.lock();
	}

protected:
	std::weak_ptr< Element > _parent;
	std::weak_ptr< Element > _previous;
	std::weak_ptr< spec_reader::SpecNode >  _specNode;
	// std::shared_ptr< spec_reader::SpecNode >  _specNode;
	std::vector< std::shared_ptr< Element > > _children;
	
	Properties _prop;
	bool       _checkedGroup;
	
};

}

#endif
