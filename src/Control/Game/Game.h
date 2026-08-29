#ifndef LEMBALL_CONTROL_GAME_GAME_H
#define LEMBALL_CONTROL_GAME_GAME_H

#include "../../Common.h"

// SIZE 0x70
class Game {
public:
	Game(char* p_arg0);
	bool IsValidResource();
	void LoadFrontendResources(int p_mode);
	void NextProcess(eFlowProcesses p_flow);
	void Process();
	void RefreshViews();
	void Run();
	void UnLoadFrontendResources();
	~Game();

	friend int Vsmain(int p_argc, char** p_argv);
	friend class Main2DDisplay;

private:
	char m_runtimeName[80];       // 0x00
	BaseStat* m_processingStat;   // 0x50
	BaseStat* m_refreshingStat;   // 0x54
	unsigned int m_flowTicks;     // 0x58
	class Process* m_process;     // 0x5c
	unsigned int m_quit;          // 0x60
	Main2DDisplay* m_mainDisplay; // 0x64
	eFlowProcesses m_currentFlow; // 0x68
	void* m_frontendResources;    // 0x6c
};

extern int* g_pSentinel;
extern unsigned int g_dwGameTick;
#endif
