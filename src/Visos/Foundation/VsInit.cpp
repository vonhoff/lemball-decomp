#include "VsInit.h"

jmp_buf g_vsExitJumpBuffer;

// 68K 0x10213030 _STRM_Init__Fv
// STUB: LEMBALL 0x00458f70
bool StrmInit()
{
	return 0;
}

// 68K 0x10213176 _STRM_Quit__Fv
// STUB: LEMBALL 0x004590b0
bool StrmQuit()
{
	return 0;
}

// 68K 0x10213258 _INP_Init__Fv
// STUB: LEMBALL 0x00459130
void InpInit()
{
	// STRING: LEMBALL 0x004a0fb4 "Master Input Queue."
}

// 68K 0x1021332e _INP_Quit__Fv
// STUB: LEMBALL 0x004591f0
bool InpQuit()
{
	return 0;
}

// 68K 0x102133dc INIT_SubSystems__Fv
// ASSERT: _VSRELassert("EnoughMemory", "VSINIT.CPP", 414)
// STUB: LEMBALL 0x00459250
void InitSubSystems()
{
	// STRING: LEMBALL 0x004a0fc8 "VSINIT.CPP"
	// STRING: LEMBALL 0x004a0fd4 "EnoughMemory"
	// STRING: LEMBALL 0x004a0fe4 "ViSOS v"
	// STRING: LEMBALL 0x004a1000 " Visual Sciences Ltd\n\n"
	// STRING: LEMBALL 0x004a0ea0 "1994,1995"
	// STRING: LEMBALL 0x004a1028 "...\t("
	// STRING: LEMBALL 0x004a1018 "_MEM_Init   : "
	// STRING: LEMBALL 0x004a1034 "_STRM_Init  : "
	// STRING: LEMBALL 0x004a104c "_DBG_Init   : "
	// STRING: LEMBALL 0x004a1064 "_INP_Init   : "
	// STRING: LEMBALL 0x004a107c "_TIME_Init  : "
	// STRING: LEMBALL 0x004a10a4 "...\t("
	// STRING: LEMBALL 0x004a1094 "_GDI_Init   : "
	// STRING: LEMBALL 0x004a10b0 "_STAT_Init  : "
	// STRING: LEMBALL 0x004a10c8 "_RES_Init   : "
	// STRING: LEMBALL 0x004a10e0 "Main memory arena"
}

// 68K 0x102137a8 INIT_QuitSubSystems__Fv
// STUB: LEMBALL 0x00459520
void InitQuitSubSystems()
{
	// STRING: LEMBALL 0x004a1100 "**** MEMORY LEAK, dumping memory contents ****\n"
}

// 68K 0x102138be INIT_CheckOptions__FPc
// STUB: LEMBALL 0x004595d0
bool InitCheckOptions(char* p_arg0)
{
	// STRING: LEMBALL 0x004a0f20 "paranoid"
	// STRING: LEMBALL 0x004a0f2c "nowait"
	// STRING: LEMBALL 0x004a0f21 "paranoid"
	return 0;
}

// 68K 0x102139c8 INIT_CmdLine__FPc
// STUB: LEMBALL 0x004596b0
char* InitCmdLine(char* p_arg0)
{
	return 0;
}

// 68K 0x10213b1e INIT_Main__FPc
// STUB: LEMBALL 0x00459860
int InitMain(char* p_arg0)
{
	// STRING: LEMBALL 0x004a1134 "Command line options:\n"
	// STRING: LEMBALL 0x004a0f20 "paranoid"
	// STRING: LEMBALL 0x004a114c "  Option "
	// STRING: LEMBALL 0x004a1168 "selected\n"
	return 0;
}

// 68K 0x10218b78 _STAT_Init__Fv
// STUB: LEMBALL 0x0045aa80
bool StatInit()
{
	return 0;
}

// 68K 0x10218bc6 _STAT_Quit__Fv
// STUB: LEMBALL 0x0045aab0
bool StatQuit()
{
	return 0;
}

