#ifndef LEMBALL_VISOS_FOUNDATION_VSDEBUG_H
#define LEMBALL_VISOS_FOUNDATION_VSDEBUG_H

#include "../../Common.h"

void VsExit(int p_exitCode);
void VsRelAssert(const char* p_reason, const char* p_file, unsigned int p_line);
void WriteDebugString2File(char* p_text);
bool RawOutDebugString(char* p_text);
bool RawOutErrorString(char* p_text);
bool RawOutSysString(char* p_text);
void DisplayRelAssert(const char* p_reason, const char* p_file, unsigned int p_line);
#endif
