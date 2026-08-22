#include "TargetGraphicsDriver.h"

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a076c
TargetGraphicsDriver* g_pTargetGraphicsDriver;

// GLOBAL: LEMBALL 0x00498ab0
void* g_apCResRasterConstructionVtable[15];

// GLOBAL: LEMBALL 0x004a0768
TargetGraphicsSystemState* g_pTargetGraphicsSystem;

// GLOBAL: LEMBALL 0x004a9bf8
unsigned int g_dwWinGDrawColourTable[256];
