#include <extractor/inputFile/Translator.hpp>

#include <iostream>
#include <limits>

#define BOOST_TEST_MODULE qc_extractor_translator
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace std;

const char max_char = std::numeric_limits<unsigned char>::max();
const char mid_char = 0x7F;
const char min_char = std::numeric_limits<unsigned char>::min();

const char dataInt8Low[]  = { min_char };
const char dataInt8High[] = { max_char };

const char dataInt16Low[]  = { min_char, max_char };
const char dataInt16High[] = { max_char, min_char };
const char dataInt16Full[] = { max_char, max_char };

const char dataInt32Low[]  = { min_char, min_char, min_char, max_char };
const char dataInt32High[] = { max_char, min_char, min_char, min_char };
const char dataInt32Full[] = { max_char, max_char, max_char, max_char };


const char dataFloatZero[]                = { min_char, min_char, min_char, min_char };		// 0.0 ( http://babbage.cs.qc.cuny.edu/IEEE-754/index.xhtml )

const char dataFloatOneBigEndian[]        = { 0x3f, 0x80, 0x00, 0x00 };						// 1.0
const char dataFloatMaxBigEndian[]        = { mid_char, mid_char, max_char, max_char };		// max value supported by the float type :  3.4028234e+38
const char dataFloatNegMaxBigEndian[]     = { max_char, mid_char, max_char, max_char };		// min value supported by the float type : -3.4028234e+38
const char dataFloatMinBigEndian[]        = { min_char, mid_char, max_char, max_char };		// min positive value supported by the float type : 1.17549e-38

const char dataFloatOneLittleEndian[]     = { 0x00, 0x00, 0x80, 0x3f };						// 1.0
const char dataFloatMaxLittleEndian[]     = { max_char, max_char, mid_char, mid_char };		// max value supported by the float type :  3.4028234e+38
const char dataFloatNegMaxLittleEndian[]  = { max_char, max_char, mid_char, max_char };		// min value supported by the float type : -3.4028234e+38
const char dataFloatMinLittleEndian[]     = { max_char, max_char, mid_char, min_char };		// min positive value supported by the float type : 1.17549e-38


const char dataDoubleZero[]               = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// 0.0 ( http://babbage.cs.qc.cuny.edu/IEEE-754/index.xhtml )

const char dataDoubleOneBigEndian[]       = { 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };									// 1.0
const char dataDoubleMaxBigEndian[]       = { mid_char, 0xef, max_char, max_char, max_char, max_char, max_char, max_char };		// max value supported by the double type :  1.7976931348623157e+308
const char dataDoubleNegMaxBigEndian[]    = { max_char, 0xef, max_char, max_char, max_char, max_char, max_char, max_char };		// min value supported by the double type : -1.7976931348623157e+308
const char dataDoubleMinBigEndian[]       = { min_char, 0x10, min_char, min_char, min_char, min_char, min_char, min_char };		// min positive value supported by the double type : 2.2250738585072014e−308

const char dataDoubleOneLittleEndian[]    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f };									// 1.0
const char dataDoubleMaxLittleEndian[]    = { max_char, max_char, max_char, max_char, max_char, max_char, 0xef, mid_char };		// max value supported by the double type :  1.7976931348623157e+308
const char dataDoubleNegMaxLittleEndian[] = { max_char, max_char, max_char, max_char, max_char, max_char, 0xef, max_char };		// min value supported by the double type : -1.7976931348623157e+308
const char dataDoubleMinLittleEndian[]    = { min_char, min_char, min_char, min_char, min_char, min_char, 0x10, min_char };		// min positive value supported by the double type : 2.2250738585072014e−308


