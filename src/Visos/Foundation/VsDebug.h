#ifndef LEMBALL_VISOS_FOUNDATION_VSDEBUG_H
#define LEMBALL_VISOS_FOUNDATION_VSDEBUG_H

#define DEBUG_SENTINEL ((int) 0xaa55aa55)
#define DEBUG_SENTINEL_WORD ((short) 0xaa55)
#define VS_UNINITIALISED DEBUG_SENTINEL

void _VSExit(int p_exitCode);
void _VSRELassert(const char* p_reason, const char* p_file, unsigned int p_line);
void WriteDebugString2File(char* p_text);
int _RAWOUT_DebugString(char* p_text);
int _RAWOUT_ErrorString(char* p_text);
int _RAWOUT_SysString(char* p_text);
void DisplayRelAssert(void* p_reason, void* p_file, unsigned int p_line);
void FatalWin32Error(char* p_context);
#endif
