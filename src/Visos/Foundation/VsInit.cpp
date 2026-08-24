#include "VsInit.h"

#include "../../Control/Support/PreInit.h"
#include "../Animation/BaseStat.h"
#include "../Animation/StatManager.h"
#include "../Network/FileNetwork.h"
#include "../Network/TcpIpNetwork.h"
#include "../Target/TargetPlatformServices.h"
#include "Arena.h"
#include "VsDebug.h"
#include "VsDebugStreambuf.h"
#include "VsOStream.h"
#include "VsString.h"

#include <new.h>

extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* p_security,
															 unsigned int p_stack,
															 unsigned int(__stdcall* p_start)(void*),
															 void* p_param,
															 unsigned int p_flags,
															 unsigned int* p_id);
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* p_thread, int p_priority);
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* p_security,
															 int p_manual,
															 int p_initial,
															 const char* p_name);
extern "C" __declspec(dllimport) unsigned int __stdcall WaitForSingleObject(void* p_handle, unsigned int p_ms);
extern "C" __declspec(dllimport) int __stdcall TerminateThread(void* p_thread, unsigned int p_exit);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int p_code);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window,
														  const char* p_text,
														  const char* p_caption,
														  unsigned int p_type);
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);

jmp_buf g_vsExitJumpBuffer;

// GLOBAL: LEMBALL 0x004a0e88
int g_nInitAllocBaseline = 0;

// GLOBAL: LEMBALL 0x004a0e94
int g_nVisosVersionMajor = 3;

// GLOBAL: LEMBALL 0x004a0e98
int g_nVisosVersionMinor = 0;

// GLOBAL: LEMBALL 0x004a93b0
char g_szStreamFixedBuffer[0x400];

// 68K 0x10213030 _STRM_Init__Fv
// FUNCTION: LEMBALL 0x00458f70
bool StrmInit()
{
	void* storage;

	storage = operator new(0x1c);
	if (storage != 0) {
		g_pDebugStreambuf =
			new (storage) VsDebugStreambuf(g_szStreamFixedBuffer, 0x400, (void (*)(char*)) RawOutDebugString);
	}
	else {
		g_pDebugStreambuf = 0;
	}

	storage = operator new(0x1c);
	if (storage != 0) {
		g_pSysStreambuf =
			new (storage) VsDebugStreambuf(g_szStreamFixedBuffer, 0x400, (void (*)(char*)) RawOutSysString);
	}
	else {
		g_pSysStreambuf = 0;
	}

	storage = operator new(0x1c);
	if (storage != 0) {
		g_pErrorStreambuf =
			new (storage) VsDebugStreambuf(g_szStreamFixedBuffer, 0x400, (void (*)(char*)) RawOutErrorString);
	}
	else {
		g_pErrorStreambuf = 0;
	}

	storage = operator new(0x14c);
	if (storage != 0) {
		g_pDebugOutput = new (storage) VsOStream(g_pDebugStreambuf);
	}
	else {
		g_pDebugOutput = 0;
	}

	storage = operator new(0x14c);
	if (storage != 0) {
		g_pSysOutput = new (storage) VsOStream(g_pSysStreambuf);
	}
	else {
		g_pSysOutput = 0;
	}

	storage = operator new(0x14c);
	if (storage != 0) {
		g_pErrorOutput = new (storage) VsOStream(g_pErrorStreambuf);
	}
	else {
		g_pErrorOutput = 0;
	}

	return 1;
}

// 68K 0x10213176 _STRM_Quit__Fv
// STUB: LEMBALL 0x004590b0
bool StrmQuit()
{
	return 0;
}

// 68K 0x10213258 _INP_Init__Fv
// STUB: LEMBALL 0x00459130
bool InpInit()
{
	// STRING: LEMBALL 0x004a0fb4 "Master Input Queue."
	return 0;
}

// 68K 0x1021332e _INP_Quit__Fv
// STUB: LEMBALL 0x004591f0
bool InpQuit()
{
	return 0;
}

