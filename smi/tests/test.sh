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

(cat $TESTS_DIR/toggle_events.sm | $VALGRIND_SMI -d $TESTS_DIR/toggle.sm \
    | diff $TESTS_DIR/toggle_actions.sm -) || exit 1

echo "Test wrong arguments errors"
expect_fail_result_but_ok_mem -d non/existing.sm
expect_fail_result_but_ok_mem -d $TESTS_DIR/toggle.sm -i non/existing.sm
expect_fail_result_but_ok_mem -d $TESTS_DIR/toggle.sm \
                              -i $TESTS_DIR/toggle_events.sm -o non/existing.sm
expect_fail_result_but_ok_mem -d $TESTS_DIR/syntax_error.sm \
                              -i $TESTS_DIR/toggle_events.sm -o /dev/null

echo "Validate-only bad case"
expect_fail_result_but_ok_mem -d $TESTS_DIR/syntax_error.sm --validate-only
expect_fail_result_but_ok_mem -d $TESTS_DIR/toggle_errors.sm --validate-only

echo "Validate-only good case"
($VALGRIND_SMI -d $TESTS_DIR/toggle.sm --validate-only) || exit 1

echo "Enumerate states, events and actions - good case"
($VALGRIND_SMI -d $TESTS_DIR/toggle.sm -S) || exit 1
($VALGRIND_SMI -d $TESTS_DIR/toggle.sm -E) || exit 1
($VALGRIND_SMI -d $TESTS_DIR/toggle.sm -A) || exit 1

echo "Enumerate states, events and actions - bad case"
expect_fail_result_but_ok_mem -d $TESTS_DIR/syntax_error.sm -S
expect_fail_result_but_ok_mem -d $TESTS_DIR/syntax_error.sm -E
expect_fail_result_but_ok_mem -d $TESTS_DIR/syntax_error.sm -A



