#ifndef _COMMON_COLOR_HPP_
#define _COMMON_COLOR_HPP_

#include <string>

namespace common
{

namespace details
{

static const std::string kColorStd      ( "\x1b[0m" );

static const std::string kColorBlack    ( "\x1b[30m" );
static const std::string kColorRed      ( "\x1b[31m" );
static const std::string kColorGreen    ( "\x1b[32m" );
static const std::string kColorYellow   ( "\x1b[33m" );
static const std::string kColorBlue     ( "\x1b[34m" );
static const std::string kColorMagenta  ( "\x1b[35m" );
static const std::string kColorCyan     ( "\x1b[36m" );
static const std::string kColorWhite    ( "\x1b[37m" );

static const std::string kColorBoldBlack    ( "\x1b[30;1m" );
static const std::string kColorBoldRed      ( "\x1b[31;1m" );
static const std::string kColorBoldGreen    ( "\x1b[32;1m" );
static const std::string kColorBoldYellow   ( "\x1b[33;1m" );
static const std::string kColorBoldBlue     ( "\x1b[34;1m" );
static const std::string kColorBoldMagenta  ( "\x1b[35;1m" );
static const std::string kColorBoldCyan     ( "\x1b[36;1m" );
static const std::string kColorBoldWhite    ( "\x1b[37;1m" );

}

}

#endif
