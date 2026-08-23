#ifndef LEMBALL_CONTROL_GAME_GAMETIME_H
#define LEMBALL_CONTROL_GAME_GAMETIME_H

#include "../../Common.h"

void ClockEditMode(unsigned char p_enabled);
void ResetGameTimes();
void SetGameTime();
void SetRemoteGameTimeReal(unsigned long p_timestamp);
Demo* DemoInit(int p_arg0);
void DemoQuit();

extern unsigned long g_dwSimulationTimestamp;

#endif
