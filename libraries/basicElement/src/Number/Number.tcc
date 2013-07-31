#include <cstdlib>

namespace basic_element
{
namespace number_element
{

template< typename NumberType >
Number< NumberType >::Number()
	: Element( Element::eTypeNumber )
{
	setSubType();
	setSize();
}

template< typename NumberType >
Number< NumberType >::~Number()
{
}

template< typename NumberType >
void Number< NumberType >::translate( const char* data )
{
	char* buffer = new char [_size];
	getEndianOrderedData( buffer, data );
	reverseEndianness( _numData.data, buffer ); // @todo swap system if big endian (little endian here)
	delete[] buffer;
	//BE_LOG_TRACE( " Number: \t_numData.value: " << _numData.value );
}

template<>
void Number< int8 >::translate( const char* data )
{
	std::memcpy( _numData.data, data, _size );
	//BE_LOG_TRACE( " Number: \t_numData.value: " << (int) _numData.value );
}

template<>
void Number< uint8 >::translate( const char* data )
{
	std::memcpy( _numData.data, data, _size );
	//BE_LOG_TRACE( " Number: \t_numData.value: " << (int) _numData.value );
}


template< typename NumberType >
std::string Number< NumberType >::toString() const
{
	std::stringstream sstr;
	sstr << _numData.value;
	return sstr.str();
}

template< >
std::string Number< int8 >::toString( ) const
{
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}

template< >
std::string Number< uint8 >::toString( ) const
{
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}


template< typename NumberType >
void Number< NumberType >::setData( const char* data, const size_t& size )
{
	//BE_LOG_TRACE( " Number: \tSET DATA " << " @ " << &data << " to @ " << &_numData.data );
	if( size != _size )
		throw std::runtime_error( "Invalid data size." );
	translate( data );
}

template< typename NumberType >
void Number< NumberType >::getData( char* buffer ) const
{
	//BE_LOG_TRACE( " Number: \tGET DATA from @ " << &_numData.data << " to @ " << &buffer << " | size: " << _size );
	std::reverse_copy( _numData.data, _numData.data + _size, buffer );
	//BE_LOG_TRACE( " Number: \tdata  : " << (int) _numData.data[0] << "\t\t\t@ " << &_numData.data );
	//BE_LOG_TRACE( " Number: \tbuffer: " << (int)(unsigned char) buffer[0] << "\t\t\t@ " << &buffer );
}

template< typename NumberType >
NumberType Number< NumberType >::getValue() const
{
	return _numData.value;
}



template< typename NumberType >
Range< NumberType >& Number< NumberType >::getRange()
{
	return _range;
}

template< typename NumberType >
Map< NumberType >& Number< NumberType >::getMap()
{
	return _map;
}

template< typename NumberType >
void Number< NumberType >::setSize()
{
	_size = sizeof( NumberType );
}

template< >
void Number< ieeeExtended >::setSize()
{
	_size = 10;
}

template< typename NumberType >
size_t Number< NumberType >::getSize() const
{
	return _size;
}

template< typename NumberType >
Element::EStatus Number< NumberType >::checkData()
{
	_status = eStatusInvalid;
	
	if( !_range.isSet() )
	{
		_status = eStatusPassOver;
		return eStatusPassOver;
	}
	
	if( _range.isInRange( _numData.value ) )
		_status = eStatusValid;
	
	return _status;
}

template< typename NumberType >
std::vector< std::pair< std::string, std::string > > Number< NumberType >::getElementInfo()
{
	std::vector< std::pair< std::string, std::string > > elemInfo;
	std::string status;
	switch( getStatus() )
	{
		case Element::eStatusUnknown  : status = "Unknown";   break;
		case Element::eStatusValid    : status = "Valid";     break;
		case Element::eStatusInvalid  : status = "Invalid";   break;
		case Element::eStatusPassOver : status = "Pass over"; break;
	}

	elemInfo.push_back( std::make_pair( "id",     getId()             ) );
	elemInfo.push_back( std::make_pair( "label",  getLabel()          ) );
	elemInfo.push_back( std::make_pair( "status", status              ) );
	elemInfo.push_back( std::make_pair( "type",   getStringFromType() ) );
	elemInfo.push_back( std::make_pair( "value",  toString()          ) );
	return elemInfo;
}

template< >
std::string Number< int8 >::getStringFromType() { return "int8"; }

template< >
std::string Number< uint8 >::getStringFromType() { return "uint8"; }

template< >
std::string Number< int16 >::getStringFromType() { return "int16"; }

template< >
std::string Number< uint16 >::getStringFromType() { return "uint16"; }

template< >
std::string Number< int32 >::getStringFromType() { return "int32"; }

template< >
std::string Number< uint32 >::getStringFromType() { return "uint32"; }

template< >
std::string Number< int64 >::getStringFromType() { return "int64"; }

template< >
std::string Number< uint64 >::getStringFromType() { return "uint64"; }

template< >
std::string Number< float >::getStringFromType() { return "float"; }

template< >
std::string Number< double >::getStringFromType() { return "double"; }

template< >
std::string Number< ieeeExtended >::getStringFromType() { return "ieeeExtended"; }


template< >
void Number<  int8  >::setSubType() { _subType = Element::eNumberTypeInt8; }

template< >
void Number< uint8  >::setSubType() { _subType = Element::eNumberTypeUInt8; }

template< >
void Number<  int16 >::setSubType() { _subType = Element::eNumberTypeInt16; }

template< >
void Number< uint16 >::setSubType() { _subType = Element::eNumberTypeUInt16; }

template< >
void Number<  int32 >::setSubType() { _subType = Element::eNumberTypeInt32; }

template< >
void Number< uint32 >::setSubType() { _subType = Element::eNumberTypeUInt32; }

template< >
void Number<  int64 >::setSubType() { _subType = Element::eNumberTypeInt64; }

template< >
void Number< uint64 >::setSubType() { _subType = Element::eNumberTypeUInt64; }

template< >
void Number<  float >::setSubType() { _subType = Element::eNumberTypeFloat; }

template< >
void Number< double >::setSubType() { _subType = Element::eNumberTypeDouble; }

template< >
void Number< ieeeExtended >::setSubType() { _subType = Element::eNumberTypeIeeeExtended; }

}
}
