#include <stdarg.h>
#include <stdio.h>
#include <sys/syslog.h>
#include <pthread.h>

#ifndef FDL_LOG_JOURNALD
#define FDL_LOG_JOURNALD 1
#endif

#define FDL_LOG_NO_JOURNALD 0

static pthread_mutex_t _lck = PTHREAD_MUTEX_INITIALIZER;
static int _lvl = LOG_INFO;

static void _log(FILE *stream, int lvl, int journald, const char *format, va_list args, int force);

#define DECLARE_LOG_FUNC(__lvl, __name, __stream, __journald, __force) \
  void __name(const char *format, ...) {                               \
    va_list args;                                                      \
    va_start(args, format);                                            \
    _log(__stream, LOG_##__lvl, __journald, format, args, __force);    \
    va_end(args);                                                      \
  }

DECLARE_LOG_FUNC(EMERG, EMERG, stderr, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(ALERT, ALERT, stderr, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(CRIT, CRIT, stderr, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(ERR, ERROR, stderr, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(WARNING, WARN, stderr, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(NOTICE, NOTICE, stderr, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(INFO, INFO, stdout, FDL_LOG_JOURNALD, 0)
DECLARE_LOG_FUNC(DEBUG, DEBUG, stderr, FDL_LOG_JOURNALD, 0)

DECLARE_LOG_FUNC(INFO, PRINT, stdout, FDL_LOG_NO_JOURNALD, 1)

static void _log(FILE *stream, int lvl, int journald, const char *format, va_list args, int force) {
  pthread_mutex_lock(&_lck);
  if (lvl <= _lvl || force) {
    if (journald)
      fprintf(stream, "<%d>", lvl);

    vfprintf(stream, format, args);
  }
  pthread_mutex_unlock(&_lck);
}

void fdl_log_setlevel(int lvl) {
  pthread_mutex_lock(&_lck);
  _lvl = lvl;
  pthread_mutex_unlock(&_lck);
}
