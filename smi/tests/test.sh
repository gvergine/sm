#!/bin/bash
SMI="valgrind $1"
TESTS_DIR=$2

set -e

echo "Test basic usage as unix filter"
cat $TESTS_DIR/toggle_events.sm | $SMI -d $TESTS_DIR/toggle.sm | diff $TESTS_DIR/toggle_actions.sm -

