#pragma once

#ifdef STDLIB_IMPLEMENTATION
#define STB_SPRINTF_IMPLEMENTATION
#endif
#include "stb_sprintf.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PAGESIZE 4096

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

#define ENOMEM 12
#define EINVAL 22

#define CLOCK_REALTIME            0
#define CLOCK_MONOTONIC           1
#define CLOCK_PROCESS_CPUTIME_ID  2
#define CLOCK_THREAD_CPUTIME_ID   3
#define CLOCK_MONOTONIC_RAW       4
#define CLOCK_REALTIME_COARSE     5
#define CLOCK_MONOTONIC_COARSE    6
#define CLOCK_BOOTTIME            7
#define CLOCK_REALTIME_ALARM      8
#define CLOCK_BOOTTIME_ALARM      9
#define CLOCK_SGI_CYCLE          10
#define CLOCK_TAI                11

#define M_PI 3.14159265358979323846

typedef long ssize_t;
typedef long off_t;

struct timespec;
struct timeval;

struct stat {
  long st_dev;
  long st_ino;
  unsigned st_nlink;

  unsigned st_mode;
  unsigned st_uid;
  unsigned st_gid;
  unsigned __pad0;
  long st_rdev;
  long st_size;
  long st_blksize;
  long st_blocks;

  struct {long tv_sec; long tv_nsec;} st_atim;
  struct {long tv_sec; long tv_nsec;} st_mtim;
  struct {long tv_sec; long tv_nsec;} st_ctim;
  long __unused[3];
};

void * syscall0(long);
void * syscall1(long, long);
void * syscall2(long, long, long);
void * syscall3(long, long, long, long);
void * syscall4(long, long, long, long, long);
void * syscall5(long, long, long, long, long, long);
void * syscall6(long, long, long, long, long, long, long);

void * dlopen(char *, int);
void * dlsym(void *, char *);
int dlclose(void *);

static inline void * stdlib_memset(void * s, int c, size_t n) {
  unsigned char * cs = (unsigned char *)s;
  for (int i = 0; i < n; i += 1)
    cs[i] = c;
  return s;
}

static inline void * stdlib_memcpy(void * dest, const void * src, size_t n) {
  const unsigned char * csrc = (const unsigned char *)src;
  unsigned char * cdest = (unsigned char *)dest;
  for (int i = 0; i < n; i += 1)
    cdest[i] = csrc[i];
  return dest;
}

static inline int stdlib_log2i(int n) {
  return 31 - __builtin_clz(n);
}

