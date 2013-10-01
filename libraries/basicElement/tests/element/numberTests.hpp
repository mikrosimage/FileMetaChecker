
#include <limits>

const char max_char = static_cast< char >( std::numeric_limits<unsigned char>::max() );
const char mid_char = static_cast< char >( 0x7F );
const char h80_char = static_cast< char >( 0x80 );
const char hEF_char = static_cast< char >( 0xEF );
const char hF0_char = static_cast< char >( 0xF0 );
const char hFE_char = static_cast< char >( 0xFE );
const char min_char = static_cast< char >( std::numeric_limits<unsigned char>::min() );

const char dataInt8Low[]  = { min_char };
const char dataInt8High[] = { max_char };

const char dataInt16Low[]  = { min_char, max_char };
const char dataInt16High[] = { max_char, min_char };
const char dataInt16Full[] = { max_char, max_char };

const char dataInt32Low[]  = { min_char, min_char, min_char, max_char };
const char dataInt32High[] = { max_char, min_char, min_char, min_char };
const char dataInt32Full[] = { max_char, max_char, max_char, max_char };

const char dataInt64Low[]  = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, max_char };
const char dataInt64High[] = { max_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };
const char dataInt64Full[] = { max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char };


const char dataFloatZero[]                = { min_char, min_char, min_char, min_char };		// 0.0 ( http://babbage.cs.qc.cuny.edu/IEEE-754/index.xhtml )

const char dataFloatOneBigEndian[]        = {     0x3f, h80_char,     0x00,     0x00 };						// 1.0
const char dataFloatMaxBigEndian[]        = { mid_char, mid_char, max_char, max_char };		// max value supported by the float type :  3.4028234e+38
const char dataFloatNegMaxBigEndian[]     = { max_char, mid_char, max_char, max_char };		// min value supported by the float type : -3.4028234e+38
const char dataFloatMinBigEndian[]        = { min_char, mid_char, max_char, max_char };		// min positive value supported by the float type : 1.17549e-38

const char dataFloatOneLittleEndian[]     = {     0x00,     0x00, h80_char,     0x3f };						// 1.0
const char dataFloatMaxLittleEndian[]     = { max_char, max_char, mid_char, mid_char };		// max value supported by the float type :  3.4028234e+38
const char dataFloatNegMaxLittleEndian[]  = { max_char, max_char, mid_char, max_char };		// min value supported by the float type : -3.4028234e+38
const char dataFloatMinLittleEndian[]     = { max_char, max_char, mid_char, min_char };		// min positive value supported by the float type : 1.17549e-38


const char dataDoubleZero[]               = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// 0.0 ( http://babbage.cs.qc.cuny.edu/IEEE-754/index.xhtml )

const char dataDoubleOneBigEndian[]       = {     0x3f, hF0_char,     0x00,     0x00,     0x00,    0x00,      0x00,     0x00 };									// 1.0
const char dataDoubleMaxBigEndian[]       = { mid_char, hEF_char, max_char, max_char, max_char, max_char, max_char, max_char };		// max value supported by the double type :  1.7976931348623157e+308
const char dataDoubleNegMaxBigEndian[]    = { max_char, hEF_char, max_char, max_char, max_char, max_char, max_char, max_char };		// min value supported by the double type : -1.7976931348623157e+308
const char dataDoubleMinBigEndian[]       = { min_char, 0x10, min_char, min_char, min_char, min_char, min_char, min_char };		// min positive value supported by the double type : 2.2250738585072014e−308

const char dataDoubleOneLittleEndian[]    = {     0x00,     0x00,     0x00,     0x00,     0x00,     0x00, hF0_char,     0x3f };									// 1.0
const char dataDoubleMaxLittleEndian[]    = { max_char, max_char, max_char, max_char, max_char, max_char, hEF_char, mid_char };		// max value supported by the double type :  1.7976931348623157e+308
const char dataDoubleNegMaxLittleEndian[] = { max_char, max_char, max_char, max_char, max_char, max_char, hEF_char, max_char };		// min value supported by the double type : -1.7976931348623157e+308
const char dataDoubleMinLittleEndian[]    = { min_char, min_char, min_char, min_char, min_char, min_char,     0x10, min_char };		// min positive value supported by the double type : 2.2250738585072014e−308


const char dataIeeeExtendedZero[]            = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// 0

const char dataIeeeExtendedOneBigEndian[]    = {     0x3f, max_char, h80_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// 1
const char dataIeeeExtendedMaxBigEndian[]    = { mid_char, hFE_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char };	// max value supported by the IEEE Extended type :   1.18973149535723176502e+4932
const char dataIeeeExtendedNegMaxBigEndian[] = { max_char, hFE_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char };	// min value supported by the IEEE Extended type :  -1.18973149535723176502e+4932
const char dataIeeeExtendedMinBigEndian[]    = { min_char, min_char, h80_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// min positive value supported by the double type : 3.36210314311209350626e-4932

const char dataIeeeExtendedOneLittleEndian[]    = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, h80_char, max_char,     0x3f };	// 1
const char dataIeeeExtendedMaxLittleEndian[]    = { max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char, hFE_char, mid_char };	// max value supported by the IEEE Extended type :   1.18973149535723176502e+4932
const char dataIeeeExtendedNegMaxLittleEndian[] = { max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char, hFE_char, max_char };	// min value supported by the IEEE Extended type :  -1.18973149535723176502e+4932
const char dataIeeeExtendedMinLittleEndian[]    = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, h80_char, min_char, min_char };	// min positive value supported by the double type : 3.36210314311209350626e-4932

BOOST_AUTO_TEST_SUITE( basic_element_test_number )

