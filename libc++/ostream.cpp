#include <iostream>

std::ostream::ostream() { }
std::ostream std::ostream::operator<<(const char *str) {
    FBuf::terminal_writestring(str);
    return *this;
}
