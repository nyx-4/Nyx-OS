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

void FBuf::terminal_putchar(unsigned short int ch) {
    static int      px = 0, py = 0, state = 0;
    static uint32_t fg = 0xEEEEEE, bg = 0x101010;
    int             x, y, line, mask;

    PSF_font *font         = (PSF_font *)_binary_fnt_psf_start;
    int       BytesPerLine = (font->width + 7) / 8;

// define's for state.
#define _NOP 0
#define _ESC 1
#define _CSI 2
#define _DCS 3
#define _OSC 4
    // Check state, continue if _NOP
    if (state != _NOP) { return; }

    switch (ch) {
        case '\n':
            py++;
            px = 0;
            return;
        case '\t': px = (px / 4 + 1) * 4; return;
        case '\b': px--; return;
        case '\r': px = 0; return;
        case '\v': py++; return;
        case '\033': state = _ESC; return;
    }

    // else Print this character
    // Calculate glyph (FontStart + ch * BytesPerGlyph)
    unsigned char *glyph =
        (unsigned char *)_binary_fnt_psf_start + font->headersize +
        (ch > 0 && ch < font->numglyph ? ch : 0) * font->bytesperglyph;

    int start = py * font->height * fb_info.framebuffer_width *
                    fb_info.framebuffer_bpp / 8 +
                px * (font->width + 1) * fb_info.framebuffer_bpp / 8;

    // Put the character on screen.
    for (y = 0; y < font->height; y++) {
        // save the starting position of the line
        line = start;
        mask = 1 << (font->width - 1);
        // display a row
        for (x = 0; x < font->width; x++) {
            *((uint32_t *)(fb_info.framebuffer_addr_lower + line)) =
                *((unsigned int *)glyph) & mask ? fg : bg;
            // adjust to the next pixel
            mask >>= 1;
            line  += fb_info.framebuffer_bpp / 8;
        }
        // adjust to the next line
        glyph += BytesPerLine;
        start += fb_info.framebuffer_width * fb_info.framebuffer_bpp / 8;
    }

    // Move to next character, next line or page up.
    if (px + 2 > fb_info.framebuffer_width / (font->width + 1)) {
        py++;
        // If at last line, move all lines up by 5.
        if (py >= fb_info.framebuffer_height / (font->height)) {
            memmove((void *)fb_info.framebuffer_addr_lower,
                    (void *)(fb_info.framebuffer_addr_lower +
                             5 * fb_info.framebuffer_width * font->height *
                                 fb_info.framebuffer_bpp / 8),
                    (fb_info.framebuffer_width - 5) *
                        fb_info.framebuffer_height * fb_info.framebuffer_bpp /
                        8);
            memset((void *)(fb_info.framebuffer_addr_lower +
                            (fb_info.framebuffer_height - 5 * font->height) *
                                fb_info.framebuffer_width *
                                fb_info.framebuffer_bpp / 8 +
                            fb_info.framebuffer_bpp / 8),
                   0x000000,
                   5 * fb_info.framebuffer_width * fb_info.framebuffer_height *
                       fb_info.framebuffer_bpp / 8);
            py -= 5;
        }
        px = 0;
    } else
        px++;
}

void FBuf::terminal_write(const char *data, size_t size) {
    for (int i = 0; i < size; i++) terminal_putchar(data[i]);
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
void terminal_putchar(unsigned short int c) {
    FBuf fb;
    fb.terminal_putchar(c);
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
