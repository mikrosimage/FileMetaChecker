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


const char dataFloatZero[]               = { min_char, min_char, min_char, min_char };

const char dataFloatOneLittleEndian[]    = { 0x3f, 0x80, 0x00, 0x00 };
const char dataFloatMaxLittleEndian[]    = { mid_char, mid_char, max_char, max_char };
const char dataFloatNegMaxLittleEndian[] = { max_char, mid_char, max_char, max_char };
const char dataFloatMinLittleEndian[]    = { min_char, mid_char, max_char, max_char };

const char dataFloatOneBigEndian[]    = { 0x00, 0x00, 0x80, 0x3f };
const char dataFloatMaxBigEndian[]    = { max_char, max_char, mid_char, mid_char };
const char dataFloatNegMaxBigEndian[] = { max_char, max_char, mid_char, max_char };
const char dataFloatMinBigEndian[]    = { max_char, max_char, mid_char, min_char };

BOOST_AUTO_TEST_SUITE( translator_tests_suite01 )

BOOST_AUTO_TEST_CASE( translator_hexa )
{
	{
		Translator<Hexa> trChar;
		Hexa ret = trChar.translate( dataInt8Low, 1 );
		BOOST_CHECK_EQUAL( ret.value, "00" );
	}
	{
		Translator<Hexa> trChar;
		Hexa ret = trChar.translate( dataInt8High, 1 );
		BOOST_CHECK_EQUAL( ret.value, "ff" );
	}
}

