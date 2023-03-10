#ifndef UTIL_H_
#define UTIL_H_

#include <errno.h>
#include <time.h>
#include <stdbool.h>

int randomInt(int min, int max) { return rand() % (max - min + 1) + min; }

int msleep(long msec) {
  struct timespec ts;
  int res;
  if (msec < 0) {
    errno = EINVAL;
    return -1;
  }
  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;
  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
  return res;
}

#endif