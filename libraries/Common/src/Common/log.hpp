#ifndef _COMMON_LOG_HPP_
#define _COMMON_LOG_HPP_

#include "color.hpp"

#include <iostream>

namespace common
{

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

#define LOG_TRACE( __ARGS__ )   if( common::level <= common::eLogTrace   ) std::cout << ( __ARGS__ ) << std::endl
#define LOG_INFO( __ARGS__ )    if( common::level <= common::eLogInfo    ) std::cout << ( __ARGS__ ) << std::endl
#define LOG_WARNING( __ARGS__ ) if( common::level <= common::eLogWarning ) std::cout << common::details::kColorYellow  << ( __ARGS__ ) << common::details::kColorStd << std::endl
#define LOG_ERROR( __ARGS__ )   if( common::level <= common::eLogError   ) std::cout << common::details::kColorRed     << ( __ARGS__ ) << common::details::kColorStd << std::endl
#define LOG_FATAL( __ARGS__ )   if( common::level <= common::eLogFatal   ) std::cout << common::details::kColorBoldRed << ( __ARGS__ ) << common::details::kColorStd << std::endl


#endif
