# ERROR REPORT: ASSIGNMENT 2

## 001: MAKEFILE SYNTAX
```
WROTE: CC = $(gcc)
EXPECTED: CC = $(CROSS_COMPILE)gcc
```
Q: $(gcc) expands what variable? A: undefined. Result: empty.

## 002: DOUBLE COMPILER
```
WROTE: $(CC) $(CROSS_COMPILE)gcc -o writer
EXPECTED: $(CC) -o writer
```
Q: If CC=aarch64-linux-gnu-gcc, what executes? A: "aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc" → nonsense.

## 003: SYSLOG DECLARATION VS CALL
```
WROTE: syslog(int pri, const char *fmt, ...)
EXPECTED: syslog(LOG_ERR, "message")
```
Q: Types belong where? A: Declaration. Values belong where? A: Call.

## 004: ASSIGNMENT.TXT=1
```
LEFT: assignment1
NEEDED: assignment2
```
Q: Line 37 of finder-test.sh does what when assignment≠1? A: mkdir. Skipped → crash.

## 005: BASH SPACE
```
WROTE: Y= $(grep)
EXPECTED: Y=$(grep)
```
Q: What does `Y=` mean alone? A: Y="". What runs next? A: $(grep) as command.

## 006: NO $? CHECK
```
MISSING: if [ $? -ne 0 ]; then exit 1; fi
```
Q: How know mkdir failed? A: $?. Not checked → silent failure.

## 007: WRITER.SH NOT WRITER
```
LINE 57: ./writer.sh
NEEDED: ./writer
```
Q: Which is C binary? A: writer. Which is shell? A: writer.sh. Used wrong one.

## 008: COMMENTED MAKE
```
LINE 52: #make clean
LINE 53: #make
```
Q: What does # mean? A: Comment. Uncommented? No.

## 009: SELF-HOSTED VS UBUNTU
```
CHANGED: runs-on: ubuntu-latest
NEEDED: runs-on: self-hosted
```
Q: Course container needs what? A: Self-hosted runner. GitHub cloud? No.

## 010: INCLUDE ORDER
```
WROTE: #include "broken.h" THEN #include <string.h>
NEEDED: #include <string.h> THEN #include "broken.h"
```
Q: When is memmove declared? A: In string.h. Broken.h uses memmove at line 33. Order matters.

## 011: ARGV INDEX
```
CONFUSED: argv[?]
ANSWER: argv[1]=file, argv[2]=text
```
Q: argv[0]=? A: Program name. Not user input.

## 012: FOPEN MODE
```
CONFUSED: fopen(f, "???")
ANSWER: "w"
```
Q: Truncate+create = ? A: "w". Append = ? A: "a".

## 013: VIM BATCH DELETE
```
ASKED: 4 times for same operation
ANSWER: :73,74s/Variable: //g OR j.
```
Q: Repeat last command? A: dot.

## 014: CROSS_COMPILE ORIGIN
```
QUESTION: "how does it know magically"
ANSWER: Command line. make CROSS_COMPILE=xxx
```
Q: Where does variable come from? A: You type it.

## 015: DOUBLE URL
```
TYPED: url1url2
EXPECTED: url1
```
Q: Did you read before pasting? A: No.

## 016: WRONG FOLDER
```
CREATED: assignment2/cross-compile.txt
EXPECTED: assignments/assignment2/cross-compile.txt
```
Q: Did you read the path? A: No.

---

## PATTERN

| Type | Count |
|------|-------|
| Syntax | 4 |
| Incomplete | 4 |
| Unread Docs | 5 |
| Tool | 3 |

## ORTHOGONAL QUESTIONS

1. YOU: "It should work" → REALITY: Did you test?
2. YOU: "I understand" → REALITY: Can you reproduce from memory?
3. YOU: "I read it" → REALITY: Did you execute each sentence?
4. YOU: "Trivial" → REALITY: Then why did it fail?
5. YOU: "Next step" → REALITY: Is current step verified?
