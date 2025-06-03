#include <string.h>

void *memcpy(void *dest, const void *src, size_t N) {
    return (void *)strncpy((char *)dest, (const char *)src, N);
}

int memcmp(const void *ptr1, const void *ptr2, size_t N) {
    return strncmp((const char *)ptr1, (const char *)ptr2, N);
}

void *memmove(void *dest, const void *src, size_t N) {
    unsigned char       *destPtr = (unsigned char *)dest;
    const unsigned char *srcPtr  = (const unsigned char *)src;
    if (dest < src)
        for (size_t i = 0; i < N; i++) destPtr[i] = srcPtr[i];
    else
        for (size_t i = N; i > 0; i--) destPtr[i - 1] = srcPtr[i - 1];
    return dest;
}

void *memset(void *dest, int value, size_t N) {
    unsigned char *destPtr = (unsigned char *)dest;
    for (size_t i = 0; i < N; i++) destPtr[i] = (unsigned char)value;
    return dest;
}

char *strcpy(char *dest, const char *src) {
    return strncpy(dest, src, strlen(src));
}

char *strncpy(char *dest, const char *src, size_t N) {
    for (size_t i = 0; i < N; i++) dest[i] = src[i];
    return dest;
}

char *strcat(char *dest, const char *src) {
    return strncat(dest, src, strlen(src));
}

char *strncat(char *dest, const char *src, size_t N) {
    dest[strlen(dest) + N] = '\0';
    return strncpy(dest + strlen(dest), src, N);
}

int strcmp(const char *str1, const char *str2) {
    return strncmp(str1, str2, max(strlen(str1), strlen(str2)));
}

int strncmp(const char *str1, const char *str2, size_t N) {
    for (size_t i = 0; i < N; i++)
        if (str1[i] < str2[i]) return -1;
        else if (str1[i] > str2[i])
            return 1;
    return 0;
}

size_t strlen(const char *str) {
    size_t Len = 0;
    while (str[Len++] != '\0');
    return Len - 1;
}
