#include "Checker.hpp"

#include <ElementChecker/Translator/Translator.hpp>
#include <ElementChecker/Ranges/Ranges.hpp>
#include <ElementChecker/ExpressionParser/ExpressionParser.hpp>

#include <Common/log.hpp>

#include <algorithm>

namespace be = basic_element;

namespace element_checker
{

Checker::Checker()
{
}

void Checker::check( const std::shared_ptr< basic_element::Element > element )
{
	if( element->_size == 0 )	// @todo: parse count expression
		LOG_WARNING( element->_id << ": Null data size !" );

	// if nothing to compare
	if( element->_values.empty() && element->_rangeExpr.empty() )
	{
		element->_status = eStatusPassOver;
		_elementList.push_back( element );
		return;
	}

	// element check :
	EStatus status = eStatusInvalid;
	switch( element->_type )
	{
		case eTypeUnknown : LOG_ERROR( "Unknown element type, cannot check it" ); break;
		case eTypeInt8         :
		{
			if( Ranges< be::int8         >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::int8         >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt8        :
		{
			if( Ranges< be::uint8        >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::uint8        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeInt16        :
		{
			if( Ranges< be::int16        >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::int16        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt16       :
		{
			if( Ranges< be::uint16       >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::uint16       >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeInt32        :
		{
			if( Ranges< be::int32        >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::int32        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt32       :
		{
			if( Ranges< be::uint32       >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::uint32       >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeInt64        :
		{
			if( Ranges< be::int64        >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::int64        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt64       :
		{
			if( Ranges< be::uint64       >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::uint64       >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeFloat        :
		{
			if( Ranges< float            >( element->_rangeExpr ).isInRanges( Translator( element ).get< float            >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeDouble       :
		{
			if( Ranges< double           >( element->_rangeExpr ).isInRanges( Translator( element ).get< double           >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeIeeeExtended :
		{
			if( Ranges< be::ieeeExtended >( element->_rangeExpr ).isInRanges( Translator( element ).get< be::ieeeExtended >() ) )
				status = eStatusValid;
			break;
		}

		case eTypeAscii :
		case eTypeHexa  :
		{
			std::string orig = Translator( element ).get();
			std::string lowCase = orig;
			std::string upCase  = orig;
			std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
			std::transform(  upCase.begin(),  upCase.end(),  upCase.begin(), ::toupper );

			for( std::string value : element->_values )
				if( value == orig || value == lowCase || value == upCase )
					status = eStatusValid;
			
			if( status == eStatusInvalid )
				element->_error += "Invalid value - ";
			break;
		}

		case eTypeRaw   :
		{
			// @todo: get count (ExpressionParser)
			status = eStatusPassOver;
			break;
		}

		case eTypeExif :
		{
			break;
		}
		case eTypeKlv  :		
		{
			break;
		}
	}

	element->_status = status;

	// if element invalid and repeated : checked if iterations valid
	if( status == eStatusInvalid && ! element->_repetExpr.empty() )
	{
		std::string errorMessage;
		if( ! isIterationValid( element->getPrevious(), errorMessage ) )
		{
			LOG_ERROR( element->_id << ": " << errorMessage );
			element->_error += errorMessage;
			_elementList.push_back( element );
		}
		return;
	}

	_elementList.push_back( element );
}

bool Checker::isIterationValid( const std::shared_ptr< basic_element::Element > element, std::string& errorMessage )
{
	if( element->_repetExpr.empty() )
		return true;

	std::stringstream error;
	for( std::pair< std::string, std::string > repetPair : element->_repetExpr )
	{
		if( repetPair.first == repetPair.second )
		{
			ExpressionParser repetParser( _elementList );
			size_t repetNumber = repetParser.getExpressionResult< size_t >( repetPair.first );
			// LOG_FATAL( "////// REPETITION : " << element->_iteration << " / " << repetNumber );
			if( element->_iteration == repetNumber )
				return true;
			error << element->_iteration << " / " << repetNumber;
		}
		else
		{
			ExpressionParser repetParser( _elementList );
			size_t repetMin = 0;
			size_t repetMax = 0;

			if( ! repetPair.first.empty() )
				repetMin = repetParser.getExpressionResult< size_t >( repetPair.first );
			if( ! repetPair.second.empty() )
				repetMax = repetParser.getExpressionResult< size_t >( repetPair.second );

			// LOG_FATAL( "////// REPETITIONS : " << element->_iteration << " / [" << repetMin << ", " << repetMax << "]" );
			if( repetMin <= element->_iteration )
				if( repetMax == 0 || repetMax >= element->_iteration )
					return true;
			error << element->_iteration << " / [" << repetMin << ", " << repetMax << "]";
		}
	}
	errorMessage = "Out of repetition range (" + error.str() + ") - ";
	return false;
}


}

