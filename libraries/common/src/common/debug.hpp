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

/// @see LOG
#define LOG_DEBUG            LOG_INFO

/// @see  LOG_INFOS
#define LOG_INFOS_DEBUG      LOG_INFOS

/// @see  LOG_INFOS
#define LOG_WITHINFOS_DEBUG  LOG_WITHINFOS

/// @see COMMON_IF_DEBUG
#define LOG_IF_DEBUG(... )  __VA_ARGS__

#define LOG_X_DEBUG          LOG_X
#define LOG_VAR_DEBUG        LOG_VAR
#define LOG_VAR2_DEBUG       LOG_VAR2
#define LOG_VAR3_DEBUG       LOG_VAR3
#define LOG_VAR4_DEBUG       LOG_VAR4
#define LOG_INFOS_DEBUG      LOG_INFOS
#define LOG_WITHINFOS_DEBUG  LOG_WITHINFOS
#define LOG_WARNING_DEBUG    LOG_WARNING
#define LOG_ERROR_DEBUG      LOG_ERROR
#define LOG_FATALERROR_DEBUG LOG_FATALERROR
#define LOG_EXCEPTION_DEBUG  LOG_EXCEPTION

#endif
