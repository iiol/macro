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
	struct test_list *list = list_empty(typeof (*list));
	struct test_list *entry;

	for (i = 0; i < 5; ++i) {
		list = list_alloc_at_end(list);
		list->num = i;
	}

	i = 0;
	list_foreach(list_get_head(list), entry) {
		CU_ASSERT_FATAL(entry->num == i);
		++i;
	}

	CU_ASSERT(i == 5);
	list_free_full(list);

	list = list_empty(typeof (*list));

	for (i = 0; i < 5; ++i) {
		list = list_alloc_at_start(list);
		list->num = i;
	}

	i = 0;
	list_rofeach(list_get_tail(list), entry) {
		CU_ASSERT_FATAL(entry->num == i);
		++i;
	}

	CU_ASSERT(i == 5);
	list_free_full(list);
}

bool
list_empty_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;

	return true;
}
