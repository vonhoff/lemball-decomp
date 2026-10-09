#ifndef LEMBALL_CONTROL_GAME_GAMETIME_H
#define LEMBALL_CONTROL_GAME_GAMETIME_H

#define GAME_TICK_MILLISECONDS 50
#define HAZARD_DEATH_DELAY_TICKS 26

void ClockEditMode(unsigned int p_enabled);
void ResetGameTimes();
void SetGameTime();
void SetRemoteGameTimeReal(unsigned long p_timestamp);

extern unsigned long g_dwSimulationTimestamp;
extern unsigned long g_dwRemoteGameTick;
extern unsigned long g_dwNetworkSimulationTimestamp;
extern unsigned long g_dwLastRemoteTimestamp;
extern unsigned long g_dwLastElapsedMilli;
extern unsigned long g_dwGameTimeTick;
extern unsigned long g_dwCurrentMilli;
extern unsigned long g_dwPausedMilli;
extern unsigned int g_dwClockEditMode;

extern unsigned long g_dwGameTick;

#endif
