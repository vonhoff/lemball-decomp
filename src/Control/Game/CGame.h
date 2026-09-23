#ifndef LEMBALL_CONTROL_GAME_CGAME_H
#define LEMBALL_CONTROL_GAME_CGAME_H

#include "../../Frontend/Base/FlowProcesses.h"

class CBaseProcess;
class CMain2DDisplay;
class CTimeStat;
// SIZE 0x70
class CGame {
public:
	CGame(char* p_arg0);
	bool IsValidResource();
	void LoadFrontendResources(int p_mode);
	void NextProcess(eFlowProcesses p_flow);
	void Process();
	void RefreshViews();
	void Run();
	void StreamRuntimeStats();
	void UnLoadFrontendResources();
	~CGame();

	friend int VSmain(int p_argc, char** p_argv);
	friend class CMain2DDisplay;

private:
	char m_runtimeName[80];        // 0x00
	CTimeStat* m_processingStat;   // 0x50
	CTimeStat* m_refreshingStat;   // 0x54
	unsigned int m_flowTicks;      // 0x58
	CBaseProcess* m_process;       // 0x5c
	unsigned int m_quit;           // 0x60
	CMain2DDisplay* m_mainDisplay; // 0x64
	eFlowProcesses m_currentFlow;  // 0x68
	void* m_frontendResources;     // 0x6c
};

extern int* g_pSentinel;
extern unsigned int g_dwGameTick;
#endif
