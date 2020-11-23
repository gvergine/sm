#include <stdlib.h>
#include <check.h>

/* declare here all the suites */
extern Suite * state_machine_test_suite(void);
extern Suite * list_test_suite(void);
extern Suite * validation_test_suite(void);
/* end */
    
int main(void)
{
    int number_failed;

    SRunner *sr;

    sr = srunner_create(0);
    srunner_set_fork_status(sr,CK_NOFORK);
    
    /* add here all the suites */
    srunner_add_suite(sr,list_test_suite());
    srunner_add_suite(sr,state_machine_test_suite());
    srunner_add_suite(sr,validation_test_suite());
    /* end */
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

