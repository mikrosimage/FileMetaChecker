#ifndef _SPEC_READER_SPECIFICATION_HPP_
#define _SPEC_READER_SPECIFICATION_HPP_

#include "SpecNode.hpp"

namespace spec_reader
{

class Specification
{
public:
	Specification();
	~Specification();

	void setFromTree  ( const boost::property_tree::ptree&  spec );
	bool setFromFile  ( const std::string& filepath );
	void setFromString( const std::string& string );

	std::string getId();
	std::string getLabel();
	std::string getType();
	std::vector< std::string > getSupportedExtensions();

	SpecNode* getFirstNode();
	boost::property_tree::ptree::const_iterator end() const;

private:
	boost::property_tree::ptree _specTree;
};

}

#endif
