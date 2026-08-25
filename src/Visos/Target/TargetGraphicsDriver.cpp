#include "TargetGraphicsDriver.h"
#include "TargetGraphicsSystemState.h"

// STUB: LEMBALL 0x00457e10
bool TargetGraphicsSystemState::SelectDriver(int p_driverMode)
{
	(void) p_driverMode;
	return 0;
}

TargetGraphicsDriver::~TargetGraphicsDriver()
{
}

// GLOBAL: LEMBALL 0x004a076c
TargetGraphicsDriver* g_pTargetGraphicsDriver = 0;

// GLOBAL: LEMBALL 0x004a0768
TargetGraphicsSystemState* g_pTargetGraphicsSystem = 0;

// GLOBAL: LEMBALL 0x004a9bf8
unsigned int g_dwWinGDrawColourTable[256];
