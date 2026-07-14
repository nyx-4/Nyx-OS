#include <string.h>

int memcmp(const void *lhs, const void *rhs, size_t size) {
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

    // 0 if both are similar
    return 0;
}

void *memcpy(void *restrict dst, const void *restrict src, size_t size) {
    unsigned char       *dst_p = (unsigned char *)dst;
    const unsigned char *src_p = (const unsigned char *)src;

    for (size_t i = 0; i < size; i++) {
        dst_p[i] = src_p[i];
    }

    return dst;
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
        if (left[i] == '\0' && right[i] == '\0') {
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

size_t strlen(const char *str) {
    size_t len = 0;

    // loop until \0 is found
    while (str[len] != '\0') {
        len++;
    }

    return len;
}
