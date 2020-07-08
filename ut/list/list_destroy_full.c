#include <stdio.h>
#include <stdbool.h>

#include <CUnit/CUnit.h>

#include "macro.h"

#define ENTRY_COUNT 5

struct test_list {
	int num;
};

bool cleaned[ENTRY_COUNT];

static void
destroy_func(void *entry)
{
	struct test_list *node = entry;

	CU_ASSERT(cleaned[node->num] == false);
	cleaned[node->num] = true;
}

static void
valid_cases(void)
{
	int i;
	struct test_list *node;

	list_setfunc_destroy(destroy_func);

	for (i = 0, node = NULL; i < ENTRY_COUNT; ++i) {
		node = list_alloc_at_end(node);
		node->num = i;
	}

	list_destroy_full(node);

	for (i = 0; i < ENTRY_COUNT; ++i)
		CU_ASSERT(cleaned[i]);

}

static void
invalid_cases(void)
{
	list_destroy_full(NULL);
	CU_ASSERT(true);
}

bool
list_destroy_full_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
