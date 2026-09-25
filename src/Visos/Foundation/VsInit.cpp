#include "VsInit.h"

#include "Control/Game/GameMain.h"
#include "Control/Support/PreInit.h"
#include "Visos/Animation/CMogloadStat.h"
#include "Visos/Animation/CStatManager.h"
#include "Visos/Foundation/CArena.h"
#include "Visos/Foundation/CSmallMemory.h"
#include "Visos/Foundation/CVSDebugStreambuf.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsDebug.h"
#include "Visos/Foundation/VsString.h"
#include "Visos/Target/System/CPlatformServices.h"

#include <ctype.h>
#include <memory.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>

#pragma intrinsic(memcpy, memset)

enum InitCmdOptionField {
	kInitCmdOptionName,
	kInitCmdOptionValue,
};

typedef void* InitCmdOption[2];

// GLOBAL: LEMBALL 0x004a9368
jmp_buf g_vsExitJumpBuffer;

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

extern char g_szOptionParanoid[12];

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

// GLOBAL: LEMBALL 0x004a0f20
char g_szOptionParanoid[12] = "paranoid";

// GLOBAL: LEMBALL 0x004a8320
int g_cParsedArgs = 0;

// GLOBAL: LEMBALL 0x004a8328
char* g_apszParsedArgs[16];

// GLOBAL: LEMBALL 0x004a9328
int g_afInitOptionSelected[14];

// FUNCTION: LEMBALL 0x00459250
void INIT_SubSystems()
{
	int memOk;
	int strmOk;
	int dbgOk;
	int inpOk;
	int timeOk;
	int gdiOk;
	int statOk;
	int resOk;
	CBaseStat* stat;
	void* storage;

	memOk = _MEM_Init();
	if (memOk == 0) {
		_VSRELassert("EnoughMemory", "VSINIT.CPP", 0x19e);
	}

	strmOk = _STRM_Init();
	dbgOk = _DBG_Init();
	g_nDebugInitialized = dbgOk;
	InitPlatformServices();

	*g_pSysOutput << "ViSOS v" << g_nVisosVersionMajor << "." << g_nVisosVersionMinor << "(" << 201 << ")"
				  << "\n";
	*g_pSysOutput << "(c)" << "1994,1995" << " Visual Sciences Ltd\n\n";
	*g_pSysOutput << "_MEM_Init   : " << OkFailed(memOk) << "...\t(" << (int) g_preInitActive.m_memoryBudget << ")\n";
	*g_pSysOutput << "_STRM_Init  : " << OkFailed(strmOk) << "...\n";
	*g_pSysOutput << "_DBG_Init   : " << OkFailed(dbgOk) << "...\n";

	g_nInitAllocBaseline = g_pMasterArena->GetAllocSize();

	inpOk = _INP_Init();
	*g_pSysOutput << "_INP_Init   : " << OkFailed(inpOk) << "...\n";

	timeOk = _TIME_Init();
	*g_pSysOutput << "_TIME_Init  : " << OkFailed(timeOk) << "...\n";

	gdiOk = _GDI_Init();
	*g_pSysOutput << "_GDI_Init   : " << OkFailed(gdiOk) << "...\t(" << (int) g_preInitActive.m_flags << ")\n";

	statOk = _STAT_Init();
	*g_pSysOutput << "_STAT_Init  : " << OkFailed(statOk) << "...\n";

	resOk = _RES_Init();
	*g_pSysOutput << "_RES_Init   : " << OkFailed(resOk) << "...\n";

	storage = operator new(0x20);
	if (storage != 0) {
		stat = (CBaseStat*) storage;
		new (storage) CMogloadStat("Main memory arena");
	}
	else {
		stat = 0;
	}
	g_pStatManager->Register(stat);
	g_pMasterArena->m_usageStat = (void*) stat;
}

// FUNCTION: LEMBALL 0x00459520
void INIT_QuitSubSystems()
{
	*g_pSysOutput << g_szQuitNewlineSys;
	*g_pDebugOutput << g_szQuitNewlineDebug;
	*g_pErrorOutput << g_szQuitNewlineError;
	_RES_Quit();
	_STAT_Quit();
	_TIME_Quit();
	_GDI_Quit();
	_INP_Quit();
	if (g_pMasterArena->GetAllocSize() != (unsigned long) g_nInitAllocBaseline) {
		*g_pErrorOutput << g_szMemoryLeakDump;
		CArena* arena = g_pMasterArena;
		arena->StreamOut(*g_pErrorOutput) << g_szMemoryLeakNewline;
	}
	QuitPlatformServices();
	_DBG_Quit(g_nStartupNoWait);
	_STRM_Quit();
	_MEM_Quit();
}