const char dataIeeeExtendedZero[]            = { min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// 0

const char dataIeeeExtendedOneBigEndian[]    = {     0x3f, max_char,     0x80, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// 1
const char dataIeeeExtendedMaxBigEndian[]    = {     0x7f,     0xfe, max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char };	// max value supported by the IEEE Extended type :   1.18973149535723176502e+4932
const char dataIeeeExtendedNegMaxBigEndian[] = { max_char,     0xfe, max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char };	// min value supported by the IEEE Extended type :  -1.18973149535723176502e+4932
const char dataIeeeExtendedMinBigEndian[]    = { min_char, min_char,     0x80, min_char, min_char, min_char, min_char, min_char, min_char, min_char };	// min positive value supported by the double type : 3.36210314311209350626e-4932

const char dataIeeeExtendedOneLittleEndian[]    = { min_char, min_char, min_char, min_char, min_char, min_char, min_char,     0x80, max_char,     0x3f };	// 1
const char dataIeeeExtendedMaxLittleEndian[]    = { max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char,     0xfe,     0x7f };	// max value supported by the IEEE Extended type :   1.18973149535723176502e+4932
const char dataIeeeExtendedNegMaxLittleEndian[] = { max_char, max_char, max_char, max_char, max_char, max_char, max_char, max_char,     0xfe, max_char };	// min value supported by the IEEE Extended type :  -1.18973149535723176502e+4932
const char dataIeeeExtendedMinLittleEndian[]    = { min_char, min_char, min_char, min_char, min_char, min_char, min_char,     0x80, min_char, min_char };	// min positive value supported by the double type : 3.36210314311209350626e-4932


BOOST_AUTO_TEST_SUITE( translator_tests_suite01 )

BOOST_AUTO_TEST_CASE( translator_hexa )
{
	{
		Translator<Hexa> trChar;
		Hexa ret = trChar.translate( dataInt8Low, 1 );
		BOOST_CHECK_EQUAL( ret.originalCaseValue, "00" );
	}
	{
		Translator<Hexa> trChar;
		Hexa ret = trChar.translate( dataInt8High, 1 );
		BOOST_CHECK_EQUAL( ret.originalCaseValue, "ff" );
	}
}

BOOST_AUTO_TEST_CASE( translator_int8 )
{
	{
		Translator<int8> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt8Low ), 0 );
	}
	{
		Translator<int8> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt8High ), -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_sint8 )
{
	{
		Translator<sint8> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt8Low ), 0 );
	}
	{
		Translator<sint8> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt8High ), -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint8 )
{
	{
		Translator<uint8> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt8Low ), 0 );
	}
	{
		Translator<uint8> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt8High ), 0xff );
	}
}

BOOST_AUTO_TEST_CASE( translator_int16_be )
{
	{
		Translator<int16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Low ), 0x00ff );
	}
	{
		Translator<int16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16High ), -256 );
	}
	{
		Translator<int16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Full ), -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint16_be )
{
	{
		Translator<uint16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Low ), 0x00ff );
	}
	{
		Translator<uint16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16High ), 65280 );
	}
	{
		Translator<uint16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Full ), 0xffff );
	}
}

BOOST_AUTO_TEST_CASE( translator_int16_le )
{
	{
		Translator<int16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Low, 2, false ), -256 );
	}
	{
		Translator<int16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16High, 2, false ), 255 );
	}
	{
		Translator<int16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Full, 2, false ), -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint16_le )
{
	{
		Translator<uint16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Low, 2, false ), 65280 );
	}
	{
		Translator<uint16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16High, 2, false ), 255 );
	}
	{
		Translator<uint16> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt16Full, 2, false ), 0xffff );
	}
}

BOOST_AUTO_TEST_CASE( translator_int32_be )
{
	{
		Translator<int32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Low ), 255 );
	}
	{
		Translator<int32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32High ), -16777216 );
	}
	{
		Translator<int32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Full ), -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint32_be )
{
	{
		Translator<uint32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Low ), 255 );
	}
	{
		Translator<uint32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32High ), 4278190080 );
	}
	{
		Translator<uint32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Full ), std::numeric_limits<unsigned int>::max() );
	}
}

BOOST_AUTO_TEST_CASE( translator_int32_le )
{
	{
		Translator<int32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Low, 2, false ), -16777216 );
	}
	{
		Translator<int32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32High, 2, false ), 255 );
	}
	{
		Translator<int32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Full, 2, false ), -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint32_le )
{
	{
		Translator<uint32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Low, 2, false ), 4278190080 );
	}
	{
		Translator<uint32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32High, 2, false ), 255 );
	}
	{
		Translator<uint32> trChar;
		BOOST_CHECK_EQUAL( trChar.translate( dataInt32Full, 2, false ), std::numeric_limits<unsigned int>::max() );
	}
}


