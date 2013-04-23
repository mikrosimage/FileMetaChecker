#include "NodeSpecification.hpp"
#include "ValuesSpecification.hpp"
#include "NumbersSpecification.hpp"

#include <extractor/inputFile/File.hpp>
#include <extractor/inputFile/Translator.hpp>
#include <extractor/expressionParser/ExpressionParser.hpp>


#include <boost/foreach.hpp>

#include <common/global.hpp>
#include <iomanip>
#include <cstdlib>
#include <cmath>

NodeSpecification::NodeSpecification( File* file )
	: _file     ( file )
	, message   ( "" )
	, id        ( "" )
	, label     ( "" )
	, typeValue ( "" )
	, count     ( "" )
	, groupSize ( "" )
	, asciiValues( )
	, hexaValues( )
	, isValidNode ( false )
	, endianValue ( false )
	, optional    ( false )
	, group       ( false )
{

}

bool NodeSpecification::isValid( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport )
{
	isValidNode = false;
	id          = subSpec.second.get< std::string >( kId );
	label       = subSpec.second.get< std::string >( kLabel, "" );
	typeValue   = subSpec.second.get< std::string >( kType, "" );
	count       = subSpec.second.get< std::string >( kCount, "" );
	group       = subSpec.second.get_child_optional( kGroup );
	groupSize   = subSpec.second.get< std::string >( kGroupSize, "" );
	
	asciiValues = getMultipleValues< std::string >( subSpec, kAscii );
	hexaValues  = getMultipleValues< std::string >( subSpec, kHexa  );

	endianValue = ( subSpec.second.get<std::string>( kEndian, kEndianBig ) == kEndianBig );
	optional    = ( subSpec.second.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );


	// COMMON_COUT( "label " << label );
	
	if( asciiValues.size() != 0 )
	{
		message += kAscii + " => ";

		size_t size = asciiValues[0].size();
		for( size_t i = 0; i < asciiValues.size(); i++ )
		{
			Ascii value;

			if( i > 0 )
			{
				_file->goBack( size );
			}

			//COMMON_COUT( "read data " << size );
			char buffer[ size ];
			if( ! _file->readData( buffer, size ) )
				return optional;
			
			Translator<Ascii> tr;
			value = tr.translate( buffer, size );

			if( asciiValues[i] ==  value.value )
			{
				isValidNode = true;
				message += asciiValues[i];

				nodeReport.put( "optional", optional );
				nodeReport.put( "value", asciiValues[i] );
				nodeReport.put( "type", "ascii" );
			}
			// COMMON_COUT_VAR2( asciiValues[i], value.value );
		}
		
		if( optional && !isValidNode )
		{
			_file->goBack( size );
			return true;
		}

		groupProperties.addSize( size );
	}

	if( hexaValues.size() != 0 )
	{
		message += kHexa + " => ";
		size_t size = 0.5 * hexaValues[0].size() ;
		
		for( size_t i = 0; i < hexaValues.size(); i++ )
		{
			Hexa value;

			if( i > 0 )
			{
				_file->goBack( size );
			}

			//COMMON_COUT( "read data " << size );
			char buffer[ size ];
			if( ! _file->readData( buffer, size ) )
				return optional;

			Translator<Hexa> tr;
			value = tr.translate( buffer, size );

			if( hexaValues[i] ==  value.value )
			{
				isValidNode = true;
				message += hexaValues[i];

				nodeReport.put( "optional", optional );
				nodeReport.put( "value", hexaValues[i] );
				nodeReport.put( "type", "hexa" );
			}
			// COMMON_COUT_VAR2( hexaValues[i], value.value );
		}
		groupProperties.addSize( size );
	}

	if( typeValue != "" )
	{
		message += kType + " => " + typeValue;

		uint8  uint8Val  = 0;
		int8   int8Val   = 0;
		uint16 uint16Val = 0;
		int16  int16Val  = 0;
		uint32 uint32Val = 0;
		int32  int32Val  = 0;

		float  floatVal  = 0;
		double doubleVal = 0;
		
		bool validUInt8  = isValidNumber<uint8> ( _file, message, typeValue, endianValue, subSpec, uint8Val );
		bool validInt8   = isValidNumber<int8>  ( _file, message, typeValue, endianValue, subSpec, int8Val );
		bool validUInt16 = isValidNumber<uint16>( _file, message, typeValue, endianValue, subSpec, uint16Val );
		bool validInt16  = isValidNumber<int16> ( _file, message, typeValue, endianValue, subSpec, int16Val );
		bool validUInt32 = isValidNumber<uint32>( _file, message, typeValue, endianValue, subSpec, uint32Val );
		bool validInt32  = isValidNumber<int32> ( _file, message, typeValue, endianValue, subSpec, int32Val );

		bool validFloat  = isValidNumber<float> ( _file, message, typeValue, endianValue, subSpec, floatVal );
		bool validDouble = isValidNumber<double>( _file, message, typeValue, endianValue, subSpec, doubleVal );

		bool validData   = false;
		
		if( validUInt8 )
		{
			_headerElements[ id ] = uint8Val;
			// COMMON_COUT( "add id : " << id << " = " << _headerElements[ id ] );
			groupProperties.addSize( sizeof( uint8 ) );
		}
		if( validInt8 )
		{
			_headerElements[ id ] = int8Val;
			groupProperties.addSize( sizeof( int8 ) );
		}
		if( validUInt16 )
		{
			_headerElements[ id ] = uint16Val;
			groupProperties.addSize( sizeof( uint16 ) );
		}
		if( validInt16 )
		{
			_headerElements[ id ] = int16Val;
			groupProperties.addSize( sizeof( int16 ) );
		}
		if( validUInt32 )
		{
			_headerElements[ id ] = uint32Val;
			groupProperties.addSize( sizeof( uint32 ) );
		}
		if( validInt32 )
		{
			_headerElements[ id ] = int32Val;
			groupProperties.addSize( sizeof( int32 ) );
		}

		if( validFloat )
		{
			_headerElements[ id ] = floatVal;
			groupProperties.addSize( sizeof( float ) );
		}
		if( validDouble )
		{
			_headerElements[ id ] = doubleVal;
			groupProperties.addSize( sizeof( double ) );
		}
		
		
		if( typeValue == "data" )
		{
			size_t size = 0;
			
			ExpressionParser ep = ExpressionParser();
			ep.setVariables( _headerElements );

			if( count != "" )
			{
				size = ep.parseExpression<size_t>( count );
			}
			
			message += " ( size = " + getPrintable( size ) + " )";
			
			// COMMON_COUT( "*** Before error ***" );
			// char buffer[ size ];
			// COMMON_COUT( "*** After error ***" );

			// if( ! _file->readData( buffer, size ) )
			// {
			// 	return optional;
			// }

			_file->goForward( size );
			
			validData = true;
			groupProperties.addSize( size );
			nodeReport.put( "optional", optional );
			nodeReport.put( "size", getPrintable( size ) );
			nodeReport.put( "type", "data" );
		}
		
		isValidNode = validUInt8 | validInt8 | validUInt16 | validInt16 | validUInt32 | validInt32 | validFloat | validDouble | validData;
	}

	if( group )
	{
		bool groupIsValid = true;

		GroupProperties groupProp;

		COMMON_COUT( "--- Chunk (begin) ---");
		BOOST_FOREACH( SubSpec &n, subSpec.second.get_child( kGroup ) )
		{
			if( ! this->isValid( n, groupProp, nodeReport ) )
			{
				COMMON_COUT( n.second.get< std::string >( "id" ) );
				groupIsValid = false;
			}
		}
		COMMON_COUT( "--- Chunk (end) ---");

		_file->goBack( groupProp.getSize() );

		isValidNode = groupIsValid;

		ExpressionParser groupLength = ExpressionParser();
		groupLength.setVariables( _headerElements );

		if( groupSize != "" )
		{
			size_t gSize = groupLength.parseExpression<size_t>( groupSize );
			_file->goForward( gSize );

			if( groupProp.getSize() < gSize )
			{
				COMMON_COUT( common::details::kColorYellow << "\tWarning : " << common::details::kColorStd << gSize - groupProp.getSize() << " unused bytes" );
			}
			if( groupProp.getSize() > gSize )
			{
				isValidNode = false;
				COMMON_COUT( common::details::kColorRed << "\tError : " << groupProp.getSize() - gSize << " bytes difference" << common::details::kColorStd );
			}
		}
	}
	
	COMMON_COUT( ( isValidNode ? common::details::kColorGreen : common::details::kColorRed ) << "\t" << std::left << std::setw(40) << ( label + " - " + id ) << "\t" << common::details::kColorStd << message );

	nodeReport.put( "id", id );
	nodeReport.put( "label", label );
	nodeReport.put( "status", isValidNode );
	return isValidNode;
}
