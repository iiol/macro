#include "misc.h"

inline static void*
xmalloc(size_t size)
{
	void *ret;

	errno = 0;
	ret = malloc(size);

	if (size != 0 && ret == NULL) {
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	return ret;
}

inline static void*
xrealloc(void *ptr, size_t size)
{
	void *ret;

	ret = realloc(ptr, size);

	if (size != 0 && ret == NULL) {
		perror("realloc()");
		exit(EXIT_FAILURE);
	}

	return ret;
}
