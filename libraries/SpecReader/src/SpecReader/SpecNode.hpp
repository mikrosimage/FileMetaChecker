#ifndef _SPEC_READER_SPECNODE_HPP_
#define _SPEC_READER_SPECNODE_HPP_

#include <Common/common.hpp>
#include <rapidjson/document.h>

#include <set>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include <boost/property_tree/ptree.hpp>

namespace spec_reader
{

/**
 *  Define specification iterator
 */
typedef rapidjson::Value::ConstValueIterator TreeNodeIt;

class Specification;

/**
 *  Specification node, extracting information from the Specification tree.
 */
class SpecNode
{
	friend class Specification;

public:

	/**
	 * SpecNode's constructor.
	 * @param spec   Specification reference.
	 * @param node   Specification iterator.
	 * @param parent Parent SpecNode reference. Default value is nullptr.
	 */
	SpecNode( const Specification* spec,
		      const TreeNodeIt node,
		      const SpecNode* parent = nullptr );

	/**
	 * Get SpecNode unique ID.
	 * @return Unique ID.
	 */
	size_t getUId( ) const { return _uId; }

	/**
	 * Get SpecNode field value.
	 */
	std::string  getId()          const; /**< @return ID value           */
	std::string  getLabel()       const; /**< @return Label value        */
	EType        getType()        const; /**< @return Type value         */
	EDisplayType getDisplayType() const; /**< @return Display type value */
	std::string  getCount()       const; /**< @return Count value        */
	std::string  getRequirement() const; /**< @return Requirement value  */
	std::string  getGroupSize()   const; /**< @return Group Size value   */
	char         getEndChar()     const; /**< @return Ending character value */
	
	bool isGroup()         const; /**< @return true if the node contains a group, false otherwise. */
	bool isOrdered()       const; /**< @return true if the node contains an unordered group, false otherwise. */
	bool isOptional()      const; /**< @return true if the node is optional, false otherwise. */
	bool isBigEndian()     const; /**< @return true if the node's endianness is big-endian, false otherwise. */
	bool isCaseSensitive() const; /**< @return true if the node's value is case sensitive, false otherwise. */
	bool keepEndingChar()  const; /**< @return true if the node's ending char is kept, false otherwise. */
	bool isDetailed()      const; /**< @return true if the node is detailed, false otherwise. */

	std::vector< std::string >                           getValues()      const; /**< @return Vector of the node values */
	std::vector< std::pair< std::string, std::string > > getRange()       const; /**< @return Vector of the node value ranges */
	std::vector< std::pair< std::string, std::string > > getRepetitions() const; /**< @return Vector of the node repetition ranges  */
	std::map< std::string, std::string >                 getMap()         const; /**< @return Vector of the node value map  */
	
	/**
	 * Get current SpecNode's parent reference.
	 * @return Parent SpecNode reference.
	 */
	const SpecNode* getParent() const { return _parent; }

	/**
	 * Get next SpecNode reference.
	 * @return Next SpecNode reference if it exists, nullptr otherwise.
	 */
	std::shared_ptr< spec_reader::SpecNode > next      () const;

	/**
	 * Get SpecNode's first child reference.
	 * @return    First child SpecNode.
	 * @exception std::runtime_error if the current SpecNode has no group.
	 */
	std::shared_ptr< spec_reader::SpecNode > firstChild() const;
	
	/**
	 * Get SpecNode's children IDs list.
	 * @return    List of children IDs.
	 * @exception std::runtime_error if the current SpecNode has no group.
	 */
	std::set< std::string > getChildrenNodes() const;
	
protected:
	TreeNodeIt getIterator() const { return _node; }

private:
	size_t               _uId;
	TreeNodeIt           _node;
	const SpecNode*      _parent;
	const Specification* _specification;
	
	static size_t _globalIndex;
};

}

#endif
