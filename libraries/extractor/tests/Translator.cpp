#include <extractor/inputFile/Translator.hpp>

#include <iostream>
#include <limits>

#define BOOST_TEST_MODULE extractor_translator
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;
using namespace std;

const char max_char = std::numeric_limits<unsigned char>::max();
const char min_char = std::numeric_limits<unsigned char>::min();

const char dataInt8[] = { max_char };

const char dataInt16Low[]  = { min_char, max_char };
const char dataInt16High[] = { max_char, min_char };
const char dataInt16Full[] = { max_char, max_char };

const char dataInt32Low[]  = { min_char, min_char, min_char, max_char };
const char dataInt32High[] = { max_char, min_char, min_char, min_char };
const char dataInt32Full[] = { max_char, max_char, max_char, max_char };

BOOST_AUTO_TEST_SUITE( translator_tests_suite01 )

BOOST_AUTO_TEST_CASE( translator_int8 )
{
    Translator<int8> trChar;
    int8 ret = trChar.translate( dataInt8 );
    BOOST_CHECK_EQUAL( ret, -1 );
}

BOOST_AUTO_TEST_CASE( translator_sint8 )
{
    Translator<sint8> trChar;
    sint8 ret = trChar.translate( dataInt8 );
    BOOST_CHECK_EQUAL( ret, -1 );
}

BOOST_AUTO_TEST_CASE( translator_uint8 )
{
    Translator<uint8> trChar;
    uint8 ret = trChar.translate( dataInt8 );
    BOOST_CHECK_EQUAL( ret, 0xff );
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
        BOOST_CHECK_EQUAL( ret, -16843009 );
    }
}

BOOST_AUTO_TEST_CASE( translator_uint32_be )
{
    {
        Translator<uint32> trChar;
        uint32 ret = trChar.translate( dataInt32Low );
        BOOST_CHECK_EQUAL( ret, 4294967295 );
    }
    {
        Translator<uint32> trChar;
        uint32 ret = trChar.translate( dataInt32High );
        BOOST_CHECK_EQUAL( ret, 4278190080 );
    }
    {
        Translator<uint32> trChar;
        uint32 ret = trChar.translate( dataInt32Full );
        BOOST_CHECK_EQUAL( ret, 4278124287 );
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
        BOOST_CHECK_EQUAL( ret, -16843009 );
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
        BOOST_CHECK_EQUAL( ret, 4294967295 );
    }
    {
        Translator<uint32> trChar;
        uint32 ret = trChar.translate( dataInt32Full, 2, false );
        BOOST_CHECK_EQUAL( ret, 4278124287 );
    }
}

BOOST_AUTO_TEST_SUITE_END()
