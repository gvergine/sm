#!/bin/bash
VALGRIND_ERR=10
VALGRIND_SMI="valgrind --leak-check=full --show-leak-kinds=all -s \
              --error-exitcode=$VALGRIND_ERR --trace-children=yes \
              --errors-for-leak-kinds=all $1"
TESTS_DIR=$2

function expect_fail_result_but_ok_mem() {
    $VALGRIND_SMI $@
    RET=$?
    if [[ $RET -eq 0 ]]; then echo "Should have failed"; exit 1; fi
    if [[ $RET -eq $VALGRIND_ERR ]]; then 
        echo "Valgrind found errors"; exit 1
    fi
}

echo "Test basic usage as unix filter"

cat $TESTS_DIR/toggle_events.sm | $VALGRIND_SMI -d $TESTS_DIR/toggle.sm \
    | diff $TESTS_DIR/toggle_actions.sm -

echo "Test wrong arguments errors"
expect_fail_result_but_ok_mem -d non/existing.sm
expect_fail_result_but_ok_mem -d $TESTS_DIR/toggle.sm -i non/existing.sm
expect_fail_result_but_ok_mem -d $TESTS_DIR/toggle.sm \
                              -i $TESTS_DIR/toggle_events.sm -o non/existing.sm
expect_fail_result_but_ok_mem -d $TESTS_DIR/syntax_error.sm \
                              -i $TESTS_DIR/toggle_events.sm -o /dev/null
