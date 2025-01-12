#!/bin/bash

make clean
clang-format -i src/*.c
clang-format -i src/idt/*.c
clang-format -i src/memory/*.c
# Ask for commit message
echo "Enter commit message: "
read commit_message

# Add changes to staging
git add .

# Commit changes
git commit -m "[Kernel] - $commit_message"

# Push changes to remote repository
git push