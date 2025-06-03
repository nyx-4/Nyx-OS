#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copy block of memory
 * @param dest Pointer to the destination array where the content is to be
 * copied, type-casted to a pointer of type void*.
 * @param src Pointer to the source of data to be copied, type-casted to a
 * pointer of type const void*.
 * @param N Number of bytes to copy. `size_t` is an unsigned integral type.
 * @return `dest` is returned
 * @attention
 * - To avoid overflows, the size of the arrays pointed to by both the
 * destination and source parameters, shall be at least N bytes.
 * - The memory blocks should not overlap (for overlapping memory blocks,
 * memmove is a safer approach).
 * @note The function does not check for any terminating null character in
 * source - it always copies exactly `N` bytes.
 */
void *memcpy(void *dest, const void *src, size_t N);

/**
 * @brief Compares two block of memory
 * @param ptr1 Pointer to block of memory
 * @param ptr2 Pointer to block of memory
 * @param N Number of bytes to compare
 * @return
 * - `<0` the first byte that does not match in both memory blocks has a lower
 * value in `ptr1` than in `ptr2` (if evaluated as unsigned char values)
 *
 * - `0`  if the contents of both memory blocks are equal
 *
 * - `>0` the first byte that does not match in both memory blocks has a greater
 * value in `ptr1` than in `ptr2` (if evaluated as unsigned char values)
 * @note  Notice that, unlike strcmp, the function does not stop comparing after
 * finding a null character.
 */
int memcmp(const void *ptr1, const void *ptr2, size_t N);

/**
 * @brief Move block of memory
 * @param dest Pointer to the destination array where the content is to be
 * copied, type-casted to a pointer of type void*.
 * @param src Pointer to the source of data to be copied, type-casted to a
 * pointer of type const void*.
 * @param N Number of bytes to copy. `size_t` is an unsigned integral type.
 * @return `dest` is returned
 * @attention
 * - To avoid overflows, the size of the arrays pointed by both the
 * destination and source parameters, shall be at least N bytes.
 * @note  The function does not check for any terminating null character in
 * source - it always copies exactly N bytes.
 */
void *memmove(void *dest, const void *src, size_t N);

/**
 * @brief Fill block of memory
 * @param dest Pointer to the block of memory to fill.
 * @param value Value to be set. The value is passed as an int, but the function
 * fills the block of memory using the unsigned char conversion of this value.
 * @param N Number of bytes to be set to the value. `size_t` is an unsigned
 * integral type.
 * @return `dest` is returned
 */
void *memset(void *dest, int value, size_t N);

/**
 * @brief Copy string
 * @param dest Pointer to the destination array where the content is to be
 * copied.
 * @param src C string to be copied.
 * @return `dest` is returned
 * @attention
 * - To avoid overflows, the size of the array pointed by destination shall be
 * long enough to contain the same C string as source (including the terminating
 * null character).
 *
 * - The `dest` should not overlap in memory with `src`.
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief Copy characters from string
 * @param dest Pointer to the destination array where the content is to be
 * copied.
 * @param src C string to be copied.
 * @param N Maximum number of characters to be copied from source. `size_t` is
 * an unsigned integral type.
 * @return `dest` is returned
 * @attention
 * - `dest` and `src` shall not overlap (see memmove for a safer
 * alternative when overlapping).
 */
char *strncpy(char *dest, const char *src, size_t N);

/**
 * @brief Concatenate strings
 * @param dest Pointer to the destination array, which should contain a C
 * string, and be large enough to contain the concatenated resulting string.
 * @param src C string to be appended. This should not overlap `src`.
 * @return `dest` is returned
 * @attention
 * - The `dest` should not overlap with `src`.
 */
char *strcat(char *dest, const char *src);

/**
 * @brief Append characters from string
 * @param dest Pointer to the destination array, which should contain a C
 * string, and be large enough to contain the concatenated resulting string,
 * including the additional null-character.
 * @param src C string to be appended.
 * @param N Maximum number of characters to be appended. `size_t` is an unsigned
 * integral type.
 * @return `dest` is returned
 * @note
 * - If the length of the C string in source is less than num, only the content
 * up to the terminating null-character is copied.
 */
char *strncat(char *dest, const char *src, size_t N);

/**
 * @brief Compare two strings
 * @param str1 C string to be compared.
 * @param str2 C string to be compared.
 * @return
 * - `<0` if the first character that does not match has a lower value in ptr1
 * than in ptr2
 *
 * - `0`  if the contents of both strings are equal
 *
 * - `>0` if the first character that does not match has a greater value in ptr1
 * than in ptr2
 * @note
 * This function starts comparing the first character of each string. If they
 * are equal to each other, it continues with the following pairs until the
 * characters differ or until a terminating null-character is reached.
 */
int strcmp(const char *str1, const char *str2);

/**
 * @brief Compare characters of two strings
 * @param str1 C string to be compared.
 * @param str2 C string to be compared.
 * @param N Maximum number of characters to compare. `size_t` is an unsigned
 * integral type.
 * @return
 * - `<0` if the first character that does not match has a lower value in ptr1
 * than in ptr2
 *
 * - `0`  if the contents of both strings are equal
 *
 * - `>0` if the first character that does not match has a greater value in ptr1
 * than in ptr2
 * @note
 * This function starts comparing the first character of each string. If they
 * are equal to each other, it continues with the following pairs until the
 * characters differ, until a terminating null-character is reached, or until
 * num characters match in both strings, whichever happens first.
 */
int strncmp(const char *str1, const char *str2, size_t N);

/**
 * @brief Get string length
 * @param str C string
 * @return The length of string
 * @note
 * - This should not be confused with the size of the array that holds the
 * string.
 */
size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif
