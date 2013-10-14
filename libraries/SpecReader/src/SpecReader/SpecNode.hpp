#ifndef _SPEC_READER_SPECNODE_HPP_
#define _SPEC_READER_SPECNODE_HPP_

#include <Common/common.hpp>

#include <set>
#include <string>

namespace basic_element
{
	class Element;
}

namespace spec_reader
{

class SpecNode
{
public:
	SpecNode( basic_element::Element* p = NULL );
	
	SpecNode* next( basic_element::Element* parent ) const;
	
	void setId  ( const std::string& i ){ _id = i;   }
	void setType( const EType& t )      { _type = t; }
	
	std::string             getId( )    const { return _id; }
	size_t                  getIndex( ) const { return _index; }
	EType                   getType( )  const { return _type; }
	basic_element::Element* getParent() const { return _parent; }
	
	bool   isGroup()    const;
	bool   isOrdered()  const;
	bool   isOptional() const;
	size_t isRepeated() const;
		
	SpecNode* firstChild( basic_element::Element* e ) const;
	
	std::set< std::string > getChildNodes() const;
	
private:
	std::string             _id;
	size_t                  _index;
	EType                   _type;
	basic_element::Element* _parent;
	
	static size_t _globalIndex;
};

}

#endif
