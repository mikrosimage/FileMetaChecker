#include <common/global.hpp>
#include <boost/foreach.hpp>
#include <cstdlib>



ExpressionParser::ExpressionParser() 
{
	Py_Initialize();
	_main_module = boost::python::import( "__main__" );
	_main_namespace = _main_module.attr( "__dict__" );
}

ExpressionParser::~ExpressionParser() 
{
	Py_Finalize();
}

void ExpressionParser::setVariables( const ElementsMap& headerElements )
{
	BOOST_FOREACH( ElementsPair pair, headerElements )
	{
		std::ostringstream oss;
		oss << pair.first << " = " << pair.second << std::endl;
		_contextString += oss.str();
	}
}

template< typename Type >
Type ExpressionParser::parseExpression( const std::string& expression )
{
	Type result = 0;
	try
	{
		// COMMON_COUT( _contextString.c_str() );
		// COMMON_COUT( expression );
		boost::python::exec( _contextString.c_str(), _main_namespace );
		boost::python::object returnText = boost::python::eval( expression.c_str(), _main_namespace );
		result = boost::python::extract<Type>( returnText );
	}
	catch( boost::python::error_already_set const & )
	{
		PyErr_Print();
		throw;

		// PyObject *type, *value, *traceback;
		// PyErr_Fetch(&type, &value, &traceback);
		// std::string message = "Python error: ";
		// if (type) {
		//     type = PyObject_Str(type);
		//     message += PyString_AsString(type);
		// }
		// if (value) {
		//     value = PyObject_Str(value);
		//     message += ": ";
		//     message += "\n";
		//     message += PyString_AsString(value);
		// }
		// Py_XDECREF(type);
		// Py_XDECREF(value);
		// Py_XDECREF(traceback);
		// COMMON_CERR( message );
		// reference : https://svn.boost.org/trac/boost/ticket/2781
	}
	return result;
}

template< >
std::string ExpressionParser::parseExpression<std::string>( const std::string& expression )
{
	std::string result = "";
	try
	{
		boost::python::exec( _contextString.c_str(), _main_namespace );
		boost::python::object returnText = boost::python::eval( expression.c_str(), _main_namespace );
		result = boost::python::extract<std::string>( returnText );
	}
	catch( boost::python::error_already_set const & )
	{
		PyErr_Print();
		throw;
	}
	return result;
}


void ExpressionParser::addPythonHeader( const std::string& pythonString )
{
	_contextString.insert( 0, pythonString + "\n" );
}

std::string ExpressionParser::getPythonHeader()
{
	return _contextString;
}
