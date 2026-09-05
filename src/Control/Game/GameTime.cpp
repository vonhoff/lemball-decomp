#include "GameTime.h"

#include "../../Visos/Foundation/VsTime.h"
#include "Demo.h"
#include "Game.h"

#include <new.h>

// 68K 0x10701228 ClockEditMode__FUc
// FUNCTION: LEMBALL 0x00408080
void ClockEditMode(unsigned int p_enabled)
{
	g_dwClockEditMode = p_enabled;
}

// 68K 0x1070124e ResetGameTimes__Fv
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

// 68K 0x107012a4 SetGameTime__Fv
// STUB: LEMBALL 0x004080d0
void SetGameTime()
{
}

// 68K 0x10701386 SetRemoteGameTimeReal__FUl
// FUNCTION: LEMBALL 0x00408190
void SetRemoteGameTimeReal(unsigned long p_timestamp)
{
	if (g_dwLastRemoteTimestamp < p_timestamp) {
		g_dwNetworkSimulationTimestamp = p_timestamp;
		g_dwRemoteGameTick = p_timestamp / GAME_TICK_MILLISECONDS;
		g_dwLastRemoteTimestamp = p_timestamp;
	}
}

// 68K 0x107009e2 _DEMO_Init__Fi
// STUB: LEMBALL 0x00409150
Demo* DemoInit(int p_arg0)
{
	void* storage;

	storage = operator new(0x58);
	if (storage != 0) {
		g_pDemo = new (storage) Demo(p_arg0);
		return g_pDemo;
	}
	g_pDemo = 0;
	return 0;
}

// 68K 0x10700a26 _DEMO_Quit__Fv
// FUNCTION: LEMBALL 0x00409180
void DemoQuit()
{
	if (g_pDemo != 0) {
		delete g_pDemo;
		g_pDemo = 0;
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
