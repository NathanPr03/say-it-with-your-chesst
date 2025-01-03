#include <CUnit/Basic.h>
#include <stdlib.h>

void test_main_against_bad_bot(void) {
    int exit_code = system("./say_it_with_your_chesst bad-bot");
    CU_ASSERT_EQUAL(WEXITSTATUS(exit_code), 0);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("MainFunctionTests", NULL, NULL);
    CU_add_test(suite, "test_main", test_main_against_bad_bot);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}