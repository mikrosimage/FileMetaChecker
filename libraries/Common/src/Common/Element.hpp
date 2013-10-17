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
public:
	Element( const spec_reader::SpecNode* node, const std::shared_ptr< Element > prev );
	
	const spec_reader::SpecNode* next( );
	
	std::string getId()        const { return _id; }
	EType       getType()      const { return _type; }
	EStatus     getStatus()          { return _status; }
	size_t      getIndex()     const { return _index; }
	size_t      getIteration() const { return _iteration; }
	
	std::string getStringStatus() const;
	
	std::weak_ptr< Element > getParent() const { return _parent; }
	size_t getChildrenNumber() const;
	
	virtual void check() = 0;

private:
	std::string _id;
	EType       _type;
	char*       _data;
	bool        _checkedGroup;
	
protected:
	size_t      _index;
	size_t      _iteration;
	EStatus     _status;
	
	std::weak_ptr< Element >                  _parent;
	const std::weak_ptr< Element >            _previous;
	const spec_reader::SpecNode*              _specNode;
	std::vector< std::shared_ptr< Element > > _children;
};

}

#endif
