#include <check.h>
#include <sm/state_machine.h>

START_TEST(new_and_delete)
{
    state_machine_t * sm = state_machine_new(0);
    ck_assert_ptr_ne(0,sm);
    state_machine_delete(sm);
}
END_TEST

Suite * state_machine_test_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("state_machine_test_suite");
    tc_core = tcase_create("Core");

    /* add here all the tests */
    tcase_add_test(tc_core, new_and_delete);
    /* end */
    
    suite_add_tcase(s, tc_core);
    return s;
}

