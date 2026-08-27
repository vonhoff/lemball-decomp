#include "Entry.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/BaseQueueHandler.h"
#include "../../Visos/Foundation/VsInit.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../../Visos/Graphics/Wnd.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Target/TargetPlatformServices.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void* g_hApplicationIcon = 0;

// FUNCTION: LEMBALL 0x004564e0
extern "C" int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	g_pApplicationInstance = hInstance;
	return InitMain(lpCmdLine);
}

// STUB: LEMBALL 0x00456500
bool TargetPumpEvents()
{
	MSG message;

	Wnd::ProcessMouseMoves();
	g_dwWindowQuitRequested = 0;
	if (g_pBaseNetwork != 0) {
	}

	if (PeekMessageA(&message, 0, 0, 0, 0) != 0) {
		while (PeekMessageA(&message, 0, 0, 0, 0) != 0) {
			GetMessageA(&message, 0, 0, 0);
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}

	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->ProcessNMsgs(g_pMasterInputQueue->GetMessageCount());
	}
	if (g_pMogRes != 0) {
		g_pMogRes->AgeResources();
	}
	if (g_pCursor != 0) {
		g_pCursor->Process();
	}
	return g_dwWindowQuitRequested == 1;
}