// 68K 0x102133dc INIT_SubSystems__Fv
// ASSERT: _VSRELassert("EnoughMemory", "VSINIT.CPP", 414)
// FUNCTION: LEMBALL 0x00459250
void InitSubSystems()
{
	int memOk;
	int strmOk;
	int dbgOk;
	int inpOk;
	int timeOk;
	int gdiOk;
	int statOk;
	int resOk;
	BaseStat* stat;
	void* storage;

	memOk = MemInit();
	if (memOk == 0) {
		// STRING: LEMBALL 0x004a0fd4 "EnoughMemory"
		// STRING: LEMBALL 0x004a0fc8 "VSINIT.CPP"
		VsRelAssert("EnoughMemory", "VSINIT.CPP", 0x19e);
	}

	strmOk = StrmInit();
	dbgOk = DbgInit();
	g_nDebugInitialized = dbgOk;
	TargetPlatformServicesInit();

	// STRING: LEMBALL 0x004a0fe4 "ViSOS v"
	*g_pSysOutput << "ViSOS v" << g_nVisosVersionMajor << "." << g_nVisosVersionMinor << "(" << 201 << ")"
				  << "\n";
	// STRING: LEMBALL 0x004a0ea0 "1994,1995"
	// STRING: LEMBALL 0x004a1000 " Visual Sciences Ltd\n\n"
	*g_pSysOutput << "(c)" << "1994,1995" << " Visual Sciences Ltd\n\n";
	// STRING: LEMBALL 0x004a1018 "_MEM_Init   : "
	// STRING: LEMBALL 0x004a1028 "...\t("
	*g_pSysOutput << "_MEM_Init   : " << OkFailed(memOk) << "...\t(" << (int) g_preInitActive.m_memoryBudget << ")\n";
	// STRING: LEMBALL 0x004a1034 "_STRM_Init  : "
	*g_pSysOutput << "_STRM_Init  : " << OkFailed(strmOk) << "...\n";
	// STRING: LEMBALL 0x004a104c "_DBG_Init   : "
	*g_pSysOutput << "_DBG_Init   : " << OkFailed(dbgOk) << "...\n";

	g_nInitAllocBaseline = g_pMasterArena->GetAllocSize();

	inpOk = InpInit();
	// STRING: LEMBALL 0x004a1064 "_INP_Init   : "
	*g_pSysOutput << "_INP_Init   : " << OkFailed(inpOk) << "...\n";

	timeOk = TimeInit();
	// STRING: LEMBALL 0x004a107c "_TIME_Init  : "
	*g_pSysOutput << "_TIME_Init  : " << OkFailed(timeOk) << "...\n";

	gdiOk = GdiInit();
	// STRING: LEMBALL 0x004a1094 "_GDI_Init   : "
	// STRING: LEMBALL 0x004a10a4 "...\t("
	*g_pSysOutput << "_GDI_Init   : " << OkFailed(gdiOk) << "...\t(" << (int) g_preInitActive.m_flags << ")\n";

	statOk = StatInit();
	// STRING: LEMBALL 0x004a10b0 "_STAT_Init  : "
	*g_pSysOutput << "_STAT_Init  : " << OkFailed(statOk) << "...\n";

	resOk = ResInit();
	// STRING: LEMBALL 0x004a10c8 "_RES_Init   : "
	*g_pSysOutput << "_RES_Init   : " << OkFailed(resOk) << "...\n";

	// STRING: LEMBALL 0x004a10e0 "Main memory arena"
	storage = operator new(0x20);
	if (storage != 0) {
		stat = (BaseStat*) storage;
		new (storage) MogloadStat("Main memory arena");
	}
	else {
		stat = 0;
	}
	g_pStatManager->Register(stat);
	g_pMasterArena->m_parentArena = (Arena*) stat;
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
	// STRING: LEMBALL 0x004a114c "  Option "
	// STRING: LEMBALL 0x004a1168 "selected\n"
	return 0;
}

// 68K 0x10218b78 _STAT_Init__Fv
// FUNCTION: LEMBALL 0x0045aa80
bool StatInit()
{
	void* storage;

	storage = operator new(0x14);
	if (storage != 0) {
		storage = new (storage) StatManager(0x20);
	}
	else {
		storage = 0;
	}
	g_pStatManager = (StatManager*) storage;

	return g_pStatManager != 0;
}

// 68K 0x10218bc6 _STAT_Quit__Fv
// FUNCTION: LEMBALL 0x0045aab0
bool StatQuit()
{
	if (g_pStatManager != 0) {
		g_pStatManager->StreamOut(*g_pSysOutput);
		g_pStatManager->~StatManager();
		operator delete(g_pStatManager);
		g_pStatManager = 0;
	}
	return 1;
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
// FUNCTION: LEMBALL 0x00462e60
bool TimeInit()
{
	return 1;
}

// 68K 0x1011012e _TIME_Quit__Fv
// FUNCTION: LEMBALL 0x00462e70
bool TimeQuit()
{
	return 1;
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

// STUB: LEMBALL 0x0046f210
unsigned int __stdcall FileNetworkMessageThread(void* p_unused)
{
	(void) p_unused;
	return 0;
}

// 68K 0x10106d86 VSFNET_Init__Fv
// FUNCTION: LEMBALL 0x0046f3b0
bool VsFNetInit()
{
	unsigned long startTime;

	g_hFileNetworkThread =
		CreateThread(0, 0, FileNetworkMessageThread, 0, 0, &g_dwFileNetworkThreadId);
	if (g_hFileNetworkThread == 0) {
		// STRING: LEMBALL 0x004a2274 "Unable to start 'VSNET Message loop' thread\n"
		// STRING: LEMBALL 0x004a226c "ERROR"
		MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hFileNetworkThread, 2);

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pBaseNetwork == 0) {
	}
	if (g_pBaseNetwork == 0) {
		// STRING: LEMBALL 0x004a22a4 "Network initialisation timed out\n"
		*g_pErrorOutput << "Network initialisation timed out\n";
		return 0;
	}

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pNetworkStatusQueue == 0) {
	}
	if (g_pNetworkStatusQueue == 0) {
		// STRING: LEMBALL 0x004a22c8 "Network queue initialisation timed out\n"
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return 0;
	}

	return 1;
}

