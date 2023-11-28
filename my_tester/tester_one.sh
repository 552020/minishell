#!/bin/bash

# Path to your minishell executable
MINISHELL_PATH="../minishell"

# Test command
TEST_COMMAND="cat <minishell.h|ls"

# Function to remove ANSI color codes and prompts (adjust as necessary)
cleanup_output() {
    sed 's/\x1B\[[0-9;]\{1,\}[A-Za-z]//g' | grep -v '^$>'
}

# Run test command in minishell
MINISHELL_OUTPUT=$(echo -e "$TEST_COMMAND\nexit\n" | $MINISHELL_PATH | cleanup_output)
MINISHELL_EXIT_CODE=$?

# Run test command in bash
BASH_OUTPUT=$(bash -c "$TEST_COMMAND" 2> /dev/null)
BASH_EXIT_CODE=$?

# Output comparison
echo "Testing command: $TEST_COMMAND"
echo "Minishell Output:"
echo "$MINISHELL_OUTPUT"
echo "Minishell Exit Code: $MINISHELL_EXIT_CODE"
echo "Bash Output:"
echo "$BASH_OUTPUT"
echo "Bash Exit Code: $BASH_EXIT_CODE"

# Check if outputs and exit codes match
if [[ "$MINISHELL_OUTPUT" == "$BASH_OUTPUT" && "$MINISHELL_EXIT_CODE" -eq "$BASH_EXIT_CODE" ]]; then
    echo "Test Passed ✅"
else
    echo "Test Failed ❌"
fi
