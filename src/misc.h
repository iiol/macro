#ifndef _MACRO_MISC_H
#define _MACRO_MISC_H

#include "includes.h"

#define typeof(a) __typeof__(a)

#define sqr(x) ((double)(x)*(x))

#define UNUSED(x) (void)x

#define SWAP(a, b) do {typeof (a) __tmp = a; a = b; b = __tmp;} while (0)
#define SWAP_MW(a, b) do {a ^= b; b ^= a; a ^= b;} while(0)

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a > b) ? b : a)

#define SYSCALL(estat, func, ...)					\
({									\
	int __ret;							\
									\
									\
	while ((__ret = ((func)(__VA_ARGS__))) == -1 && errno == EINTR)	\
		;							\
									\
	if (__ret == -1) {						\
		perror(#func "(" #__VA_ARGS__ ")");			\
		if (estat != 0)						\
			exit(estat);					\
	}								\
									\
	__ret;								\
})

inline static void* xmalloc(size_t size);
inline static void* xrealloc(void *ptr, size_t size);

#endif // _MACRO_MISC_H
