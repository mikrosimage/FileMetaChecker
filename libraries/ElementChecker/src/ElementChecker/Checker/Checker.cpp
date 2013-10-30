#include "Checker.hpp"

#include <ElementChecker/Translator/Translator.hpp>
#include <ElementChecker/Ranges/Ranges.hpp>

#include <Common/log.hpp>

#include <algorithm>

namespace be = basic_element;

namespace element_checker
{

Checker::Checker( const std::shared_ptr< be::Element > element )
	: _element( element )
{
}

void Checker::check()
{
	LOG_INFO( _element->_type );

	// switch( _element->_type )
	// {
	// 	case eTypeUnknown : 
	// 	{
	// 		LOG_ERROR( "Unknown element type, cannot check it" );
	// 		_element->_status = eStatusInvalid;
	// 		break;
	// 	}
	// 	case eTypeNumber  :
	// 	{
	// 		std::shared_ptr< be::Number > number = std::static_pointer_cast< be::Number >( _element );
	// 		// _element->_status = checkNumber( number );
	// 		break;
	// 	}
	// 	case eTypeExif    :
	// 	{
	// 		break;
	// 	}
	// 	case eTypeData    :
	// 	{
	// 		std::shared_ptr< be::Data > data = std::static_pointer_cast< be::Data >( _element );
	// 		// _element->_status = checkData( data );
	// 		break;
	// 	}
	// 	case eTypeKlv     :		
	// 	{
	// 		break;
	// 	}
	// }

}

// EStatus Checker::checkData( std::shared_ptr< be::Data > data )
// {
// 	if( data->getSize() == 0 )
// 		LOG_WARNING( data->getId() << ": Null data size !" );

// 	if( data->getValues().empty() )
// 	{
// 		return eStatusPassOver;
// 	}

// 	EStatus status = eStatusInvalid;
// 	switch( data->getSubType() )
// 	{
// 		case eSubTypeAscii :
// 		case eSubTypeHexa :
// 		{
// 			std::string orig = Translator( data.get() ).get();
// 			std::string lowCase = orig;
// 			std::string upCase  = orig;
// 			std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
// 			std::transform(  upCase.begin(),  upCase.end(),  upCase.begin(), ::toupper );

// 			for( std::string value : data->getValues() )
// 				if( value == orig || value == lowCase || value == upCase )
// 					status = eStatusValid;
			
// 			if( status == eStatusInvalid )
// 				data->addErrorLabel( "Invalid value" );
// 			return status;
// 		}

// 		case eSubTypeRaw :
// 		{
// 			return eStatusPassOver;
// 		}
		
// 		case eSubTypeUnknown :
// 		default:
// 		{
// 			data->addErrorLabel( "Invalid subtype" );
// 			break;
// 		}
// 	}
// 	return status;
// }

// EStatus Checker::checkNumber( std::shared_ptr< be::Number > num )
// {
// 	if( num->getRange().empty() )
// 	{
// 		return eStatusPassOver;
// 	}
	
// 	bool inRange = false;
// 	switch( num->getSubType() )
// 	{
// 		case eSubTypeInt8         : inRange = Ranges< be::int8         >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::int8         >() ); break;
// 		case eSubTypeUInt8        : inRange = Ranges< be::uint8        >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::uint8        >() ); break;
// 		case eSubTypeInt16        : inRange = Ranges< be::int16        >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::int16        >() ); break;
// 		case eSubTypeUInt16       : inRange = Ranges< be::uint16       >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::uint16       >() ); break;
// 		case eSubTypeInt32        : inRange = Ranges< be::int32        >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::int32        >() ); break;
// 		case eSubTypeUInt32       : inRange = Ranges< be::uint32       >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::uint32       >() ); break;
// 		case eSubTypeInt64        : inRange = Ranges< be::int64        >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::int64        >() ); break;
// 		case eSubTypeUInt64       : inRange = Ranges< be::uint64       >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::uint64       >() ); break;
// 		case eSubTypeFloat        : inRange = Ranges< float            >( num->getRange() ).isInRanges( Translator( num.get() ).get< float            >() ); break;
// 		case eSubTypeDouble       : inRange = Ranges< double           >( num->getRange() ).isInRanges( Translator( num.get() ).get< double           >() ); break;
// 		case eSubTypeIeeeExtended : inRange = Ranges< be::ieeeExtended >( num->getRange() ).isInRanges( Translator( num.get() ).get< be::ieeeExtended >() ); break;
// 		default: break;
// 	}

// 	if( inRange )
// 		return eStatusValid;
	
// 	num->addErrorLabel( "Invalid subtype" );
// 	return eStatusInvalid;
// }

}