static inline int stdlib_abs(int a) {
  return a > 0 ? a : -a;
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

static inline float stdlib_rsqrtf(float x) {
  float xhalf = 0.5f * x;
  int i = *(int*)&x;
  i = 0x5f375a86 - (i >> 1);
  x = *(float*)&i;
  x = x * (1.5f - xhalf * x * x);
  x = x * (1.5f - xhalf * x * x);
  x = x * (1.5f - xhalf * x * x);
  return x;
}

static inline float stdlib_sqrtf(float x) {
  return x * stdlib_rsqrtf(x);
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

static inline int stdlib_snprintf(char * s, size_t n, const char * fmt, ...) {
  va_list arg_list;
  va_start(arg_list, fmt);
  int size = stbsp_vsnprintf(s, n, fmt, arg_list);
  va_end(arg_list);
  return size;
}

#define stdlib_printf(fmt, ...) do {                     \
  int size = stdlib_snprintf(NULL, 0, fmt, __VA_ARGS__); \
  char s[size + 1];                                      \
  stdlib_snprintf(s, size + 1, fmt, __VA_ARGS__);        \
  syscall5(1, 1, (long)s, (long)size, 0, 0);             \
} while (0)

static inline int stdlib_nstreq(size_t n, const char * a, const char * b) {
  for (size_t i = 0; i < n; i += 1) {
    if (a[i] != b[i])
      return 0;
  }
  return 1;
}

static inline size_t stdlib_strlen(const char *s) {
  const char * a = s;
  const size_t * w = NULL;
  for (; (size_t)s % (sizeof(size_t)); s++) if (!*s) return s-a;
  for (w = (const size_t *)s; !((*w)-((size_t)-1/255) & ~(*w) & (((size_t)-1/255) * (255/2+1))); w++);
  for (s = (const char *)w; *s; s++);
  return s-a;
}

static inline int stdlib_stat(const char * path, void * stat_buf) {
  return (int)(long)syscall2(4, (long)path, (long)stat_buf);
}

static inline int stdlib_open(const char * pathname, int flags) {
  return (int)(long)syscall3(2, (long)pathname, (long)flags, 0);
}

static inline int stdlib_open3(const char * pathname, int flags, int mode) {
  return (int)(long)syscall3(2, (long)pathname, (long)flags, (long)mode);
}

static inline int stdlib_close(int fd) {
  return (int)(long)syscall1(3, (long)fd);
}

static inline ssize_t stdlib_read(int fd, void * buf, size_t count) {
  return (ssize_t)syscall3(0, (long)fd, (long)buf, (long)count);
}

static inline ssize_t stdlib_write(int fd, void * buf, size_t count) {
  return (ssize_t)syscall3(1, (long)fd, (long)buf, (long)count);
}

static inline int stdlib_fstat(int fd, struct stat * st) {
  return (int)(long)syscall2(5, (long)fd, (long)st);
}

static inline void * stdlib_mmap(void * start, size_t len, int prot, int flags, int fd, off_t off) {
  return syscall6(9, (long)start, (long)len, (long)prot, (long)flags, (long)fd, (long)off);
}

static inline int stdlib_munmap(void * addr, size_t len) {
  return (int)(long)syscall2(11, (long)addr, (long)len);
}

static inline void * stdlib_malloc(size_t size) {
  size += sizeof(size_t);
  int r = size % PAGESIZE;
  if (r > 0) {
    size += PAGESIZE - r;
  }
  void * ptr = stdlib_mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (ptr == MAP_FAILED) {
    return NULL;
  }
  *(size_t *)ptr = size;
  return (size_t *)ptr + 1;
}

static inline void stdlib_free(void * ptr) {
  if (ptr == NULL) {
    return;
  }
  size_t * malloc_ptr = (size_t *)ptr - 1;
  stdlib_munmap(malloc_ptr, *malloc_ptr);
}

static inline void * stdlib_realloc(void * ptr, size_t new_size) {
  void * new_ptr = stdlib_malloc(new_size);
  if (new_ptr == NULL) {
    return NULL;
  }
  if (ptr == NULL) {
    return new_ptr;
  }
  size_t old_size = *((size_t *)ptr - 1);
  old_size -= sizeof(size_t);
  if (new_size < old_size) {
    old_size = new_size;
  }
  stdlib_memcpy(new_ptr, ptr, old_size);
  stdlib_free(ptr);
  return new_ptr;
}

static inline int stdlib_posix_memalign(void ** res, size_t align, size_t len) {
  unsigned char * mem = NULL;
  if (align < sizeof(void *)) {
    return EINVAL;
  }
  if ((align & -align) != align) {
    return EINVAL;
  }
  if (len > 0xffffffffffffffffu - align) {
    return ENOMEM;
  }
  if (align <= (4 * sizeof(size_t))) {
    mem = (unsigned char *)stdlib_malloc(len);
    if (mem == NULL) {
      return ENOMEM;
    }
    *res = mem;
    return 0;
  }
  if (!(mem = (unsigned char *)stdlib_malloc(len + align-1))) {
    return ENOMEM;
  }
  unsigned char * new_mem = (unsigned char *)((size_t)mem + align-1 & -align);
  if (new_mem == mem) {
    *res = mem;
    return 0;
  }
  *res = new_mem;
  return 0;
}

static inline void * stdlib_aligned_alloc(size_t alignment, size_t size) {
  if (alignment < sizeof(void *)) {
    alignment = sizeof(void *);
  }
  void * pointer = NULL;
  if (stdlib_posix_memalign(&pointer, alignment, size) == 0) {
    return pointer;
  }
  return NULL;
}

static inline void * stdlib_memmove(void * dest, const void * src, size_t n) {
  const unsigned char * csrc = (const unsigned char *)src;
  unsigned char * cdest = (unsigned char *)dest;
  unsigned char * temp = (unsigned char *)stdlib_malloc(n);
  for (int i = 0; i < n; i += 1)
    temp[i] = csrc[i];
  for (int i = 0; i < n; i += 1)
    cdest[i] = temp[i];
  stdlib_free(temp);
  return dest;
}

static inline _Noreturn void _stdlib_assert(const char * expr, const char * file, int line, const char * func) {
  stdlib_printf("Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
  syscall3(234, (long)syscall0(186), (long)syscall0(186), 6);
  syscall3(234, (long)syscall0(186), (long)syscall0(186), 9);
  for (;;);
}

#define stdlib_assert(x) ((void)((x) || (_stdlib_assert(#x, __FILE__, __LINE__, __func__),0)))

static inline int stdlib_clock_gettime(int clk, struct timespec * ts) {
  return (int)(long)syscall2(228, (long)clk, (long)ts);
}

static inline int stdlib_gettimeofday(struct timeval * tv, void * tz) {
  if (tv == NULL)
    return 0;
  struct {long tv_sec; long tv_usec;} * tval;
  tval = (typeof(tval))tv;
  struct {long tv_sec; long tv_nsec;} ts;
  syscall2(228, 0, (long)&ts);
  tval->tv_sec = ts.tv_sec;
  syscall3(96, 0, (long)&ts, 0);
  tval->tv_usec = (int)ts.tv_nsec / 1000;
  return 0;
}

static inline int stdlib_nanosleep(const struct timespec * req, struct timespec * rem) {
  return (int)(long)syscall2(35, (long)req, (long)rem);
}

#ifdef STDLIB_IMPLEMENTATION
void * memset(void * s, int c, size_t n) {
  return stdlib_memset(s, c, n);
}

void * memcpy(void * dest, const void * src, size_t n) {
  return stdlib_memcpy(dest, src, n);
}

void * memmove(void * dest, const void * src, size_t n) {
  return stdlib_memmove(dest, src, n);
}
#else
void * memset(void * s, int c, size_t n);
void * memcpy(void * dest, const void * src, size_t n);
void * memmove(void * dest, const void * src, size_t n);
#endif

#ifdef __cplusplus
}
#endif