// FUNCTION: LEMBALL 0x004595d0
bool INIT_CheckOptions(char* p_option)
{
	InitCmdOption* option;
	char* colon;
	char* optionText = p_option;
	unsigned int maxCount;
	int index;
	char* end;

	if (*optionText == '-' || *optionText == '/') {
		optionText++;
		option = g_aInitCmdOptions;
		index = 0;
		do {
			colon = strchr((char*) (*option)[kInitCmdOptionName], ':');
			if (colon != 0) {
				maxCount = (unsigned int) (colon - (char*) (*option)[kInitCmdOptionName]);
			}
			else {
				maxCount = strlen((char*) (*option)[kInitCmdOptionName]);
			}
			if (strncmp(optionText, (char*) (*option)[kInitCmdOptionName], maxCount) == 0) {
				if (strlen((char*) g_aInitCmdOptions[index][kInitCmdOptionName]) != maxCount) {
					*(int*) g_aInitCmdOptions[index][kInitCmdOptionValue] = strtol(optionText + maxCount + 1, &end, 10);
				}
				else {
					int* value = (int*) g_aInitCmdOptions[index][kInitCmdOptionValue];
					*value = *value ^ 1;
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

// FUNCTION: LEMBALL 0x004596b0
void INIT_CmdLine(char* p_commandLine)
{
	int i;
	int remaining;
	char** args;
	int shift;
	int count;

	g_cParsedArgs = 0;
	g_apszParsedArgs[0] = p_commandLine;
	memset(g_afInitOptionSelected, 0, sizeof(g_afInitOptionSelected));
	if (p_commandLine != 0) {
		while (*p_commandLine != '\0' && isspace(*p_commandLine)) {
			p_commandLine = p_commandLine + 1;
		}
		if (*p_commandLine != '\0') {
			g_cParsedArgs = 0;
			while (*p_commandLine != '\0') {
				if (isupper(*p_commandLine)) {
					*p_commandLine = (char) tolower(*p_commandLine);
				}
				if (isspace(*p_commandLine)) {
					do {
						*p_commandLine = '\0';
						p_commandLine = p_commandLine + 1;
					} while (isspace(*p_commandLine));
					if (*p_commandLine != '\0') {
						g_cParsedArgs = g_cParsedArgs + 1;
						g_apszParsedArgs[g_cParsedArgs] = p_commandLine;
					}
					p_commandLine = p_commandLine - 1;
				}
				p_commandLine = p_commandLine + 1;
			}
			i = 0;
			g_cParsedArgs = g_cParsedArgs + 1;
			if (0 < g_cParsedArgs) {
				remaining = g_cParsedArgs;
				do {
					if (INIT_CheckOptions(g_apszParsedArgs[i]) != 0) {
						count = g_cParsedArgs;
						count = count - 1;
						if (i < count) {
							shift = g_cParsedArgs;
							shift = shift - i;
							shift = shift - 1;
							args = &g_apszParsedArgs[i];
							do {
								shift = shift - 1;
								*args = args[1];
								args = args + 1;
							} while (shift != 0);
						}
						g_cParsedArgs = count;
					}
					else {
						i = i + 1;
					}
					remaining = remaining - 1;
				} while (remaining != 0);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00459860
int INIT_Main(char* p_commandLine)
{
	unsigned int i;
	int result;
	int mainResult;

	INIT_CmdLine(p_commandLine);
	INIT_PreInit();
	INIT_SubSystems();
	*g_pDebugOutput << g_szCommandLineOptions;
	i = 0;
	do {
		*g_pDebugOutput << g_szOptionPrefix << (char*) g_aInitCmdOptions[i][kInitCmdOptionName] << g_szOptionIs;
		if (g_afInitOptionSelected[i] == 0) {
			*g_pDebugOutput << g_szOptionNot;
		}
		*g_pDebugOutput << g_szOptionSelected;
		i = i + 1;
	} while (i < 0xe);
	result = setjmp(g_vsExitJumpBuffer);
	if (result != 0) {
		INIT_QuitSubSystems();
		return result;
	}
	result = setjmp(g_vsDebugJumpBuffer);
	if (result != 0) {
		_DBG_Quit(g_nStartupNoWait);
		return result;
	}
	mainResult = VSmain(g_cParsedArgs, g_apszParsedArgs);
	INIT_QuitSubSystems();
	return mainResult;
}

// FUNCTION: LEMBALL 0x004727b0
void INIT_PreInit()
{
	unsigned int* capability;
	int i;
	PreInit* result;
	unsigned int value;
	int displayMode;

	capability = g_anPreInitCapabilities;
	for (i = 7; i != 0; i = i - 1) {
		*capability = 0x100;
		capability = capability + 1;
	}
	g_preInitActive.m_memoryBudget = g_preInitActive.m_memoryBudget << 0x13;
	result = VSPreInit(&g_preInitActive);
	if (result != 0) {
		g_preInitActive = *result;
	}
	if (7 < g_preInitActive.m_capabilityCount) {
		g_preInitActive.m_capabilityCount = 7;
	}
	if (g_preInitActive.m_capabilityCount <= 0) {
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
	displayMode = g_preInitActive.m_shift;
	switch (displayMode) {
	case 0:
		g_nGraphicsDriverCds = 0;
		g_nGraphicsDriverWing = 1;
		g_nGraphicsDriverGdk = 0;
		return;
	case 1:
		g_nGraphicsDriverWing = 0;
		g_nGraphicsDriverCds = 1;
		g_nGraphicsDriverGdk = 0;
		return;
	case 2:
		g_nGraphicsDriverWing = 0;
		g_nGraphicsDriverGdk = 1;
		g_nGraphicsDriverCds = 0;
		return;
	}
}
