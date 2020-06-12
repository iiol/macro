#ifndef _MACRO_MISC_H
#define _MACRO_MISC_H

#include "includes.h"

#define typeof(a) __typeof__(a)

#define sqr(x) ((double)(x)*(x))

#define UNUSED(x) (void)x

#define SWAP(a, b) do {typeof (a) __tmp = a; a = b; b = __tmp;} while (0)
#define SWAP_MW(a, b) do {a ^= b; b ^= a; a ^= b;} while (0)

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a > b) ? b : a)

#define SYSCALL(rc, ecode, func, ...)					\
do {									\
	int _;								\
	(void)_;							\
									\
	while ((_ = ((func)(__VA_ARGS__))) < 0 && errno == EINTR)	\
		;							\
									\
	if (_ < 0) {							\
		char str[] = #func "(" #__VA_ARGS__ ")";		\
									\
		if (ecode == 0)						\
			warning("%s%s", str, strerror(errno));		\
		else							\
			error(ecode, "%s%s", str, strerror(errno));	\
	}								\
} while (0)

inline static void* xmalloc(size_t size);
inline static void* xrealloc(void *ptr, size_t size);

#endif // _MACRO_MISC_H
