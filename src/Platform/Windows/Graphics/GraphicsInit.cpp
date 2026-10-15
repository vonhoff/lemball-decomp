#include "GraphicsInit.h"

#include <stddef.h>

#include "CGDIDevice.h"
#include "CGraphicsDriver.h"
#include "CGraphicsState.h"
#include "CSurface.h"
#include "Engine/Startup/tagPRE_INIT.h"
#include "Platform/Windows/Input/CCursor.h"

class GrafPort;

// FUNCTION: LEMBALL 0x0046ba80
bool _GDI_Init()
{
	g_pTargetGraphicsSystem = new CGraphicsState();
	g_pTargetGraphicsSystem->SelectDriver(8);
	g_pGdiDevice = new CGDIDevice(g_preInitActive.m_flags);
	g_pGdiHelperTarget = new CSurface((GrafPort*) NULL);
	g_pCursor = new CCursor();

	if (g_pGdiDevice != NULL && g_pGdiHelperTarget != NULL) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0046bb70
bool _GDI_Quit()
{
	delete g_pCursor;
	delete g_pGdiHelperTarget;
	delete g_pGdiDevice;
	g_pGdiHelperTarget = NULL;
	g_pGdiDevice = NULL;
	delete g_pTargetGraphicsSystem;
	return true;
}
