#ifndef _MACRO_LOG_H
#define _MACRO_LOG_H

#include <sys/time.h>

#include "includes.h"

#undef assert
#if defined(DEBUG_TRACE)
#   define _logit      _logit_trace

#   define prologue    _prologue
#   define return(val) _return(val)

#   define debug       _debug
#   define warning     _warning
#   define perror      _perror
#   define error       _error
#   define assert      _assert
#elif defined(DEBUG_DEV)
#   define _logit      _logit_dev

#   define prologue    nop
#   define return      return

#   define debug       _debug
#   define warning     _warning
#   define perror      _perror
#   define error       _error
#   define assert      _assert
#else // defined(DEBUG_PROD)
#   define _logit      _logit_prod

#   define prologue    nop
#   define return      return

#   define debug       nop
#   define warning     _warning
#   define perror      _perror
#   define error       _error
#   define assert      _assert
#endif

#define _logit_trace(fp, prefix, fmt, ...)				\
do {									\
	struct timeval tv;						\
									\
	if (gettimeofday(&tv, NULL) < 0)				\
		break;							\
									\
	fprintf(fp, ">>%010ld.%06ld %s%7.7s:%-5d %20.20s| " fmt "\n",	\
		tv.tv_sec, tv.tv_usec, prefix, __FILE__, __LINE__,	\
		__func__, ##__VA_ARGS__);				\
	fflush(fp);							\
} while (0)

#define _logit_dev(fp, prefix, fmt, ...)					  \
do {										  \
	fprintf(fp, "%s%20.20s| " fmt "\n", prefix, __FUNCTION__, ##__VA_ARGS__); \
	fflush(fp);								  \
} while (0)

#define _logit_prod(fp, prefix, fmt, ...)			\
do {								\
	fprintf(fp, "%s" fmt "\n", prefix, ##__VA_ARGS__);	\
	fflush(fp);						\
} while (0)

#define _prologue() _logit(stderr, "    ", "[start]")

#define _return(val)				\
do {						\
	_logit(stderr, "    ", "[end]");	\
	return val;				\
} while (0)

#define _debug(fmt, ...)   _logit(stderr, "[D] ", fmt, ##__VA_ARGS__)
#define _warning(fmt, ...) _logit(stderr, "[W] ", fmt, ##__VA_ARGS__)
#define _perror(str)       _logit(stderr, "[E] ", "%s: %s", str, strerror(errno))

#define _error(ecode, fmt, ...)				\
do {							\
	_logit(stderr, "[E] ", fmt, ##__VA_ARGS__);	\
	exit(ecode);					\
} while (0)

#define _assert(exp)					\
do {							\
	if (!(exp)) {					\
		_logit(stderr, "[A] ", "%s", #exp);	\
		exit(1);				\
	}						\
} while (0)

#endif // _MACRO_LOG_H
