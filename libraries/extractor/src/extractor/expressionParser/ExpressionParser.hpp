#ifndef _EXTRACTOR_EXPRESSION_PARSER_EXPRESSION_PARSER_HPP_
#define _EXTRACTOR_EXPRESSION_PARSER_EXPRESSION_PARSER_HPP_

#include <map>
#include <string>
#include <sstream>

#include <boost/python.hpp>
#include <Python.h>

typedef std::map<std::string, unsigned int> ElementsMap;
typedef std::pair<std::string, unsigned int> ElementsPair;

class ExpressionParser
{
public:
	ExpressionParser();
	~ExpressionParser();

	void setVariables( const ElementsMap& headerElements );

	template< typename Type >
	Type parseExpression( const std::string& expression );

	void addPythonHeader( const std::string& pythonString );
	std::string getPythonHeader();

private:
	boost::python::object _main_module;
	boost::python::object _main_namespace;
	std::string _contextString;
};

#include "ExpressionParser.tcc"

#endif