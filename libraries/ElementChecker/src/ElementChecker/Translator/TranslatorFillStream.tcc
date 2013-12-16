
namespace element_checker
{

template< EDisplayType >
void Translator::fillStream( std::ostringstream& osstr )
{
	osstr << get< std::string >();
}

template< >
void Translator::fillStream< eDisplayTypeAscii >( std::ostringstream& osstr )
{
	size_t size = _ref->_data.size();
	if( ! _ref->_keepEndingChar )
		--size;
	for( size_t i = 0; i < size; ++i )
		osstr << _ref->_data.at( i );
}

template< >
void Translator::fillStream< eDisplayTypeHexa >( std::ostringstream& osstr )
{
	for( size_t i = 0; i < _ref->_data.size(); ++i )
	{
		osstr << std::hex 
		      << std::setfill('0') 
		      << std::setw(2) 
		      << (int)(unsigned char) _ref->_data.at( i );
	}
}

template< >
void Translator::fillStream< eDisplayTypeNumbers >( std::ostringstream& osstr )
{
	for( char value : get< std::vector< char > >() )
		osstr << (short) value;
}

}
