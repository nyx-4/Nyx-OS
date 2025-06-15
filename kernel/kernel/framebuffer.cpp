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
    static uint32_t px = 0, py = 0, state = 0, args[5] = {0}, N_args = 0;
    static uint32_t fg = 0xFFFFFF, bg = 0x000000;
    uint32_t        x, y, line, mask;

    PSF_font *font         = (PSF_font *)_binary_fnt_psf_start;
    int       BytesPerLine = (font->width + 7) / 8;

/*              Explanation of ESC (\033)
 * ANSI Escape Sequences:
 * Gist: https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
 *
 *      -- State --
 * _NOP - Just print ch. Normal behavior.
 * _ESC - The current ch is \033.
 * _CSI - Control Sequence Introducer: sequence starting with ESC [
 * _DCS - Device Control String: sequence starting with ESC P
 * _OSC - Operating System Command: sequence starting with ESC
 *
 *      -- CSI --
 * Cursor Controls: px and py defines current column and row. Manipulate them.
 * Erase Functions: Just call dummy functions, I'll implement later.
 * Colors / Graphics Mode: Use args[] to store args. Hint: fg and bg. Implement:
 *      - 8-16 Colors, use FBColor(string).
 *      - 256 Colors, use some function to get 24-bit color.
 *      - RGB, use FBColor(uint8_t, uint8_t, uint_8)
 *
 * Tips:
 * - The escape seq for RGB color is`ESC[38;2;{r};{g};{b}m`
 *      Store args[] = { 38, 2, R, G, B }
 * - *Remember* to clear args, when done with them
 *
 */

// #define's for state. Define more if needed.
#define _NOP 0
#define _ESC 1
#define _CSI 2
#define _DCS 3
#define _OSC 4
    // Check state, continue if _NOP
    if (state != _NOP) {
        if (state == _ESC) switch (ch) {
                case '[': state = _CSI; return;
                case 'P': state = _DCS; return;
                case ']': state = _OSC; return;
                default: state = _NOP;
            }
        // An example of how to implement \033.
        if (state == _CSI) {
            if (ch >= '0' && ch <= '9')
                args[N_args] = args[N_args] * 10 + ch - '0';
            else if (ch == ';')
                N_args++;
            else if (ch == 'H' && N_args == 0)
                state = px = py = 0;
            else if (ch == 'H' && N_args == 1) {
                py    = args[0];
                px    = args[1];
                state = args[0] = args[1] = N_args = 0;
            } else if (ch == 'H')
                state = args[0] = args[1] = N_args = 0;
            else if (ch == 'm' && N_args == 0) {
                switch (args[0]) {
                    case 30: fg = 0x000000; break;
                    case 31: fg = 0xFF0000; break;
                    case 32: fg = 0x00FF00; break;
                    case 33: fg = 0xFFFF00; break;
                    case 34: fg = 0x0000FF; break;
                    case 35: fg = 0xFF00FF; break;
                    case 36: fg = 0x00FFFF; break;
                    case 37: fg = 0xFFFFFF; break;
                    case 39: fg = 0xFFFFFF; break;
                    case 40: bg = 0x000000; break;
                    case 41: bg = 0xFF0000; break;
                    case 42: bg = 0x00FF00; break;
                    case 43: bg = 0xFFFF00; break;
                    case 44: bg = 0x0000FF; break;
                    case 45: bg = 0xFF00FF; break;
                    case 46: bg = 0x00FFFF; break;
                    case 47: bg = 0xFFFFFF; break;
                    case 49: bg = 0xFFFFFF; break;
                }
                state = args[0] = N_args = 0;
                return;
            }
        }
        // Return, because ch is handled.
        return;
    }

    switch (ch) {
        case '\n':
            py++;
            px = 0;
            return;
        case '\b':
            px--;
            terminal_putchar(' ');
            px--;
            return;
        case '\t': px = (px / 4 + 1) * 4; return;
        case '\r': px = 0; return;
        case '\v': py++; return;
        case '\033': state = _ESC; return;
        default: break;
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
    for (size_t i = 0; i < size; i++) terminal_putchar(data[i]);
}

void FBuf::terminal_writestring(const char *data) {
    terminal_write(data, strlen(data));
}

void FBuf::terminal_writehex(uint32_t num) {
    for (int32_t i = 7; i >= 0; i--)
        terminal_putchar("0123456789ABCDEF"[(num >> (4 * i)) & 0xF]);
}

void FBuf::test() {
    if (fb_info.framebuffer_bpp == 32 && fb_info.framebuffer_type == 1) {
        // Fill the screen with red
        for (uint32_t y = 0; y < fb_info.framebuffer_height; y++) {
            for (uint32_t x = 0; x < fb_info.framebuffer_width; x++) {
                uint32_t *fb = (uint32_t *)fb_info.framebuffer_addr_lower;
                fb[(int)y * (fb_info.framebuffer_pitch / 4) + x] =
                    (uint32_t)((y) + (x - 0x80));
            }
        }
    }
}

void terminal_initialize(void) { FBuf::terminal_initialize(); }
void terminal_putchar(unsigned short int c) { FBuf::terminal_putchar(c); }
void terminal_write(const char *data, size_t size) {
    FBuf::terminal_write(data, size);
}
void terminal_writestring(const char *data) {
    FBuf::terminal_writestring(data);
}
// void terminal_writeint(int32_t num) {
//     FBuf::terminal_writeint(num);
// }
// void terminal_writeuint(uint32_t num) {
//     FBuf::terminal_writeuint(num);
// }
void terminal_writehex(uint32_t num) { FBuf::terminal_writehex(num); }

int32_t FBuf::FBColor(std::string color) {
    if (color == "white") return FBColor(0xFFFFFF);
    if (color == "black") return FBColor(0x000000);
    return 0x000000;
}

int32_t FBuf::FBColor(uint32_t color) {
    switch (fb_info.framebuffer_bpp) {
        case 24: return color;
        case 32: return color << 8;
        default: return color;
    }
}

int32_t FBuf::FBColor(uint8_t Red, uint8_t Green, uint8_t Blue) {
    return FBColor(Red << 16 | Green << 8 | Blue);
}

int32_t FBColor(std::string color) { return FBuf::FBColor(color); }
int32_t FBColor(uint32_t color) { return FBuf::FBColor(color); }
int32_t FBColor(uint8_t Red, uint8_t Green, uint8_t Blue) {
    return FBuf::FBColor(Red, Green, Blue);
}
