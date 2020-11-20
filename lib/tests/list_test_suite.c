#include <check.h>
#include <sm/list.h>

START_TEST(new_and_delete)
{
    list_t * l = list_new(0);
    ck_assert_ptr_ne(0,l);
    list_delete(l);
}
END_TEST

Suite * list_test_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("list_test_suite");
    tc_core = tcase_create("Core");

    /* add here all the tests */
    tcase_add_test(tc_core, new_and_delete);
    /* end */
    
    suite_add_tcase(s, tc_core);
    return s;
}

