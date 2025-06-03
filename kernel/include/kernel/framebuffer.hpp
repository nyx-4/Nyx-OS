#ifndef _KERNEL_FRAMEBUFFER_HPP
#define _KERNEL_FRAMEBUFFER_HPP

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string>

typedef struct {
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint32_t framebuffer_addr_lower;
    uint32_t framebuffer_addr_upper;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    // Bits per pixel
    uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED  0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB      1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
    uint8_t framebuffer_type;
    union {
        // Valid if MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED
        struct {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        };
        // Valid if MULTIBOOT_FRAMEBUFFER_TYPE_RGB
        struct {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
} __attribute__((packed)) video_info_t;

#define PSF_FONT_MAGIC 0x864ab572
typedef struct {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
} PSF_font;

extern char _binary_fnt_psf_start[];

extern "C" {
void terminal_initialize(void);
void terminal_putchar(unsigned short int c, int cx, int cy, uint32_t fg,
                      uint32_t bg);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
}

// For common colors, like Blue and Magenta.
int32_t FBColor(string color);
// To convert 24-bit color to currently-loaded color-depth
int32_t FBColor(uint32_t color);
// To convert RGB value to currently-loaded color-depth.
int32_t FBColor(uint8_t Red, uint32_t Green, uint32_t Blue);

class FBuf {
  private:
    static video_info_t fb_info;
    static bool         isValidFB;
    static bool         isValidVBE;

  public:
    FBuf(video_info_t framebuffer_info, bool isValidFB = false,
         bool isValidVBE = false);
    FBuf();
    void terminal_initialize(void);
    void terminal_putchar(unsigned short int c, int cx, int cy, uint32_t fg,
                          uint32_t bg);
    void terminal_write(const char *data, size_t size);
    void terminal_writestring(const char *data);
    void test();
};

#endif