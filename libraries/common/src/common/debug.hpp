#ifndef _COMMON_DEBUG_HPP_
#define _COMMON_DEBUG_HPP_

// pre-processeur directives :
//	# : convert to string
//	## : concatenate (or before __VA_ARGS__ to indicate that it may be empty)
//	__FILE__ : filename
//	__LINE__ : line number
//	__FUNCTION__ : function declaration
//	__PRETTY_FUNCTION__ : function name
//	__DATE__ : "Mmm dd yyyy"
//	__TIME__ : "hh:mm:ss"

//_____________________________________________________________________________
// Macros to output on terminal only in debug mode

/// @see COMMON_COUT
#define COMMON_COUT_DEBUG COMMON_COUT

/// @see  COMMON_COUT_INFOS
#define COMMON_COUT_INFOS_DEBUG COMMON_COUT_INFOS

/// @see  COMMON_COUT_INFOS
#define COMMON_COUT_WITHINFOS_DEBUG COMMON_COUT_WITHINFOS

/// @see COMMON_IF_DEBUG
#define COMMON_IF_DEBUG(... )  __VA_ARGS__

#define COMMON_COUT_X_DEBUG COMMON_COUT_X
#define COMMON_COUT_VAR_DEBUG COMMON_COUT_VAR
#define COMMON_COUT_VAR2_DEBUG COMMON_COUT_VAR2
#define COMMON_COUT_VAR3_DEBUG COMMON_COUT_VAR3
#define COMMON_COUT_VAR4_DEBUG COMMON_COUT_VAR4
#define COMMON_COUT_INFOS_DEBUG COMMON_COUT_INFOS
#define COMMON_COUT_WITHINFOS_DEBUG COMMON_COUT_WITHINFOS
#define COMMON_COUT_WARNING_DEBUG COMMON_COUT_WARNING
#define COMMON_COUT_ERROR_DEBUG COMMON_COUT_ERROR
#define COMMON_COUT_FATALERROR_DEBUG COMMON_COUT_FATALERROR
#define COMMON_COUT_EXCEPTION_DEBUG COMMON_COUT_EXCEPTION

#endif
