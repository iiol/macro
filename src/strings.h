#ifndef _MACRO_STRINGS_H
#define _MACRO_STRINGS_H

#include "includes.h"
#include "log.h"
#include "misc.h"

typedef char* string;

enum str_opts {
	STROPT_DYNAMIC,
};

struct str_string {
	int isdynamic;
	size_t len;
	size_t bufsize;
};

inline static string str_new(size_t len);
inline static void str_setopt(string str, enum str_opts opt, int val);
inline static string str_set(string str, const char *s);
inline static string str_cat(string str, const char *src);
inline static string str_clone(string str);
inline static string str_range(string str, size_t start, size_t end);
inline static size_t str_len(string str);
inline static string* str_split(string str, const char *delim);
inline static void str_free(string str);
inline static void str_arr_free(string *strs);

#endif // _MACRO_STRINGS_H
