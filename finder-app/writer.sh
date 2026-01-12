#!/bin/sh
# -----------------------------------------------------------------------------
# STEP 1: PARSE ARGUMENTS
# -----------------------------------------------------------------------------
# GOAL: Check if user provided 2 arguments.
# VARS: $1 (writefile), $2 (writestr)
# LOGIC: If missing -> EXIT 1.
# -----------------------------------------------------------------------------

writefile=$1
writestr=$2

# ... YOUR CODE HERE for checking arguments ...
if [   -z "$writefile" ] || [ -z "$writestr" ]; then
    echo "Error: Two arguments required. (1) Write File, (2) Write String."
    exit 1
fi

# -----------------------------------------------------------------------------
# STEP 2: CREATE DIRECTORY PATH
# -----------------------------------------------------------------------------
# GOAL: Ensure the folder structure exists.
# INPUT: /tmp/a/b/c/file.txt
# ACTION: mkdir -p /tmp/a/b/c
# -----------------------------------------------------------------------------

# ... YOUR CODE HERE for creating parent directories ...
mkdir -p "$(dirname "$writefile")"

if [ $? -ne 0 ]; then
    echo "Error: Could not create directory '$(dirname "$writefile")'."
    exit 1
fi

# -----------------------------------------------------------------------------
# STEP 3: WRITE THE FILE
# -----------------------------------------------------------------------------
# GOAL: Write '$writestr' to '$writefile'. Overwrite if exists.
# LOGIC: Check exit code. If fail -> EXIT 1.
# -----------------------------------------------------------------------------

# ... YOUR CODE HERE for writing logic ...
echo "$writestr" > "$writefile"

if [ $? -ne 0 ]; then
    echo "Error: Could not write to file '$writefile'."
    exit 1
fi

exit 0
