#include <kernel/PS2_keyboard.hpp>

std::queue<char> Keyboard::buf;
uint32_t         Keyboard::buf_size = 0;
uint32_t         Keyboard::FLAGS;

char Keyboard::toASCII(uint8_t keycode) {
    clear_FLAG(9);

    if (get_FLAG(10)) { // If last keycode was 0xE0
        clear_FLAG(10);
        switch (keycode) {
            case 0x1D: set_FLAG(4); return '\000';
            case 0x9D: clear_FLAG(4); return '\000';
            case 0x38: set_FLAG(6); return '\000';
            case 0xB8: clear_FLAG(6); return '\000';
            case 0x5B: set_FLAG(7); return '\000';
            case 0xDB: clear_FLAG(7); return '\000';
            case 0x5C: set_FLAG(8); return '\000';
            case 0xDC: clear_FLAG(8); return '\000';
            case 0x5D: set_FLAG(11); return '\000';
            case 0xDD: clear_FLAG(11); return '\000';

            case 0x35: set_FLAG(9); return '/';
        }
    }

    switch (keycode) {
        case 0x3A: set_FLAG(0); return '\000';
        case 0x2A: set_FLAG(1); return '\000';
        case 0xAA: clear_FLAG(1); return '\000';
        case 0x36: set_FLAG(2); return '\000';
        case 0xB6: clear_FLAG(2); return '\000';
        case 0x1D: set_FLAG(3); return '\000';
        case 0x9D: clear_FLAG(3); return '\000';
        case 0x38: set_FLAG(5); return '\000';
        case 0xB8: clear_FLAG(5); return '\000';

        case 0xE0: set_FLAG(10); return '\000';

        case 0x39: return ' ';
        case 0x37: set_FLAG(9); return '*';
        case 0x4A: set_FLAG(9); return '-';
        case 0x4E: set_FLAG(9); return '+';
        case 0x53: set_FLAG(9); return '.';
        case 0x52: set_FLAG(9); return '0';
        case 0x4F: set_FLAG(9); return '1';
        case 0x50: set_FLAG(9); return '2';
        case 0x51: set_FLAG(9); return '3';
        case 0x4B: set_FLAG(9); return '4';
        case 0x4C: set_FLAG(9); return '5';
        case 0x4D: set_FLAG(9); return '6';
        case 0x47: set_FLAG(9); return '7';
        case 0x48: set_FLAG(9); return '8';
        case 0x49: set_FLAG(9); return '9';
    }

    if (keycode <= 0x35) { // ASCII keys lookup table
        if (isShift() && isCapsLk()) {
            return "\000\033!@#$%^&*()_+\010\011qwertyuiop{}\012\000asdfghjkl:"
                   "\"~\000|zxcvbnm<>?"[keycode];
        } else if (isShift() && !isCapsLk()) {
            return "\000\033!@#$%^&*()_+\010\011QWERTYUIOP{}\012\000ASDFGHJKL:"
                   "\"~\000|ZXCVBNM<>?"[keycode];
        } else if (!isShift() && isCapsLk()) {
            return "\000\0331234567890-=\010\011QWERTYUIOP[]\012\000ASDFGHJKL;"
                   "'`\000\\ZXCVBNM,./"[keycode];
        } else if (!isShift() && !isCapsLk()) {
            return "\000\0331234567890-=\010\011qwertyuiop[]\012\000asdfghjkl;"
                   "'`\000\\zxcvbnm,./"[keycode];
        }
    }
    return '\000';
}

void Keyboard::handle_key(uint8_t keycode) {
    buf.push(toASCII(keycode));
    char ch = buf.pop();
    if (ch) terminal_putchar(ch);
    // terminal_putchar(' ');
    // if (toASCII(keycode)) terminal_putchar(toASCII(keycode));
}

char Keyboard::poll_kb() { }

void handle_key(uint8_t keycode) { Keyboard::handle_key(keycode); }
char poll_kb() { return Keyboard::poll_kb(); }
