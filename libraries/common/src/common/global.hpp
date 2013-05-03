#ifndef _COMMON_GLOBAL_HPP_
#define _COMMON_GLOBAL_HPP_

////////////////////////////////////////////////////////////////////////////////
// System stuff
#include <common/system/system.hpp>
#include <common/system/compatibility.hpp>
#include <common/system/windows/windows.h>

#include "Formatters.hpp"

////////////////////////////////////////////////////////////////////////////////
// Assert needs to be everywhere
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/current_function.hpp>

#include <boost/log/trivial.hpp>

////////////////////////////////////////////////////////////////////////////////
// Define functions to display infos in the console
#include <iostream>

#ifdef NDEBUG
#  if defined( _MSC_VER )
#    define LOG_FORCEINLINE __forceinline
#  elif defined( __GNUC__ ) && __GNUC__ > 3
#    define LOG_FORCEINLINE inline __attribute__ ( ( always_inline ) )
#  else
#    define LOG_FORCEINLINE inline
#  endif
#else
#  define LOG_FORCEINLINE inline
#endif

#ifndef LOG

/**
 * @def   COMMON_INFOS
 * @brief informations : filename, line number, function name
 **/
#define GET_INFOS_FILE      "in file:  " << __FILE__ << ",  line: " << __LINE__
#define GET_INFOS_FUNCTION  "function: " << BOOST_CURRENT_FUNCTION
#define GET_INFOS           GET_INFOS_FILE << GET_INFOS_FUNCTION

#define GET_VAR ( a )          # a << ": " << a
#define GET_VAR2( a, b )       GET_VAR ( a ) << ", " << GET_VAR ( b )
#define GET_VAR3( a, b, c )    GET_VAR ( a ) << ", " << GET_VAR ( b ) << ", " << GET_VAR ( c )
#define GET_VAR4( a, b, c, d ) GET_VAR ( a ) << ", " << GET_VAR ( b ) << ", " << GET_VAR ( c ) << ", " << GET_VAR ( d )

#define LOG_TRACE( ... )   BOOST_LOG_TRIVIAL(trace) << __VA_ARGS__
#define LOG_INFO( ... )    BOOST_LOG_TRIVIAL(info) << __VA_ARGS__
#define LOG_WARNING( ... ) BOOST_LOG_TRIVIAL(warning) << common::Color::get()->_yellow << __VA_ARGS__ << common::Color::get()->_std
#define LOG_ERROR( ... )   BOOST_LOG_TRIVIAL(error)   << common::Color::get()->_error  << __VA_ARGS__ << common::Color::get()->_std
#define LOG_FATAL( ... )   BOOST_LOG_TRIVIAL(fatal)   << common::Color::get()->_error  << __VA_ARGS__ << common::Color::get()->_std

#define LOG( MODE, ... ) BOOST_LOG_TRIVIAL( MODE )   << __VA_ARGS__

#define LOG_VAR( MODE, a )          LOG( MODE, GET_VAR ( a ) )
#define LOG_VAR2( MODE, a, b )       LOG( MODE, GET_VAR2( a, b ) )
#define LOG_VAR3( MODE, a, b, c )    LOG( MODE, GET_VAR3( a, b, c ) )
#define LOG_VAR4( MODE, a, b, c, d ) LOG( MODE, GET_VAR4( a, b, c, d ) )

#define LOG_X( N, ... ) \
	for( std::size_t i = 0; i < N; ++i ) { ::std::cout << __VA_ARGS__; } \
	::std::cout << ::std::endl

/**
 * @brief terminal information display
 **/
#define LOG_INFOS LOG( GET_INFOS )

/**
 * @param[in] ... : all parameters with an operator << defined
 * @brief terminal information display
 **/
#define LOG_WITHINFOS(... )  \
	LOG( LOG_INFOS << \
		  ::std::endl << "\t" << __VA_ARGS__ )

#endif

////////////////////////////////////////////////////////////////////////////////
// Some specifics things to debug or release version
#ifdef DEBUG
 #include "debug.hpp"
#else
 #include "release.hpp"
#endif

////////////////////////////////////////////////////////////////////////////////
// TLOG_* defines are used by developers for temporary displays during development stages.
// They are removed in production mode.

#ifndef COMMON_PRODUCTION
	#define TLOG           LOG
	#define TLOG_X         LOG_X
	#define TLOG_VAR       LOG_VAR
	#define TLOG_VAR2      LOG_VAR2
	#define TLOG_VAR3      LOG_VAR3
	#define TLOG_VAR4      LOG_VAR4
	#define TLOG_INFOS     LOG_INFOS
	#define TLOG_WITHINFOS LOG_WITHINFOS
	#define TLOG_EXCEPTION LOG_EXCEPTION
#else
	#define TLOG           LOG_DEBUG
	#define TLOG_X         LOG_X_DEBUG
	#define TLOG_VAR       LOG_VAR_DEBUG
	#define TLOG_VAR2      LOG_VAR2_DEBUG
	#define TLOG_VAR3      LOG_VAR3_DEBUG
	#define TLOG_VAR4      LOG_VAR4_DEBUG
	#define TLOG_INFOS     LOG_INFOS_DEBUG
	#define TLOG_WITHINFOS LOG_WITHINFOS_DEBUG
	#define TLOG_EXCEPTION LOG_EXCEPTION_DEBUG
#endif

#endif
