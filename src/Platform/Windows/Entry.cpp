#include "Entry.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CBaseQueueHandler.h"
#include "../../Visos/Foundation/VsInit.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CWnd.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Resources/CMogRes.h"
#include "../../Visos/Target/CGraphicsDriver.h"
#include "../../Visos/Target/CPlatformServices.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void* g_hApplicationIcon = 0;

// FUNCTION: LEMBALL 0x004564e0
extern "C" int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	g_pApplicationInstance = hInstance;
	return InitMain(lpCmdLine);
}

// FUNCTION: LEMBALL 0x00456500
bool PumpEvents()
{
	MSG message;
	unsigned int count;

	CWnd::ProcessMouseMoves();
	g_dwWindowQuitRequested = 0;
	if (g_pBaseNetwork != 0 && g_pNetworkPacketQueue != 0) {
		do {
			count = ((CBaseQueue*) g_pNetworkPacketQueue)->GetMessageCount();
			if (count != 0) {
				((CBaseQueue*) g_pNetworkPacketQueue)->ProcessNMsgs(count);
			}
		} while (count != 0);
	}

	if (PeekMessageA(&message, 0, 0, 0, 0) != 0) {
		if (PeekMessageA(&message, 0, 0, 0, 0) != 0) {
			BOOL(WINAPI * translateMessage)(const MSG*) = TranslateMessage;
			LONG(WINAPI * dispatchMessage)(const MSG*) = DispatchMessageA;
			BOOL(WINAPI * getMessage)(MSG*, HWND, UINT, UINT) = GetMessageA;
			do {
				getMessage(&message, 0, 0, 0);
				translateMessage(&message);
				dispatchMessage(&message);
			} while (PeekMessageA(&message, 0, 0, 0, 0) != 0);
		}
	}

	g_pMasterInputQueue->ProcessNMsgs(g_pMasterInputQueue->GetMessageCount());
	if (g_pMogRes != 0) {
		g_pMogRes->AgeResources();
	}
	if (g_pCursor != 0) {
		g_pCursor->Process();
	}
	return g_dwWindowQuitRequested == 1;
}

// FUNCTION: LEMBALL 0x00456600
void SyncLoadProgress()
{
}
