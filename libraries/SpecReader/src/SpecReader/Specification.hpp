#ifndef _SPEC_READER_SPECIFICATION_HPP_
#define _SPEC_READER_SPECIFICATION_HPP_

#include <memory>

#include "SpecNode.hpp"

#include <iostream>
#include <vector>

#include <rapidjson/document.h>

namespace spec_reader
{

/**
 *  Specification tree, containing the format description.
 */
class Specification
{
public:

	/**
	 * Specification's constructor.
	 */
	Specification();

	/**
	 * Specification's destructor.
	 */
	~Specification();

	/**
	 * Set specification document from JSON string.
	 * @param[in] jsonString JSON tree string.
	 */
	void setFromString( const std::string& jsonString );

	/**
	 * Set specification document from JSON file.
	 * @param[in] filepath JSON file path.
	 * @return             Returns true if the document has been correctly set, false otherwise.
	 */
	bool setFromFile( const std::string& filepath );

	/**
	 * Set specification document from a tree node.
	 * @param node Node reference.
	 * @param key  Root member name.
	 * @return     Returns true if the document has been correctly set, false otherwise.
	 */
	bool setFromNode( std::shared_ptr< SpecNode > node, const std::string& key );

	/**
	 * Get format specification ID.
	 * @return    format ID.
	 */
	std::string getId();

	/**
	 * Get format specification full name.
	 * @return    format label.
	 */
	std::string getLabel();

	/**
	 * Get format specification type.
	 * @return    format type.
	 */
	std::string getType();

	/**
	 * Get format specification supported extensions.
	 * @return    Vector of supported extensions strings.
	 */
	std::vector< std::string > getSupportedExtensions();

	/**
	 * Get a reference to the first node of the specification.
	 * @return Reference to the first SpecNode of the format description.
	 */
	std::shared_ptr< SpecNode > getFirstNode();


	/**
	 * Get the specification end iterator.
	 * @return Format description end iterator.
	 */
	rapidjson::Value::ConstValueIterator end() const;

protected:
	std::string getSpecInfo( const std::string& key );
	void includeExtNode( rapidjson::Value::Member* member, rapidjson::Value::ConstValueIterator node );

private:
	rapidjson::Document _specDoc;
	std::string         _rootKey;
};

}

#endif
