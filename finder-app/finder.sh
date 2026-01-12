#!/bin/sh
# -----------------------------------------------------------------------------
# STEP 1: PARSE ARGUMENTS
# -----------------------------------------------------------------------------
# GOAL: Check if user provided 2 arguments.
# VARS: $1 (filesdir), $2 (searchstr)
# LOGIC: If missing -> EXIT 1.
# -----------------------------------------------------------------------------

filesdir=$1
searchstr=$2

# ... YOUR CODE HERE for checking arguments ...

if [ -z "$filesdir" ] || [ -z "$searchstr" ]; then
    echo "Error: Two arguments required. (1) Directory Path, (2) Search String."
    exit 1
fi

# -----------------------------------------------------------------------------
# STEP 2: CHECK DIRECTORY
# -----------------------------------------------------------------------------
# GOAL: Check if 'filesdir' is a real directory on disk.
# LOGIC: If not real -> EXIT 1.
# -----------------------------------------------------------------------------

# ... YOUR CODE HERE for checking directory existence ...

if [ ! -d "$filesdir" ]; then
    echo "Error: Directory $filesdir does not exist."
    exit 1
fi

# -----------------------------------------------------------------------------
# STEP 3: COUNT FILES (X)
# -----------------------------------------------------------------------------
# GOAL: Count number of files in 'filesdir' and subdirs.
# HINT: recursion
# -----------------------------------------------------------------------------

X=$(find "$filesdir" -type f | wc -l)   # <--- REPLACE 0 with calculation

# -----------------------------------------------------------------------------
# STEP 4: COUNT MATCHING LINES (Y)
# -----------------------------------------------------------------------------
# GOAL: Count number of lines matching '$searchstr' in all files.
# -----------------------------------------------------------------------------

Y=$(grep -r "$searchstr" "$filesdir" | wc -l)   # <--- REPLACE 0 with calculation

# -----------------------------------------------------------------------------
# STEP 5: OUTPUT
# -----------------------------------------------------------------------------
echo "The number of files are $X and the number of matching lines are $Y"
exit 0
