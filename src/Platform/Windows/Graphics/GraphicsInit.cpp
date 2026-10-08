#include "CGDIDevice.h"
#include "CGraphicsDriver.h"
#include "CGraphicsState.h"
#include "CSurface.h"
#include "Engine/Startup/PreInit.h"
#include "Engine/Startup/VsInit.h"
#include "Platform/Windows/Input/CCursor.h"

#include <new.h>
#include <stddef.h>

class GrafPort;

// FUNCTION: LEMBALL 0x0046ba80
bool _GDI_Init()
{
	void* storage;
	CCursor* cursor;
	CGraphicsState* system;

	storage = operator new(sizeof(CGraphicsState));
	system = (CGraphicsState*) storage;
	if (system != NULL) {
		system->m_targetWindow = 0;
		system->m_fallbackWarningShown = 0;
		g_pTargetGraphicsSystem = system;
	}
	else {
		g_pTargetGraphicsSystem = NULL;
	}
	g_pTargetGraphicsSystem->SelectDriver(8);

	storage = operator new(sizeof(CGDIDevice));
	if (storage != NULL) {
		g_pGdiDevice = new (storage) CGDIDevice(g_preInitActive.m_flags);
	}
	else {
		g_pGdiDevice = NULL;
	}

	storage = operator new(sizeof(CSurface));
	if (storage != NULL) {
		g_pGdiHelperTarget = new (storage) CSurface((GrafPort*) NULL);
	}
	else {
		g_pGdiHelperTarget = NULL;
	}

	cursor = (CCursor*) operator new(sizeof(CCursor));
	if (cursor != NULL) {
		new (cursor) CCursor();
		g_pCursor = cursor;
	}
	else {
		g_pCursor = NULL;
	}

	if (g_pGdiDevice != NULL && g_pGdiHelperTarget != NULL) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0046bb70
bool _GDI_Quit()
{
	CSurface* surface;
	CGDIDevice* device;
	CGraphicsState* system;

	delete g_pCursor;
	surface = g_pGdiHelperTarget;
	if (surface != NULL) {
		surface->~CSurface();
		operator delete(surface);
	}
	device = g_pGdiDevice;
	if (device != NULL) {
		device->~CGDIDevice();
		operator delete(device);
	}
	g_pGdiHelperTarget = NULL;
	g_pGdiDevice = NULL;
	system = g_pTargetGraphicsSystem;
	if (system != NULL) {
		if (g_pTargetGraphicsDriver != NULL) {
			delete g_pTargetGraphicsDriver;
		}
		operator delete(system);
	}
	return true;
}
