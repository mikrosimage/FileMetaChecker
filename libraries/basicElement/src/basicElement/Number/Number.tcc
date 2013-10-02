#include <cstdlib>

namespace basic_element
{
namespace number_element
{


Number::Number( const std::string& id, const ESubType& subType, const EDisplayType& dispType  )
	: Element( id, eTypeNumber, subType, dispType )
	, _data( NULL )
{
	setSize();
	if( _subType == eSubTypeAscii || _subType == eSubTypeHexa || _subType == eSubTypeRaw )
		throw std::runtime_error( "Invalid sub-type." );
}

Number::Number( const spec_reader::SpecNode& node )
	: Element ( node )
	, _data   ( NULL )
{
	setMap( node.getMap() );
	setRanges( node.getRange() );
}

Number::~Number()
{
}

/********************************
 **      NumberTranslator      **
 ********************************/

template< typename NumberType >
Number::NumberTranslator< NumberType >::NumberTranslator( const Number* parent, const char* data )
	: _parent( parent )
{
	setData( data );
}

template< typename NumberType >
void Number::NumberTranslator< NumberType >::setData( const char* data )
{
	char* buffer = new char [ _parent->_size ];
	_parent->getEndianOrderedData( buffer, data );
	_parent->reverseEndianness( _numData.data, buffer ); // @todo swap system if big endian (little endian here)
	delete[] buffer;
	// LOG_TRACE( "setData: " << _numData.value );
}

template<>
void Number::NumberTranslator< int8 >::setData( const char* data )
{
	std::memcpy( _numData.data, data, _parent->_size );
}

template<>
void Number::NumberTranslator< uint8 >::setData( const char* data )
{
	std::memcpy( _numData.data, data, _parent->_size );
}

template< typename NumberType >
NumberType Number::NumberTranslator< NumberType >::getValue()
{
	return _numData.value;
}

template< typename NumberType >
NumberType Number::NumberTranslator< NumberType >::fromString( const std::string& value )
{
	try
	{
		NumberType ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "NumberTranslator::fromString: " << ret );
		return ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
int8 Number::NumberTranslator< int8 >::fromString( const std::string& value )
{
	try
	{
		short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "NumberTranslator::fromString: " << ret );
		if( ret > 127 || ret < -128 )
			throw std::range_error( "fromString: string cannot be converted to int8" );
		return (int8) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< >
uint8 Number::NumberTranslator< uint8 >::fromString( const std::string& value )
{
	try
	{
		unsigned short ret;
		std::stringstream sstr( value );
		sstr >> ret;
		// LOG_TRACE( "NumberTranslator::fromString: " << ret );
		if( ret > 255 || ret < 0 )
			throw std::range_error( "fromString: string cannot be converted to uint8" );
		return (uint8) ret;
	}
	catch( const std::range_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
}

template< typename NumberType >
std::string Number::NumberTranslator< NumberType >::getMapLabel( const std::map< std::string, std::string >& map )
{
	for( const std::pair< std::string, std::string > mapPair : map )
		_map.addPair( fromString( mapPair.first ), mapPair.second );

	return _map.getLabel( _numData.value );
}

template< typename NumberType >
bool Number::NumberTranslator< NumberType >::isValueInRanges( const std::vector< std::pair< std::string, std::string > >& rawRanges )
{
	for( std::pair< std::string, std::string > rawRange : rawRanges )
	{
		Range< NumberType > range;
		range.setRange( fromString( rawRange.first ), fromString( rawRange.second ) );
		_ranges.addRange( range );
	}
	// LOG_TRACE( "isValueInRanges: " << _numData.value );
	return _ranges.isInRanges( _numData.value );
}

/********************************
 ********************************/


void Number::set( const char* data, const size_t& size )
{
	// LOG_TRACE( "Number: \tSET: " << &data << " - " << size );
	if( ! isSizeValid( size ) )
		throw std::runtime_error( "Invalid data size." );
	_size = size;
	_data = const_cast< char* >( data );
}

void Number::setRanges( const std::vector< std::pair< std::string, std::string > >& ranges )
{
	_rawRanges = ranges;
}

void Number::setMap( const std::map< std::string, std::string >& map )
{
	_rawMap = map;
}

template< typename OutputType, EDisplayType DisplayType >
OutputType Number::get() const
{
	if( _data == NULL )
		throw std::runtime_error( "No data." );

	NumberTranslator< OutputType > numTrans( this, _data );
	// LOG_TRACE( "Number::get< >: " << numTrans.getValue() );
	return numTrans.getValue();
}

template< >
std::string Number::get< std::string, eDisplayTypeAscii >() const
{
	if( _data == NULL )
		throw std::runtime_error( "No data." );

	std::stringstream sstr;
	switch( _subType )
	{
		case eSubTypeInt8         : sstr << (int) NumberTranslator<  int8  >( this, _data ).getValue(); break;
		case eSubTypeUInt8        : sstr << (int) NumberTranslator< uint8  >( this, _data ).getValue(); break;
		case eSubTypeInt16        : sstr << NumberTranslator<  int16       >( this, _data ).getValue(); break;
		case eSubTypeUInt16       : sstr << NumberTranslator< uint16       >( this, _data ).getValue(); break;
		case eSubTypeInt32        : sstr << NumberTranslator<  int32       >( this, _data ).getValue(); break;
		case eSubTypeUInt32       : sstr << NumberTranslator< uint32       >( this, _data ).getValue(); break;
		case eSubTypeInt64        : sstr << NumberTranslator<  int64       >( this, _data ).getValue(); break;
		case eSubTypeUInt64       : sstr << NumberTranslator< uint64       >( this, _data ).getValue(); break;
		case eSubTypeFloat        : sstr << NumberTranslator< float        >( this, _data ).getValue(); break;
		case eSubTypeDouble       : sstr << NumberTranslator< double       >( this, _data ).getValue(); break;
		case eSubTypeIeeeExtended : sstr << NumberTranslator< ieeeExtended >( this, _data ).getValue(); break;
		default                   : sstr << ""; break;
	}
	// LOG_TRACE( "Number::get< std::string, ascii >: " << sstr.str() );
	return sstr.str();
}

std::vector< std::pair< std::string, std::string > > Number::getElementInfo()
{
	std::vector< std::pair< std::string, std::string > > elemInfo;
	std::vector< std::pair< std::string, std::string > > commonInfo = getCommonElementInfo();
	
	elemInfo.insert( elemInfo.end(), commonInfo.begin(), commonInfo.end() );

	elemInfo.push_back( std::make_pair( kType, getSubType< std::string >() ) );

	if( _rawMap.size() == 0 )
	{
		elemInfo.push_back( std::make_pair( kValue, get< std::string, eDisplayTypeAscii >() ) );
	}
	else
	{
		std::string value;
		switch( _subType )
		{
			case eSubTypeInt8         : value = NumberTranslator<  int8        >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeUInt8        : value = NumberTranslator< uint8        >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeInt16        : value = NumberTranslator<  int16       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeUInt16       : value = NumberTranslator< uint16       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeInt32        : value = NumberTranslator<  int32       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeUInt32       : value = NumberTranslator< uint32       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeInt64        : value = NumberTranslator<  int64       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeUInt64       : value = NumberTranslator< uint64       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeFloat        : value = NumberTranslator< float        >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeDouble       : value = NumberTranslator< double       >( this, _data ).getMapLabel( _rawMap ); break;
			case eSubTypeIeeeExtended : value = NumberTranslator< ieeeExtended >( this, _data ).getMapLabel( _rawMap ); break;
			default                   : value = "";
		}
		
		if( ! value.empty() )
			value.append( " (" + get< std::string, eDisplayTypeAscii >() + ")" );
		else
			value = "----- (" + get< std::string, eDisplayTypeAscii >() + ")";
		
		elemInfo.push_back( std::make_pair( kValue, value ) );
	}

	return elemInfo;
}

Element::EStatus Number::checkData()
{
	_status = eStatusInvalid;

	if( _rawRanges.empty() )
	{
		_status = eStatusPassOver;
		return eStatusPassOver;
	}
	
	if( isNumberInRange() )
		_status = eStatusValid;
	// LOG_TRACE( "status: " << _status );
	return _status;
}

void Number::setSize()
{
	switch( _subType )
	{
		case eSubTypeInt8         : _size =  1; break;
		case eSubTypeUInt8        : _size =  1; break;
		case eSubTypeInt16        : _size =  2; break;
		case eSubTypeUInt16       : _size =  2; break;
		case eSubTypeInt32        : _size =  4; break;
		case eSubTypeUInt32       : _size =  4; break;
		case eSubTypeInt64        : _size =  8; break;
		case eSubTypeUInt64       : _size =  8; break;
		case eSubTypeFloat        : _size =  4; break;
		case eSubTypeDouble       : _size =  8; break;
		case eSubTypeIeeeExtended : _size = 10; break;
		default : _size = 0;
	}
}

bool Number::isNumberInRange()
{
	if( _rawRanges.empty() )
		return false;

	switch( _subType )
	{
		case eSubTypeInt8         : return NumberTranslator<  int8        >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeUInt8        : return NumberTranslator< uint8        >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeInt16        : return NumberTranslator<  int16       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeUInt16       : return NumberTranslator< uint16       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeInt32        : return NumberTranslator<  int32       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeUInt32       : return NumberTranslator< uint32       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeInt64        : return NumberTranslator<  int64       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeUInt64       : return NumberTranslator< uint64       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeFloat        : return NumberTranslator< float        >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeDouble       : return NumberTranslator< double       >( this, _data ).isValueInRanges( _rawRanges ); break;
		case eSubTypeIeeeExtended : return NumberTranslator< ieeeExtended >( this, _data ).isValueInRanges( _rawRanges ); break;
		default                   : return false;
	}
	return false;
}

bool Number::isSizeValid( const size_t& size )
{
	switch( size )
	{
		case  1 : return true;
		case  2 : return true;
		case  4 : return true;
		case  8 : return true;
		case 10 : return true;
		default : return false;
	}
	return false;
}

}
}
