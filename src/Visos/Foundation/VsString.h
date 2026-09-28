#ifndef LEMBALL_VISOS_FOUNDATION_VSSTRING_H
#define LEMBALL_VISOS_FOUNDATION_VSSTRING_H

int StrCmpI(const char* p_left, const char* p_right, int p_maxLength);
void vsLtoa(long p_value, char* p_buffer, int p_radix);
char* vsULtoa(unsigned long p_value, char* p_buffer, int p_radix);
char* OkFailed(int p_success);
int strtol(char* p_text, char** p_end, int p_base);
#endif
