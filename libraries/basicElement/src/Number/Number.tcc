#include <cstdlib>

namespace basic_element
{
namespace number_element
{

template< typename NumberType >
Number< NumberType >::Number()
	: Element( Element::eTypeNumber )
{
	setSize();
}

template< typename NumberType >
void Number< NumberType >::translate( const char* data )
{
	LOG_INFO( " Number: \tTRANSLATE (generic) " );
	char* buffer = new char [_size];
	getEndianOrderedData( buffer, data );
	reverseEndianness( _numData.data, buffer ); // @todo swap system if big endian (little endian here)
	delete[] buffer;
	LOG_INFO( " Number: \t_numData.value: " << _numData.value );
}

template<>
void Number< int8 >::translate( const char* data )
{
	LOG_INFO( " Number: \tTRANSLATE (int8) " );
	std::memcpy( _numData.data, data, _size );
	LOG_INFO( " Number: \t_numData.value: " << (int) _numData.value );
}

template<>
void Number< uint8 >::translate( const char* data )
{
	LOG_INFO( " Number: \tTRANSLATE (uint8) " );
	std::memcpy( _numData.data, data, _size );
	LOG_INFO( " Number: \t_numData.value: " << (int) _numData.value );
}


template< typename NumberType >
std::string Number< NumberType >::toString() const
{
	LOG_INFO( " Number: \tTO STRING (generic) " );
	std::stringstream sstr;
	sstr << _numData.value;
	return sstr.str();
}

template< >
std::string Number< int8 >::toString( ) const
{
	LOG_INFO( " Number: \tTO STRING (int8) " );
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}

template< >
std::string Number< uint8 >::toString( ) const
{
	LOG_INFO( " Number: \tTO STRING (uint8) " );
	std::stringstream sstr;
	sstr << (int) _numData.value;
	return sstr.str();
}


template< typename NumberType >
void Number< NumberType >::setData( const char* data, const size_t& size )
{
	LOG_INFO( " Number: \tSET DATA " << " @ " << &data << " to @ " << &_numData.data );
	if( size != _size )
		throw std::runtime_error( "Invalid data size." );
	translate( data );
}

template< typename NumberType >
void Number< NumberType >::getData( char* buffer ) const
{
	LOG_INFO( " Number: \tGET DATA from @ " << &_numData.data << " to @ " << &buffer << " | size: " << _size );
	std::reverse_copy( _numData.data, _numData.data + _size, buffer );
	LOG_INFO( " Number: \tdata  : " << (int) _numData.data[0] << "\t\t\t@ " << &_numData.data );
	LOG_INFO( " Number: \tbuffer: " << (int)(unsigned char) buffer[0] << "\t\t\t@ " << &buffer );
}

template< typename NumberType >
NumberType Number< NumberType >::getValue() const
{
	LOG_INFO( " Number: \tGET VALUE: " << _numData.value );
	return _numData.value;
}



template< typename NumberType >
Range< NumberType >& Number< NumberType >::getRange()
{
	LOG_INFO( " Number: \tGET RANGE " );
	return _range;
}

template< typename NumberType >
Map< NumberType >& Number< NumberType >::getMap()
{
	LOG_INFO( " Number: \tGET MAP " );
	return _map;
}

template< typename NumberType >
void Number< NumberType >::setSize()
{
	LOG_INFO( " Number: \tSET SIZE (generic) " );
	_size = sizeof( NumberType );
}

template< >
void Number< ieeeExtended >::setSize()
{
	LOG_INFO( " Number: \tSET SIZE (ieeeExtended) " );
	_size = 10;
}

template< typename NumberType >
size_t Number< NumberType >::getSize() const
{
	LOG_INFO( " Number: \tGET SIZE " );
	return _size;
}

template< typename NumberType >
Element::EStatus Number< NumberType >::checkData()
{
	LOG_INFO( " Number: \tCHECK DATA " );
	if( !_range.isSet() )
	{
		setStatus( eStatusPassOver );
		return eStatusPassOver;
	}
	
	Element::EStatus status = eStatusInvalid;
	if( _range.isInRange( _numData.value ) )
		status = eStatusValid;
	
	setStatus( status );
	return status;
}

template< typename NumberType >
std::string Number< NumberType >::getStringFromType() { return ""; }

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


}
}
