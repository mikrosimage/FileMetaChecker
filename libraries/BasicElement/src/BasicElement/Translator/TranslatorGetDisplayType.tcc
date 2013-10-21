
namespace basic_element
{

std::string Translator::display( const EDisplayType dispType )
{
	if( _ref->getData() == nullptr )
		throw std::runtime_error( "Undefined data" );

	std::ostringstream osstr;
	switch( dispType )
	{
		case eDisplayTypeDefault :
		{
			switch( _ref->getSubType() )
			{
				case eSubTypeUnknown      : break;
				case eSubTypeInt8         : osstr << (short) get<          char >(); break;
				case eSubTypeUInt8        : osstr << (short) get< unsigned char >(); break;
				case eSubTypeInt16        : osstr << get<          short     >(); break;
				case eSubTypeUInt16       : osstr << get< unsigned short     >(); break;
				case eSubTypeInt32        : osstr << get<          int       >(); break;
				case eSubTypeUInt32       : osstr << get< unsigned int       >(); break;
				case eSubTypeInt64        : osstr << get<          long long >(); break;
				case eSubTypeUInt64       : osstr << get< unsigned long long >(); break;
				case eSubTypeFloat        : osstr << get<          float     >(); break;
				case eSubTypeDouble       : osstr << get<          double    >(); break;
				case eSubTypeIeeeExtended : osstr << get<     long double    >(); break;
				case eSubTypeAscii        : osstr << display( eDisplayTypeAscii    ); break;
				case eSubTypeHexa         : osstr << display( eDisplayTypeHexa     ); break;
				case eSubTypeRaw          : osstr << display( eDisplayTypeRaw      ); break;
			}
			break;
		}
		case eDisplayTypeNumbers : fillStream< eDisplayTypeNumbers >( osstr ); break;
		case eDisplayTypeAscii   : fillStream< eDisplayTypeAscii   >( osstr ); break;
		case eDisplayTypeHexa    : fillStream< eDisplayTypeHexa    >( osstr ); break;
		case eDisplayTypeRaw     : fillStream< eDisplayTypeRaw     >( osstr ); break;
	}
	return osstr.str();
}

}
