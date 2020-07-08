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
	struct test_list *node;
	struct test_list *prev;

	for (i = 0, node = NULL; i < 5; ++i) {
		node = list_alloc_at_end(node);
		node->num = i;
	}

	head = list_get_head(node);
	prev = NULL;

	list_foreach (head, node) {
		CU_ASSERT(list_get_prev(node) == prev);
		prev = node;
	}

	list_free_full(head);
}

static void
invalid_cases(void)
{
	CU_ASSERT(list_get_prev(NULL) == NULL);
}

bool
list_get_prev_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
