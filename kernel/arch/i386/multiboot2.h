#ifndef ARCH_I386_MULTIBOOT2_H
#define ARCH_I386_MULTIBOOT2_H

#include <stddef.h>
#include <stdint.h>

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36D76289

struct multiboot2_info {
    uint32_t total_size;
    uint32_t reserved;
};

struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

#define MULTIBOOT2_TAG_TYPE_CMDLINE 1
struct multiboot2_tag_string {
    uint32_t type;
    uint32_t size;
    char     string[];
};

#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8
struct multiboot2_tag_framebuffer_common {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;

#define MULTIBOOT2_FRAMEBUFFER_TYPE_INDEXED  0
#define MULTIBOOT2_FRAMEBUFFER_TYPE_RGB      1
#define MULTIBOOT2_FRAMEBUFFER_TYPE_EGA_TEXT 2
    uint8_t framebuffer_type;

    uint16_t reserved;
};

struct multiboot2_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct multiboot2_tag_framebuffer {
    struct multiboot2_tag_framebuffer_common common;
    union {
        struct {
            uint16_t                framebuffer_palette_num_colors;
            struct multiboot2_color framebuffer_palette[0];
        };
        struct {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
};

// Find the first tag in the multiboot2 information.
static inline struct multiboot2_tag *
multiboot2_tag_begin(struct multiboot2_info *info) {
    uintptr_t ptr  = (uintptr_t)info;
    ptr           += sizeof(*info);
    ptr = -(-ptr & ~7UL);  // Align upwards: Negate, align downwards, negate.
    struct multiboot2_tag *tag = (struct multiboot2_tag *)ptr;
    return (tag->type == 0) ? NULL : tag;
}

// Find the next tag following an existing tag.
static inline struct multiboot2_tag *
multiboot2_tag_next(struct multiboot2_tag *tag) {
    uintptr_t ptr  = (uintptr_t)tag;
    ptr           += tag->size;
    ptr            = -(-ptr & ~7UL);
    tag            = (struct multiboot2_tag *)ptr;
    return (tag->type == 0) ? NULL : tag;
}

// Search through the multiboot2 tags for a tag with the desired type.
static inline struct multiboot2_tag *
multiboot2_tag_lookup(struct multiboot2_info *info, uint32_t type) {
    struct multiboot2_tag *tag = multiboot2_tag_begin(info);
    while (tag != NULL) {
        if (tag->type == type) {
            return tag;
        }
        tag = multiboot2_tag_next(tag);
    }
    return NULL;
}

#endif