// 68K 0x10201420 _RES_Init__Fv
// STUB: LEMBALL 0x0045b900
bool ResInit()
{
	return 0;
}

// 68K 0x10201580 _RES_Quit__Fv
// STUB: LEMBALL 0x0045ba50
bool ResQuit()
{
	return 0;
}

// 68K 0x10110112 _TIME_Init__Fv
// STUB: LEMBALL 0x00462e60
bool TimeInit()
{
	return 0;
}

// 68K 0x1011012e _TIME_Quit__Fv
// STUB: LEMBALL 0x00462e70
bool TimeQuit()
{
	return 0;
}

// 68K 0x10107ab6 _GDI_Init__Fv
// STUB: LEMBALL 0x0046ba80
bool GdiInit()
{
	return 0;
}

// 68K 0x10107ca0 _GDI_Quit__Fv
// STUB: LEMBALL 0x0046bb70
bool GdiQuit()
{
	return 0;
}

// 68K 0x1010fd50 _MEM_Init__Fv
// STUB: LEMBALL 0x0046f060
bool MemInit()
{
	// STRING: LEMBALL 0x004a2184 "Master Main Ram Arena"
	return 0;
}

// 68K 0x1010ff2e _MEM_Quit__Fv
// STUB: LEMBALL 0x0046f120
bool MemQuit()
{
	// STRING: LEMBALL 0x004a219c "Master Main RAM Arena memory was locked, when it should not have been\n"
	// STRING: LEMBALL 0x004a21e4 "attempting to unlock it iteratively\n"
	// STRING: LEMBALL 0x004a220c "Freeing the Master Main RAM Arena memory failed\n"
	// STRING: LEMBALL 0x004a2240 "GetLastError()=="
	return 0;
}

// 68K 0x10106d86 VSFNET_Init__Fv
// STUB: LEMBALL 0x0046f3b0
bool VsFNetInit()
{
	// STRING: LEMBALL 0x004a226c "ERROR"
	// STRING: LEMBALL 0x004a2274 "Unable to start 'VSNET Message loop' thread\n"
	// STRING: LEMBALL 0x004a22a4 "Network initialisation timed out\n"
	// STRING: LEMBALL 0x004a22c8 "Network queue initialisation timed out\n"
	return 0;
}

// 68K 0x10106dc8 VSFNET_Quit__Fv
// STUB: LEMBALL 0x0046f480
bool VsFNetQuit()
{
	// STRING: LEMBALL 0x004a22f0 "Network quit timed out\n"
	return 0;
}

// 68K 0x1010c64e VSNET_Init__Fv
// STUB: LEMBALL 0x0046fbb0
bool VsNetInit()
{
	// STRING: LEMBALL 0x004a23c8 "ERROR"
	// STRING: LEMBALL 0x004a23d0 "Unable to start 'VSNET Message loop' thread\n"
	// STRING: LEMBALL 0x004a2400 "Network initialisation timed out\n"
	// STRING: LEMBALL 0x004a2424 "Network queue initialisation timed out\n"
	return 0;
}

// 68K 0x1010c6d8 VSNET_Quit__Fv
// STUB: LEMBALL 0x0046fc80
bool VsNetQuit()
{
	// STRING: LEMBALL 0x004a244c "Network quit timed out\n"
	return 0;
}

// 68K 0x10215cfc INIT_PreInit__Fv
// STUB: LEMBALL 0x004727b0
void InitPreInit()
{
}

// 68K 0x1010fd1a _DBG_Init__Fv
// STUB: LEMBALL 0x00472be0
bool DbgInit()
{
	// STRING: LEMBALL 0x004a29f8 "Sync_Debug"
	// STRING: LEMBALL 0x004a2a30 "ERROR"
	// STRING: LEMBALL 0x004a2a38 "Unable to start 'Debug Message loop' thread\n"
	return 0;
}

// 68K 0x1010fd34 _DBG_Quit__FUc
// STUB: LEMBALL 0x00472c70
bool DbgQuit(unsigned char p_force)
{
	return 0;
}
