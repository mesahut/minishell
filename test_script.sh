#!/bin/bash
echo "nonexistentcommand" | ./minishell > output.txt 2>&1
echo "echo \$?" | ./minishell >> output.txt 2>&1
cat output.txt
rm output.txt
