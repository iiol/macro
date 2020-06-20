#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>

#include "test.h"
#include "macro.h"

#ifdef UT_TEST_CONSOLE
#  define CU_run_tests() CU_console_run_tests()
#else
#  define CU_run_tests()				\
do {							\
	CU_basic_set_mode(CU_BRM_VERBOSE);		\
	CU_basic_run_tests();				\
	printf("\n");					\
	CU_basic_show_failures(CU_get_failure_list());	\
	printf("\n\n");					\
} while (0)
#endif

int
main(void)
{
	int i;

	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	for (i = 0; i < (int)ARR_SZ(suits); ++i) {
		CU_pSuite cu_suite;

		cu_suite = CU_add_suite(suits[i].suitename, suits[i].init_suite, suits[i].clean_suite);
		if (!cu_suite || !suits[i].func(cu_suite))
			goto finalize;
	}

	CU_run_tests();

finalize:
	CU_cleanup_registry();

	return CU_get_error();
}
