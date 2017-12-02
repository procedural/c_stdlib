#pragma once

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#define O_RDONLY  00
#define O_WRONLY  01
#define O_RDWR    02

#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_DSYNC      010000
#define O_SYNC     04010000
#define O_RSYNC    04010000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_CLOEXEC  02000000

#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_LARGEFILE       0
#define O_NOATIME  01000000
#define O_PATH    010000000
#define O_TMPFILE 020200000
#define O_NDELAY O_NONBLOCK

#define MAP_FAILED ((void *) -1)

#define PROT_NONE      0
#define PROT_READ      1
#define PROT_WRITE     2
#define PROT_EXEC      4
#define PROT_GROWSDOWN 0x01000000
#define PROT_GROWSUP   0x02000000

#define MAP_SHARED     0x01
#define MAP_PRIVATE    0x02
#define MAP_FIXED      0x10

#define MAP_TYPE       0x0f
#define MAP_FILE       0x00
#define MAP_ANON       0x20
#define MAP_ANONYMOUS  MAP_ANON
#define MAP_NORESERVE  0x4000
#define MAP_GROWSDOWN  0x0100
#define MAP_DENYWRITE  0x0800
#define MAP_EXECUTABLE 0x1000
#define MAP_LOCKED     0x2000
#define MAP_POPULATE   0x8000
#define MAP_NONBLOCK   0x10000
#define MAP_STACK      0x20000
#define MAP_HUGETLB    0x40000

#define M_PI 3.14159265358979323846

void * syscall0(long);
void * syscall1(long, long);
void * syscall2(long, long, long);
void * syscall3(long, long, long, long);
void * syscall4(long, long, long, long, long);
void * syscall5(long, long, long, long, long, long);
void * syscall6(long, long, long, long, long, long, long);
void * dlsym(void *, char *);

void * memset(void * s, int c, size_t n) {
  unsigned char * cs = s;
  for (int i = 0; i < n; i += 1)
    cs[i] = c;
  return s;
}

void * memcpy(void * dest, const void * src, size_t n) {
  unsigned char * csrc  = src;
  unsigned char * cdest = dest;
  for (int i = 0; i < n; i += 1)
    cdest[i] = csrc[i];
  return dest;
}

static inline int stdlib_log2i(int n) {
  return 31 - __builtin_clz(n);
}

static inline float stdlib_fabsf(float a) {
  return a > 0 ? a : -a;
}

static inline double stdlib_floor(double x) {
  union {double f; long i;} u = {x};
  int e = u.i >> 52 & 0x7ff;
  if (e >= 0x3ff + 52 || x == 0)
    return x;
  volatile double y;
  if (u.i >> 63)
    y = (double)(x - 0x1p52) + 0x1p52 - x;
  else
    y = (double)(x + 0x1p52) - 0x1p52 - x;
  if (e <= 0x3ff - 1)
    return u.i >> 63 ? -1 : 0;
  if (y > 0)
    return x + y - 1;
  return x + y;
}

static inline float stdlib_cosf(float x) {
  float tp = 1.0 / (2.0 * M_PI);
  x *= tp;
  x -= 0.25 + (float)stdlib_floor(x + 0.25);
  x *= 16.0 * (stdlib_fabsf(x) - 0.5);
  x += 0.225 * x * (stdlib_fabsf(x) - 1.0);
  return x;
}

static inline float stdlib_sinf(float x) {
  return stdlib_cosf(x - M_PI / 2.0);
}

static inline float stdlib_tanf(float x) {
  return stdlib_sinf(x) / stdlib_cosf(x);
}

static inline int stdlib_nprintf(int n, char * fmt, ...) {
  va_list arg_list;
  va_start(arg_list, fmt);
  char s[n];
  int size = stbsp_vsnprintf(s, n, fmt, arg_list);
  va_end(arg_list);
  syscall5(1, 1, (long)s, (long)size, 0, 0);
  return size;
}

static inline int stdlib_snprintf(char * s, size_t n, char * fmt, ...) {
  va_list arg_list;
  va_start(arg_list, fmt);
  int size = stbsp_vsnprintf(s, n, fmt, arg_list);
  va_end(arg_list);
  return size;
}

static inline int stdlib_nstreq(size_t n, char * a, char * b) {
  for (size_t i = 0; i < n; i += 1) {
    if (a[i] != b[i])
      return 0;
  }
  return 1;
}

static inline int stdlib_stat(char * path, void * stat_buf) {
  return (int)syscall2(4, (long)path, (long)stat_buf);
}

static inline int stdlib_open(char * pathname, int flags) {
  return (int)syscall3(2, (long)pathname, (long)flags, 0);
}

static inline int stdlib_open3(char * pathname, int flags, int mode) {
  return (int)syscall3(2, (long)pathname, (long)flags, mode);
}

static inline int stdlib_close(int fd) {
  return (int)syscall1(3, (long)fd);
}

ssize_t stdlib_read(int fd, void * buf, size_t count) {
  return (ssize_t)syscall3(0, fd, (long)buf, count);
}

ssize_t stdlib_write(int fd, void * buf, size_t count) {
  return (ssize_t)syscall3(1, fd, (long)buf, count);
}

static inline void * stdlib_mmap(void * start, size_t len, int prot, int flags, int fd, off_t off) {
  return syscall6(9, (long)start, (long)len, (long)prot, (long)flags, (long)fd, (long)off);
}

static inline int stdlib_munmap(void * addr, size_t len) {
  return (int)syscall2(11, (long)addr, (long)len);
}

static inline _Noreturn void _stdlib_assert(char * expr, char * file, int line, char * func) {
  stdlib_nprintf(4096, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line); // Assumption
  syscall3(234, (long)syscall0(186), (long)syscall0(186), 6);
  syscall3(234, (long)syscall0(186), (long)syscall0(186), 9);
  for (;;);
}

#define stdlib_assert(x) ((void)((x) || (_stdlib_assert(#x, __FILE__, __LINE__, __func__),0)))

static inline int stdlib_gettimeofday(struct timeval * restrict tv, void * restrict tz) {
  if (tv == NULL)
    return 0;
  struct timespec ts;
  syscall2(228, 0, (long)&ts);
  tv->tv_sec = ts.tv_sec;
  syscall3(96, 0, (long)&ts, 0);
  tv->tv_usec = (int)ts.tv_nsec / 1000;
  return 0;
}
