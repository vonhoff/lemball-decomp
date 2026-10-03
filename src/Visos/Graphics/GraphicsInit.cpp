#include "Control/Support/PreInit.h"
#include "Visos/Foundation/VsInit.h"
#include "Visos/Graphics/CCursor.h"
#include "Visos/Graphics/CGDIDevice.h"
#include "Visos/Graphics/CSurface.h"
#include "Visos/Target/Graphics/CGraphicsDriver.h"
#include "Visos/Target/Graphics/CGraphicsState.h"

#include <new.h>

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
	surface = (CSurface*) g_pGdiHelperTarget;
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
