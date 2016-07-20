#if defined(__MINGW32__)
#include <fcntl.h>
#include <io.h>
#endif

#include "overrides.h"

extern pid_t fork(void);
extern int execv(const char *path, char * const *argv);

#if defined(__MINGW32__)
pid_t _fork(void) {
  return 0;
}

int pipe(int pipefd[2])
{
  return _pipe(pipefd, 5000, _O_BINARY);
}

#else
pid_t _fork(void) {
  return fork();
}

int _execv(const char *path, char * const *argv) {
  return execv(path, argv);
}
#endif
