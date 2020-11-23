#include <check.h>
#include <sm/state_machine.h>

START_TEST(new_and_delete)
{
    state_machine_t * sm = state_machine_new(0);
    ck_assert_ptr_ne(0,sm);
    state_machine_delete(sm);
}
END_TEST


START_TEST(add_new_and_existing_state)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);
    ret = state_machine_add_state(sm,"TEST");
    ck_assert_int_eq(1,ret);
    ret = state_machine_add_state(sm,"TEST");
    ck_assert_int_eq(0,ret);
    state_machine_delete(sm);
}
END_TEST

START_TEST(set_initial_state_existing_and_not)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);
    ret = state_machine_set_initial_state(sm,"TEST");
    ck_assert_int_eq(0,ret);
    state_machine_add_state(sm,"TEST");
    ret = state_machine_set_initial_state(sm,"TEST");
    ck_assert_int_eq(1,ret);
    state_machine_delete(sm);
}
END_TEST


START_TEST(set_transition_tests)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"ONE");
    state_machine_add_state(sm,"TWO");
    ret = state_machine_set_transition(sm,"ONE","TWO","EVENT");
    ck_assert_int_eq(1,ret);
    ret = state_machine_set_transition(sm,"ONE","TWO","EVENT");
    ck_assert_int_eq(0,ret);
    ret = state_machine_set_transition(sm,"ONE","NON_EXISTING","EVENT");
    ck_assert_int_eq(0,ret);
    ret = state_machine_set_transition(sm,"NON_EXISTING","TWO","EVENT");
    ck_assert_int_eq(0,ret);
    ret = state_machine_set_transition(sm,"NON_EXISTING","NON_EXISTING","EVENT");
    ck_assert_int_eq(0,ret);
    state_machine_delete(sm);
}
END_TEST


START_TEST(add_actions)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE");
    
    ret = state_machine_add_enter_action(sm,"STATE","A");
    ck_assert_int_eq(1,ret);
    ret = state_machine_add_enter_action(sm,"STATE","A");
    ck_assert_int_eq(0,ret);
    ret = state_machine_add_enter_action(sm,"NON_EXISTING","A");
    ck_assert_int_eq(0,ret);
    
    ret = state_machine_add_exit_action(sm,"STATE","A");
    ck_assert_int_eq(1,ret);
    ret = state_machine_add_exit_action(sm,"STATE","A");
    ck_assert_int_eq(0,ret);
    ret = state_machine_add_exit_action(sm,"NON_EXISTING","A");
    ck_assert_int_eq(0,ret);
    
    ret = state_machine_add_internal_action(sm,"STATE","EVENT","A");
    ck_assert_int_eq(1,ret);
    ret = state_machine_add_internal_action(sm,"STATE","NON_EXISTING","A");
    ck_assert_int_eq(1,ret);
    ret = state_machine_add_internal_action(sm,"NON_EXISTING","NON_EXISTING_2","A");
    ck_assert_int_eq(0,ret);
    
    state_machine_delete(sm);
}
END_TEST

START_TEST(reset_state_machine)
{
    int ret, c;
    char* action;
    list_element_t * iterator;
    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE");
    state_machine_set_initial_state(sm,"STATE");
    state_machine_add_enter_action(sm,"STATE","A");
    
    list_t * actions = state_machine_reset(sm);
    
    iterator = 0;
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_ne(iterator,0);
    action = (char*)list_get_data(iterator);
    ck_assert_str_eq(action,"A");
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_eq(iterator,0);
    
    list_delete(actions);
    state_machine_delete(sm);
}
END_TEST

START_TEST(dispatch_event)
{
    int ret, c;
    char* action;
    list_element_t * iterator;
    list_t* actions;

    state_machine_t * sm = state_machine_new(0);
    state_machine_add_state(sm,"STATE_AC");
    state_machine_add_state(sm,"STATE_BB");
    state_machine_set_initial_state(sm,"STATE_AC");
    state_machine_add_exit_action(sm,"STATE_AC","1");
    state_machine_add_exit_action(sm,"STATE_AC","1bis");
    state_machine_add_enter_action(sm,"STATE_BB","2");
    state_machine_add_internal_action(sm,"STATE_BB","INT","3");
    state_machine_add_exit_action(sm,"STATE_BB","4");
    state_machine_set_transition(sm,"STATE_AC","STATE_BB","TRANS");
    state_machine_set_transition(sm,"STATE_BB","STATE_AC","TRANS");
    actions = state_machine_reset(sm);
    list_delete(actions);
        
    actions = state_machine_dispatch_event(sm,"TRANS");
    iterator = 0;
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_ne(iterator,0);
    action = (char*)list_get_data(iterator);
    ck_assert_str_eq(action,"1");
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_ne(iterator,0);
    action = (char*)list_get_data(iterator);
    ck_assert_str_eq(action,"1bis");
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_ne(iterator,0);
    action = (char*)list_get_data(iterator);
    ck_assert_str_eq(action,"2");
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_eq(iterator,0);
    list_delete(actions);
    
    
    
    actions = state_machine_dispatch_event(sm,"INT");
    iterator = 0;
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_ne(iterator,0);
    action = (char*)list_get_data(iterator);
    ck_assert_str_eq(action,"3");
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_eq(iterator,0);
    list_delete(actions);
        
    actions = state_machine_dispatch_event(sm,"TRANS");
    iterator = 0;
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_ne(iterator,0);
    action = (char*)list_get_data(iterator);
    ck_assert_str_eq(action,"4");
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_eq(iterator,0);
    list_delete(actions);
    
    actions = state_machine_dispatch_event(sm,"NON_EXISTING_EVENT");
    iterator = 0;
    iterator = list_find_next_element(actions,iterator);
    ck_assert_ptr_eq(iterator,0);
    list_delete(actions);
    
    
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
    tcase_add_test(tc_core, add_new_and_existing_state);
    tcase_add_test(tc_core, set_initial_state_existing_and_not);
    tcase_add_test(tc_core, set_transition_tests);
    tcase_add_test(tc_core, add_actions);
    tcase_add_test(tc_core, reset_state_machine);
    tcase_add_test(tc_core, dispatch_event);
    /* end */
    
    suite_add_tcase(s, tc_core);
    return s;
}

