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
	 */
	void  set( const std::vector< char >& data );

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
	 * Add a child to the current Element.
	 * @param[in] element Child reference.
	 */
	void addChild( const ShPtrElement& element );

	/**
	 * Get the current Element children.
	 * @return Vector of children references.
	 */
	std::vector< ShPtrElement > getChildren() { return _children; }

	/**
	 * Get flags for the Element properties.
	 * @return Properties flags.
	 */
	std::string getPropertiesFlag();
protected:
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
	
	const size_t  _iteration;

	EStatus       _status;
	
	std::vector< std::string > _error;
	std::vector< std::string > _warning;
	
	std::vector< char > _data;

	const char    _endChar;

	const bool    _isGroup;
	const bool    _isOrdered;
	const bool    _isOptional;
	const bool    _isBigEndian;
	const bool    _isCaseSensitive;
	const bool    _keepEndingChar;
	bool          _checkedGroup;

	static std::string getLabelsLegend()
	{
		std::stringstream legend;
		legend << "========= LEGEND =========" << std::endl;
		legend << " v = value                " << std::endl;
		legend << " t = type                 " << std::endl;
		legend << " i = iteration            " << std::endl;
		legend << " S = status               " << std::endl;
		legend << " s = size                 " << std::endl;
		legend << " @ = address              " << std::endl;
		legend << " E = Error                " << std::endl;
		legend << " W = Warning              " << std::endl;
		legend                                 << std::endl;
		return legend.str();
	}

	static std::string getElementPropertiesLegend()
	{
		std::stringstream props;
		props << "=== ELEMENT PROPERTIES ===" << std::endl;
		props << "B.... = Big endian        " << std::endl;
		props << "l.... = little endian     " << std::endl;
		props << ".O... = Optional          " << std::endl;
		props << "..G.. = Group             " << std::endl;
		props << "...U. = Unordered         " << std::endl;
		props << "....R = Repeated          " << std::endl;
		props                                 << std::endl;
		return props.str();
	}

};

}

#endif
