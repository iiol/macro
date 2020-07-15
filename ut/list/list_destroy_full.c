#include <stdio.h>
#include <stdbool.h>

#include <CUnit/CUnit.h>

#include "macro.h"

#define ENTRY_COUNT 5

struct test_list {
	int num;
};

bool cleaned[ENTRY_COUNT];

static struct test_list*
init_list(void)
{
	int i;
	struct test_list *node;

	for (i = 0, node = NULL; i < ENTRY_COUNT; ++i) {
		node = list_alloc_at_end(node);
		node->num = i;
	}

	return node;
}

static void
destroy_func(void *entry)
{
	struct test_list *node = entry;

	CU_ASSERT(cleaned[node->num] == false);
	cleaned[node->num] = true;
}

static void
global_destroy(void)
{
	int i;
	struct test_list *node = init_list();

	list_setfunc_destroy(destroy_func);

	memset(cleaned, 0, sizeof (cleaned));
	list_destroy_full(node);

	for (i = 0; i < ENTRY_COUNT; ++i)
		CU_ASSERT(cleaned[i]);
}

static void
local_destroy(void)
{
	int i;
	struct test_list *node;

	node = init_list();

	list_setfunc_destroy(NULL);
	list_setlfunc_destroy(node, destroy_func);

	memset(cleaned, 0, sizeof (cleaned));
	list_destroy_full(node);

	for (i = 0; i < ENTRY_COUNT; ++i)
		CU_ASSERT_FATAL(cleaned[i]);

	node = init_list();

	// set invalid global destroy function and valid local
	list_setfunc_destroy(destroy_func + 10);
	list_setlfunc_destroy(node, destroy_func);

	memset(cleaned, 0, sizeof (cleaned));
	list_destroy_full(node);

	for (i = 0; i < ENTRY_COUNT; ++i)
		CU_ASSERT_FATAL(cleaned[i]);
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
	if (NULL == CU_add_test(suite, "Valid cases with global destroy function", global_destroy))
		return false;
	if (NULL == CU_add_test(suite, "Valid cases with local destroy function", local_destroy))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
