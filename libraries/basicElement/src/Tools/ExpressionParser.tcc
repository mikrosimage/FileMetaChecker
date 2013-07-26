
#include <boost/foreach.hpp>
#include <cstdlib>

namespace basic_element
{
namespace expression_parser
{

ExpressionParser::ExpressionParser() 
{
	Py_Initialize();
	_mainModule = bpy::import( "__main__" );
	_mainNamespace = _mainModule.attr( "__dict__" );
}

template< typename NumberType >
ExpressionParser::ExpressionParser( const std::map < std::string, NumberType >& varMap ) 
{
	Py_Initialize();
	_mainModule = bpy::import( "__main__" );
	_mainNamespace = _mainModule.attr( "__dict__" );

	setVariables( varMap );
}

ExpressionParser::~ExpressionParser() 
{
	Py_Finalize();
}


template< typename NumberType >
void ExpressionParser::setVariables( const std::map < std::string, NumberType >& varMap )
{
	typedef std::pair< std::string, NumberType > VarType;
	BOOST_FOREACH( VarType var, varMap )
	{
		std::ostringstream oss;
		oss << var.first << " = " << var.second << std::endl;
		_contextString += oss.str();
	}
}

template< typename Type >
Type ExpressionParser::getExpressionResult( const std::string& expression )
{
	Type result = 0;
	try
	{
		LOG_TRACE( _contextString.c_str() );
		LOG_TRACE( expression );
		bpy::exec( _contextString.c_str(), _mainNamespace );
		bpy::object returnText = bpy::eval( expression.c_str(), _mainNamespace );
		result = bpy::extract< Type >( returnText );
	}
	catch( const bpy::error_already_set& error )
	{
		printPythonError( error );
		throw;
	}
	return result;
}

template< >
std::string ExpressionParser::getExpressionResult<std::string>( const std::string& expression )
{
	std::string result = "";
	try
	{
		bpy::exec( _contextString.c_str(), _mainNamespace );
		bpy::object returnText = bpy::eval( expression.c_str(), _mainNamespace );
		result = bpy::extract< std::string >( returnText );
	}
	catch( const bpy::error_already_set& error )
	{
		printPythonError( error );
		throw;
	}
	return result;
}

void ExpressionParser::addPythonHeader( const std::string& pythonString )
{
	_contextString.insert( 0, pythonString + "\n" );
}

std::string ExpressionParser::getContextString()
{
	return _contextString;
}


void ExpressionParser::printPythonError( const bpy::error_already_set& exception )
{
	PyObject *type, *value, *traceback;
	PyErr_Fetch( &type, &value, &traceback );
	
	std::string message = "Python error: ";
	
	if( type )
	{
		type = PyObject_Str( type );
		message += PyString_AsString( type );
	}
	if( value )
	{
		value = PyObject_Str( value );
		message += ": ";
		message += "\n\t=> ";
		message += PyString_AsString( value );
	}

	Py_XDECREF( type );
	Py_XDECREF( value );
	Py_XDECREF( traceback );
	LOG_ERROR( message );
}

}
}
