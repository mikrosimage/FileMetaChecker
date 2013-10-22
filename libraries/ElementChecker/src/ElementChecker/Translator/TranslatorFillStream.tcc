
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
	for( size_t i = 0; i < _ref->getSize(); ++i )
		osstr << _ref->getData()[i];
}

template< >
void Translator::fillStream< eDisplayTypeHexa >( std::ostringstream& osstr )
{
	for( size_t i = 0; i < _ref->getSize(); ++i )
	{
		osstr << std::hex 
		      << std::setfill('0') 
		      << std::setw(2) 
		      << (int)(unsigned char) _ref->getData()[i];
	}
}

template< >
void Translator::fillStream< eDisplayTypeNumbers >( std::ostringstream& osstr )
{
	for( char value : get< std::vector< char > >() )
		osstr << (short) value;
}

}
