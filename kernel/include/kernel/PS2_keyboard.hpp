#ifndef _KERNEL_PS2_KEYBOARD_HPP
#define _KERNEL_PS2_KEYBOARD_HPP

#include <kernel/framebuffer.hpp>
#include <queue>
#include <stdint.h>

class Keyboard {
  private:
    static std::queue<char> buf;
    static uint32_t         buf_size;

    static char toASCII(uint8_t keycode); // Converts keycode to ASCII.

    // Set bit of FLAGS to Zero (0)
    static void clear_FLAG(uint32_t bit) { FLAGS &= ~(1 << bit); }
    // Set bit of FLAGS to One  (1)
    static void set_FLAG(uint32_t bit) { FLAGS |= (1 << bit); }
    // Toggle (invert) the bit of FLAGS
    static void toggle_FLAG(uint32_t bit) {
        get_FLAG(bit) ? clear_FLAG(bit) : set_FLAG(bit);
    }
    // Get bit of FLAGS
    static bool get_FLAG(uint32_t bit) { return FLAGS & (1 << bit); }

    //! Stores Key states for Alt, Super etc.
    // Bit 0: Caps Lock
    // Bit 1: Left Shift
    // Bit 2: Right Shift
    // Bit 3: Left Ctrl
    // Bit 4: Right Ctrl
    // Bit 5: Left Alt
    // Bit 6: Right Alt
    // Bit 7: Left GUI
    // Bit 8: Right GUI
    // Bit 9: Keypad Key
    // Bit 10: E0 was last input
    // Bit 11: Apps
    static uint32_t FLAGS;

  public:
    static void handle_key(uint8_t keycode); // Get key from int, and store it.
    static char poll_kb();                   // Returns next char in buffer.

    static bool isCapsLk() { return get_FLAG(0); }
    static bool isShift() { return get_FLAG(1) || get_FLAG(2); }
    static bool isCtrl() { return get_FLAG(3) || get_FLAG(4); }
    static bool isAlt() { return get_FLAG(5) || get_FLAG(6); }
    static bool isGUI() { return get_FLAG(7) || get_FLAG(8); }
};

void handle_key(uint8_t keycode);
char poll_kb();

#endif