#ifndef LEMBALL_VISOS_FOUNDATION_VSINIT_H
#define LEMBALL_VISOS_FOUNDATION_VSINIT_H

#include <setjmp.h>

extern jmp_buf g_vsExitJumpBuffer;

void INIT_SubSystems();
void INIT_QuitSubSystems();
bool INIT_CheckOptions(char* p_option);
void INIT_CmdLine(char* p_commandLine);
int INIT_Main(char* p_commandLine);
void INIT_PreInit();

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
