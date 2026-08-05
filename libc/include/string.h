#ifndef _STRING_H
#define _STRING_H 1

#include <stdbool.h>
#include <stddef.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *lhs, const void *rhs, size_t size);
void *memcpy(void *__restrict dst, const void *__restrict src, size_t size);
void *memmove(void *dst, const void *src, size_t size);
void *memset(void *buf, int value, size_t size);
int strcmp(const char *lhs, const char *rhs);
size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif
