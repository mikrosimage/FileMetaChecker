#ifndef _SPEC_READER_SPECIFICATION_HPP_
#define _SPEC_READER_SPECIFICATION_HPP_

#include <memory>

#include "SpecNode.hpp"

#include <iostream>
#include <vector>

#include <rapidjson/document.h>

namespace spec_reader
{

class Specification
{
public:
	Specification();
	~Specification();

	void setFromString( const std::string& string );
	bool setFromFile( const std::string& filepath );

	std::string getId();
	std::string getLabel();
	std::string getType();
	std::vector< std::string > getSupportedExtensions();

	std::shared_ptr< SpecNode > getFirstNode();
	rapidjson::Value::ConstValueIterator end() const;

protected:
	std::string getSpecInfo( const std::string& key );

private:
	rapidjson::Document _specDoc;
};

}

#endif
