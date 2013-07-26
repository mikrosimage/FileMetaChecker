
namespace basic_element
{
namespace translator_tool
{

template< typename InputType, typename OutputType >
OutputType ElementTranslator< InputType, OutputType >::translate( const InputType& element )
{
	OutputType translation;

	char* buffer = new char [ element.getSize() ];
	element.getData( buffer );
	translation.setData( buffer, element.getSize() );
	translation.setBigEndianness( element.getBigEndianness() );
	delete[] buffer;
	return translation;
}

}
}

