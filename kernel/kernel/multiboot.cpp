#include <kernel/multiboot.hpp>

bool     Multiboot::isValidMultiboot = false;
bool     Multiboot::isValidMemMap    = false;
bool     Multiboot::isValidFB        = false;
uint32_t Multiboot::flags            = 0;

extern "C" void get_multiboot(uint32_t magic, multiboot_info_t *mbi) {
    Multiboot(magic, mbi);
}

Multiboot::Multiboot(uint32_t magic, multiboot_info_t *mbi) {
    if (magic != 0x2BADB002) return;
    flags            = mbi->flags;
    isValidMultiboot = true;

    // Check if memory map is available
    if (mbi->flags & (1 << 6)) {
        isValidMemMap = true;
        for (mmap_info_t *mmap = (mmap_info_t *)mbi->mmap_addr;
             mbi->mmap_length + mbi->mmap_addr > (uint32_t)mmap; mmap++)
            if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE &&
                mmap->length_lower > (uint32_t)0x00100000) {
                // Save this memory in Memory class.
                Memory(mmap->base_addr_lower, mmap->length_lower,
                       mbi->flags & (1 << 6));
            }
    }

    if (mbi->flags & (1 << 12)) isValidFB = true;
    video_info_t fb_info;

    fb_info.vbe_control_info  = mbi->vbe_control_info;
    fb_info.vbe_mode_info     = mbi->vbe_mode_info;
    fb_info.vbe_mode          = mbi->vbe_mode;
    fb_info.vbe_interface_seg = mbi->vbe_interface_seg;
    fb_info.vbe_interface_off = mbi->vbe_interface_off;
    fb_info.vbe_interface_len = mbi->vbe_interface_len;

    fb_info.framebuffer_addr_lower = mbi->framebuffer_addr_lower;
    fb_info.framebuffer_addr_upper = mbi->framebuffer_addr_upper;
    fb_info.framebuffer_pitch      = mbi->framebuffer_pitch;
    fb_info.framebuffer_width      = mbi->framebuffer_width;
    fb_info.framebuffer_height     = mbi->framebuffer_height;
    fb_info.framebuffer_bpp        = mbi->framebuffer_bpp;
    fb_info.framebuffer_type       = mbi->framebuffer_type;

    if (fb_info.framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
        fb_info.framebuffer_red_field_position =
            mbi->framebuffer_red_field_position;
        fb_info.framebuffer_red_mask_size = mbi->framebuffer_red_mask_size;

        fb_info.framebuffer_green_field_position =
            mbi->framebuffer_green_field_position;
        fb_info.framebuffer_green_mask_size = mbi->framebuffer_green_mask_size;

        fb_info.framebuffer_blue_field_position =
            mbi->framebuffer_blue_field_position;
        fb_info.framebuffer_blue_mask_size = mbi->framebuffer_blue_mask_size;
    } else {
        fb_info.framebuffer_palette_addr = mbi->framebuffer_palette_addr;
        fb_info.framebuffer_palette_num_colors =
            mbi->framebuffer_palette_num_colors;
    }

    // Load this information in Framebuffer.
    FBuf fb(fb_info, mbi->flags & (1 << 12), mbi->flags & (1 << 11));
}
