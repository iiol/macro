#include <stdio.h>
#include <stdbool.h>

#include <CUnit/CUnit.h>

#include "macro.h"

struct test_list {
	int num;
};

static void
valid_cases(void)
{
	int i;
	struct test_list *list, *tsil, *entry;

	for (i = 0, list = NULL; i < 5; ++i) {
		list = list_alloc_at_end(list);
		list->num = i;
	}

	i = 0;
	tsil = list_dup(list);
	tsil = list_reverse(tsil);

	list_foreach (list_get_head(list), entry) {
		CU_ASSERT_FATAL(entry->num == i);
		++i;
	}

	CU_ASSERT_FATAL(i == 5);

	list_foreach(list_get_head(tsil), entry) {
		--i;
		CU_ASSERT_FATAL(entry->num == i);
	}

	CU_ASSERT_FATAL(i == 0);

	list_free_full(list);
	list_free_full(tsil);
}

static void
invalid_cases(void)
{
	CU_ASSERT(!list_reverse(NULL));
}

bool
list_reverse_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
