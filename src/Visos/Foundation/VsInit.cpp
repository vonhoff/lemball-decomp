#include "VsFile.h"
#include "VsTime.h"
#include "VsInit.h"

#include "../../Control/Game/GameMain.h"
#include "../../Control/Support/PreInit.h"
#include "../Animation/BaseStat.h"
#include "../Animation/StatManager.h"
#include "../Graphics/BasePalManager.h"
#include "../Graphics/Cursor.h"
#include "../Graphics/GdiDevice.h"
#include "../Graphics/VsGdi.h"
#include "../Network/FileNetwork.h"
#include "../Network/TcpIpNetwork.h"
#include "../Resources/ResourceTypeList.h"
#include "../Target/TargetGraphicsDriver.h"
#include "../Target/TargetGraphicsSystemState.h"
#include "../Target/TargetPlatformServices.h"
#include "Arena.h"
#include "MasterInput.h"
#include "RamArena.h"
#include "SmallMemory.h"
#include "TimedQueue.h"
#include "Translator.h"
#include "VsDebug.h"
#include "VsDebugStreambuf.h"
#include "VsOStream.h"
#include "VsString.h"

#include <ctype.h>
#include <memory.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>

#pragma intrinsic(memcpy)

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
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int p_flags, unsigned int p_bytes);
extern "C" __declspec(dllimport) void* __stdcall GlobalLock(void* p_mem);
extern "C" __declspec(dllimport) int __stdcall GlobalUnlock(void* p_mem);
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* p_mem);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLastError();

struct InitCmdOption {
	char* m_name;
	int* m_value;
};

// GLOBAL: LEMBALL 0x004a9368
jmp_buf g_vsExitJumpBuffer;

// GLOBAL: LEMBALL 0x004a82e0
jmp_buf g_vsDebugJumpBuffer;

// GLOBAL: LEMBALL 0x004a0e70
int g_nGraphicsDriverWing = 0;

// GLOBAL: LEMBALL 0x004a0e74
int g_nGraphicsDriverCds = 1;

// GLOBAL: LEMBALL 0x004a0e78
int g_nGraphicsDriverGdk = 0;

// GLOBAL: LEMBALL 0x004a0e6c
int g_nParanoidMode = 0;

// GLOBAL: LEMBALL 0x004a0e7c
int g_nFullscreen = 0;

// GLOBAL: LEMBALL 0x004a0e8c
int g_nGarbageCollector = 0;

// GLOBAL: LEMBALL 0x004a0e90
int g_nStartupNoWait = 0;

// GLOBAL: LEMBALL 0x004a0e88
int g_nInitAllocBaseline = 0;

// GLOBAL: LEMBALL 0x004a1d70
int g_nShowUnloading = 0;

// GLOBAL: LEMBALL 0x004a1d74
int g_nShowLoading = 0;

// GLOBAL: LEMBALL 0x004a0e94
int g_nVisosVersionMajor = 3;

// GLOBAL: LEMBALL 0x004a0e98
int g_nVisosVersionMinor = 0;

// GLOBAL: LEMBALL 0x004a93b0
char g_szStreamFixedBuffer[0x400];

// GLOBAL: LEMBALL 0x004a0f20
char g_szOptionParanoid[12] = "paranoid";

// GLOBAL: LEMBALL 0x004a0f2c
char g_szOptionNoWait[8] = "nowait";

// GLOBAL: LEMBALL 0x004a0f34
char g_szOptionNoSmallMemory[16] = "nosmallmemory";

// GLOBAL: LEMBALL 0x004a0f44
char g_szOptionWing[8] = "wing";

// GLOBAL: LEMBALL 0x004a0f4c
char g_szOptionCds[4] = "cds";

// GLOBAL: LEMBALL 0x004a0f50
char g_szOptionGdk[4] = "gdk";

// GLOBAL: LEMBALL 0x004a0f54
char g_szOptionFull[8] = "full";

// GLOBAL: LEMBALL 0x004a0f5c
char g_szOptionShowUnloading[16] = "showunloading";

// GLOBAL: LEMBALL 0x004a0f6c
char g_szOptionShowLoading[12] = "showloading";

// GLOBAL: LEMBALL 0x004a0f78
char g_szOptionSurfaces[12] = "surfaces:";

