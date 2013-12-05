#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <Common/common.hpp>

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

namespace spec_reader
{
	class SpecNode;
}

namespace basic_element
{

/**
 * Define number types:
 */
typedef          char       int8;     /**<   Signed 1-byte  integer */
typedef unsigned char      uint8;     /**< Unsigned 1-byte  integer */
typedef          short      int16;    /**<   Signed 2-bytes integer */
typedef unsigned short     uint16;    /**< Unsigned 2-bytes integer */
typedef          int        int32;    /**<   Signed 4-bytes integer */
typedef unsigned int       uint32;    /**< Unsigned 4-bytes integer */
typedef   signed long long  int64;    /**<   Signed 8-bytes integer */
typedef unsigned long long uint64;    /**< Unsigned 8-bytes integer */
typedef long double  ieeeExtended;    /**< 10 bytes IEEE Standard 754 floating point */

/**
 *  Basic element, storing all the information of a file segment.
 */
class Element
{
	typedef std::vector< std::pair< std::string, std::string > > ExpressionList;
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	typedef std::shared_ptr< spec_reader::SpecNode  > ShPtrSpecNode;
public:
	
	/**
	 * Element's constructor.
	 * @param node     Bound SpecNode reference.
	 * @param previous Previous Element reference.
	 * @param parent   Parent Element reference.
	 */
	Element( const ShPtrSpecNode node, const ShPtrElement previous = ShPtrElement(), const ShPtrElement parent = nullptr );
	
	/**
	 * Get the next specification node.
	 * @return Next SpecNode reference.
	 */
	ShPtrSpecNode next( );

	/**
	 * Set raw data from buffer.
	 * @param[in] data Data buffer to copy from.
	 * @param[in] size Buffer size (in bytes).
	 */
	void  set( const char* data, const size_t& size );

	/**
	 * Get the current Element parent.
	 * @return Parent Element reference.
	 */
	ShPtrElement  getParent()   const { return _parent.lock(); }

	/**
	 * Get the previous Element.
	 * @return Previous Element reference.
	 */
	ShPtrElement  getPrevious() const { return _previous.lock(); }

	/**
	 * Get the bound specification node.
	 * @return Bound SpecNode reference.
	 */
	ShPtrSpecNode getSpecNode()       { return _specNode; }

	/**
	 * Get the current Element children.
	 * @return Vector of children references.
	 */
	std::vector< ShPtrElement > getChildren() { return _children; }

	std::string getPropertiesFlag();
protected:
	static size_t getElementSize( const std::string& id, const EType type, const std::vector<std::string>& values );
	static size_t getElementIteration( const std::string& id, const ExpressionList& repetExpr, const ShPtrElement& previous, const ShPtrElement& parent );

protected:
	std::weak_ptr< Element >    _parent;
	std::weak_ptr< Element >    _previous;
	ShPtrSpecNode               _specNode;
	std::vector< ShPtrElement > _children;

public:
	const std::string   _id;
	const std::string   _label;
	
	const size_t        _uId;
	size_t              _size;
	const size_t        _iteration;
	size_t              _specGroupSize;
	size_t              _childrenSize;
	
	const std::string   _countExpr;
	const std::string   _requiredExpr;
	const std::string   _groupSizeExpr;
	
	const std::vector< std::string >           _values;
	const std::map< std::string, std::string > _map;

	std::string   _mapValue;
	std::string   _dispValue;
	
	const ExpressionList _rangeExpr;
	const ExpressionList _repetExpr;

	const EType         _type;
	const EDisplayType  _displayType;
	
	EStatus       _status;
	
	std::string   _error;
	std::string   _warning;
	
	const bool    _isGroup;
	const bool    _isOrdered;
	const bool    _isOptional;
	const bool    _isBigEndian;
	bool          _checkedGroup;

	char*         _data;

};

static std::string getPropertiesLegend()
{
	std::stringstream legend;
	legend << "========= LEGEND =========" << std::endl;
	legend << " v = value                " << std::endl;
	legend << " i = iteration            " << std::endl;
	legend << " s = status               " << std::endl;
	legend << " E = Error                " << std::endl;
	legend << " W = Warning              " << std::endl;
	legend                                 << std::endl;
	legend << "=== ELEMENT PROPERTIES ===" << std::endl;
	legend << "B.... = Big endian        " << std::endl;
	legend << "l.... = little endian     " << std::endl;
	legend << ".O... = Optional          " << std::endl;
	legend << "..G.. = Group             " << std::endl;
	legend << "...U. = Unordered         " << std::endl;
	legend << "....R = Repeated          " << std::endl;
	legend                                 << std::endl;
	return legend.str();
}

}

#endif
