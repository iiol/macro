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
	struct test_list *head;
	struct test_list *tail;

	for (i = 0, tail = NULL; i < 5; ++i) {
		tail = list_alloc_at_end(tail);
		tail->num = i;
	}

	head = list_get_head(tail);

	for (; tail != NULL; ++i, tail = list_get_prev(tail))
		CU_ASSERT(head == list_get_head(tail));

	list_free_full(head);
}

static void
invalid_cases(void)
{
	CU_ASSERT(list_get_head(NULL) == NULL);
}

bool
list_get_head_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
