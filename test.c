#include "macro.h"

int
main(void)
{
	string str, str2;
	int i;
	string *strs;

	str = str_set(NULL, "Hi World");
	debug("str1 [%2ld] '%s'", str_len(str), str);

	str = str_cat(str, str);
	debug("str1 [%2ld] '%s'", str_len(str), str);

	str = str_set(str, "Hello World!!!");
	debug("str1 [%2ld] '%s'", str_len(str), str);

	str = str_set(str, "Hello");
	debug("str1 [%2ld] '%s'", str_len(str), str);

	str_setopt(str, STROPT_DYNAMIC, 0);

	str = str_set(str, "Hello brave new world");
	debug("str1 [%2ld] '%s'", str_len(str), str);

	str2 = str_clone(str);
	str2 = str_range(str2, 6, 10);

	debug("Clone first string to second string and get range [6,10)");
	debug("str2 [%2ld] '%s'", str_len(str2), str2);

	debug("Split first string with delimeter ' '");

	strs = str_split(str, " ");

	for (i = 0; strs[i] != NULL; ++i)
		debug("[%2ld] '%s'", str_len(strs[i]), strs[i]);

	str_free(str);
	str_free(str2);
	str_arr_free(strs);

	return 0;
}