BOOST_AUTO_TEST_CASE( translator_int8 )
{
	{
		Translator<int8> trChar;
		int8 ret = trChar.translate( dataInt8Low );
		BOOST_CHECK_EQUAL( ret, 0 );
	}
	{
		Translator<int8> trChar;
		int8 ret = trChar.translate( dataInt8High );
		BOOST_CHECK_EQUAL( ret, -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_sint8 )
{
	{
		Translator<sint8> trChar;
		sint8 ret = trChar.translate( dataInt8Low );
		BOOST_CHECK_EQUAL( ret, 0 );
	}
	{
		Translator<sint8> trChar;
		sint8 ret = trChar.translate( dataInt8High );
		BOOST_CHECK_EQUAL( ret, -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint8 )
{
	{
		Translator<uint8> trChar;
		uint8 ret = trChar.translate( dataInt8Low );
		BOOST_CHECK_EQUAL( ret, 0 );
	}
	{
		Translator<uint8> trChar;
		uint8 ret = trChar.translate( dataInt8High );
		BOOST_CHECK_EQUAL( ret, 0xff );
	}
}

BOOST_AUTO_TEST_CASE( translator_int16_be )
{
	{
		Translator<int16> trChar;
		int16 ret = trChar.translate( dataInt16Low );
		BOOST_CHECK_EQUAL( ret, 0x00ff );
	}
	{
		Translator<int16> trChar;
		int16 ret = trChar.translate( dataInt16High );
		BOOST_CHECK_EQUAL( ret, -256 );
	}
	{
		Translator<int16> trChar;
		int16 ret = trChar.translate( dataInt16Full );
		BOOST_CHECK_EQUAL( ret, -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint16_be )
{
	{
		Translator<uint16> trChar;
		uint16 ret = trChar.translate( dataInt16Low );
		BOOST_CHECK_EQUAL( ret, 0x00ff );
	}
	{
		Translator<uint16> trChar;
		uint16 ret = trChar.translate( dataInt16High );
		BOOST_CHECK_EQUAL( ret, 65280 );
	}
	{
		Translator<uint16> trChar;
		uint16 ret = trChar.translate( dataInt16Full );
		BOOST_CHECK_EQUAL( ret, 0xffff );
	}
}

BOOST_AUTO_TEST_CASE( translator_int16_le )
{
	{
		Translator<int16> trChar;
		int16 ret = trChar.translate( dataInt16Low, 2, false );
		BOOST_CHECK_EQUAL( ret, -256 );
	}
	{
		Translator<int16> trChar;
		int16 ret = trChar.translate( dataInt16High, 2, false );
		BOOST_CHECK_EQUAL( ret, 255 );
	}
	{
		Translator<int16> trChar;
		int16 ret = trChar.translate( dataInt16Full, 2, false );
		BOOST_CHECK_EQUAL( ret, -1 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint16_le )
{
	{
		Translator<uint16> trChar;
		uint16 ret = trChar.translate( dataInt16Low, 2, false );
		BOOST_CHECK_EQUAL( ret, 65280 );
	}
	{
		Translator<uint16> trChar;
		uint16 ret = trChar.translate( dataInt16High, 2, false );
		BOOST_CHECK_EQUAL( ret, 255 );
	}
	{
		Translator<uint16> trChar;
		uint16 ret = trChar.translate( dataInt16Full, 2, false );
		BOOST_CHECK_EQUAL( ret, 0xffff );
	}
}

BOOST_AUTO_TEST_CASE( translator_int32_be )
{
	{
		Translator<int32> trChar;
		int32 ret = trChar.translate( dataInt32Low );
		BOOST_CHECK_EQUAL( ret, -1 );
	}
	{
		Translator<int32> trChar;
		int32 ret = trChar.translate( dataInt32High );
		BOOST_CHECK_EQUAL( ret, -16777216 );
	}
	{
		Translator<int32> trChar;
		int32 ret = trChar.translate( dataInt32Full );
		BOOST_CHECK_EQUAL( ret, -257 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint32_be )
{
	{
		Translator<uint32> trChar;
		uint32 ret = trChar.translate( dataInt32Low );
		BOOST_CHECK_EQUAL( ret, 255 );
	}
	{
		Translator<uint32> trChar;
		uint32 ret = trChar.translate( dataInt32High );
		BOOST_CHECK_EQUAL( ret, 4278190080 );
	}
	{
		Translator<uint32> trChar;
		uint32 ret = trChar.translate( dataInt32Full );
		BOOST_CHECK_EQUAL( ret, std::numeric_limits<unsigned int>::max() );
	}
}

BOOST_AUTO_TEST_CASE( translator_int32_le )
{
	{
		Translator<int32> trChar;
		int32 ret = trChar.translate( dataInt32Low, 2, false );
		BOOST_CHECK_EQUAL( ret, -16777216 );
	}
	{
		Translator<int32> trChar;
		int32 ret = trChar.translate( dataInt32High, 2, false );
		BOOST_CHECK_EQUAL( ret, -1 );
	}
	{
		Translator<int32> trChar;
		int32 ret = trChar.translate( dataInt32Full, 2, false );
		BOOST_CHECK_EQUAL( ret, -257 );
	}
}

BOOST_AUTO_TEST_CASE( translator_uint32_le )
{
	{
		Translator<uint32> trChar;
		uint32 ret = trChar.translate( dataInt32Low, 2, false );
		BOOST_CHECK_EQUAL( ret, 4278190080 );
	}
	{
		Translator<uint32> trChar;
		uint32 ret = trChar.translate( dataInt32High, 2, false );
		BOOST_CHECK_EQUAL( ret, 255 );
	}
	{
		Translator<uint32> trChar;
		uint32 ret = trChar.translate( dataInt32Full, 2, false );
		BOOST_CHECK_EQUAL( ret, std::numeric_limits<unsigned int>::max() );
	}
}

BOOST_AUTO_TEST_CASE( translator_float_le )
{
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatZero, 0, false );
		BOOST_CHECK_CLOSE( ret, 0.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatOneLittleEndian, 0, false );
		BOOST_CHECK_CLOSE( ret, 1.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatMaxLittleEndian, 0, false );
		BOOST_CHECK_CLOSE( ret, 3.40282e+38, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatNegMaxLittleEndian, 0, false );
		BOOST_CHECK_CLOSE( ret, -3.40282e+38, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatMinLittleEndian, 0, false );
		BOOST_CHECK_CLOSE( ret, 1.17549e-38, 0.001 );
	}
}

BOOST_AUTO_TEST_CASE( translator_float_be )
{
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatZero, 0, true );
		BOOST_CHECK_CLOSE( ret, 0.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatOneBigEndian, 0, true );
		BOOST_CHECK_CLOSE( ret, 1.00, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatMaxBigEndian, 0, true );
		BOOST_CHECK_CLOSE( ret, 3.40282e+38, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatNegMaxBigEndian, 0, true );
		BOOST_CHECK_CLOSE( ret, -3.40282e+38, 0.001 );
	}
	{
		Translator<float> trFloat;
		float ret = trFloat.translate( dataFloatMinBigEndian, 0, true );
		BOOST_CHECK_CLOSE( ret, 1.17549e-38, 0.001 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
