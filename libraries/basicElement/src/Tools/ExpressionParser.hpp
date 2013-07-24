#ifndef _BASIC_ELEMENT_TOOLS_EXPRESSION_PARSER_HPP_
#define _BASIC_ELEMENT_TOOLS_EXPRESSION_PARSER_HPP_

#include <map>

#include <boost/python.hpp>
#include <Python.h>

namespace bpy = boost::python;

namespace basic_element
{
namespace expression_parser
{

class ExpressionParser
{
public:
	ExpressionParser();

	template< typename NumberType >
	ExpressionParser( const std::map < std::string, NumberType >& varMap );
	
	~ExpressionParser();

	template< typename NumberType >
	void setVariables( const std::map < std::string, NumberType >& varMap );

	template< typename Type >
	Type getExpressionResult( const std::string& expression );

	void addPythonHeader( const std::string& pythonString );
	
	std::string getContextString();

protected:
	void printPythonError( const bpy::error_already_set& exception );

private:
	bpy::object _mainModule;
	bpy::object _mainNamespace;
	std::string _contextString;
};
	
}
}

#include "ExpressionParser.tcc"

#endif
