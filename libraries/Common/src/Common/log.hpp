#ifndef _COMMON_LOG_HPP_
#define _COMMON_LOG_HPP_

#include "color.hpp"

#include <iostream>

namespace common
{

/**
 * Log display levels definition
 */
enum ELogLevel
{
	eLogTrace = 0,
	eLogInfo,
	eLogWarning,
	eLogError,
	eLogFatal,
};

static ELogLevel level = eLogWarning;

}

#define LOG_TRACE( ... )   if( common::level <= common::eLogTrace   ) std::cout << __VA_ARGS__ << std::endl
#define LOG_INFO( ... )    if( common::level <= common::eLogInfo    ) std::cout << __VA_ARGS__ << std::endl
#define LOG_WARNING( ... ) if( common::level <= common::eLogWarning ) std::cout << common::details::kColorYellow  << "Warning: " << __VA_ARGS__ << common::details::kColorStd << std::endl
#define LOG_ERROR( ... )   if( common::level <= common::eLogError   ) std::cout << common::details::kColorRed     << "Error: "   << __VA_ARGS__ << common::details::kColorStd << std::endl
#define LOG_FATAL( ... )   if( common::level <= common::eLogFatal   ) std::cout << common::details::kColorBoldRed << "Fatal: "   << __VA_ARGS__ << common::details::kColorStd << std::endl

#define LOG( ... )              std::cout << __VA_ARGS__

#define LOG_COLOR( color, ... ) LOG( color << __VA_ARGS__ << common::details::kColorStd )

#define LOG_ENDL()              std::cout << std::endl

#endif
