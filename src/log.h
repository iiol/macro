#ifndef _MACRO_LOG_H
#define _MACRO_LOG_H

#include "includes.h"

#ifndef FULL_LOG
#define _int_logit _int_logit_short
#else
#define _int_logit _int_logit_full
#endif

#define _int_logit_short(fp, prefix, fmt, ...)				\
do {									\
	fprintf(fp, prefix "%20.20s| " fmt "\n",			\
	    __FUNCTION__, ##__VA_ARGS__);				\
	fflush(fp);							\
} while (0)

#define _int_logit_full(fp, prefix, fmt, ...)				\
do {									\
	fprintf(fp, prefix "%7.7s:%-5d %20.20s| " fmt "\n",		\
	    __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);		\
	fflush(fp);							\
} while (0)

#define debug(fmt, ...)   _int_logit(stderr, "[D] ", fmt, ##__VA_ARGS__)
#define warning(fmt, ...) _int_logit(stderr, "[W] ", fmt, ##__VA_ARGS__)

#define error(ecode, fmt, ...)						\
do {									\
	_int_logit(stderr, "[E] ", fmt, ##__VA_ARGS__);			\
	exit(ecode);							\
} while (0)

#define perror(str)							\
do {									\
	_int_logit(stderr, "[E] ", "%s: %s", str, strerror(errno));	\
} while (0)

#undef assert
#define assert(exp)							\
do {									\
	if (!(exp)) {							\
		_int_logit_full(stderr, "[A] ", "%s", #exp);		\
		exit(1);						\
	}								\
} while (0)

#endif // _MACRO_LOG_H
