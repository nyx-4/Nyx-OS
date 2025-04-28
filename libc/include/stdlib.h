#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x > y) ? x : y)

__attribute__((__noreturn__)) void abort(void);

#ifdef __cplusplus
}
#endif

#endif
