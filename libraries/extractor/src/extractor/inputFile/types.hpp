#ifndef _EXTRACTOR_INPUT_FILE_TYPE_HPP_
#define _EXTRACTOR_INPUT_FILE_TYPE_HPP_

#include <string>

typedef          char       int8;
typedef   signed char      sint8;
typedef unsigned char      uint8;
typedef          short      int16;
typedef unsigned short     uint16;
typedef          int        int32;
typedef unsigned int       uint32;
typedef          long long  int64;
typedef   signed long long  int64;
typedef unsigned long long uint64;

struct Hexa{
	std::string value;
};
typedef struct Hexa Hexa;

struct Ascii{
	std::string originalCaseValue;
	std::string lowCaseValue;
	std::string upCaseValue;
};
typedef struct Ascii Ascii;

#endif
