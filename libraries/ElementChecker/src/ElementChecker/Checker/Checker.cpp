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
	
	if( _element->_size == 0 )
		LOG_WARNING( _element->_id << ": Null data size !" );

	if( _element->_values.empty() && _element->_rangeExpr.empty() )
	{
		_element->_status = eStatusPassOver;
		return;
	}

	EStatus status = eStatusInvalid;
	switch( _element->_type )
	{
		case eTypeUnknown : LOG_ERROR( "Unknown element type, cannot check it" ); break;
		case eTypeInt8         :
		{
			if( Ranges< be::int8         >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::int8         >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt8        :
		{
			if( Ranges< be::uint8        >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::uint8        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeInt16        :
		{
			if( Ranges< be::int16        >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::int16        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt16       :
		{
			if( Ranges< be::uint16       >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::uint16       >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeInt32        :
		{
			if( Ranges< be::int32        >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::int32        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt32       :
		{
			if( Ranges< be::uint32       >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::uint32       >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeInt64        :
		{
			if( Ranges< be::int64        >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::int64        >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeUInt64       :
		{
			if( Ranges< be::uint64       >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::uint64       >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeFloat        :
		{
			if( Ranges< float            >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< float            >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeDouble       :
		{
			if( Ranges< double           >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< double           >() ) )
				status = eStatusValid;
			break;
		}
		case eTypeIeeeExtended :
		{
			if( Ranges< be::ieeeExtended >( _element->_rangeExpr ).isInRanges( Translator( _element.get() ).get< be::ieeeExtended >() ) )
				status = eStatusValid;
			break;
		}

		case eTypeAscii :
		case eTypeHexa  :
		{
			std::string orig = Translator( _element.get() ).get();
			std::string lowCase = orig;
			std::string upCase  = orig;
			std::transform( lowCase.begin(), lowCase.end(), lowCase.begin(), ::tolower );
			std::transform(  upCase.begin(),  upCase.end(),  upCase.begin(), ::toupper );

			for( std::string value : _element->_values )
				if( value == orig || value == lowCase || value == upCase )
					status = eStatusValid;
			
			if( status == eStatusInvalid )
				_element->_error += "Invalid value ";
			break;
		}

		case eTypeRaw   :
		{
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
	_element->_status = status;

}

}

