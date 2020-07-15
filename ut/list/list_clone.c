#include <stdio.h>
#include <stdbool.h>

#include <CUnit/CUnit.h>

#include "macro.h"

struct test_list {
	int num;
};

void
copyfunc(struct test_list *to, struct test_list *from)
{
	to->num = 2*from->num;
}

static void
valid_cases(void)
{
	int i;
	struct test_list *list, *clone;
	struct test_list *entry, *clone_entry;

	for (i = 0, list = NULL; i < 5; ++i) {
		list = list_alloc_at_end(list);
		list->num = i;
	}

	list = entry = list_get_head(list);
	list_setlfunc_copy(list, (void (*)(void*, void*))&copyfunc);
	clone = clone_entry = list_clone(list);

	list_foreach(list, entry) {
		CU_ASSERT_FATAL(2*entry->num == clone_entry->num);
		clone_entry = list_get_next(clone_entry);
	}

	entry = list;
	list_foreach(clone, clone_entry) {
		CU_ASSERT_FATAL(2*entry->num == clone_entry->num);
		entry = list_get_next(entry);
	}

	list_free_full(list);
	list_free_full(clone);
}

static void
invalid_cases(void)
{
	CU_ASSERT(!list_clone(NULL));
}

bool
list_clone_suite(CU_pSuite suite)
{
	if (NULL == CU_add_test(suite, "Valid cases", valid_cases))
		return false;
	if (NULL == CU_add_test(suite, "Invalid cases", invalid_cases))
		return false;

	return true;
}
