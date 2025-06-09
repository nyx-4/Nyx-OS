#include <string>

namespace std {
std::string::string(const char *str) { strcpy(this->Str, str); }
std::string::string(std::string &str) { strcpy(this->Str, str.Str); }
} // namespace std

std::string &std::string::operator=(std::string str) {
    strcpy(this->Str, str.Str);
    return *this;
}
std::string std::string::operator+(std::string str) const {
    string tmp = this->Str;
    strcat(tmp.Str, str.Str);
    return tmp;
}
std::string &std::string::operator+=(std::string str) {
    strcat(this->Str, str.Str);
    return *this;
}
std::string &std::string::operator+=(char ch) {
    const char tmp[] = {ch, '\0'};
    strcat(this->Str, tmp);
    return *this;
}
std::string std::string::operator*(int times) const {
    std::string tmp = "";
    for (; times > 0; times--) tmp += this->Str;
    return tmp;
}
std::string &std::string::operator*=(int times) {
    for (; times > 0; times--) *this += this->Str;
    return *this;
}
char       &std::string::operator[](int idx) { return this->Str[idx]; }
const char &std::string::operator[](int idx) const { return this->Str[idx]; }

bool std::string::operator==(string str) const {
    return strcmp(this->Str, str.Str) == 0;
}
bool std::string::operator!=(string str) const {
    return strcmp(this->Str, str.Str) != 0;
}
bool std::string::operator<(string str) const {
    return strcmp(this->Str, str.Str) < 0;
}
bool std::string::operator<=(string str) const {
    return strcmp(this->Str, str.Str) <= 0;
}
bool std::string::operator>(string str) const {
    return strcmp(this->Str, str.Str) > 0;
}
bool std::string::operator>=(string str) const {
    return strcmp(this->Str, str.Str) >= 0;
}

bool std::string::operator==(const char *str) const {
    return *this == (string)str;
}
bool std::string::operator!=(const char *str) const {
    return *this != (string)str;
}
bool std::string::operator>(const char *str) const {
    return *this > (string)str;
}
bool std::string::operator>=(const char *str) const {
    return *this >= (string)str;
}
bool std::string::operator<(const char *str) const {
    return *this < (string)str;
}
bool std::string::operator<=(const char *str) const {
    return *this <= (string)str;
}

std::string::operator const char *() { return this->Str; }

long        strtol(std::string s) { }
std::string ltostr(long s) { }
void        swap(char &a, char &b) {
    char tmp = a;
    a        = b;
    b        = tmp;
}
std::string strrev(std::string s) { }
