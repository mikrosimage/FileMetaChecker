#ifndef _COMMON_RELEASE_HPP_
#define _COMMON_RELEASE_HPP_

/*
 * In release mode, LOG_*_DEBUG are disabled.
 */

/// @see LOG
#define LOG_DEBUG( ... )
/// @see  LOG_INFOS
#define LOG_INFOS_DEBUG
/// @see  LOG_INFOS
#define LOG_WITHINFOS_DEBUG( ... )
/// @see LOG_IF_DEBUG
#define LOG_IF_DEBUG( ... )

#define LOG_X_DEBUG( N, ... )
#define LOG_VAR_DEBUG( ... )
#define LOG_VAR2_DEBUG( ... )
#define LOG_VAR3_DEBUG( ... )
#define LOG_VAR4_DEBUG( ... )
#define LOG_WITHINFOS_DEBUG( ... )
#define LOG_WARNING_DEBUG( ... )
#define LOG_ERROR_DEBUG( ... )
#define LOG_FATALERROR_DEBUG( ... )
#define LOG_EXCEPTION_DEBUG( ... )

#endif
