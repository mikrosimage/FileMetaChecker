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
{

}

bool NodeSpecification::isValid( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport )
{
	std::string message( "" );
	std::string id          = subSpec.second.get< std::string >( kId );
	std::string label       = subSpec.second.get< std::string >( kLabel, "" );
	std::string typeValue   = subSpec.second.get< std::string >( kType, "" );
	std::string count       = subSpec.second.get< std::string >( kCount, "" );
	std::string groupSize   = subSpec.second.get< std::string >( kGroupSize, "" );
	
	std::vector< std::string > asciiValues = getMultipleValues< std::string >( subSpec, kAscii );
	std::vector< std::string > hexaValues  = getMultipleValues< std::string >( subSpec, kHexa  );

	bool endianValue = ( subSpec.second.get<std::string>( kEndian, kEndianBig ) == kEndianBig );
	bool optional    = ( subSpec.second.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );
	bool group       = subSpec.second.get_child_optional( kGroup );
	bool isValidNode = false;
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

				nodeReport.put( "<xmlattr>.optional", optional );
				nodeReport.put_value( asciiValues[i] );
				nodeReport.put( "<xmlattr>.type", "ascii" );
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

				nodeReport.put( "<xmlattr>.optional", optional );
				nodeReport.put_value( hexaValues[i] );
				nodeReport.put( "<xmlattr>.type", "hexa" );
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
		
		exportValidData( validUInt8,  id, uint8Val,  groupProperties, nodeReport );
		exportValidData( validInt8,   id, int8Val,   groupProperties, nodeReport );
		exportValidData( validUInt16, id, uint16Val, groupProperties, nodeReport );
		exportValidData( validInt16,  id, int16Val,  groupProperties, nodeReport );
		exportValidData( validUInt32, id, uint32Val, groupProperties, nodeReport );
		exportValidData( validInt32,  id, int32Val,  groupProperties, nodeReport );
		exportValidData( validFloat,  id, floatVal,  groupProperties, nodeReport );
		exportValidData( validDouble, id, doubleVal, groupProperties, nodeReport );
		
		if( typeValue == "data" )
		{
			size_t size = 0;
			
			ExpressionParser ep = ExpressionParser();
			ep.setVariables( _headerElements );

			nodeReport.clear();

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
			nodeReport.put( "<xmlattr>.optional", optional );
			nodeReport.put( "<xmlattr>.size", getPrintable( size ) );
			nodeReport.put( "<xmlattr>.type", "data" );
		}
		
		isValidNode = validUInt8 | validInt8 | validUInt16 | validInt16 | validUInt32 | validInt32 | validFloat | validDouble | validData;
	}

	if( group )
	{
		bool groupIsValid = true;

		GroupProperties groupProp;

		COMMON_COUT( "--- Chunk (begin) ---");
		BOOST_FOREACH( SubSpec& n, subSpec.second.get_child( kGroup ) )
		{
			bpt::ptree subNodeReport;
			if( ! isValid( n, groupProp, subNodeReport ) )
			{
				COMMON_COUT( n.second.get< std::string >( "id" ) );
				groupIsValid = false;
			}
			else
			{
				if( subNodeReport.size() > 0 )
				{
					nodeReport.push_back( bpt::ptree::value_type( n.second.get< std::string >( "id" ), subNodeReport ) );
				}
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

	// nodeReport.put( "<xmlattr>.id", id );
	nodeReport.put( "<xmlattr>.label", label );
	nodeReport.put( "<xmlattr>.status", ( isValidNode ? "valid" : "invalid" ) );
	return isValidNode;
}


template< typename DataType >
void NodeSpecification::exportValidData( const bool isValid, const std::string& id, const DataType& data, GroupProperties& groupProperties, bpt::ptree& nodeReport )
{
	if( !isValid )
		return;

	_headerElements[ id ] = data;
	groupProperties.addSize( sizeof( DataType ) );
	nodeReport.put_value( data );
	nodeReport.put( "<xmlattr>.type", getStringForType<DataType>() );
}