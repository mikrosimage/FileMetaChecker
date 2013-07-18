#ifndef _BASIC_ELEMENT_COMMON_HPP_
#define _BASIC_ELEMENT_COMMON_HPP_

#include <common/global.hpp>

// #define LOG_BASIC_ELEMENT

#ifdef  LOG_BASIC_ELEMENT
	#define BE_LOG_TRACE( ... )   LOG_TRACE( __VA_ARGS__ )
	#define BE_LOG_INFO( ... )    LOG_INFO( __VA_ARGS__ )
	#define BE_LOG_WARNING( ... ) LOG_WARNING( __VA_ARGS__ )
	#define BE_LOG_ERROR( ... )   LOG_ERROR( __VA_ARGS__ )
	#define BE_LOG_FATAL( ... )   LOG_FATAL( __VA_ARGS__ )
#else
	#define BE_LOG_TRACE( ... )
	#define BE_LOG_INFO( ... )
	#define BE_LOG_WARNING( ... )
	#define BE_LOG_ERROR( ... )
	#define BE_LOG_FATAL( ... )
#endif

#endif
