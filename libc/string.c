#include <string.h>

int memcmp(const void *lhs, const void *rhs, size_t size) {
    return strncmp(lhs, rhs, size);
}

void *memcpy(void *restrict dst, const void *restrict src, size_t size) {
    return strncpy(dst, src, size);
}

void *memmove(void *dst, const void *src, size_t size) {
    unsigned char       *dst_p = (unsigned char *)dst;
    const unsigned char *src_p = (const unsigned char *)src;

    // if-else to avoid over-writting for over-lapping regions
    if (dst_p < src_p) {
        for (size_t i = 0; i < size; i++) {
            dst_p[i] = src_p[i];
        }

    } else {
        for (size_t i = size; i > 0; i--) {
            dst_p[i - 1] = src_p[i - 1];
        }
    }

    return dst;
}

void *memset(void *buf, int value, size_t size) {
    unsigned char *buf_p = (unsigned char *)buf;

    for (size_t i = 0; i < size; i++) {
        buf_p[i] = (unsigned char)value;
    }

    return buf;
}

int strcmp(const char *lhs, const char *rhs) {
    const unsigned char *left  = (const unsigned char *)lhs;
    const unsigned char *right = (const unsigned char *)rhs;

    for (size_t i = 0; true; i++) {
        // return -1 if left is smaller; +1 if right is smaller
        if (left[i] == '\0' && right[i] == '\0') {  // both str's are same
            return 0;
        }
        if (left[i] < right[i]) {
            return -1;
        }
        if (left[i] > right[i]) {
            return +1;
        }
    }
}

int strncmp(const char *lhs, const char *rhs, size_t size) {
    const unsigned char *left  = (const unsigned char *)lhs;
    const unsigned char *right = (const unsigned char *)rhs;

    for (size_t i = 0; i < size; i++) {
        // return -1 if left is smaller; +1 if right is smaller
        if (left[i] < right[i]) {
            return -1;
        }
        if (left[i] > right[i]) {
            return +1;
        }
    }

    return 0;
}

char *strcpy(char *restrict dst, const char *restrict src) {
    for (size_t i = 0; true; i++) {
        dst[i] = src[i];

        if (src[i] == '\0') {
            break;
        }
    }

    return dst;
}

char *strncpy(char *restrict dst, const char *restrict src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }

    return dst;
}

char *strcat(char *restrict dst, const char *restrict src) {
    return strncat(dst, src, strlen(src));
}

char *strncat(char *restrict dst, const char *restrict src, size_t size) {
    // dst_p points to end of dst (i.e., at '\0' of dst)
    unsigned char *dst_p = (unsigned char *)dst + strlen(dst);

    for (size_t i = 0; i < size; i++) {
        dst_p[i] = src[i];
    }

    dst_p[size] = '\0';  // add NULL terminator at end of new string

    return dst;
}

size_t strlen(const char *str) {
    size_t len = 0;

    // loop until \0 is found
    while (str[len] != '\0') {
        len++;
    }

    return len;
}
