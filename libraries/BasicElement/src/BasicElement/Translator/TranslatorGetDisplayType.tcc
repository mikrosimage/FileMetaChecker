
std::string Translator::get( const EDisplayType dispType )
{
	if( ref->getData() == NULL )
		throw std::runtime_error( "Undefined data" );

	std::ostringstream osstr;
	switch( dispType )
	{
		case eDisplayTypeDefault :
		{
			switch( ref->getSubType() )
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
				case eSubTypeAscii        : osstr << get( eDisplayTypeAscii    ); break;
				case eSubTypeHexa         : osstr << get( eDisplayTypeHexa     ); break;
				case eSubTypeRaw          : osstr << get( eDisplayTypeRaw      ); break;
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

