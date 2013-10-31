#ifndef _ELEMENT_CHECKER_EXPRESSION_PARSER_HPP_
#define _ELEMENT_CHECKER_EXPRESSION_PARSER_HPP_

#include <map>
#include <BasicElement/Element.hpp>

#include <boost/python.hpp>
#include <Python.h>

namespace bpy = boost::python;

namespace element_checker
{

class ExpressionParser
{
public:
	ExpressionParser();

	ExpressionParser( const std::vector< std::shared_ptr< basic_element::Element > >& elementList );

	~ExpressionParser();

	void setVariables( const std::vector< std::shared_ptr< basic_element::Element > >& elementList );

	template< typename ResultType >
	ResultType getExpressionResult( const std::string& expression );

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

#include "ExpressionParser.tcc"

#endif
