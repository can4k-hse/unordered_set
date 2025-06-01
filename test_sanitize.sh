#!/bin/bash

# TEST_SOURCE
# TEST_EXEC
if [ -f .env ]; then
    source .env
fi

# Specify the logs file
OUTPUT_FILE="sanitizer_output.txt"

# Compile the program with AddressSanitizer, UndefinedBehaviorSanitizer, and LeakSanitizer
g++ -fsanitize=address,undefined,leak -fno-omit-frame-pointer -g $TEST_SOURCE -o $TEST_EXEC

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."

    # Run the program with additional options for stricter checking
    ASAN_OPTIONS="detect_leaks=1:check_initialization_order=true:strict_init_order=true" \
    UBSAN_OPTIONS="print_stacktrace=1" \
    ./$TEST_EXEC > $OUTPUT_FILE 2>&1

    # Check the output for sanitizer errors
    if grep -q "ERROR: AddressSanitizer\|ERROR: LeakSanitizer\|ERROR: UndefinedBehaviorSanitizer" $OUTPUT_FILE; then
        echo "Sanitizer errors detected. Details in $OUTPUT_FILE"
    else
        echo "No sanitizer errors detected."
    fi
else
    echo "Compilation error."
fi