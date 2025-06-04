#include <iostream>

ostream::ostream() { }
ostream ostream::operator<<(const char *str) { terminal_writestring(str); }
