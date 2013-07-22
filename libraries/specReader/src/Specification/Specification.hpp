#ifndef _SPECREADER_SPECIFICATION_HPP_
#define _SPECREADER_SPECIFICATION_HPP_


#include <SpecNode/SpecNode.hpp>

#include <boost/property_tree/json_parser.hpp>

class Specification
{
public:
	Specification();
	~Specification();

	void setFromTree  ( const bpt::ptree& spec );
	bool setFromFile  ( const std::string& filepath );
	void setFromString( const std::string& string );

	std::string getId();
	std::string getLabel();
	std::string getType();
	std::vector< std::string > getSupportedExtensions();

	SpecNode getFirstNode();
	// bpt::ptree::const_iterator getLastChild( );		// @todo

private:
	bpt::ptree::const_iterator getBody( );

private:
	bpt::ptree _specTree;
};

#endif