// 68K 0x10106dc8 VSFNET_Quit__Fv
// FUNCTION: LEMBALL 0x0046f480
bool VsFNetQuit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->Process();
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 10000 && g_pBaseNetwork != 0) {
		}
		if (g_pBaseNetwork != 0) {
			// STRING: LEMBALL 0x004a22f0 "Network quit timed out\n"
			*g_pErrorOutput << "Network quit timed out\n";
			return 0;
		}
		return 1;
	}
	return 0;
}

// STUB: LEMBALL 0x0046fa10
unsigned int __stdcall TcpIpNetworkMessageThread(void* p_unused)
{
	(void) p_unused;
	return 0;
}

// 68K 0x1010c64e VSNET_Init__Fv
// FUNCTION: LEMBALL 0x0046fbb0
bool VsNetInit()
{
	unsigned long startTime;

	g_hTCPIPNetworkThread =
		CreateThread(0, 0, TcpIpNetworkMessageThread, 0, 0, &g_dwTCPIPNetworkThreadId);
	if (g_hTCPIPNetworkThread == 0) {
		// STRING: LEMBALL 0x004a23d0 "Unable to start 'VSNET Message loop' thread\n"
		// STRING: LEMBALL 0x004a23c8 "ERROR"
		MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hTCPIPNetworkThread, 2);

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pBaseNetwork == 0) {
	}
	if (g_pBaseNetwork == 0) {
		// STRING: LEMBALL 0x004a2400 "Network initialisation timed out\n"
		*g_pErrorOutput << "Network initialisation timed out\n";
		return 0;
	}

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pNetworkStatusQueue == 0) {
	}
	if (g_pNetworkStatusQueue == 0) {
		// STRING: LEMBALL 0x004a2424 "Network queue initialisation timed out\n"
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return 0;
	}

	return 1;
}

// 68K 0x1010c6d8 VSNET_Quit__Fv
// FUNCTION: LEMBALL 0x0046fc80
bool VsNetQuit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->Process();
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 10000 && g_pBaseNetwork != 0) {
		}
		if (g_pBaseNetwork != 0) {
			// STRING: LEMBALL 0x004a244c "Network quit timed out\n"
			*g_pErrorOutput << "Network quit timed out\n";
			return 0;
		}
		return 1;
	}
	return 0;
}

// 68K 0x10215cfc INIT_PreInit__Fv
// STUB: LEMBALL 0x004727b0
void InitPreInit()
{
}

// STUB: LEMBALL 0x00472b10
unsigned int __stdcall DebugMessageThreadMain(void* p_unused)
{
	(void) p_unused;
	return 0;
}

// 68K 0x1010fd1a _DBG_Init__Fv
// FUNCTION: LEMBALL 0x00472be0
bool DbgInit()
{
	if (g_nAsyncDebugEnabled == 1) {
		// STRING: LEMBALL 0x004a29f8 "Sync_Debug"
		g_pDebugSyncEvent = CreateEventA(0, 0, 0, "Sync_Debug");
		g_pDebugThread =
			CreateThread(0, 0, DebugMessageThreadMain, 0, 0, (unsigned int*) &g_nDebugThreadId);
		if (g_pDebugThread == 0) {
			// STRING: LEMBALL 0x004a2a38 "Unable to start 'Debug Message loop' thread\n"
			// STRING: LEMBALL 0x004a2a30 "ERROR"
			MessageBoxA(0, "Unable to start 'Debug Message loop' thread\n", "ERROR", 0);
			ExitProcess(0xbbbb);
		}

		SetThreadPriority(g_pDebugThread, 1);
		WaitForSingleObject(g_pDebugSyncEvent, 0xffffffff);
	}

	return 1;
}

// 68K 0x1010fd34 _DBG_Quit__FUc
// STUB: LEMBALL 0x00472c70
bool DbgQuit(unsigned char p_force)
{
	return 0;
}
