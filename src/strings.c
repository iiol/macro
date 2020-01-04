#include "strings.h"

inline static string
__str_get_start(string str)
{
	size_t i;

	for (i = 1; str[-i] != '\0'; ++i)
		;

	return str - i + 1;
}

inline static struct str_string*
__str_get_header(string str)
{
	str = __str_get_start(str);

	return (struct str_string*)(str - sizeof (struct str_string) - 1);
}

inline static string
str_new(size_t len)
{
	string str;
	struct str_string *head;
	size_t size;

	size = sizeof (struct str_string) + len + 2;
	head = xmalloc(size);
	str = (string)head + sizeof (struct str_string) + 1;

	str[-1] = '\0';
	str[ 0] = '\0';

	head->isdynamic = (len == 0) ? 1 : 0;
	head->len = 0;
	head->bufsize = len;

	return str;
}

inline static void
str_setopt(string str, enum str_opts opt, int val)
{
	struct str_string *head;

	assert(str);

	head = __str_get_header(str);

	switch (opt) {
	case STROPT_DYNAMIC:
		head->isdynamic = (val) ? 1 : 0;
		break;

	default:
		error(1, "Unknown option: 0x%x", opt);
	}
}

inline static string
str_set(string str, const char *s)
{
	struct str_string *head;
	size_t slen;
	size_t size;

	assert(s);

	if (str == NULL)
		str = str_new(0);

	head = __str_get_header(str);
	str = __str_get_start(str);
	slen = strlen(s);

	if (slen > head->bufsize && head->isdynamic) {
		size = sizeof (struct str_string) + slen + 2;
		head = xrealloc(head, size);
		str = (string)head + sizeof (struct str_string) + 1;
		head->bufsize = slen;
	}

	strncpy(str, s, head->bufsize);
	str[head->bufsize] = '\0';

	head->len = strlen(str);

	return str;
}

inline static string
str_cat(string str, const char *src)
{
	struct str_string *head;
	size_t slen;
	size_t size;
	char *s;

	assert(str);
	assert(src);

	head = __str_get_header(str);
	slen = strlen(src);

	s = alloca(slen + 1);
	strcpy(s, src);

	if (slen + head->len > head->bufsize && head->isdynamic) {
		size = sizeof (struct str_string) + slen + head->len + 2;
		head = xrealloc(head, size);
		str = (string)head + sizeof (struct str_string) + 1;
		head->bufsize = head->len + slen;
	}

	strncpy(str + head->len, s, head->bufsize - head->len);
	str[head->bufsize] = '\0';
	head->len = strlen(str);

	return str;
}

inline static string
str_clone(string str)
{
	struct str_string *head, *ret;
	size_t size;

	head = __str_get_header(str);
	size = sizeof (struct str_string) + head->bufsize + 2;
	ret = xmalloc(size);
	memcpy(ret, head, size);

	return (string)ret + sizeof (struct str_string) + 1;
}

inline static string
str_range(string str, size_t start, size_t end)
{
	struct str_string *head;
	char *s;
	size_t len;

	assert(str);

	head = __str_get_header(str);

	if (end > head->len)
		end = head->len;
	if (start > end)
		start = end;

	len = end - start;
	s = alloca(len + 1);

	memcpy(s, str + start, len);
	memcpy(str, s, len);

	str[len] = '\0';
	head->len = len;

	return str;
}

inline static size_t
str_len(string str)
{
	assert(str);

	return __str_get_header(__str_get_start(str))->len;
}

inline static string*
str_split(string str, const char *delim)
{
	size_t i, j;
	size_t count, start;
	string *strs;

	assert(str);
	assert(delim);

	strs = xmalloc(sizeof (string*));
	strs[0] = NULL;
	count = 1;

	for (i = start = 0; str[i] != '\0'; ++i) {
		for (j = 0; delim[j] != '\0'; ++j) {
			if (str[i] != delim[j])
				continue;

			strs = xrealloc(strs, ++count * sizeof (string*));
			strs[count - 1] = NULL;
			strs[count - 2] = str_clone(str);
			strs[count - 2] = str_range(strs[count - 2], start, i);

			start = i + 1;
		}
	}

	strs = xrealloc(strs, ++count * sizeof (string*));
	strs[count - 1] = NULL;
	strs[count - 2] = str_clone(str);
	strs[count - 2] = str_range(strs[count - 2], start, i);

	return strs;
}

inline static void
str_free(string str)
{
	if (str == NULL)
		return;

	free(__str_get_header(str));
}

inline static void
str_arr_free(string *strs)
{
	size_t i;

	if (strs == NULL)
		return;

	for (i = 0; strs[i] != NULL; ++i)
		str_free(strs[i]);

	free(strs);
}
