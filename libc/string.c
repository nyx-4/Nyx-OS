#include <string.h>

// int memcmp(const void *aptr, const void *bptr, size_t size) {
//     const unsigned char *a = (const unsigned char *)aptr;
//     const unsigned char *b = (const unsigned char *)bptr;
//     for (size_t i = 0; i < size; i++) {
//         if (a[i] < b[i]) return -1;
//         else if (b[i] < a[i])
//             return 1;
//     }
//     return 0;
// }

// void *memcpy(void *dstptr, const void *srcptr, size_t size) {
//     unsigned char       *dst = (unsigned char *)dstptr;
//     const unsigned char *src = (const unsigned char *)srcptr;
//     for (size_t i = 0; i < size; i++) dst[i] = src[i];
//     return dstptr;
// }

// void *memmove(void *dstptr, const void *srcptr, size_t size) {
//     unsigned char       *dst = (unsigned char *)dstptr;
//     const unsigned char *src = (const unsigned char *)srcptr;
//     if (dst < src) {
//         for (size_t i = 0; i < size; i++) dst[i] = src[i];
//     } else {
//         for (size_t i = size; i != 0; i--) dst[i - 1] = src[i - 1];
//     }
//     return dstptr;
// }

// void *memset(void *bufptr, int value, size_t size) {
//     unsigned char *buf = (unsigned char *)bufptr;
//     for (size_t i = 0; i < size; i++) buf[i] = (unsigned char)value;
//     return bufptr;
// }

void *memcpy(void *dest, const void *src, size_t N) { }

int memcmp(const void *ptr1, const void *ptr2, size_t N) { }

void *memmove(void *dest, const void *src, size_t N) { }

void *memset(void *dest, int, size_t N) { }

char *strcpy(char *dest, const char *src) { }

char *strncpy(char *dest, const char *src, size_t N) { }

char *strcat(char *dest, const char *src) { }

char *strncat(char *dest, const char *, size_t N) { }

int strcmp(char *str1, const char *str2) {
    return strncmp(str1, str2, strlen(str1) + 1);
}

int strncmp(char *str1, const char *str2, size_t N) {
    return memcmp((void *)str1, (void *)str2, N);
}

size_t strlen(const char *str) {
    size_t Len = 0;
    while (str[Len++] != '\0');
    return Len - 1;
}