// GLOBAL: LEMBALL 0x004a0f84
char g_szOptionMemorySize[12] = "memorysize:";

// GLOBAL: LEMBALL 0x004a0f90
char g_szOptionNoDebug[8] = "nodebug";

// GLOBAL: LEMBALL 0x004a0f98
char g_szOptionDebugFile[12] = "debugfile";

// GLOBAL: LEMBALL 0x004a0fa4
char g_szOptionGc[4] = "gc:";

// GLOBAL: LEMBALL 0x004a0fb4
char g_szMasterInputQueue[20] = "Master Input Queue.";

// GLOBAL: LEMBALL 0x004a10f4
char g_szQuitNewlineSys[4] = "\n";

// GLOBAL: LEMBALL 0x004a10f8
char g_szQuitNewlineDebug[4] = "\n";

// GLOBAL: LEMBALL 0x004a10fc
char g_szQuitNewlineError[4] = "\n";

// GLOBAL: LEMBALL 0x004a1100
char g_szMemoryLeakDump[48] = "**** MEMORY LEAK, dumping memory contents ****\n";

// GLOBAL: LEMBALL 0x004a1130
char g_szMemoryLeakNewline[4] = "\n";

// GLOBAL: LEMBALL 0x004a1134
char g_szCommandLineOptions[24] = "Command line options:\n";

// GLOBAL: LEMBALL 0x004a114c
char g_szOptionPrefix[12] = "  Option ";

// GLOBAL: LEMBALL 0x004a1158
char g_szOptionIs[8] = " is ";

// GLOBAL: LEMBALL 0x004a1160
char g_szOptionNot[8] = "not ";

// GLOBAL: LEMBALL 0x004a1168
char g_szOptionSelected[12] = "selected\n";

// GLOBAL: LEMBALL 0x004a2184
char g_szMasterMainRamArena[24] = "Master Main Ram Arena";

// GLOBAL: LEMBALL 0x004a219c
char g_szMasterArenaStillLocked[72] =
	"Master Main RAM Arena memory was locked, when it should not have been\n";

// GLOBAL: LEMBALL 0x004a21e4
char g_szMasterArenaUnlockIteratively[40] = "attempting to unlock it iteratively\n";

// GLOBAL: LEMBALL 0x004a220c
char g_szMasterArenaFreeFailed[52] = "Freeing the Master Main RAM Arena memory failed\n";

// GLOBAL: LEMBALL 0x004a2240
char g_szGetLastErrorEq[20] = "GetLastError()==";

// GLOBAL: LEMBALL 0x004a2254
char g_szColonHexPrefix[8] = ": 0x";

// GLOBAL: LEMBALL 0x004a225c
char g_szGetLastErrorNewline[4] = "\n";

// GLOBAL: LEMBALL 0x004a0eb0
InitCmdOption g_aInitCmdOptions[14] = {
	{g_szOptionParanoid, &g_nParanoidMode},
	{g_szOptionNoWait, &g_nStartupNoWait},
	{g_szOptionNoSmallMemory, &g_nSmallMemoryEnabled},
	{g_szOptionWing, &g_nGraphicsDriverWing},
	{g_szOptionCds, &g_nGraphicsDriverCds},
	{g_szOptionGdk, &g_nGraphicsDriverGdk},
	{g_szOptionFull, &g_nFullscreen},
	{g_szOptionShowUnloading, &g_nShowUnloading},
	{g_szOptionShowLoading, &g_nShowLoading},
	{g_szOptionSurfaces, (int*) &g_preInitActive.m_flags},
	{g_szOptionMemorySize, (int*) &g_preInitActive.m_memoryBudget},
	{g_szOptionNoDebug, &g_nAsyncDebugEnabled},
	{g_szOptionDebugFile, &g_nDebugFileOutputEnabled},
	{g_szOptionGc, &g_nGarbageCollector},
};

// GLOBAL: LEMBALL 0x004a8320
int g_cParsedArgs = 0;

// GLOBAL: LEMBALL 0x004a8328
char* g_apszParsedArgs[16];

