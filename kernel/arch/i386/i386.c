
#include <stdint.h>
#include <string.h>

#include <kernel/fb.h>
#include <kernel/kernel.h>
#include <kernel/screen.h>
#include <kernel/tty.h>
#include <kernel/vt.h>

#include "gdt.h"
#include "multiboot2.h"
#include "ttyS.h"
#include "vga.h"

struct fb   fb;
struct vga  vga;
struct ttyS ttyS1_tty;
struct vt   tty1_tty;

typedef void initfunc_t(void);
extern initfunc_t *__init_array_start[], *__init_array_end[];  // NOLINT

static void handle_init_array(void) {
    for (initfunc_t **ptr = __init_array_start; ptr != __init_array_end;
         ptr++) {
        (*ptr)();
    }
}

void kernel_early(uint32_t magic, struct multiboot2_info *multiboot2) {
    // Verify the multiboot2 magic number.
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        return;
    }

    struct multiboot2_tag_string *cmdline_tag =
        (struct multiboot2_tag_string *)multiboot2_tag_lookup(
            multiboot2, MULTIBOOT2_TAG_TYPE_CMDLINE);
    const char *cmdline = cmdline_tag ? cmdline_tag->string : "";

    // Create a /dev/ttyS0 device for the first serial line.
    const uint16_t       SERIAL_PORT_COM1 = 0x3F8;
    const unsigned short WIDTH            = 80;
    const unsigned short HEIGHT           = 25;
    struct tty *ttyS0 = ttyS_init(&ttyS1_tty, SERIAL_PORT_COM1, WIDTH, HEIGHT);

    // Try to locate a multiboot2 tag with framebuffer information
    struct multiboot2_tag_framebuffer *fbinfo =
        (struct multiboot2_tag_framebuffer *)multiboot2_tag_lookup(
            multiboot2, MULTIBOOT2_TAG_TYPE_FRAMEBUFFER);

    struct multiboot2_tag_framebuffer_common *fb_common =
        (fbinfo ? &fbinfo->common : NULL);

    // Initialize a screen for tty1 on the framebuffer.
    struct screen *tty1_screen;

    if (!fb_common ||
        fb_common->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_EGA_TEXT) {
        tty1_screen = vga_init(&vga);  // use vga text mode

    } else if (fb_common->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_RGB) {
        if (fb_common->framebuffer_bpp != 32) {
            return;  // TODO: support other framebuffer_bpp also
        }

        tty1_screen =
            fb_init(&fb, fb_common->framebuffer_addr,
                    fb_common->framebuffer_width, fb_common->framebuffer_height,
                    fb_common->framebuffer_pitch, fb_common->framebuffer_bpp);

    } else {  // unsupported framebuffer type
        return;
    }

    // Create a /dev/tty1 device connected to the screen
    struct tty *tty1 = vt_init(&tty1_tty, tty1_screen);

    // Select the appropriate terminal device as the kernel /dev/console.
    if (!strcmp(cmdline, "--console=ttyS0")) {
        dev_console = ttyS0;
    } else {  // --console=tty1 is default
        dev_console = tty1;
    }

    handle_init_array();  // handle global constructors

    gdt_init();
    setup_gdt(sizeof(gdt), (uintptr_t)&gdt);  // setup GDT

    // Run the main kernel.
    kernel_main();
}
