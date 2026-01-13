/*
 * AXIOM 1: THE GEOGRAPHY OF TIME (struct timeval)
 * -----------------------------------------------
 * A "Time Value" is a coordinate in 2D space:
 *   X-Axis: Seconds      (tv_sec)  -> Range: 0 to Billions
 *   Y-Axis: Microseconds (tv_usec) -> Range: 0 to 999,999
 *
 * MEMORY MAP (64-bit machine):
 * [ 0x00 ] tv_sec  (8 bytes) -> integer
 * [ 0x08 ] tv_usec (8 bytes) -> integer
 * Total Size: 16 bytes.
 *
 * AXIOM 2: THE INTERVAL TIMER (struct itimerval)
 * ----------------------------------------------
 * An "Interval Timer" is a machine with TWO states:
 *   State A: "Current Countdown" (it_value)    -> The bomb ticking NOW.
 *   State B: "Reload Value"      (it_interval) -> The bomb loaded NEXT.
 *
 * MEMORY MAP (struct itimerval):
 * [ 0x00 ] it_interval (struct timeval) -> 16 bytes (Reload)
 * [ 0x10 ] it_value    (struct timeval) -> 16 bytes (Current)
 * Total Size: 32 bytes.
 *
 * TASK:
 * We want to replicate ‘alarm(unsigned int seconds)‘.
 * The standard ‘alarm(5)‘ means:
 *   1. Trigger SIGALRM in 5 seconds.
 *   2. Do NOT repeat (One-shot).
 *
 * YOUR JOB:
 * Fill in the ‘struct itimerval new_v;‘ below with specific NUMBERS
 * to match the behavior of ‘alarm(seconds)‘.
 */

#include <stddef.h>
#include <sys/time.h>

unsigned int my_alarm(unsigned int seconds) {
  struct itimerval new_v;
  struct itimerval old_v;

  // ------------------------------------------------------------------------
  // STEP 1: CONFIGURE THE RELOAD (it_interval)
  // ------------------------------------------------------------------------
  // Q: Does 'alarm()' repeat?
  // A: No.
  // CALCULATION: What is the number for "Do not repeat"?
  // FILL:
  // new_v.it_interval.tv_sec  = ?;
  // new_v.it_interval.tv_usec = ?;

  // ------------------------------------------------------------------------
  // STEP 2: CONFIGURE THE BOMB (it_value)
  // ------------------------------------------------------------------------
  // Q: When should it fire?
  // A: In 'seconds'.
  // CALCULATION:
  // If seconds = 0, the alarm is CANCELLED.
  // If seconds = 5, the alarm is 5 sec, 0 usec.
  // FILL:
  // new_v.it_value.tv_sec  = ?;
  // new_v.it_value.tv_usec = ?;

  // ------------------------------------------------------------------------
  // STEP 3: THE SYSTEM CALL
  // ------------------------------------------------------------------------
  // We use ITIMER_REAL to count "Wall Clock Time".
  // It returns 'old_v' (previous time remaining).
  setitimer(ITIMER_REAL, &new_v, &old_v);

  // ------------------------------------------------------------------------
  // STEP 4: RETURN VALUE (The Legacy Behavior)
  // ------------------------------------------------------------------------
  // 'alarm()' returns the number of seconds REMAINING on the previous alarm.
  // The 'old_v' struct contains {sec, usec} remaining.
  // We must return ONLY seconds.
  // TRICK: If old_v.tv_usec > 0, do we round up? Round down?
  // AXIOM: alarm() returns 0 if no alarm was pending.
  return old_v.it_value.tv_sec;
}

int main() {
  // Test code will go here once you implement my_alarm.
  return 0;
}
