#ifndef LEMBALL_VISOS_FOUNDATION_VSSTRING_H
#define LEMBALL_VISOS_FOUNDATION_VSSTRING_H

#include "../../Common.h"

int StrCmpI(const char* p_left, const char* p_right, int p_maxLength);
void VsLtoa(long p_arg0, char* p_arg1, int p_arg2);
char* VsULtoa(unsigned long p_arg0, char* p_arg1, int p_arg2);
char* OkFailed(int p_arg0);
int Strtol(char* p_arg0, char** p_arg1, int p_arg2);
#endif
