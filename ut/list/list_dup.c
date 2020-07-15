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
	struct test_list *list, *dup;
	struct test_list *entry, *dup_entry;

	for (i = 0, list = NULL; i < 5; ++i) {
		list = list_alloc_at_end(list);
		list->num = i;
	}

	list = entry = list_get_head(list);
	dup = dup_entry = list_dup(list);

	list_foreach(list, entry) {
		CU_ASSERT_FATAL(!memcmp(entry, dup_entry, sizeof (*entry)));
		dup_entry = list_get_next(dup_entry);
	}

	entry = list;
	list_foreach(dup, dup_entry) {
		CU_ASSERT_FATAL(!memcmp(entry, dup_entry, sizeof (*entry)));
		entry = list_get_next(entry);
	}

	list_free_full(list);
	list_free_full(dup);
}

static void
invalid_cases(void)
{
	CU_ASSERT(!list_dup(NULL));
}

bool
list_dup_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
