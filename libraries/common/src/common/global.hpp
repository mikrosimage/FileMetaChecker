#ifndef _COMMON_GLOBAL_HPP_
#define _COMMON_GLOBAL_HPP_

//#define COMMON_NO_COUT

////////////////////////////////////////////////////////////////////////////////
// System stuff
#include <common/system/system.hpp>
#include <common/system/compatibility.hpp>
#include <common/system/windows/windows.h>

#include "color.hpp"

////////////////////////////////////////////////////////////////////////////////
// Assert needs to be everywhere
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/current_function.hpp>

////////////////////////////////////////////////////////////////////////////////
// Define functions to display infos in the console
#include <iostream>

#ifdef NDEBUG
#  if defined( _MSC_VER )
#    define COMMON_FORCEINLINE __forceinline
#  elif defined( __GNUC__ ) && __GNUC__ > 3
#    define COMMON_FORCEINLINE inline __attribute__ ( ( always_inline ) )
#  else
#    define COMMON_FORCEINLINE inline
#  endif
#else
#  define COMMON_FORCEINLINE inline
#endif

#ifndef COMMON_COUT

/**
 * @def   COMMON_INFOS
 * @brief informations : filename, line number, function name
 **/
 #define COMMON_INFOS  "file: " << __FILE__ << ",  line: " << __LINE__ << ::std::endl << "function: " << BOOST_CURRENT_FUNCTION

 #define COMMON_VAR( a )  # a << ": " << a
 #define COMMON_VAR2( a, b )  # a << ": " << a << ", " << # b << ": " << b
 #define COMMON_VAR3( a, b, c )  # a << ": " << a << ", " << # b << ": " << b << ", " << # c << ": " << c
 #define COMMON_VAR4( a, b, c, d )  # a << ": " << a << ", " << # b << ": " << b << ", " << # c << ": " << c << ", " << # d << ": " << d
 #define COMMON_VAR_ENDL( a )  # a << ":" << ::std::endl << a

#ifndef COMMON_NO_COUT
/**
 * @param[in] ... : all parameters with an operator << defined
 * @brief terminal display
 **/
 #define COMMON_COUT(... )  ::std::cout << __VA_ARGS__ << ::std::endl
 #define COMMON_CERR(... )  ::std::cerr << __VA_ARGS__ << ::std::endl

 #define COMMON_COUT_X( N, ... ) \
	for( std::size_t i = 0; i < N; ++i ) { ::std::cout << __VA_ARGS__; } \
	::std::cout << ::std::endl

#else
 #define COMMON_COUT(...)
 #define COMMON_CERR(...)
 #define COMMON_COUT_X( N, ... )
#endif


 #define COMMON_COUT_VAR( a )  COMMON_COUT( COMMON_VAR( a ) )
 #define COMMON_COUT_VAR2( a, b )  COMMON_COUT( COMMON_VAR2( a, b ) )
 #define COMMON_COUT_VAR3( a, b, c )  COMMON_COUT( COMMON_VAR3( a, b, c ) )
 #define COMMON_COUT_VAR4( a, b, c, d )  COMMON_COUT( COMMON_VAR4( a, b, c, d ) )

/**
 * @brief terminal information display
 **/
 #define COMMON_COUT_INFOS COMMON_COUT( COMMON_INFOS )

/**
 * @param[in] ... : all parameters with an operator << defined
 * @brief terminal information display
 **/
 #define COMMON_COUT_WITHINFOS(... )  \
	COMMON_COUT( COMMON_INFOS << \
		  ::std::endl << "\t" << __VA_ARGS__ )

 #define COMMON_COUT_WARNING(... )  \
	COMMON_CERR( "Quality Check - Warning:" << \
	::std::endl << COMMON_INFOS << \
	::std::endl << "\t" << __VA_ARGS__ )

 #define COMMON_COUT_ERROR(... )  \
	COMMON_CERR( common::details::kColorError << "Quality Check - Error:" << \
	::std::endl << COMMON_INFOS << \
	::std::endl << "\t" << __VA_ARGS__ << common::details::kColorStd )

 #define COMMON_COUT_FATALERROR(... )  \
	COMMON_CERR( common::details::kColorError << "Quality Check - Fatal error:" << \
	::std::endl << COMMON_INFOS << \
	::std::endl << "\t" << __VA_ARGS__ << common::details::kColorStd )

#endif

////////////////////////////////////////////////////////////////////////////////
// Some specifics things to debug or release version
#ifdef DEBUG
 #include "debug.hpp"
#else
 #include "release.hpp"
#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON_TCOUT* defines are used by developers for temporary displays during development stages.
// They are removed in production mode.
#ifndef COMMON_PRODUCTION
	#define COMMON_TCOUT COMMON_COUT
	#define COMMON_TCOUT_X COMMON_COUT_X
	#define COMMON_TCOUT_VAR COMMON_COUT_VAR
	#define COMMON_TCOUT_VAR2 COMMON_COUT_VAR2
	#define COMMON_TCOUT_VAR3 COMMON_COUT_VAR3
	#define COMMON_TCOUT_VAR4 COMMON_COUT_VAR4
	#define COMMON_TCOUT_INFOS COMMON_COUT_INFOS
	#define COMMON_TCOUT_WITHINFOS COMMON_COUT_WITHINFOS
	#define COMMON_TCOUT_EXCEPTION COMMON_COUT_EXCEPTION
#else
	#define COMMON_TCOUT COMMON_COUT_DEBUG
	#define COMMON_TCOUT_X COMMON_COUT_X_DEBUG
	#define COMMON_TCOUT_VAR COMMON_COUT_VAR_DEBUG
	#define COMMON_TCOUT_VAR2 COMMON_COUT_VAR2_DEBUG
	#define COMMON_TCOUT_VAR3 COMMON_COUT_VAR3_DEBUG
	#define COMMON_TCOUT_VAR4 COMMON_COUT_VAR4_DEBUG
	#define COMMON_TCOUT_INFOS COMMON_COUT_INFOS_DEBUG
	#define COMMON_TCOUT_WITHINFOS COMMON_COUT_WITHINFOS_DEBUG
	#define COMMON_TCOUT_EXCEPTION COMMON_COUT_EXCEPTION_DEBUG
#endif

#endif
