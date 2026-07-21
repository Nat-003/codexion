#!/bin/bash

# adjust these to match your Makefile output / source files
gcc  parser.c main.c -o test_parser || exit 1

run() {
	echo "=========================================="
	echo "EXPECT: $1"
	echo "RUN:    ./test_parser $2"
	shift 2
	./test_parser "$@"
	echo "exit code: $?"
}

# --- should SUCCEED ---
run "OK fifo"        "4 800 200 200 200 5 100 fifo" 4 800 200 200 200 5 100 fifo
run "OK edf"         "4 800 200 200 200 5 100 edf"  4 800 200 200 200 5 100 edf
run "OK 1 coder"     "1 800 200 200 200 5 100 fifo" 1 800 200 200 200 5 100 fifo
run "OK cooldown 0"  "4 800 200 200 200 5 0 fifo"   4 800 200 200 200 5 0 fifo
run "OK debug 0"     "4 800 200 0 0 5 100 edf"      4 800 200 0 0 5 100 edf
run "OK plus sign"   "+4 800 200 200 200 5 100 fifo" +4 800 200 200 200 5 100 fifo
run "OK INT_MAX"     "2147483647 ..."               2147483647 800 200 200 200 5 100 fifo

# --- should FAIL (Error + exit 1) ---
run "FAIL too few"      "4 800 200"                4 800 200
run "FAIL too many"     "... extra arg"            4 800 200 200 200 5 100 fifo extra
run "FAIL zero coders"  "0 800 200 ..."            0 800 200 200 200 5 100 fifo
run "FAIL zero burnout" "4 0 200 ..."              4 0 200 200 200 5 100 fifo
run "FAIL zero compile" "4 800 0 ..."              4 800 0 200 200 5 100 fifo
run "FAIL zero compiles_req" "4 800 200 200 200 0 100 fifo" 4 800 200 200 200 0 100 fifo
run "FAIL garbage"      "4 abc 200 ..."            4 abc 200 200 200 5 100 fifo
run "FAIL partial num"  "4 12x 200 ..."            4 12x 200 200 200 5 100 fifo
run "FAIL overflow"     "99999999999 ..."          99999999999 800 200 200 200 5 100 fifo
run "FAIL INT_MAX+1"    "2147483648 ..."           2147483648 800 200 200 200 5 100 fifo
run "FAIL negative"     "-4 800 ..."               -4 800 200 200 200 5 100 fifo
run "FAIL bad sched"    "4 ... FIFO (caps)"        4 800 200 200 200 5 100 FIFO
run "FAIL empty num"    "'' 800 ..."               "" 800 200 200 200 5 100 fifo
run "FAIL space in num" "'4 ' ..."                 "4 " 800 200 200 200 5 100 fifo

echo "=========================================="