// GLOBAL: LEMBALL 0x004a9328
int g_afInitOptionSelected[14];

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
	void* storage;

	storage = operator new(0x58);
	if (storage == 0) {
		g_pMasterInputQueue = 0;
	}
	else {
		g_pMasterInputQueue = new (storage) TimedQueue(10, g_szMasterInputQueue);
	}

	storage = operator new(0x18);
	if (storage == 0) {
		g_pMasterInput = 0;
	}
	else {
		g_pMasterInput = new (storage) MasterInput(g_pMasterInputQueue);
	}

	storage = operator new(0x10);
	if (storage == 0) {
		g_pInputTranslator = 0;
	}
	else {
		g_pInputTranslator = new (storage) Translator();
	}

	g_pMasterInputQueue->Attach(g_pInputTranslator, -0x32);
	return TargetInputInit();
}

// 68K 0x1021332e _INP_Quit__Fv
// STUB: LEMBALL 0x004591f0
bool InpQuit()
{
	int result;

	result = TargetInputQuit();
	g_pMasterInputQueue->Detach(g_pInputTranslator, -0x32);
	if (g_pInputTranslator != 0) {
		delete g_pInputTranslator;
	}
	if (g_pMasterInput != 0) {
		delete g_pMasterInput;
	}
	if (g_pMasterInputQueue != 0) {
		delete g_pMasterInputQueue;
	}
	return result;
}

// 68K 0x102133dc INIT_SubSystems__Fv
// ASSERT: _VSRELassert("EnoughMemory", "VSINIT.CPP", 414)
// STUB: LEMBALL 0x00459250
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
		VsRelAssert("EnoughMemory", "VSINIT.CPP", 0x19e);
	}

	strmOk = StrmInit();
	dbgOk = DbgInit();
	g_nDebugInitialized = dbgOk;
	TargetPlatformServicesInit();

	*g_pSysOutput << "ViSOS v" << g_nVisosVersionMajor << "." << g_nVisosVersionMinor << "(" << 201 << ")"
				  << "\n";
	*g_pSysOutput << "(c)" << "1994,1995" << " Visual Sciences Ltd\n\n";
	*g_pSysOutput << "_MEM_Init   : " << OkFailed(memOk) << "...\t(" << (int) g_preInitActive.m_memoryBudget << ")\n";
	*g_pSysOutput << "_STRM_Init  : " << OkFailed(strmOk) << "...\n";
	*g_pSysOutput << "_DBG_Init   : " << OkFailed(dbgOk) << "...\n";

	g_nInitAllocBaseline = g_pMasterArena->GetAllocSize();

	inpOk = InpInit();
	*g_pSysOutput << "_INP_Init   : " << OkFailed(inpOk) << "...\n";

	timeOk = TimeInit();
	*g_pSysOutput << "_TIME_Init  : " << OkFailed(timeOk) << "...\n";

	gdiOk = GdiInit();
	*g_pSysOutput << "_GDI_Init   : " << OkFailed(gdiOk) << "...\t(" << (int) g_preInitActive.m_flags << ")\n";

	statOk = StatInit();
	*g_pSysOutput << "_STAT_Init  : " << OkFailed(statOk) << "...\n";

	resOk = ResInit();
	*g_pSysOutput << "_RES_Init   : " << OkFailed(resOk) << "...\n";

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
	*g_pSysOutput << g_szQuitNewlineSys;
	*g_pDebugOutput << g_szQuitNewlineDebug;
	*g_pErrorOutput << g_szQuitNewlineError;
	ResQuit();
	StatQuit();
	TimeQuit();
	GdiQuit();
	InpQuit();
	if (g_pMasterArena->GetAllocSize() != (unsigned long) g_nInitAllocBaseline) {
		*g_pErrorOutput << g_szMemoryLeakDump;
		g_pMasterArena->StreamOut(*g_pErrorOutput) << g_szMemoryLeakNewline;
	}
	TargetPlatformServicesQuit();
	DbgQuit(g_nStartupNoWait);
	StrmQuit();
	MemQuit();
}

