#include "Control/Support/PreInit.h"
#include "Visos/Foundation/VsInit.h"
#include "Visos/Graphics/CCursor.h"
#include "Visos/Graphics/CGdiDevice.h"
#include "Visos/Graphics/CSurface.h"
#include "Visos/Target/Graphics/CGraphicsDriver.h"
#include "Visos/Target/Graphics/CGraphicsState.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0046ba80
bool InternalGdiInit()
{
	void* storage;
	CCursor* cursor;
	CGraphicsState* system;

	storage = operator new(0xc);
	system = (CGraphicsState*) storage;
	if (system != 0) {
		system->m_reserved04 = 0;
		system->m_fallbackWarningShown = 0;
		g_pTargetGraphicsSystem = system;
	}
	else {
		g_pTargetGraphicsSystem = 0;
	}
	g_pTargetGraphicsSystem->SelectDriver(8);

	storage = operator new(sizeof(CGdiDevice));
	if (storage != 0) {
		g_pGdiDevice = new (storage) CGdiDevice(g_preInitActive.m_flags);
	}
	else {
		g_pGdiDevice = 0;
	}

	storage = operator new(0x5a0);
	if (storage != 0) {
		g_pGdiHelperTarget = new (storage) CSurface((GrafPort*) 0);
	}
	else {
		g_pGdiHelperTarget = 0;
	}

	cursor = (CCursor*) operator new(sizeof(CCursor));
	if (cursor != 0) {
		new (cursor) CCursor();
		g_pCursor = cursor;
	}
	else {
		g_pCursor = 0;
	}

	if (g_pGdiDevice != 0 && g_pGdiHelperTarget != 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046bb70
bool InternalGdiQuit()
{
	CSurface* surface;
	CGdiDevice* device;
	CGraphicsState* system;

	delete g_pCursor;
	surface = (CSurface*) g_pGdiHelperTarget;
	if (surface != 0) {
		surface->~CSurface();
		operator delete(surface);
	}
	device = g_pGdiDevice;
	if (device != 0) {
		device->~CGdiDevice();
		operator delete(device);
	}
	system = g_pTargetGraphicsSystem;
	g_pGdiHelperTarget = 0;
	g_pGdiDevice = 0;
	if (system != 0) {
		if (g_pTargetGraphicsDriver != 0) {
			delete g_pTargetGraphicsDriver;
		}
		operator delete(system);
	}
	return 1;
}
