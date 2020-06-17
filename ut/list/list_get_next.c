#include <stdio.h>
#include <stdbool.h>

#include <CUnit/CUnit.h>

#include "test.h"
#include "macro.h"

struct test_list {
	int num;
};

static void
valid_cases(void)
{
	int i;
	struct test_list *tail;
	struct test_list *node;
	struct test_list *next;

	for (i = 0, node = NULL; i < 5; ++i) {
		node = list_alloc_at_end(node);
		node->num = i;
	}

	tail = list_get_tail(node);
	next = NULL;

	list_rofeach (tail, node) {
		CU_ASSERT(list_get_next(node) == next);
		next = node;
	}

	list_free_full(tail);
}

static void
invalid_cases(void)
{
	CU_ASSERT(list_get_prev(NULL) == NULL);
}

bool
list_get_next_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
