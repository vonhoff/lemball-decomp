#include "Gameplay/Simulation/GameTime.h"

#include "Engine/Time/VsTime.h"
#include "Application/CDemo.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x0049ce04
unsigned int g_dwGameTick = 0;

// FUNCTION: LEMBALL 0x00408080
void ClockEditMode(unsigned int p_enabled)
{
	g_dwClockEditMode = p_enabled;
}

// FUNCTION: LEMBALL 0x00408090
void ResetGameTimes()
{
	g_dwLastRemoteTimestamp = 0;
	g_dwLastElapsedMilli = 0;
	g_dwSimulationTimestamp = 0;
	g_dwGameTick = 0;
	g_dwPausedMilli = 0;
	g_dwCurrentMilli = CurrentMilliTimer();
	g_dwGameTimeTick = g_dwCurrentMilli / GAME_TICK_MILLISECONDS;
}

#define DEMO_FRAME_DURATION_MILLISECONDS 80
#define MAX_GAME_TICKS_PER_UPDATE 2
// FUNCTION: LEMBALL 0x004080d0
void SetGameTime()
{
	if (g_dwClockEditMode != 0) {
		return;
	}
	if (g_pDemo != NULL && g_pDemo->m_demoMode != 0) {
		unsigned int nextElapsed = g_dwLastElapsedMilli + DEMO_FRAME_DURATION_MILLISECONDS;
		unsigned int elapsed;
		do {
			elapsed = CurrentMilliTimer() - g_dwCurrentMilli;
		} while (elapsed < nextElapsed);
		g_dwPausedMilli += DEMO_FRAME_DURATION_MILLISECONDS;
		g_dwLastElapsedMilli = elapsed;
		g_dwGameTick = g_dwPausedMilli / GAME_TICK_MILLISECONDS;
		g_dwSimulationTimestamp = g_dwGameTick * GAME_TICK_MILLISECONDS;
		return;
	}

	unsigned int gameTick = CurrentMilliTimer() / GAME_TICK_MILLISECONDS - g_dwGameTimeTick;
	if (gameTick > g_dwLastElapsedMilli + MAX_GAME_TICKS_PER_UPDATE) {
		g_dwGameTimeTick += gameTick - g_dwLastElapsedMilli - MAX_GAME_TICKS_PER_UPDATE;
		gameTick = g_dwLastElapsedMilli + MAX_GAME_TICKS_PER_UPDATE;
	}
	g_dwGameTick = gameTick;
	g_dwSimulationTimestamp = gameTick * GAME_TICK_MILLISECONDS;
	g_dwLastElapsedMilli = gameTick;
}
#undef DEMO_FRAME_DURATION_MILLISECONDS
#undef MAX_GAME_TICKS_PER_UPDATE

// FUNCTION: LEMBALL 0x00408190
void SetRemoteGameTimeReal(unsigned long p_timestamp)
{
	if (g_dwLastRemoteTimestamp < p_timestamp) {
		g_dwNetworkSimulationTimestamp = p_timestamp;
		g_dwRemoteGameTick = p_timestamp / GAME_TICK_MILLISECONDS;
		g_dwLastRemoteTimestamp = p_timestamp;
	}
}

// FUNCTION: LEMBALL 0x00409150
void _DEMO_Init(int p_sourceId)
{
	g_pDemo = new CDemo(p_sourceId);
}

// FUNCTION: LEMBALL 0x00409180
void _DEMO_Quit()
{
	if (g_pDemo != NULL) {
		delete g_pDemo;
		g_pDemo = NULL;
	}
}

// GLOBAL: LEMBALL 0x0049ce08
unsigned long g_dwSimulationTimestamp;

// GLOBAL: LEMBALL 0x0049ce0c
unsigned long g_dwRemoteGameTick;

// GLOBAL: LEMBALL 0x0049ce10
unsigned long g_dwNetworkSimulationTimestamp;

// GLOBAL: LEMBALL 0x0049ce14
unsigned long g_dwLastRemoteTimestamp;

// GLOBAL: LEMBALL 0x0049ce18
unsigned long g_dwLastElapsedMilli;

// GLOBAL: LEMBALL 0x0049ce1c
unsigned long g_dwGameTimeTick;

// GLOBAL: LEMBALL 0x0049ce20
unsigned long g_dwCurrentMilli;

// GLOBAL: LEMBALL 0x0049ce24
unsigned long g_dwPausedMilli;

// GLOBAL: LEMBALL 0x0049ce28
unsigned int g_dwClockEditMode;
