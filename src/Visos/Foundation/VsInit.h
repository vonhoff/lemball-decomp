#ifndef LEMBALL_VISOS_FOUNDATION_VSINIT_H
#define LEMBALL_VISOS_FOUNDATION_VSINIT_H

#include "../../Common.h"

#include <setjmp.h>

extern jmp_buf g_vsExitJumpBuffer;
extern jmp_buf g_vsDebugJumpBuffer;

bool InternalStrmInit();
bool InternalStrmQuit();
bool InternalInpInit();
bool InternalInpQuit();
void InitSubSystems();
void InitQuitSubSystems();
bool InitCheckOptions(char* p_arg0);
void InitCmdLine(char* p_arg0);
int InitMain(char* p_arg0);
bool InternalStatInit();
bool InternalStatQuit();
bool InternalResInit();
bool InternalResQuit();
bool InternalTimeInit();
bool InternalTimeQuit();
bool InternalGdiInit();
bool InternalGdiQuit();
bool InternalMemInit();
bool InternalMemQuit();
bool VsFNetInit();
bool VsFNetQuit();
bool VsNetInit();
bool VsNetQuit();
void InitPreInit();
bool InternalDbgInit();
bool InternalDbgQuit(unsigned int p_force);

extern int g_nGraphicsDriverWing;
extern int g_nGraphicsDriverCds;
extern int g_nGraphicsDriverGdk;
extern int g_nParanoidMode;
extern int g_nFullscreen;
extern int g_nGarbageCollector;
extern int g_nStartupNoWait;
extern int g_nShowUnloading;
extern int g_nShowLoading;
extern int g_cParsedArgs;
extern char* g_apszParsedArgs[16];
#endif