BOOST_AUTO_TEST_CASE( basic_element_number )
{
	LOG_INFO( "\n>>> basic_element_number <<<" );

	{
		nbe::Number num( id );
		
		BOOST_CHECK_EQUAL( num.getId(), id );
		BOOST_CHECK_EQUAL( num.getType(), eTypeNumber );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeInt8 );
		BOOST_CHECK_EQUAL( num.getStatus(), be::Element::eStatusUnknown );
		BOOST_CHECK_EQUAL( num.getSize(), 1 );
	}
	{
		nbe::Number num( id, eSubTypeUInt8 );
		num.setLabel( label );
		
		BOOST_CHECK_EQUAL( num.getId(), id );
		BOOST_CHECK_EQUAL( num.getType(), eTypeNumber );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeUInt8 );
		BOOST_CHECK_EQUAL( num.getStatus(), be::Element::eStatusUnknown );
		BOOST_CHECK_EQUAL( num.getLabel(), label );
		BOOST_CHECK_EQUAL( num.getSize(), 1 );
		// BOOST_CHECK_THROW( num.get< std::string, eDisplayTypeAscii >(), std::runtime_error ); // @todo: how ????
	}

	{
		// nbe::Number num( id, eSubTypeAscii );
		BOOST_CHECK_THROW( nbe::Number num( id, eSubTypeAscii ), std::runtime_error );
	}
	{
		// nbe::Number num( id, eSubTypeHexa );
		BOOST_CHECK_THROW( nbe::Number num( id, eSubTypeHexa ), std::runtime_error );
	}
	{
		// nbe::Number num( id, eSubTypeRaw );
		BOOST_CHECK_THROW( nbe::Number num( id, eSubTypeRaw ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_number_translate )
{
	LOG_INFO( "\n>>> basic_element_number_translate <<<" );
	{
		LOG_INFO( "\n ====  int8  ==== " );
		nbe::Number num( id, eSubTypeInt8 );

		num.set( dataInt8Low, 1 );
		BOOST_CHECK_EQUAL( num.get< nbe::int8 >(), 0 );

		num.set( dataInt8High, 1 );
		BOOST_CHECK_EQUAL( num.get< nbe::int8 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint8  ==== " );
		nbe::Number num( id, eSubTypeUInt8 );

		num.set( dataInt8Low, 1 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint8 >(), 0 );

		num.set( dataInt8High, 1 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint8 >(), 255 );
	}

	{
		LOG_INFO( "\n ====  int16 ====>> big endian " );
		nbe::Number num( id, eSubTypeInt16 );

		LOG_INFO( "___ dataInt16Low ___");
		num.set( dataInt16Low, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::int16 >(), 255 );

		LOG_INFO( "___ dataInt16High ___");
		num.set( dataInt16High, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::int16 >(), -256 );

		LOG_INFO( "___ dataInt16Full ___");
		num.set( dataInt16Full, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::int16 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint16 ====>> big endian " );
		nbe::Number num( id, eSubTypeUInt16 );

		LOG_INFO( "___ dataInt16Low ___");
		num.set( dataInt16Low, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint16 >(), 0x00ff );

		LOG_INFO( "___ dataInt16High ___");
		num.set( dataInt16High, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint16 >(), 65280 );

		LOG_INFO( "___ dataInt16Full ___");
		num.set( dataInt16Full, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint16 >(), 0xffff );
	}
	{
		LOG_INFO( "\n ====  int16 ====>> little endian " );
		nbe::Number num( id, eSubTypeInt16 );
		num.setBigEndianness( false );

		LOG_INFO( "___ dataInt16Low ___");
		num.set( dataInt16Low, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::int16 >(), -256 );

		LOG_INFO( "___ dataInt16High ___");
		num.set( dataInt16High, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::int16 >(), 255 );

		LOG_INFO( "___ dataInt16Full ___");
		num.set( dataInt16Full, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::int16 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint16 ====>> little endian " );
		nbe::Number num( id, eSubTypeUInt16 );
		num.setBigEndianness( false );

		LOG_INFO( "___ dataInt16Low ___");
		num.set( dataInt16Low, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint16 >(), 65280 );

		LOG_INFO( "___ dataInt16High ___");
		num.set( dataInt16High, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint16 >(), 255 );

		LOG_INFO( "___ dataInt16Full ___");
		num.set( dataInt16Full, 2 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint16 >(), 0xffff );
	}

	{
		LOG_INFO( "\n ====  int32 ====>> big endian " );
		nbe::Number num( id, eSubTypeInt32 );

		num.set( dataInt32Low, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::int32 >(), 255 );

		num.set( dataInt32High, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::int32 >(), -16777216 );

		num.set( dataInt32Full, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::int32 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint32 ====>> big endian " );
		nbe::Number num( id, eSubTypeUInt32 );

		num.set( dataInt32Low, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint32 >(), 255 );

		num.set( dataInt32High, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint32 >(), 4278190080 );

		num.set( dataInt32Full, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint32 >(), std::numeric_limits<unsigned int>::max() );
	}
	{
		LOG_INFO( "\n ====  int32 ====>> little endian " );
		nbe::Number num( id, eSubTypeInt32 );
		num.setBigEndianness( false );

		num.set( dataInt32Low, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::int32 >(), -16777216 );

		num.set( dataInt32High, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::int32 >(), 255 );

		num.set( dataInt32Full, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::int32 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint32 ====>> little endian " );
		nbe::Number num( id, eSubTypeUInt32 );
		num.setBigEndianness( false );

		num.set( dataInt32Low, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint32 >(), 4278190080 );

		num.set( dataInt32High, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint32 >(), 255 );

		num.set( dataInt32Full, 4 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint32 >(), std::numeric_limits<unsigned int>::max() );
	}

	{
		LOG_INFO( "\n ====  int64 ====>> big endian " );
		nbe::Number num( id, eSubTypeInt64 );

		num.set( dataInt64Low, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::int64 >(), 255 );

		num.set( dataInt64High, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::int64 >(), -72057594037927936 );

		num.set( dataInt64Full, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::int64 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint64 ====>> big endian " );
		nbe::Number num( id, eSubTypeUInt64 );

		num.set( dataInt64Low, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint64 >(), 255 );

		num.set( dataInt64High, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint64 >(), 0xff00000000000000 );

		num.set( dataInt64Full, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint64 >(), std::numeric_limits<unsigned long long>::max() );
	}
	{
		LOG_INFO( "\n ====  int64 ====>> little endian " );
		nbe::Number num( id, eSubTypeInt64 );
		num.setBigEndianness( false );

		num.set( dataInt64Low, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::int64 >(), -72057594037927936 );

		num.set( dataInt64High, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::int64 >(), 255 );

		num.set( dataInt64Full, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::int64 >(), -1 );
	}
	{
		LOG_INFO( "\n ==== uint64 ====>> little endian " );
		nbe::Number num( id, eSubTypeUInt64 );
		num.setBigEndianness( false );

		num.set( dataInt64Low, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint64 >(), 0xff00000000000000 );

		num.set( dataInt64High, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint64 >(), 255 );

		num.set( dataInt64Full, 8 );
		BOOST_CHECK_EQUAL( num.get< nbe::uint64 >(), std::numeric_limits<unsigned long long>::max() );
	}

	{
		LOG_INFO( "\n ==== float ====>> big endian " );
		nbe::Number num( id, eSubTypeFloat );

		num.set( dataFloatZero, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), 0, 0.001 );

		num.set( dataFloatOneBigEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), 1, 0.001 );

		num.set( dataFloatMaxBigEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), std::numeric_limits< float >::max(), 0.001 );

		num.set( dataFloatNegMaxBigEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), -std::numeric_limits< float >::max(), 0.001 );

		num.set( dataFloatMinBigEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), std::numeric_limits< float >::min(), 0.001 );
	}
	{
		LOG_INFO( "\n ==== float ====>> little endian " );
		nbe::Number num( id, eSubTypeFloat );
		num.setBigEndianness( false );
		
		num.set( dataFloatZero, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), 0, 0.001 );

		num.set( dataFloatOneLittleEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), 1, 0.001 );

		num.set( dataFloatMaxLittleEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), std::numeric_limits< float >::max(), 0.001 );

		num.set( dataFloatNegMaxLittleEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), -std::numeric_limits< float >::max(), 0.001 );

		num.set( dataFloatMinLittleEndian, 4 );
		BOOST_CHECK_CLOSE( num.get< float >(), std::numeric_limits< float >::min(), 0.001 );
	}

	{
		LOG_INFO( "\n ==== double ====>> big endian " );
		nbe::Number num( id, eSubTypeDouble );

		num.set( dataDoubleZero, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), 0, 0.001 );

		num.set( dataDoubleOneBigEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), 1, 0.001 );

		num.set( dataDoubleMaxBigEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), std::numeric_limits< double >::max(), 0.001 );

		num.set( dataDoubleNegMaxBigEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), -std::numeric_limits< double >::max(), 0.001 );

		num.set( dataDoubleMinBigEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), std::numeric_limits< double >::min(), 0.001 );
	}
	{
		LOG_INFO( "\n ==== double ====>> little endian " );
		nbe::Number num( id, eSubTypeDouble );
		num.setBigEndianness( false );
		
		num.set( dataDoubleZero, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), 0, 0.001 );

		num.set( dataDoubleOneLittleEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), 1, 0.001 );

		num.set( dataDoubleMaxLittleEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), std::numeric_limits< double >::max(), 0.001 );

		num.set( dataDoubleNegMaxLittleEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), -std::numeric_limits< double >::max(), 0.001 );

		num.set( dataDoubleMinLittleEndian, 8 );
		BOOST_CHECK_CLOSE( num.get< double >(), std::numeric_limits< double >::min(), 0.001 );
	}

	{
		LOG_INFO( "\n ==== ieeeExtended ====>> big endian " );
		nbe::Number num( id, eSubTypeIeeeExtended );

		num.set( dataIeeeExtendedZero, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), 0, 0.001 );

		num.set( dataIeeeExtendedOneBigEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), 1, 0.001 );

		num.set( dataIeeeExtendedMaxBigEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), std::numeric_limits< nbe::ieeeExtended >::max(), 0.001 );

		num.set( dataIeeeExtendedNegMaxBigEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), -std::numeric_limits< nbe::ieeeExtended >::max(), 0.001 );

		num.set( dataIeeeExtendedMinBigEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), std::numeric_limits< nbe::ieeeExtended >::min(), 0.001 );
	}

	{
		LOG_INFO( "\n ==== ieeeExtended ====>> little endian " );
		nbe::Number num( id, eSubTypeIeeeExtended );
		num.setBigEndianness( false );

		num.set( dataIeeeExtendedZero, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), 0, 0.001 );

		num.set( dataIeeeExtendedOneLittleEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), 1, 0.001 );

		num.set( dataIeeeExtendedMaxLittleEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), std::numeric_limits< nbe::ieeeExtended >::max(), 0.001 );

		num.set( dataIeeeExtendedNegMaxLittleEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), -std::numeric_limits< nbe::ieeeExtended >::max(), 0.001 );

		num.set( dataIeeeExtendedMinLittleEndian, 10 );
		BOOST_CHECK_CLOSE( num.get< nbe::ieeeExtended >(), std::numeric_limits< nbe::ieeeExtended >::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_number_get_string )
{
	LOG_INFO( "\n>>> basic_element_number_get_string <<<" );
	{
		nbe::Number num( id, eSubTypeInt8 );
		num.set( dataInt8High, 1 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "-1" );
	}
	{
		nbe::Number num( id, eSubTypeUInt8 );
		num.set( dataInt8High, 1 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "255" );
	}
	{
		nbe::Number num( id, eSubTypeInt16 );
		num.set( dataInt16Full, 2 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "-1" );
	}
	{
		nbe::Number num( id, eSubTypeUInt16 );
		num.set( dataInt16Full, 2 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "65535" );
	}
	{
		nbe::Number num( id, eSubTypeInt32 );
		num.set( dataInt32Full, 4 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "-1" );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		num.set( dataInt32Full, 4 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "4294967295" );
	}
	{
		nbe::Number num( id, eSubTypeInt64 );
		num.set( dataInt64Full, 8 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "-1" );
	}
	{
		nbe::Number num( id, eSubTypeUInt64 );
		num.set( dataInt64Full, 8 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "18446744073709551615" );
	}
	{
		nbe::Number num( id, eSubTypeFloat );
		num.set( dataFloatOneBigEndian, 4 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1" );
	}
	{
		nbe::Number num( id, eSubTypeFloat );
		num.setBigEndianness( false );
		num.set( dataFloatOneLittleEndian, 4 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1" );
	}
	{
		nbe::Number num( id, eSubTypeFloat );
		num.set( dataFloatMaxBigEndian, 4 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "3.40282e+38" );
	}
	{
		nbe::Number num( id, eSubTypeDouble );
		num.set( dataDoubleOneBigEndian, 8 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1" );
	}
	{
		nbe::Number num( id, eSubTypeDouble );
		num.setBigEndianness( false );
		num.set( dataDoubleOneLittleEndian, 8 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1" );
	}
	{
		nbe::Number num( id, eSubTypeDouble );
		num.set( dataDoubleMaxBigEndian, 8 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1.79769e+308" );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		num.set( dataIeeeExtendedOneBigEndian, 10 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1" );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		num.setBigEndianness( false );
		num.set( dataIeeeExtendedOneLittleEndian, 10 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1" );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		num.set( dataIeeeExtendedMaxBigEndian, 10 );
		std::string res = num.get< std::string, eDisplayTypeAscii >();
		BOOST_CHECK_EQUAL( res, "1.18973e+4932" );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_number_getSubType_string )
{
	LOG_INFO( "\n>>> basic_element_number_getSubType_string <<<" );
	{
		nbe::Number num( id, eSubTypeInt8 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "int8" );
	}
	{
		nbe::Number num( id, eSubTypeUInt8 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "uint8" );
	}
	{
		nbe::Number num( id, eSubTypeInt16 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "int16" );
	}
	{
		nbe::Number num( id, eSubTypeUInt16 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "uint16" );
	}
	{
		nbe::Number num( id, eSubTypeInt32 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "int32" );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "uint32" );
	}
	{
		nbe::Number num( id, eSubTypeInt64 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "int64" );
	}
	{
		nbe::Number num( id, eSubTypeUInt64 );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "uint64" );
	}
	{
		nbe::Number num( id, eSubTypeFloat );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "float" );
	}
	{
		nbe::Number num( id, eSubTypeDouble );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "double" );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		BOOST_CHECK_EQUAL( num.getSubType< std::string >(), "ieeeExtended" );
	}
	// {
	// 	nbe::Number num( id, eSubTypeAscii );
	// 	BOOST_CHECK_EQUAL( num.getStringFromType(), "" );
	// }
	// {
	// 	nbe::Number num( id, eSubTypeHexa );
	// 	BOOST_CHECK_EQUAL( num.getStringFromType(), "" );
	// }
	// {
	// 	nbe::Number num( id, eSubTypeRaw );
	// 	BOOST_CHECK_EQUAL( num.getStringFromType(), "" );
	// }
}

BOOST_AUTO_TEST_CASE( basic_element_number_checkData )
{
	LOG_INFO( "\n>>> basic_element_number_checkData <<<" );
	{
		nbe::Number num( id, eSubTypeInt32 );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	{
		nbe::Number num( id, eSubTypeFloat );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	{
		nbe::Number num( id, eSubTypeDouble );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	
	{
		nbe::Number num( id, eSubTypeUInt32 );
		num.set( dataInt32Low, sizeof( dataInt32High ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "254", "256" ) );
		num.setRanges( ranges );
		num.set( dataInt32Low, sizeof( dataInt32High ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusValid );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "256", "256" ) );
		num.setRanges( ranges );
		num.set( dataInt32Low, sizeof( dataInt32High ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusInvalid );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "254", "256" ) );
		ranges.push_back( std::make_pair(   "0",   "1" ) );
		num.setRanges( ranges );
		num.set( dataInt32Low, sizeof( dataInt32High ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusValid );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "245", "254" ) );
		ranges.push_back( std::make_pair(   "0",   "1" ) );
		num.setRanges( ranges );
		num.set( dataInt32Low, sizeof( dataInt32High ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusInvalid );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "245",  "256" ) );
		ranges.push_back( std::make_pair( "254", "1000" ) );
		num.setRanges( ranges );
		num.set( dataInt32Low, sizeof( dataInt32High ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusValid );
	}
	
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		num.set( dataIeeeExtendedOneLittleEndian, sizeof( dataIeeeExtendedOneLittleEndian ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusPassOver );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "0", "2" ) );
		num.setRanges( ranges );
		num.set( dataIeeeExtendedOneLittleEndian, sizeof( dataIeeeExtendedOneLittleEndian ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusValid );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		std::vector< std::pair< std::string, std::string > > ranges;
		ranges.push_back( std::make_pair( "1.5", "3.2" ) );
		num.setRanges( ranges );
		num.set( dataIeeeExtendedOneLittleEndian, sizeof( dataIeeeExtendedOneLittleEndian ) );
		BOOST_CHECK_EQUAL( num.checkData(), be::Element::eStatusInvalid );
	}
}

BOOST_AUTO_TEST_CASE( basic_element_number_subType )
{
	LOG_INFO( "\n>>> basic_element_number_subType <<<" );
	{
		nbe::Number num( id, eSubTypeInt8 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeInt8 );
	}
	{
		nbe::Number num( id, eSubTypeUInt8 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeUInt8 );
	}
	{
		nbe::Number num( id, eSubTypeInt16 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeInt16 );
	}
	{
		nbe::Number num( id, eSubTypeUInt16 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeUInt16 );
	}
	{
		nbe::Number num( id, eSubTypeInt32 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeInt32 );
	}
	{
		nbe::Number num( id, eSubTypeUInt32 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeUInt32 );
	}
	{
		nbe::Number num( id, eSubTypeInt64 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeInt64 );
	}
	{
		nbe::Number num( id, eSubTypeUInt64 );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeUInt64 );
	}
	{
		nbe::Number num( id, eSubTypeFloat );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeFloat );
	}
	{
		nbe::Number num( id, eSubTypeDouble );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeDouble );
	}
	{
		nbe::Number num( id, eSubTypeIeeeExtended );
		BOOST_CHECK_EQUAL( num.getSubType(), eSubTypeIeeeExtended );
	}
}

BOOST_AUTO_TEST_SUITE_END()
