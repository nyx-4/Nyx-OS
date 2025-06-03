#include <string>

string::string(const char *str) { strcpy(this->Str, str); }
string::string(string &str) { strcpy(this->Str, str.Str); }

string &string::operator=(string str) {
    strcpy(this->Str, str.Str);
    return *this;
}
string string::operator+(string str) const {
    string tmp = this->Str;
    strcat(tmp.Str, str.Str);
    return tmp;
}
string &string::operator+=(string str) {
    strcat(this->Str, str.Str);
    return *this;
}
string &string::operator+=(char ch) {
    const char tmp[] = {ch, '\0'};
    strcat(this->Str, tmp);
    return *this;
}
string string::operator*(int times) const {
    string tmp = "";
    for (; times > 0; times--) tmp += this->Str;
    return tmp;
}
string &string::operator*=(int times) {
    for (; times > 0; times--) *this += this->Str;
    return *this;
}
char       &string::operator[](int idx) { return this->Str[idx]; }
const char &string::operator[](int idx) const { return this->Str[idx]; }

bool string::operator<=(string str) const {
    return strcmp(this->Str, str.Str) <= 0;
}
bool string::operator==(string str) const {
    return strcmp(this->Str, str.Str) == 0;
}
bool string::operator!=(string str) const {
    return strcmp(this->Str, str.Str) != 0;
}
bool string::operator<(string str) const {
    return strcmp(this->Str, str.Str) < 0;
}
bool string::operator>=(string str) const {
    return strcmp(this->Str, str.Str) >= 0;
}
bool string::operator>(string str) const {
    return strcmp(this->Str, str.Str) > 0;
}

string::operator const char *() { return this->Str; }

long   strtol(string s) { }
string ltostr(long s) { }
void   swap(char &a, char &b) {
    char tmp = a;
    a        = b;
    b        = tmp;
}
string strrev(string s) { }
