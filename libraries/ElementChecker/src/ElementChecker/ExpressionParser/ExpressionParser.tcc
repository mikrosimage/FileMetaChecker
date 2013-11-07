
// #include <cstdlib>
#include <Common/log.hpp>
#include <ElementChecker/Translator/Translator.hpp>

namespace element_checker
{

ExpressionParser::ExpressionParser() 
{
	Py_Initialize();
	_mainModule = bpy::import( "__main__" );
	_mainNamespace = _mainModule.attr( "__dict__" );
}


ExpressionParser::ExpressionParser( const std::vector< std::shared_ptr< basic_element::Element > >& elementList )
{
	Py_Initialize();
	_mainModule = bpy::import( "__main__" );
	_mainNamespace = _mainModule.attr( "__dict__" );

	setVariables( elementList );
}

ExpressionParser::~ExpressionParser() 
{
	Py_Finalize();
}

void ExpressionParser::setVariables( const std::vector< std::shared_ptr< basic_element::Element > >& elementList )
{
	std::ostringstream oss;
	for( std::shared_ptr< basic_element::Element > elem : elementList )
	{
		if( elem->_iteration > 1 )
			continue;

		switch( elem->_type )
		{
			case eTypeInt8         : oss << elem->_id << " = " << Translator( elem ).get< basic_element::int8         >() << std::endl; break;
			case eTypeUInt8        : oss << elem->_id << " = " << Translator( elem ).get< basic_element::uint8        >() << std::endl; break;
			case eTypeInt16        : oss << elem->_id << " = " << Translator( elem ).get< basic_element::int16        >() << std::endl; break;
			case eTypeUInt16       : oss << elem->_id << " = " << Translator( elem ).get< basic_element::uint16       >() << std::endl; break;
			case eTypeInt32        : oss << elem->_id << " = " << Translator( elem ).get< basic_element::int32        >() << std::endl; break;
			case eTypeUInt32       : oss << elem->_id << " = " << Translator( elem ).get< basic_element::uint32       >() << std::endl; break;
			case eTypeInt64        : oss << elem->_id << " = " << Translator( elem ).get< basic_element::int64        >() << std::endl; break;
			case eTypeUInt64       : oss << elem->_id << " = " << Translator( elem ).get< basic_element::uint64       >() << std::endl; break;
			case eTypeFloat        : oss << elem->_id << " = " << Translator( elem ).get< float                       >() << std::endl; break;
			case eTypeDouble       : oss << elem->_id << " = " << Translator( elem ).get< double                      >() << std::endl; break;
			case eTypeIeeeExtended : oss << elem->_id << " = " << Translator( elem ).get< basic_element::ieeeExtended >() << std::endl; break;
			default: oss << elem->_id << " = " << "True" << std::endl; break;
		}
		_contextString += oss.str();
	}
}


template< typename ResultType >
ResultType ExpressionParser::getExpressionResult( const std::string& expression )
{
	ResultType result = 0;
	try
	{
		// LOG_TRACE( expression );
		// LOG_TRACE( _contextString.c_str() );
		bpy::exec( _contextString.c_str(), _mainNamespace );
		bpy::object returnText = bpy::eval( expression.c_str(), _mainNamespace );
		result = bpy::extract< ResultType >( returnText );
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
		// LOG_TRACE( expression );
		// LOG_TRACE( _contextString.c_str() );
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

template< >
bool ExpressionParser::getExpressionResult< bool >( const std::string& expression )
{
	bool result = false;
	try
	{
		// LOG_TRACE( expression );
		// LOG_TRACE( _contextString.c_str() );
		bpy::exec( _contextString.c_str(), _mainNamespace );
		bpy::object returnText = bpy::eval( expression.c_str(), _mainNamespace );
		result = bpy::extract< bool >( returnText );
	}
	catch( const bpy::error_already_set& error )
	{
		return result;
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
