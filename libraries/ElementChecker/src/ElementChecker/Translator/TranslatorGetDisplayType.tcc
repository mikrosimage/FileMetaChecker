
namespace element_checker
{

std::string Translator::get( const EDisplayType dispType )
{
	if( _ref->_data.empty() )
		return "";

	std::ostringstream osstr;
	switch( dispType )
	{
		case eDisplayTypeDefault :
		{
			switch( _ref->_type )
			{
				case eTypeUnknown      : break;
				case eTypeInt8         : osstr << (short) get<          char >(); break;
				case eTypeUInt8        : osstr << (short) get< unsigned char >(); break;
				case eTypeInt16        : osstr << get<          short     >(); break;
				case eTypeUInt16       : osstr << get< unsigned short     >(); break;
				case eTypeInt32        : osstr << get<          int       >(); break;
				case eTypeUInt32       : osstr << get< unsigned int       >(); break;
				case eTypeInt64        : osstr << get<          long long >(); break;
				case eTypeUInt64       : osstr << get< unsigned long long >(); break;
				case eTypeHalf         : osstr << get<          float     >(); break;
				case eTypeFloat        : osstr << get<          float     >(); break;
				case eTypeDouble       : osstr << get<          double    >(); break;
				case eTypeIeeeExtended : osstr << get<     long double    >(); break;
				case eTypeAscii        : osstr << get( eDisplayTypeAscii    ); break;
				case eTypeHexa         : osstr << get( eDisplayTypeHexa     ); break;
				case eTypeRaw          : osstr << get( eDisplayTypeRaw      ); break;
				default: break; 
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
