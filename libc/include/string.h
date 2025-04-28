#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * @param dest
 * @param src
 * @param N
 * @return
 */
void *memcpy(void *dest, const void *src, size_t N);

/**
 * @brief
 * @param ptr1
 * @param ptr2
 * @param N
 * @return
 */
int memcmp(const void *ptr1, const void *ptr2, size_t N);

/**
 * @brief
 * @param dest
 * @param src
 * @param N
 * @return
 */
void *memmove(void *dest, const void *src, size_t N);

/**
 * @brief
 * @param dest
 * @param
 * @param N
 * @return
 */
void *memset(void *dest, int, size_t N);

/**
 * @brief
 * @param dest
 * @param src
 * @return
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief
 * @param dest
 * @param src
 * @param N
 * @return
 */
char *strncpy(char *dest, const char *src, size_t N);

/**
 * @brief
 * @param dest
 * @param src
 * @return
 */
char *strcat(char *dest, const char *src);

/**
 * @brief
 * @param dest
 * @param  src
 * @param N
 * @return
 */
char *strncat(char *dest, const char *src, size_t N);

/**
 * @brief
 * @param str1
 * @param str2
 * @return
 */
int strcmp(char *str1, const char *str2);

/**
 * @brief
 * @param str1
 * @param str2
 * @param N
 * @return
 */
int strncmp(char *str1, const char *str2, size_t N);

/**
 * @brief Returns the length of the C string str
 * @param str C string
 * @return The length of string
 */
size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif
