#ifndef LEMBALL_CONTROL_GAME_GAMETIME_H
#define LEMBALL_CONTROL_GAME_GAMETIME_H

#include "../../Common.h"

// Game and remote clocks use 50-millisecond ticks.
#define GAME_TICK_MILLISECONDS 50

void ClockEditMode(unsigned int p_enabled);
void ResetGameTimes();
void SetGameTime();
void SetRemoteGameTimeReal(unsigned long p_timestamp);
Demo* DemoInit(int p_arg0);
void DemoQuit();

extern unsigned long g_dwSimulationTimestamp;
extern unsigned long g_dwRemoteGameTick;
extern unsigned long g_dwNetworkSimulationTimestamp;
extern unsigned long g_dwLastRemoteTimestamp;
extern unsigned long g_dwLastElapsedMilli;
extern unsigned long g_dwGameTimeTick;
extern unsigned long g_dwCurrentMilli;
extern unsigned long g_dwPausedMilli;
extern unsigned int g_dwClockEditMode;

#endif
