#include <kernel/framebuffer.hpp>
#include <string.h>
#include <string>

video_info_t FBuf::fb_info;
bool         FBuf::isValidFB  = false;
bool         FBuf::isValidVBE = false;

FBuf::FBuf() { }
FBuf::FBuf(video_info_t framebuffer_info, bool isValidFB, bool isValidVBE) {
    this->isValidFB  = isValidFB;
    this->isValidVBE = isValidVBE;
    this->fb_info    = framebuffer_info;
}

void FBuf::terminal_initialize(void) {
    uint32_t *fb = (uint32_t *)fb_info.framebuffer_addr_lower;
    for (uint32_t y = 0; y < fb_info.framebuffer_height; y++) {
        for (uint32_t x = 0; x < fb_info.framebuffer_width; x++) {
            fb[y * (fb_info.framebuffer_pitch / 4) + x] = FBColor(0x000000);
        }
    }
}

void FBuf::terminal_putchar(unsigned short int ch, int cx, int cy, uint32_t fg,
                            uint32_t bg) {
    PSF_font *font         = (PSF_font *)_binary_fnt_psf_start;
    int       BytesPerLine = (font->width + 7) / 8;

    // Calculate glyph (FontStart + ch * BytesPerGlyph)
    unsigned char *glyph =
        (unsigned char *)_binary_fnt_psf_start + font->headersize +
        (ch > 0 && ch < font->numglyph ? ch : 0) * font->bytesperglyph;

    int start = cy * font->height * fb_info.framebuffer_width *
                    fb_info.framebuffer_bpp / 8 +
                cx * (font->width + 1) * fb_info.framebuffer_bpp / 8;

    int x, y, line, mask;
    for (y = 0; y < font->height; y++) {
        /* save the starting position of the line */
        line = start;
        mask = 1 << (font->width - 1);
        /* display a row */
        for (x = 0; x < font->width; x++) {
            *((uint32_t *)(fb_info.framebuffer_addr_lower + line)) =
                *((unsigned int *)glyph) & mask ? fg : bg;
            /* adjust to the next pixel */
            mask >>= 1;
            line  += fb_info.framebuffer_bpp / 8;
        }
        /* adjust to the next line */
        glyph += BytesPerLine;
        start += fb_info.framebuffer_width * fb_info.framebuffer_bpp / 8;
    }
}

void FBuf::terminal_write(const char *data, size_t size) {
    // for (int i = 0; i < size; i++) terminal_putchar(data[i]);
}

void FBuf::terminal_writestring(const char *data) {
    terminal_write(data, strlen(data));
}

void FBuf::test() {
    if (fb_info.framebuffer_bpp == 32 && fb_info.framebuffer_type == 1) {
        // Fill the screen with red
        uint32_t color = 0xFF0000;
        for (uint32_t y = 0; y < fb_info.framebuffer_height; y++) {
            for (uint32_t x = 0; x < fb_info.framebuffer_width; x++) {
                uint32_t *fb = (uint32_t *)fb_info.framebuffer_addr_lower;
                fb[(int)y * (fb_info.framebuffer_pitch / 4) + x] =
                    (uint32_t)((y) + (x - 0x80));
            }
        }
    }
}

void terminal_initialize(void) {
    FBuf fb;
    fb.terminal_initialize();
}
void terminal_putchar(unsigned short int c, int cx, int cy, uint32_t fg,
                      uint32_t bg) {
    FBuf fb;
    fb.terminal_putchar(c, cx, cy, fg, bg);
}
void terminal_write(const char *data, size_t size) {
    FBuf fb;
    fb.terminal_write(data, size);
}
void terminal_writestring(const char *data) {
    FBuf fb;
    fb.terminal_writestring(data);
}

int32_t FBColor(string color) { return 0x000000; }
int32_t FBColor(uint32_t color) { return color; }
int32_t FBColor(uint8_t Red, uint32_t Green, uint32_t Blue) { return 0x000000; }
