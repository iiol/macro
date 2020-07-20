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
	struct test_list *a = NULL, *b = NULL;
	struct test_list *entry;

	for (i = 0; i < 5; ++i) {
		a = list_alloc_at_end(a);
		b = list_alloc_at_end(b);
		a->num = i;
		b->num = i + 5;
	}

	a = list_get_head(a);
	b = list_get_head(b);
	a = list_merge(a, b, LIST_AT_END);

	i = 0;
	list_foreach(list_get_head(a), entry) {
		CU_ASSERT_FATAL(entry->num == i);
		++i;
	}

	CU_ASSERT(i == 10);

	list_free_full(a);
}

static void
invalid_cases(void)
{
	struct test_list *p = NULL;

	p = list_alloc_at_end(p);

	CU_ASSERT(!list_merge(NULL, NULL, 0));
	CU_ASSERT(list_merge(p, NULL, 0) == p);
	CU_ASSERT(list_merge(NULL, p, 0) == p);
}

bool
list_merge_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
