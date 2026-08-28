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

// FUNCTION: LEMBALL 0x00456500
bool TargetPumpEvents()
{
	MSG message;
	unsigned int count;

	Wnd::ProcessMouseMoves();
	g_dwWindowQuitRequested = 0;
	if (g_pBaseNetwork != 0) {
		if (g_pNetworkPacketQueue != 0) {
			do {
				count = ((BaseQueue*) g_pNetworkPacketQueue)->GetMessageCount();
				if (count != 0) {
					((BaseQueue*) g_pNetworkPacketQueue)->ProcessNMsgs(count);
				}
			} while (count != 0);
		}
	}

	if (PeekMessageA(&message, 0, 0, 0, 0) != 0) {
		while (PeekMessageA(&message, 0, 0, 0, 0) != 0) {
			GetMessageA(&message, 0, 0, 0);
			TranslateMessage(&message);
			DispatchMessageA(&message);
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
void TargetSynchronizeLoadProgress()
{
}
