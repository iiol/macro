#include "macro.h"

struct foo {
	int a;
	float b;
	char *str;
};

void
list_test(void)
{
	struct foo *head, *entry = NULL;

	entry = list_alloc_at_end(entry);
	entry->a = 10;
	entry->b = 10.01;
	entry->str = "1";

	entry = list_alloc_at_end(entry);
	entry->a = 100;
	entry->b = 100.001;
	entry->str = "2";

	entry = list_alloc_prev(entry);
	entry->a = 50;
	entry->b = 50.05;
	entry->str = "Hello";

	entry = list_alloc_at_start(entry);
	entry->a = 1;
	entry->b = 1.1;
	entry->str = "4";

	entry = list_alloc_at_start(entry);
	entry->a = 0;
	entry->b = 0.0;
	entry->str = "5";

	head = list_get_head(entry);
	list_foreach(head, entry)
		debug("%3d, %7.3f", entry->a, entry->b);

	entry = list_search_by_str(head, str, "Hello");
	debug("Find: '%s', %3d, %7.3f", entry->str, entry->a, entry->b);
	entry = list_search_by_elem(head, b, 10.01f);
	debug("Find: '%s', %3d, %7.3f", entry->str, entry->a, entry->b);

	list_destroy(head);
}

void
string_test(void)
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
}

int
main(void)
{
	list_test();
	putchar('\n');
	string_test();

	return 0;
}
