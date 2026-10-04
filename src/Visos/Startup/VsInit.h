#ifndef LEMBALL_VISOS_FOUNDATION_VSINIT_H
#define LEMBALL_VISOS_FOUNDATION_VSINIT_H

#include <setjmp.h>

extern jmp_buf g_vsExitJumpBuffer;
extern jmp_buf g_vsDebugJumpBuffer;

bool _STRM_Init();
bool _STRM_Quit();
bool _INP_Init();
bool _INP_Quit();
void INIT_SubSystems();
void INIT_QuitSubSystems();
bool INIT_CheckOptions(char* p_option);
void INIT_CmdLine(char* p_commandLine);
int INIT_Main(char* p_commandLine);
bool _STAT_Init();
bool _STAT_Quit();
bool _RES_Init();
bool _RES_Quit();
bool _TIME_Init();
bool _TIME_Quit();
bool _GDI_Init();
bool _GDI_Quit();
bool _MEM_Init();
bool _MEM_Quit();
bool VSFNET_Init();
bool VSFNET_Quit();
bool VSNET_Init();
bool VSNET_Quit();
void INIT_PreInit();
bool _DBG_Init();
bool _DBG_Quit(unsigned int p_force);

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
