#include <check.h>
#include <sm/state_machine_validation.h>

START_TEST(no_initial_item)
{
    list_t * issues_list;
    state_machine_t * sm = state_machine_new(0);
    state_machine_validate(sm, &issues_list);
    ck_assert_int_eq(1,list_count(issues_list));
    sm_val_item_t * item = list_get_data(list_find_next_element(issues_list,0));
    ck_assert_int_eq(SM_VAL_NOINITIALSTATE,item->issue);
    list_delete(issues_list);
    state_machine_delete(sm);
}
END_TEST

START_TEST(sm_disconnected)
{
    list_t * issues_list;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE_A");
    state_machine_add_state(sm,"STATE_B");    
    state_machine_set_initial_state(sm,"STATE_A");
    
    state_machine_validate(sm, &issues_list);
    ck_assert_int_eq(1,list_count(issues_list));
    sm_val_item_t * item = list_get_data(list_find_next_element(issues_list,0));
    ck_assert_int_eq(SM_VAL_STATENOTREACHABLE,item->issue);
    ck_assert_str_eq("STATE_B",item->item);
    list_delete(issues_list);
    state_machine_delete(sm);
}
END_TEST

START_TEST(sm_connected_single)
{
    list_t * issues_list;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE_A");
    state_machine_set_initial_state(sm,"STATE_A");
    
    state_machine_validate(sm, &issues_list);
    ck_assert_int_eq(0,list_count(issues_list));
    list_delete(issues_list);
    state_machine_delete(sm);
}
END_TEST

START_TEST(sm_disconnected_without_initial_state)
{
    list_t * issues_list;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE_A");
    state_machine_add_state(sm,"STATE_B");    
    state_machine_add_state(sm,"STATE_C");
    state_machine_add_state(sm,"STATE_D");         
    state_machine_validate(sm, &issues_list);
    ck_assert_int_eq(5,list_count(issues_list));
    list_delete(issues_list);
    state_machine_delete(sm);
}
END_TEST

START_TEST(sm_disconnected_complex)
{
    list_t * issues_list;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE_A");
    state_machine_add_state(sm,"STATE_B");    
    state_machine_add_state(sm,"STATE_C");
    state_machine_add_state(sm,"STATE_D");    
    
    state_machine_set_initial_state(sm,"STATE_A");
    
    state_machine_set_transition(sm, "STATE_A", "STATE_B", "EVENT");
    state_machine_set_transition(sm, "STATE_B", "STATE_A", "EVENT");
    state_machine_set_transition(sm, "STATE_C", "STATE_D", "EVENT");
    state_machine_set_transition(sm, "STATE_D", "STATE_C", "EVENT");    
    
    state_machine_validate(sm, &issues_list);
    ck_assert_int_eq(2,list_count(issues_list));
    list_element_t * element = list_find_next_element(issues_list,0);
    sm_val_item_t * item = list_get_data(element);
    ck_assert_int_eq(SM_VAL_STATENOTREACHABLE,item->issue);
    ck_assert_str_eq("STATE_C",item->item);

    ck_assert_int_eq(2,list_count(issues_list));
    item = list_get_data(list_find_next_element(issues_list,element));
    ck_assert_int_eq(SM_VAL_STATENOTREACHABLE,item->issue);
    ck_assert_str_eq("STATE_D",item->item);


    list_delete(issues_list);
    state_machine_delete(sm);
}
END_TEST


Suite * validation_test_suite(void)
{
    Suite *s;
    TCase *tc_core;
    s = suite_create("validation_test_suite");
    tc_core = tcase_create("Core");

    /* add here all the tests */
    tcase_add_test(tc_core, no_initial_item);
    tcase_add_test(tc_core, sm_disconnected);
    tcase_add_test(tc_core, sm_connected_single);
    tcase_add_test(tc_core, sm_disconnected_without_initial_state);
    tcase_add_test(tc_core, sm_disconnected_complex);
    /* end */
    
    suite_add_tcase(s, tc_core);
    return s;
}

