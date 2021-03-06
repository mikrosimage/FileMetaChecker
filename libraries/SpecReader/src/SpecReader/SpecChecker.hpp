#ifndef _SPEC_READER_SPECCHECKER_HPP_
#define _SPEC_READER_SPECCHECKER_HPP_

#include <Common/common.hpp>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <iostream>

namespace spec_reader
{

/**
 *  Specification files checker.
 */
class SpecChecker
{
public:
	
	/**
	 * SpecChecker's constructor.
	 * @param filepath JSON file path.
	 */
	SpecChecker( const std::string& filepath );

	/**
	 * SpecChecker's destructor.
	 */
	~SpecChecker()
	{
	}

	/**
	 * Checks the JSON document, and links with JSON subfiles.
	 */
	bool check();

	/**
	 * Get the content of the specification document as a string.
	 */
	std::string getSpecString();

private:
	void checkSubFile  ( rapidjson::Value::Member* member );
	void includeExtNode( rapidjson::Value::Member* member, rapidjson::Value::ConstValueIterator subNode );

private:
	const std::string _filepath;
	rapidjson::Document* _document;
};

}

#endif
