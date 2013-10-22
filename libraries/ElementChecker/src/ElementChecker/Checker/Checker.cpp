#include "Checker.hpp"

#include <ElementChecker/Translator/Translator.hpp>
#include <ElementChecker/Ranges/Ranges.hpp>

#include <Common/log.hpp>
#include <BasicElement/SubElements/Data.hpp>
#include <BasicElement/SubElements/Number.hpp>

#include <algorithm>

namespace element_checker
{

Checker::Checker( const std::shared_ptr< basic_element::Element > element )
	: _element( element )
{
}

void Checker::check()
{
	LOG_INFO( _element->getType() );

	switch( _element->getType() )
	{
		case eTypeUnknown : 
		{
			LOG_ERROR( "Unknown element type, cannot check it" );
			_element->setStatus( eStatusInvalid );
			break;
		}
		case eTypeNumber  :
		{
			std::shared_ptr< basic_element::Number > number = std::static_pointer_cast< basic_element::Number >( _element );
			_element->setStatus( checkNumber( number ) );
			break;
		}
		case eTypeExif    :
		{
			break;
		}
		case eTypeData    :
		{
			std::shared_ptr< basic_element::Data > data = std::static_pointer_cast< basic_element::Data >( _element );
			_element->setStatus( checkData( data ) );
			break;
		}
		case eTypeKlv     :		
		{
			break;
		}
	}

}

EStatus Checker::checkData( std::shared_ptr< basic_element::Data > data )
{
	if( data->getSize() == 0 )
		LOG_WARNING( data->getId() << ": Null data size !" );

	if( data->getValues().empty() )
	{
		return eStatusPassOver;
	}

	EStatus status = eStatusInvalid;
	switch( data->getSubType() )
	{
		case eSubTypeAscii :
		case eSubTypeHexa :
		{
			std::string orig = Translator( data.get() ).get();
			std::string lowCase = orig;
			std::string upCase  = orig;
			std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
			std::transform(  upCase.begin(),  upCase.end(),  upCase.begin(), ::toupper );

			for( std::string value : data->getValues() )
				if( value == orig || value == lowCase || value == upCase )
					status = eStatusValid;
			
			if( status == eStatusInvalid )
				data->addErrorLabel( "Invalid value" );
			return status;
		}

		case eSubTypeRaw :
		{
			return eStatusPassOver;
		}
		
		case eSubTypeUnknown :
		default:
		{
			data->addErrorLabel( "Invalid subtype" );
			break;
		}
	}
	return status;
}

EStatus Checker::checkNumber( std::shared_ptr< basic_element::Number > number )
{
	if( number->getRange().empty() )
	{
		return eStatusPassOver;
	}
	
	bool isInRange = false;
	switch( number->getSubType() )
	{
		case eSubTypeInt8         : isInRange = Ranges< basic_element::int8         >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::int8         >() ); break;
		case eSubTypeUInt8        : isInRange = Ranges< basic_element::uint8        >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::uint8        >() ); break;
		case eSubTypeInt16        : isInRange = Ranges< basic_element::int16        >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::int16        >() ); break;
		case eSubTypeUInt16       : isInRange = Ranges< basic_element::uint16       >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::uint16       >() ); break;
		case eSubTypeInt32        : isInRange = Ranges< basic_element::int32        >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::int32        >() ); break;
		case eSubTypeUInt32       : isInRange = Ranges< basic_element::uint32       >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::uint32       >() ); break;
		case eSubTypeInt64        : isInRange = Ranges< basic_element::int64        >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::int64        >() ); break;
		case eSubTypeUInt64       : isInRange = Ranges< basic_element::uint64       >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::uint64       >() ); break;
		case eSubTypeFloat        : isInRange = Ranges< float                       >( number->getRange() ).isInRanges( Translator( number.get() ).get< float                       >() ); break;
		case eSubTypeDouble       : isInRange = Ranges< double                      >( number->getRange() ).isInRanges( Translator( number.get() ).get< double                      >() ); break;
		case eSubTypeIeeeExtended : isInRange = Ranges< basic_element::ieeeExtended >( number->getRange() ).isInRanges( Translator( number.get() ).get< basic_element::ieeeExtended >() ); break;
		default: break;
	}

	if( isInRange )
		return eStatusValid;
	
	number->addErrorLabel( "Invalid subtype" );
	return eStatusInvalid;
}

}

