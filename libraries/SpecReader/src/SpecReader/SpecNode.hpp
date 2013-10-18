#ifndef _SPEC_READER_SPECNODE_HPP_
#define _SPEC_READER_SPECNODE_HPP_

#include <Common/common.hpp>

#include <set>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include <boost/property_tree/ptree.hpp>

namespace basic_element
{
	class Element;
}

namespace spec_reader
{

class SpecNode
{
public:
	SpecNode( const boost::property_tree::ptree::const_iterator node, 
		      std::shared_ptr< basic_element::Element > parent = std::shared_ptr< basic_element::Element >() );

	size_t getUId( ) const { return _uId; }

	std::string  getId()          const;
	std::string  getLabel()       const;
	EType        getType()        const;
	ESubType     getSubType()     const;
	EDisplayType getDisplayType() const;
	std::string  getCount()       const;
	std::string  getRequirement() const;
	std::string  getGroupSize()   const;
	
	bool   isGroup()     const;
	bool   isOrdered()   const;
	bool   isOptional()  const;
	bool   isBigEndian() const;
	size_t isRepeated()  const;

	std::vector< std::string >                           getValues()      const;
	std::vector< std::pair< std::string, std::string > > getRange()       const;
	std::vector< std::pair< std::string, std::string > > getRepetitions() const;
	std::map< std::string, std::string >                 getMap()         const;
	
	std::shared_ptr< basic_element::Element > getParent() const { return _parent; }
	SpecNode* next() const;
	SpecNode* next      ( std::shared_ptr< basic_element::Element > parent ) const;
	SpecNode* firstChild( std::shared_ptr< basic_element::Element > element ) const;
	
	size_t getChildrenNumber() const;
	std::set< std::string > getChildrenNodes() const;
	
private:
	std::string getProperty( const std::string& prop ) const;
	std::string getProperty( const std::string& prop, const std::string& defaultValue ) const;

private:
	size_t _uId;
	boost::property_tree::ptree::const_iterator _node;
	std::shared_ptr< basic_element::Element >   _parent;
	
	static size_t _globalIndex;
};

}

#endif
