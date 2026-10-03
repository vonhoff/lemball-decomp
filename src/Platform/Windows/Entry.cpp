#include "Entry.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CBaseQueueHandler.h"
#include "../../Visos/Foundation/VsInit.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CWnd.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Resources/CMogRes.h"
#include "../../Visos/Target/Graphics/CGraphicsDriver.h"
#include "../../Visos/Target/System/CPlatformServices.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void* g_hApplicationIcon = NULL;

// FUNCTION: LEMBALL 0x004564e0
extern "C" int __stdcall WinMain(void* p_hInstance, void* p_hPrevInstance, char* p_lpCmdLine, int p_nCmdShow)
{
	g_pApplicationInstance = p_hInstance;
	return INIT_Main(p_lpCmdLine);
}

// FUNCTION: LEMBALL 0x00456500
bool PumpEvents()
{
	MSG message;
	unsigned int count;

	CWnd::ProcessMouseMoves();
	g_dwWindowQuitRequested = 0;
	if (g_pBaseNetwork != NULL && g_pNetworkPacketQueue != NULL) {
		do {
			count = ((CBaseQueue*) g_pNetworkPacketQueue)->GetMessageCount();
			if (count != 0) {
				((CBaseQueue*) g_pNetworkPacketQueue)->ProcessNMsgs(count);
			}
		} while (count != 0);
	}

	if (PeekMessageA(&message, NULL, 0, 0, 0) != 0) {
		if (PeekMessageA(&message, NULL, 0, 0, 0) != 0) {
			BOOL(WINAPI * translateMessage)(const MSG*) = TranslateMessage;
			LONG(WINAPI * dispatchMessage)(const MSG*) = DispatchMessageA;
			BOOL(WINAPI * getMessage)(MSG*, HWND, UINT, UINT) = GetMessageA;
			do {
				getMessage(&message, NULL, 0, 0);
				translateMessage(&message);
				dispatchMessage(&message);
			} while (PeekMessageA(&message, NULL, 0, 0, 0) != 0);
		}
	}

	g_pMasterInputQueue->ProcessNMsgs(g_pMasterInputQueue->GetMessageCount());
	if (g_pMogRes != NULL) {
		g_pMogRes->AgeResources();
	}
	if (g_pCursor != NULL) {
		g_pCursor->Process();
	}
	return g_dwWindowQuitRequested == 1;
}

// FUNCTION: LEMBALL 0x00456600
void SyncLoadProgress()
{
}
