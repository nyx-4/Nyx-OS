#ifndef _KERNEL_FRAMEBUFFER_HPP
#define _KERNEL_FRAMEBUFFER_HPP

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string>

extern "C" void debug(uint32_t);

/**
 * @brief video_info_t contains information about current video mode. It is
 * taken from GRUB during boot
 *
 * @param vbe_control_info
 * @param vbe_mode_info
 * @param vbe_mode
 * @param vbe_interface_seg
 * @param vbe_interface_off
 * @param vbe_interface_len
 *
 * @param framebuffer_addr_lower The lower 32-bit of memory address where
 * Framebuffer is mapped. Write to this `(addr + offset)` to display it on
 * screen
 * @param framebuffer_addr_upper The upper 32-bit of memory address where
 * Framebuffer is mapped. This is often zero, but not always <3
 * @param framebuffer_pitch
 * @param framebuffer_width The width of screen in pixels, or characters
 * depending on `framebuffer_type`
 * @param framebuffer_height The height of screen in pixels, or characters
 * depending on `framebuffer_type`
 * @param framebuffer_bpp The numbers of bits per each characters, use it to
 * calculate offset because it might be more than what is needed
 * @param framebuffer_type This can contain 0, 1, or 2.
 *
 * 0: Indexed colors are currently used by kernel, you can get the map of
 * indexed colors by quering `framebuffer_palette_addr` and reading
 * `framebuffer_palette_num_colors` colors
 *
 * 1: RGB colors are being used by kernel, query
 * `framebuffer_{red,green,blue}_{field_position,mask_size}` to get information
 * about RGB format in use
 *
 * 2: It means kernel is using EGA TEXT mode, probably because it was requested
 * or only mode supported, write to EGA TEXT memory located at 0xBF000. You
 * shouldn't read these values but go with the one specifed in
 * `framebuffer_{width,height,bpp}`.
 *
 * @attention Make sure that you read the correct union as explained in
 * `framebuffer_type`
 *
 * @param framebuffer_palette_addr The 32-bit addr where palette information is
 * located, parse if `framebuffer_type` is 0
 * @param framebuffer_palette_num_colors Number of colors supported by current
 * mode, read this many colors from `framebuffer_palette_addr` and make sure to
 * use only these palettes
 *
 * The following param are explained using 16-bit example
 *
 * ```0123 4567 8901 2345```
 * ```RRRR RGGG GGGB BBBB```
 *
 * @param framebuffer_red_field_position `0`: Red starts at bit 0
 * @param framebuffer_red_mask_size `5`: Red takes 5 bits
 * @param framebuffer_green_field_position `5`: Green starts at bit 0
 * @param framebuffer_green_mask_size `6`: Green takes 6 bits
 * @param framebuffer_blue_field_position `11`: Blue starts at bit 0
 * @param framebuffer_blue_mask_size `5`: Blue takes 5 bits
 */
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
/**
 * @brief It's a fixed chunk at the beginning of the file. You will need to be
 * able to detect the version before you begin parsing.
 *
 */
typedef struct {
    uint32_t magic;         // magic bytes to identify PSF
    uint32_t version;       // zero
    uint32_t headersize;    // offset of bitmaps in file, 32
    uint32_t flags;         // 0 if there's no unicode table
    uint32_t numglyph;      // number of glyphs
    uint32_t bytesperglyph; // size of each glyph (in bytes)
    uint32_t height;        // height in pixels
    uint32_t width;         // width in pixels
} PSF_font;

extern char _binary_fnt_psf_start[];

extern "C" {
void terminal_initialize(void);
void terminal_putchar(unsigned short int c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_writeint(int32_t num);
void terminal_writeuint(uint32_t num);
void terminal_writehex(uint32_t num);
}

int32_t FBColor(std::string color);
int32_t FBColor(uint32_t color);
int32_t FBColor(uint8_t Red, uint8_t Green, uint8_t Blue);

class FBuf {
  private:
    static video_info_t fb_info;
    static bool         isValidFB;
    static bool         isValidVBE;

  public:
    FBuf(video_info_t framebuffer_info, bool isValidFB = false,
         bool isValidVBE = false);
    FBuf();
    static void terminal_initialize(void);
    static void terminal_putchar(unsigned short int c);
    static void terminal_write(const char *data, size_t size);
    static void terminal_writestring(const char *data);
    static void terminal_writeint(int32_t num);
    static void terminal_writeuint(uint32_t num);
    static void terminal_writehex(uint32_t num);
    static void test();

    // FBColor, converts color in common formats to the one currently used by
    // the kernel. It overloads such as FBColor("Green"), FBColor(0x80FF80).
    static int32_t FBColor(std::string color);
    static int32_t FBColor(uint32_t color);
    static int32_t FBColor(uint8_t Red, uint8_t Green, uint8_t Blue);
};

#endif