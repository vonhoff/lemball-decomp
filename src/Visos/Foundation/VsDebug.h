#ifndef LEMBALL_VISOS_FOUNDATION_VSDEBUG_H
#define LEMBALL_VISOS_FOUNDATION_VSDEBUG_H

#include "../../Common.h"

void InternalVsExit(int p_exitCode);
void InternalVsRelAssert(const char* p_reason, const char* p_file, unsigned int p_line);
void WriteDebugString2File(char* p_text);
int InternalRawOutDebugString(char* p_text);
int InternalRawOutErrorString(char* p_text);
int InternalRawOutSysString(char* p_text);
void DisplayRelAssert(void* p_reason, void* p_file, unsigned int p_line);
void FatalWin32Error(char* p_context);
#endif
