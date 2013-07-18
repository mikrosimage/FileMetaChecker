
namespace basic_element
{
namespace translator_tool
{

template< typename InputType, typename OutputType >
OutputType ElementTranslator< InputType, OutputType >::translate( const InputType& element )
{
	BE_LOG_TRACE( " ElementTranslator: \tTRANSLATE" );
	OutputType translation;
	size_t size = element.getSize();
	char* buffer = new char [ size ];
	element.getData( buffer );
	translation.setData( buffer, size );
	translation.setBigEndianness( element.getBigEndianness() );
	delete[] buffer;
	return translation;
}

}
}

