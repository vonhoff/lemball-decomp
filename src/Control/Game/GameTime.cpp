#include "GameTime.h"

#include "Demo.h"

// 68K 0x10701228 ClockEditMode__FUc
// FUNCTION: LEMBALL 0x00408080
void ClockEditMode(unsigned int p_enabled)
{
	g_dwClockEditMode = p_enabled;
}

// 68K 0x1070124e ResetGameTimes__Fv
// STUB: LEMBALL 0x00408090
void ResetGameTimes()
{
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
		g_dwRemoteGameTick = p_timestamp / 50;
		g_dwLastRemoteTimestamp = p_timestamp;
	}
}

// 68K 0x107009e2 _DEMO_Init__Fi
// STUB: LEMBALL 0x00409150
Demo* DemoInit(int p_arg0)
{
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

// GLOBAL: LEMBALL 0x0049ce28
unsigned int g_dwClockEditMode;