// 68K 0x102138be INIT_CheckOptions__FPc
// STUB: LEMBALL 0x004595d0
bool InitCheckOptions(char* p_arg0)
{
	InitCmdOption* option;
	char* colon;
	unsigned int maxCount;
	int index;
	char* end;

	if (*p_arg0 == '-' || *p_arg0 == '/') {
		option = g_aInitCmdOptions;
		index = 0;
		do {
			colon = strchr(option->m_name, ':');
			if (colon == 0) {
				maxCount = strlen(option->m_name);
			}
			else {
				maxCount = (unsigned int) (colon - option->m_name);
			}
			if (strncmp(p_arg0 + 1, option->m_name, maxCount) == 0) {
				if (strlen(g_aInitCmdOptions[index].m_name) == maxCount) {
					*g_aInitCmdOptions[index].m_value = *g_aInitCmdOptions[index].m_value ^ 1;
				}
				else {
					*g_aInitCmdOptions[index].m_value = strtol(p_arg0 + 1 + maxCount + 1, &end, 10);
				}
				g_afInitOptionSelected[index] = 1;
				return 1;
			}
			option++;
			index++;
		} while (option < g_aInitCmdOptions + 14);
	}
	return 0;
}

// 68K 0x102139c8 INIT_CmdLine__FPc
// STUB: LEMBALL 0x004596b0
char* InitCmdLine(char* p_arg0)
{
	char* cursor;
	char* next;
	int i;
	int remaining;
	int count;
	int* seen;
	char** args;
	int shift;

	g_cParsedArgs = 0;
	g_apszParsedArgs[0] = p_arg0;
	seen = g_afInitOptionSelected;
	for (i = 0xe; i != 0; i = i - 1) {
		*seen = 0;
		seen = seen + 1;
	}
	if (p_arg0 != 0) {
		while (*p_arg0 != '\0' && isspace(*p_arg0)) {
			p_arg0 = p_arg0 + 1;
		}
		if (*p_arg0 != '\0') {
			g_cParsedArgs = 0;
			cursor = p_arg0;
			while (*cursor != '\0') {
				if (isupper(*cursor)) {
					*cursor = (char) tolower(*cursor);
				}
				if (isspace(*cursor)) {
					next = cursor;
					do {
						cursor = next;
						*cursor = '\0';
						next = cursor + 1;
					} while (isspace(*next));
					if (*next != '\0') {
						g_cParsedArgs = g_cParsedArgs + 1;
						g_apszParsedArgs[g_cParsedArgs] = next;
					}
				}
				cursor = cursor + 1;
			}
			i = 0;
			g_cParsedArgs = g_cParsedArgs + 1;
			remaining = g_cParsedArgs;
			if (0 < g_cParsedArgs) {
				do {
					if (InitCheckOptions(g_apszParsedArgs[i]) == 0) {
						i = i + 1;
						count = g_cParsedArgs;
					}
					else {
						count = g_cParsedArgs - 1;
						if (i < count) {
							shift = (g_cParsedArgs - i) - 1;
							args = &g_apszParsedArgs[i];
							do {
								shift = shift - 1;
								*args = args[1];
								args = args + 1;
							} while (shift != 0);
						}
					}
					g_cParsedArgs = count;
					remaining = remaining - 1;
				} while (remaining != 0);
			}
		}
	}
	return p_arg0;
}

