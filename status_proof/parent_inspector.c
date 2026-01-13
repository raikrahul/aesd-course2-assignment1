#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void inspect(char *name, char *prog) {
  printf("--- Running %s ---\n", name);
  pid_t pid = fork();
  if (pid == 0) {
    execl(prog, prog, NULL);
    exit(1);
  } else {
    int status;
    waitpid(pid, &status, 0);
    printf("Raw status integer: %d (0x%X)\n", status, status);
    printf("WIFEXITED: %d\n", WIFEXITED(status));
    if (WIFEXITED(status))
      printf("  -> Exit Code (WEXITSTATUS): %d\n", WEXITSTATUS(status));
    printf("WIFSIGNALED: %d\n", WIFSIGNALED(status));
    if (WIFSIGNALED(status))
      printf("  -> Signal (WTERMSIG): %d\n", WTERMSIG(status));
    printf("\n");
  }
}

int main() {
  inspect("Normal Exit", "./child_exit");
  inspect("Crash", "./child_crash");
  return 0;
}
