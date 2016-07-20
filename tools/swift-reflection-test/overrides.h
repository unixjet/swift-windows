// These APIs marked unavailable on watchOS and tvOS.

#include <sys/types.h>

pid_t _fork(void);
#if defined(__MINGW32__)
int pipe(int pipefd[2]);

#else
int _execv(const char *path, char * const *argv);
#endif
