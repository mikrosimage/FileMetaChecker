#ifndef _SPEC_READER_SPECNODE_HPP_
#define _SPEC_READER_SPECNODE_HPP_

#include <Common/common.hpp>

#include <set>
#include <string>
#include <memory>

namespace basic_element
{
	class Element;
}

namespace spec_reader
{

class SpecNode
{
public:
	SpecNode( std::shared_ptr< basic_element::Element > p = std::shared_ptr< basic_element::Element >() );
	
	SpecNode* next( std::shared_ptr< basic_element::Element > parent ) const;
	
	void setId  ( const std::string& i ){ _id = i;   }
	void setType( const EType& t )      { _type = t; }
	
	std::string             getId( )    const { return _id; }
	size_t                  getIndex( ) const { return _index; }
	EType                   getType( )  const { return _type; }
	std::shared_ptr< basic_element::Element > getParent() const { return _parent; }
	
	bool   isGroup()    const;
	bool   isOrdered()  const;
	bool   isOptional() const;
	size_t isRepeated() const;
		
	SpecNode* firstChild( std::shared_ptr< basic_element::Element > e ) const;
	
	std::set< std::string > getChildNodes() const;
	
private:
	std::string             _id;
	size_t                  _index;
	EType                   _type;
	std::shared_ptr< basic_element::Element > _parent;
	
	static size_t _globalIndex;
};

}

#endif