// 68K 0x10213b1e INIT_Main__FPc
// STUB: LEMBALL 0x00459860
int InitMain(char* p_arg0)
{
	unsigned int i;
	int result;

	InitCmdLine(p_arg0);
	InitPreInit();
	InitSubSystems();
	*g_pDebugOutput << g_szCommandLineOptions;
	i = 0;
	do {
		*g_pDebugOutput << g_szOptionPrefix << g_aInitCmdOptions[i].m_name << g_szOptionIs;
		if (g_afInitOptionSelected[i] == 0) {
			*g_pDebugOutput << g_szOptionNot;
		}
		*g_pDebugOutput << g_szOptionSelected;
		i = i + 1;
	} while (i < 0xe);
	result = setjmp(g_vsExitJumpBuffer);
	if (result != 0) {
		InitQuitSubSystems();
		return result;
	}
	result = setjmp(g_vsDebugJumpBuffer);
	if (result != 0) {
		DbgQuit(g_nStartupNoWait);
		return result;
	}
	result = Vsmain(g_cParsedArgs, g_apszParsedArgs);
	InitQuitSubSystems();
	return result;
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
// STUB: LEMBALL 0x0045aab0
bool StatQuit()
{
	g_pStatManager->StreamOut(*g_pSysOutput);
	delete g_pStatManager;
	return 1;
}

// 68K 0x10201420 _RES_Init__Fv
// STUB: LEMBALL 0x0045b900
bool ResInit()
{
	ResourceTypeList* list;
	BasePalManager* palManager;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list == 0) {
		list = 0;
	}
	else {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	list->m_typeCodes[list->m_count] = 0x494e5420;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = 0x5a524c45;
	list->m_count = list->m_count + 1;
	g_pResourceTypes = list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list == 0) {
		list = 0;
	}
	else {
		list->m_capacity = 1;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	list->m_typeCodes[list->m_count] = 0x5a524c45;
	list->m_count = list->m_count + 1;
	g_pCompressedResourceTypes = list;

	list = (ResourceTypeList*) operator new(sizeof(ResourceTypeList));
	if (list == 0) {
		list = 0;
	}
	else {
		list->m_capacity = 2;
		list->m_currentIndex = -1;
		list->m_count = 0;
		list->m_typeCodes = (unsigned int*) operator new(list->m_capacity * sizeof(unsigned int));
	}
	list->m_typeCodes[list->m_count] = 0x53545247;
	list->m_count = list->m_count + 1;
	list->m_typeCodes[list->m_count] = 0x494e5420;
	list->m_count = list->m_count + 1;
	g_pPreloadedResourceTypes = list;

	palManager = (BasePalManager*) operator new(sizeof(BasePalManager));
	if (palManager != 0) {
		g_pBasePalManager = new (palManager) BasePalManager(0x20);
		return 1;
	}
	g_pBasePalManager = 0;
	return 1;
}

// 68K 0x10201580 _RES_Quit__Fv
// STUB: LEMBALL 0x0045ba50
bool ResQuit()
{
	ResourceTypeList* list;

	if (g_pBasePalManager != 0) {
		g_pBasePalManager->~BasePalManager();
		operator delete(g_pBasePalManager);
	}
	list = g_pPreloadedResourceTypes;
	if (list != 0) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	list = g_pResourceTypes;
	if (list != 0) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	list = g_pCompressedResourceTypes;
	if (list != 0) {
		operator delete(list->m_typeCodes);
		operator delete(list);
	}
	return 1;
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
// FUNCTION: LEMBALL 0x0046ba80
bool GdiInit()
{
	void* storage;
	Cursor* cursor;
	TargetGraphicsSystemState* system;

	storage = operator new(0xc);
	system = (TargetGraphicsSystemState*) storage;
	if (system != 0) {
		system->m_reserved04 = 0;
		system->m_fallbackWarningShown = 0;
		g_pTargetGraphicsSystem = system;
	}
	else {
		g_pTargetGraphicsSystem = 0;
	}
	g_pTargetGraphicsSystem->SelectDriver(8);

	storage = operator new(sizeof(GdiDevice));
	if (storage != 0) {
		g_pGdiDevice = new (storage) GdiDevice(g_preInitActive.m_flags);
	}
	else {
		g_pGdiDevice = 0;
	}

	storage = operator new(0x5a0);
	if (storage != 0) {
		g_pGdiHelperTarget = new (storage) Surface((GrafPort*) 0);
	}
	else {
		g_pGdiHelperTarget = 0;
	}

	cursor = (Cursor*) operator new(sizeof(Cursor));
	if (cursor != 0) {
		new (cursor) Cursor();
		g_pCursor = cursor;
	}
	else {
		g_pCursor = 0;
	}

	if (g_pGdiDevice != 0 && g_pGdiHelperTarget != 0) {
		return 1;
	}
	return 0;
}

// 68K 0x10107ca0 _GDI_Quit__Fv
// FUNCTION: LEMBALL 0x0046bb70
bool GdiQuit()
{
	Surface* surface;
	GdiDevice* device;
	TargetGraphicsSystemState* system;

	if (g_pCursor != 0) {
		delete g_pCursor;
	}
	surface = (Surface*) g_pGdiHelperTarget;
	if (surface != 0) {
		surface->~Surface();
		surface->PvSurface::~PvSurface();
		operator delete(surface);
	}
	device = g_pGdiDevice;
	if (device != 0) {
		device->~GdiDevice();
		operator delete(device);
	}
	system = g_pTargetGraphicsSystem;
	g_pGdiHelperTarget = 0;
	g_pGdiDevice = 0;
	if (system != 0) {
		if (g_pTargetGraphicsDriver != 0) {
			delete g_pTargetGraphicsDriver;
		}
		operator delete(system);
	}
	return 1;
}

// 68K 0x1010fd50 _MEM_Init__Fv
// STUB: LEMBALL 0x0046f060
bool MemInit()
{
	void* locked;
	void* storage;
	int smallEnabled;
	SmallMemory* smallMemory;

	g_pMasterArenaMemory = GlobalAlloc(2, g_preInitActive.m_memoryBudget);
	if (g_pMasterArenaMemory == 0) {
		return 0;
	}

	locked = GlobalLock(g_pMasterArenaMemory);
	if (locked == 0) {
		return 0;
	}

	storage = Arena::operator new(0x50, locked);
	if (storage == 0) {
		g_pMasterArena = 0;
	}
	else {
		g_pMasterArena = new (storage)
			RamArena(g_preInitActive.m_memoryBudget, g_szMasterMainRamArena, 0, 0);
	}

	smallEnabled = g_nSmallMemoryEnabled;
	smallMemory = 0;
	g_pSmallMemory = 0;
	if (g_nSmallMemoryEnabled != 0) {
		g_nSmallMemoryEnabled = 0;
		storage = operator new(0x3c);
		smallMemory = 0;
		if (storage != 0) {
			smallMemory = new (storage) SmallMemory();
		}
	}
	g_pSmallMemory = smallMemory;
	g_nSmallMemoryEnabled = smallEnabled;
	if (g_pMasterArena != 0 && (smallEnabled == 0 || smallMemory != 0)) {
		return 1;
	}
	return 0;
}

// 68K 0x1010ff2e _MEM_Quit__Fv
// STUB: LEMBALL 0x0046f120
bool MemQuit()
{
	SmallMemory* smallMemory;
	unsigned int lastError;

	smallMemory = g_pSmallMemory;
	g_nSmallMemoryEnabled = 0;
	if (g_pSmallMemory != 0) {
		g_pSmallMemory->~SmallMemory();
		operator delete(smallMemory);
	}
	if (g_pMasterArena != 0) {
		delete g_pMasterArena;
	}
	if (GlobalUnlock(g_pMasterArenaMemory) != 0) {
		*g_pErrorOutput << g_szMasterArenaStillLocked;
		*g_pErrorOutput << g_szMasterArenaUnlockIteratively;
		do {
		} while (GlobalUnlock(g_pMasterArenaMemory) != 0);
	}
	if (GlobalFree(g_pMasterArenaMemory) != 0) {
		lastError = GetLastError();
		*g_pErrorOutput << g_szMasterArenaFreeFailed;
		*g_pErrorOutput << g_szGetLastErrorEq << (unsigned long) lastError << g_szColonHexPrefix
						<< Hex(lastError) << g_szGetLastErrorNewline;
		return 0;
	}
	return 1;
}

// STUB: LEMBALL 0x0046f210
unsigned int __stdcall FileNetworkMessageThread(void* p_unused)
{
	(void) p_unused;
	return 0;
}

// 68K 0x10106d86 VSFNET_Init__Fv
// STUB: LEMBALL 0x0046f3b0
bool VsFNetInit()
{
	unsigned long startTime;

	g_hFileNetworkThread =
		CreateThread(0, 0, FileNetworkMessageThread, 0, 0, &g_dwFileNetworkThreadId);
	if (g_hFileNetworkThread == 0) {
		MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hFileNetworkThread, 2);

	startTime = CurrentMilliTimer();
	while (CurrentMilliTimer() - startTime < 10000 && g_pBaseNetwork == 0) {
	}
	if (g_pBaseNetwork == 0) {
		*g_pErrorOutput << "Network initialisation timed out\n";
		return 0;
	}

	startTime = CurrentMilliTimer();
	while (CurrentMilliTimer() - startTime < 10000 && g_pNetworkStatusQueue == 0) {
	}
	if (g_pNetworkStatusQueue == 0) {
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return 0;
	}

	return 1;
}

// 68K 0x10106dc8 VSFNET_Quit__Fv
// STUB: LEMBALL 0x0046f480
bool VsFNetQuit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->Process();
		startTime = CurrentMilliTimer();
		while (CurrentMilliTimer() - startTime < 10000 && g_pBaseNetwork != 0) {
		}
		if (g_pBaseNetwork != 0) {
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
// STUB: LEMBALL 0x0046fbb0
bool VsNetInit()
{
	unsigned long startTime;

	g_hTCPIPNetworkThread =
		CreateThread(0, 0, TcpIpNetworkMessageThread, 0, 0, &g_dwTCPIPNetworkThreadId);
	if (g_hTCPIPNetworkThread == 0) {
		MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hTCPIPNetworkThread, 2);

	startTime = CurrentMilliTimer();
	while (CurrentMilliTimer() - startTime < 10000 && g_pBaseNetwork == 0) {
	}
	if (g_pBaseNetwork == 0) {
		*g_pErrorOutput << "Network initialisation timed out\n";
		return 0;
	}

	startTime = CurrentMilliTimer();
	while (CurrentMilliTimer() - startTime < 10000 && g_pNetworkStatusQueue == 0) {
	}
	if (g_pNetworkStatusQueue == 0) {
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return 0;
	}

	return 1;
}

// 68K 0x1010c6d8 VSNET_Quit__Fv
// STUB: LEMBALL 0x0046fc80
bool VsNetQuit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->Process();
		startTime = CurrentMilliTimer();
		while (CurrentMilliTimer() - startTime < 10000 && g_pBaseNetwork != 0) {
		}
		if (g_pBaseNetwork != 0) {
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
	unsigned int* capability;
	int i;
	PreInit* result;
	unsigned int value;

	capability = g_anPreInitCapabilities;
	for (i = 7; i != 0; i = i - 1) {
		*capability = 0x100;
		capability = capability + 1;
	}
	g_preInitActive.m_memoryBudget = g_preInitActive.m_memoryBudget << 0x13;
	result = VsPreInit(&g_preInitActive);
	if (result != 0) {
		memcpy(&g_preInitActive, result, sizeof(g_preInitActive));
	}
	if (7 < g_preInitActive.m_capabilityCount) {
		g_preInitActive.m_capabilityCount = 7;
	}
	if (g_preInitActive.m_capabilityCount < 1) {
		g_nSmallMemoryEnabled = 0;
	}
	capability = g_anPreInitCapabilities;
	do {
		value = *capability;
		if ((int) value % 32 != 0) {
			*capability = ((int) (value + 0x1f) / 32) * 32;
		}
		capability = capability + 1;
	} while (capability < g_anPreInitCapabilities + 7);
	if (g_preInitActive.m_shift == 0) {
		g_nGraphicsDriverCds = 0;
		g_nGraphicsDriverWing = 1;
		g_nGraphicsDriverGdk = 0;
		return;
	}
	if (g_preInitActive.m_shift == 1) {
		g_nGraphicsDriverWing = 0;
		g_nGraphicsDriverCds = 1;
		g_nGraphicsDriverGdk = 0;
		return;
	}
	if (g_preInitActive.m_shift != 2) {
		return;
	}
	g_nGraphicsDriverWing = 0;
	g_nGraphicsDriverGdk = 1;
	g_nGraphicsDriverCds = 0;
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
		g_pDebugSyncEvent = CreateEventA(0, 0, 0, "Sync_Debug");
		g_pDebugThread =
			CreateThread(0, 0, DebugMessageThreadMain, 0, 0, (unsigned int*) &g_nDebugThreadId);
		if (g_pDebugThread == 0) {
			MessageBoxA(0, "Unable to start 'Debug Message loop' thread\n", "ERROR", 0);
			ExitProcess(0xbbbb);
		}

		SetThreadPriority(g_pDebugThread, 1);
		WaitForSingleObject(g_pDebugSyncEvent, 0xffffffff);
	}

	return 1;
}

// 68K 0x1010fd34 _DBG_Quit__FUc
// FUNCTION: LEMBALL 0x00472c70
bool DbgQuit(unsigned int p_force)
{
	if (g_nAsyncDebugEnabled == 1) {
		if (p_force == 0) {
			WaitForSingleObject(g_pDebugSyncEvent, 0xffffffff);
		}
		else {
			TerminateThread(g_pDebugThread, 0xaaaa);
		}
		g_nAsyncDebugEnabled = 0;
		return 1;
	}
	if (g_pDebugOutputFile != 0) {
		VsClose((_Filet*) g_pDebugOutputFile);
		g_pDebugOutputFile = 0;
	}
	return 1;
}
