#ifndef _SPEC_READER_SPECCHECKER_HPP_
#define _SPEC_READER_SPECCHECKER_HPP_

#include <Common/common.hpp>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <iostream>

namespace spec_reader
{

class SpecChecker
{
public:
	SpecChecker( rapidjson::Document* document, const std::string& filepath );
	~SpecChecker()
	{
	}

	void check();

private:
	void checkSubFile  ( rapidjson::Value::Member* member );
	void includeExtNode( rapidjson::Value::Member* member, rapidjson::Value::ConstValueIterator subNode );

private:
	const std::string _filepath;
	rapidjson::Document* _document;
};

}

#endif
