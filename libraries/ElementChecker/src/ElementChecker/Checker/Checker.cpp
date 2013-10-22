#include "Checker.hpp"

#include <Common/log.hpp>
#include <BasicElement/SubElements/Data.hpp>
#include <BasicElement/SubElements/Number.hpp>
#include <ElementChecker/Translator/Translator.hpp>

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
	EStatus status = eStatusInvalid;

	if( number->getRange().empty() )
	{
		return eStatusPassOver;
	}
	
	// if( isNumberInRange() )	// @todo
	// 	status = eStatusValid;

	// LOG_TRACE( "status: " << status );
	return status;
}

}

