#ifndef _TEST_H
#define _TEST_H

#include <stdbool.h>

#include <CUnit/CUnit.h>

#define FUNCLEN_MAX 64

struct suite {
	char suitename[FUNCLEN_MAX];
	int (*init_suite)(void);
	int (*clean_suite)(void);
	bool (*func)(CU_pSuite suite);
};

// list test funcs
bool list_get_head_suite(CU_pSuite suite);
bool list_get_tail_suite(CU_pSuite suite);
bool list_get_prev_suite(CU_pSuite suite);
bool list_get_next_suite(CU_pSuite suite);

bool list_destroy_full_suite(CU_pSuite suite);

bool list_dup_suite(CU_pSuite suite);
bool list_clone_suite(CU_pSuite suite);

bool list_reverse_suite(CU_pSuite suite);

static struct suite suits[] = {
	{
		.suitename = "list_get_head",
		.func = list_get_head_suite,
	},
	{
		.suitename = "list_get_tail",
		.func = list_get_tail_suite,
	},
	{
		.suitename = "list_get_prev",
		.func = list_get_prev_suite,
	},
	{
		.suitename = "list_get_next",
		.func = list_get_next_suite,
	},
	{
		.suitename = "list_destroy_full",
		.func = list_destroy_full_suite,
	},
	{
		.suitename = "list_dup",
		.func = list_dup_suite,
	},
	{
		.suitename = "list_clone",
		.func = list_clone_suite,
	},
	{
		.suitename = "list_reverse",
		.func = list_reverse_suite,
	},
};

#endif // _TEST_H