BOOST_AUTO_TEST_CASE( translator_float_be )
{
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatZero, 0, true ), 0.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatOneBigEndian, 0, true ), 1.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatMaxBigEndian, 0, true ), std::numeric_limits<float>::max(), 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatNegMaxBigEndian, 0, true ), -std::numeric_limits<float>::max(), 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatMinBigEndian, 0, true ), std::numeric_limits<float>::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_float_le )
{
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatZero, 0, false ), 0.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatOneLittleEndian, 0, false ), 1.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatMaxLittleEndian, 0, false ), std::numeric_limits<float>::max(), 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatNegMaxLittleEndian, 0, false ), -std::numeric_limits<float>::max(), 0.001 );
	}
	{
		Translator<float> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataFloatMinLittleEndian, 0, false ), std::numeric_limits<float>::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_double_be )
{
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleZero, 0, true ), 0.00, 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleOneBigEndian, 0, true ), 1.00, 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleMaxBigEndian, 0, true ), std::numeric_limits<double>::max(), 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleNegMaxBigEndian, 0, true ), -std::numeric_limits<double>::max(), 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleMinBigEndian, 0, true ), std::numeric_limits<double>::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_double_le )
{
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleZero, 0, false ), 0.00, 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleOneLittleEndian, 0, false ), 1.00, 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleMaxLittleEndian, 0, false ), std::numeric_limits<double>::max(), 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleNegMaxLittleEndian, 0, false ), -std::numeric_limits<double>::max(), 0.001 );
	}
	{
		Translator<double> trFloat;
		BOOST_CHECK_CLOSE( trFloat.translate( dataDoubleMinLittleEndian, 0, false ), std::numeric_limits<double>::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_ieeeExtended_be )
{
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_EQUAL( trIeeeExtended.translate( dataIeeeExtendedZero, 10, true ), 0 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_EQUAL( trIeeeExtended.translate( dataIeeeExtendedOneBigEndian, 10, true ), 1 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_CLOSE( trIeeeExtended.translate( dataIeeeExtendedMaxBigEndian, 10, true ), std::numeric_limits<ieeeExtended>::max(), 0.001 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_CLOSE( trIeeeExtended.translate( dataIeeeExtendedNegMaxBigEndian, 10, true ), -std::numeric_limits<ieeeExtended>::max(), 0.001 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_CLOSE( trIeeeExtended.translate( dataIeeeExtendedMinBigEndian, 10, true ), std::numeric_limits<ieeeExtended>::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_ieeeExtended_le )
{
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_EQUAL( trIeeeExtended.translate( dataIeeeExtendedZero, 10, false ), 0 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_EQUAL( trIeeeExtended.translate( dataIeeeExtendedOneLittleEndian, 10, false ), 1 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_CLOSE( trIeeeExtended.translate( dataIeeeExtendedMaxLittleEndian, 10, false ), std::numeric_limits<ieeeExtended>::max(), 0.001 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_CLOSE( trIeeeExtended.translate( dataIeeeExtendedNegMaxLittleEndian, 10, false ), -std::numeric_limits<ieeeExtended>::max(), 0.001 );
	}
	{
		Translator<ieeeExtended> trIeeeExtended;
		BOOST_CHECK_CLOSE( trIeeeExtended.translate( dataIeeeExtendedMinLittleEndian, 10, false ), std::numeric_limits<ieeeExtended>::min(), 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_translate_to_string )
{
	{
		Translator<int> trInt;
		int intValue = 123;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "123" );
	}
	{
		Translator<int8> trInt;
		int8 intValue = 8;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "8" );
	}
	{
		Translator<uint8> trInt;
		uint8 intValue = 8;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "8" );
	}
	{
		Translator<int16> trInt;
		int16 intValue = 160;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "160" );
	}
	{
		Translator<uint16> trInt;
		uint16 intValue = 160;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "160" );
	}
	{
		Translator<int32> trInt;
		int32 intValue = 3200;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "3200" );
	}
	{
		Translator<uint32> trInt;
		uint32 intValue = 3200;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "3200" );
	}
	{
		Translator<int64> trInt;
		int64 intValue = 64000;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "64000" );
	}
	{
		Translator<uint64> trInt;
		uint64 intValue = 64000;
		BOOST_CHECK_EQUAL( trInt.translate( intValue ), "64000" );
	}

	{
		Translator<float> trFloat;
		float floatValue = 0.123;
		BOOST_CHECK_EQUAL( trFloat.translate( floatValue ), "0.123" );
	}
	{
		Translator<float> trFloat;
		float floatValue = 1.23e+35;
		BOOST_CHECK_EQUAL( trFloat.translate( floatValue ), "1.23e+35" );
	}

	{
		Translator<double> trDouble;
		double doubleValue = 0.00123;
		BOOST_CHECK_EQUAL( trDouble.translate( doubleValue ), "0.00123" );
	}
	{
		Translator<double> trDouble;
		double doubleValue = 1.23e+305;
		BOOST_CHECK_EQUAL( trDouble.translate( doubleValue ), "1.23e+305" );
	}

	{
		Translator<ieeeExtended> trIeeeExtended;
		ieeeExtended ieeeExtendedValue = 1.23e+305;
		BOOST_CHECK_EQUAL( trIeeeExtended.translate( ieeeExtendedValue ), "1.23e+305" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
