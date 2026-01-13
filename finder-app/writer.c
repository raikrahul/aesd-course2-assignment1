/*
 * -----------------------------------------------------------------------------
 * AXIOM 1: THE STACK FRAME OF MAIN
 * -----------------------------------------------------------------------------
 * When you run: ./writer /tmp/file.txt "hello"
 *
 * MEMORY LAYOUT (Start of Stack):
 * [ 0x7FFF0000 ] argc = 3 (integer)
 * [ 0x7FFF0008 ] argv[0] -> Pointer to "./writer"
 * [ 0x7FFF0010 ] argv[1] -> Pointer to "/tmp/file.txt"
 * [ 0x7FFF0018 ] argv[2] -> Pointer to "hello"
 * [ 0x7FFF0020 ] argv[3] -> NULL (0x00000000)
 *
 * CALCULATION 1: ARGUMENT COUNT
 * Required: FilePath + TextString.
 * Actual: ProgramName + FilePath + TextString.
 * Count = 1 + 1 + 1 = 3.
 * FAILURE CONDITION: If argc != 3, we must exit.
 *
 * -----------------------------------------------------------------------------
 * AXIOM 2: THE SYSTEM LOG (syslog)
 * -----------------------------------------------------------------------------
 * We are not just printing to screen; we are logging to the SYSTEM.
 * Facility: LOG_USER (User-level messages).
 * Levels:
 *   LOG_ERR   (3) -> Catastrophic failure (File not writable).
 *   LOG_DEBUG (7) -> Boring info (Writing "hello" to "file.txt").
 *
 * -----------------------------------------------------------------------------
 * AXIOM 3: THE FILE HANDLE (fopen)
 * -----------------------------------------------------------------------------
 * 'fopen' takes a "Mode String" at Memory Address 0xXXXX.
 * Modes:
 *   "r"  = Read Only (Start).
 *   "w"  = Write Only (Truncate to 0 length, Create if missing).
 *   "a"  = Append (Add to end).
 *   "r+" = Read + Write (No truncate).
 *
 * YOUR TASK:
 * 1. Initialize syslog.
 * 2. Check argc using the calculation above.
 * 3. Open the file described by argv[?] using the correct mode.
 * 4. Write string argv[?] to it.
 * 5. Log actions (LOG_DEBUG) and errors (LOG_ERR).
 * 6. Close and cleanup.
 *
 * DO NOT GUESS. CALCULATE THE INDICES AND MODES.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
  // -----------------------------------------------------
  // STEP 1: SETUP LOGGING
  // -----------------------------------------------------
  // openlog(ident, option, facility)
  // ident = NULL (uses program name)
  // option = 0
  // facility = LOG_USER
  openlog(NULL, 0, LOG_USER);

  // -----------------------------------------------------
  // STEP 2: CHECK ARGUMENTS
  // -----------------------------------------------------
  // Derive the index: argv[0] is prog. argv[1] is file. argv[2] is text.
  // If we don't have exactly 3 args (including prog name), we fail.

  // FILL IN: if (argc ???) { ... }

  if (argc != 3) {
    syslog(LOG_ERR, "Usage: %s <filename> <text>", argv[0]);
    return 1;
  }
  // -----------------------------------------------------
  // STEP 3: OPEN FILE
  // -----------------------------------------------------
  const char *writefile = argv[1];
  const char *writestr = argv[2];

  FILE *fp = fopen(writefile, "w");

  if (fp == NULL) {
    syslog(LOG_ERR, "Error opening file %s: %m", writefile);
    return 1;
  }

  syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
  if (fprintf(fp, "%s", writestr) < 0) {
    syslog(LOG_ERR, "Error writing to file %s: %m", writefile);
    fclose(fp);
    return 1;
  }

  fclose(fp);
  closelog();
  return 0;
}