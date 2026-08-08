#include "Platform/Windows/Mixed/Engine/CORE/SAFEVT.H"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"
#include "Visos/Windows/Debug/CTextWin.h"

#include <stdlib.h>
#include <string.h>

DECOMP_SIZE_ASSERT(CTextWindow, 0x58);

static const char g_PLATFORM_DebugWindowTitle[] = "Debug Window";

// GLOBAL: LEMBALL 0x004a29f0
CTextWindow* g_pDebugTextWindow = 0;
static HACCEL g_hDebugWindowAccelerators = 0;
static void* g_CTextWindowVTable[2] = {(void*) NetworkSafeVtableNoop, (void*) NetworkSafeVtableNoop};
extern HANDLE g_hDebugSyncEvent;
extern void __cdecl ShowFatalGetLastErrorMessageAndExit(const char* pszMessage);

// GLOBAL: LEMBALL 0x004a2b7c
int g_fDebugTextWindowActive = 0;

class CTextWindowSyncInterface {
public:
	virtual void BeginUpdate(void);
	virtual void EndUpdate(void);
};

// FUNCTION: LEMBALL 0x00472b10
int __stdcall DebugMessageThreadMain(void)
{
	MSG Msg;
	int nMessageResult;
	void* pvTextWindow;

	pvTextWindow = AllocateVSMemBlock(0x58);
	if (pvTextWindow == 0) {
		g_pDebugTextWindow = 0;
	}
	else {
		g_pDebugTextWindow = (CTextWindow*) pvTextWindow;
		g_pDebugTextWindow->m_pVTable = g_CTextWindowVTable;
		g_pDebugTextWindow->m_pszTitle = g_PLATFORM_DebugWindowTitle;
		g_pDebugTextWindow->m_cchBuffer = 0x2800;
	}

	SetEvent(g_hDebugSyncEvent);
	nMessageResult = GetMessageA(&Msg, 0, 0, 0);
	while (nMessageResult != 0) {
		if (g_hDebugWindowAccelerators == 0 || TranslateAcceleratorA(Msg.hwnd, g_hDebugWindowAccelerators, &Msg) == 0) {
			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
		}

		nMessageResult = GetMessageA(&Msg, 0, 0, 0);
	}

	SetEvent(g_hDebugSyncEvent);
	return 1;
}

// FUNCTION: LEMBALL 0x00473d90
void CTextWindow::PostAllocatedTextControlString(const char* pszText, unsigned int dwColour)
{
	char* pszCopy;
	unsigned int cchText;

	((CTextWindowSyncInterface*) this)->BeginUpdate();
	if (g_fDebugTextWindowActive == 0) {
		return;
	}
	cchText = (unsigned int) strlen(pszText) + 1;
	pszCopy = (char*) malloc(cchText);
	if (pszCopy == 0) {
		ShowFatalGetLastErrorMessageAndExit("Unable to allocate memory for string copy");
	}
	strcpy(pszCopy, pszText);
	if (*(HWND*) ((char*) this + 0x1c) != 0) {
		PostMessageA(*(HWND*) ((char*) this + 0x1c), 0x420, (WPARAM) pszCopy, (LPARAM) dwColour);
	}
	((CTextWindowSyncInterface*) this)->EndUpdate();
}

// FUNCTION: LEMBALL 0x00473e20
void LEMBALL_FASTCALL handle_debug_text_append_message(void* pThis, int nUnused, char* pszText, int nColour)
{
	int nOldCount;
	int nNewCount;

	(void) nUnused;
	((void(__fastcall*)(void*)) (*(void**) pThis))(pThis);
	((void(__fastcall*)(void*, int, char*, int)) 0x473a10)(*(void**) ((char*) pThis + 0x24), 0, pszText, nColour);
	nOldCount = *(int*) ((char*) pThis + 0x2c);
	nNewCount = *(int*) (*(int**) ((char*) pThis + 0x24) + 8);
	*(int*) ((char*) pThis + 0x2c) = nNewCount;
	if (nNewCount == nOldCount) {
		((void(__fastcall*)(void*, int)) 0x473f60)(pThis, 0);
	}
	else {
		((void(__fastcall*)(void*, int, int, int)) 0x473f80)(pThis, 0, nOldCount, nNewCount - nOldCount);
	}
	((void(__stdcall*)(int, int, int, int, int)) 0x4ac8f4)(*(int*) ((char*) pThis + 0x1c), 1, 0, nNewCount - 1, 1);
	if (*(int*) ((char*) pThis + 0x50) == -1 || *(int*) ((char*) pThis + 0x54) == -1) {
		((void(__fastcall*)(void*, int, int)) 0x474430)(pThis, 0, nOldCount);
	}
	((void(__cdecl*)(void*)) 0x47fff0)(pszText);
	((void(__fastcall*)(void*)) (*(void**) ((char*) pThis + 4)))(pThis);
}

// FUNCTION: LEMBALL 0x00474340
void LEMBALL_FASTCALL begin_text_selection_drag(void* pThis, int nUnused, int param_1, int param_2)
{
	int nLine;

	(void) nUnused;
	((void(__fastcall*)(void*)) (*(void**) pThis))(pThis);
	nLine = ((int(__fastcall*)(void*, int, int, int)) 0x473f00)(pThis, 0, param_1, param_2);
	*(int*) ((char*) pThis + 0x48) = nLine;
	if (*(int*) ((char*) pThis + 0x2c) <= nLine) {
		((void(__fastcall*)(void*)) (*(void**) ((char*) pThis + 4)))(pThis);
		return;
	}
	((void(__stdcall*)(int)) 0x4ac890)(*(int*) ((char*) pThis + 0x1c));
	*(int*) ((char*) pThis + 0x44) = 1;
	((void(__fastcall*)(void*, int, int)) 0x4743e0)(pThis, 0, 0);
	nLine = *(int*) ((char*) pThis + 0x48);
	*(int*) ((char*) pThis + 0x54) = nLine;
	*(int*) ((char*) pThis + 0x50) = nLine;
	*(int*) ((char*) pThis + 0x4c) = nLine;
	((void(__fastcall*)(void*, int, int)) 0x4743e0)(pThis, 0, 1);
	((void(__fastcall*)(void*)) (*(void**) ((char*) pThis + 4)))(pThis);
}

// FUNCTION: LEMBALL 0x00474430
void LEMBALL_FASTCALL scroll_debug_text_line_into_view(void* pThis, int nUnused, int nTargetLine)
{
	int nPos;

	(void) nUnused;
	((void(__fastcall*)(void*)) (*(void**) pThis))(pThis);
	if (nTargetLine < *(int*) ((char*) pThis + 0x38)) {
		*(int*) ((char*) pThis + 0x38) = nTargetLine;
		((void(__stdcall*)(int, int, int, int)) 0x4ac8f0)(*(int*) ((char*) pThis + 0x1c), 1, nTargetLine, 1);
		((void(__fastcall*)(void*, int)) 0x473f60)(pThis, 0);
		((void(__fastcall*)(void*)) (*(void**) ((char*) pThis + 4)))(pThis);
		return;
	}
	if (*(int*) ((char*) pThis + 0x38) + *(int*) ((char*) pThis + 0x30) <= nTargetLine) {
		nPos = (nTargetLine - *(int*) ((char*) pThis + 0x30)) + 1;
		*(int*) ((char*) pThis + 0x38) = nPos;
		((void(__stdcall*)(int, int, int, int)) 0x4ac8f0)(*(int*) ((char*) pThis + 0x1c), 1, nPos, 1);
		((void(__fastcall*)(void*, int)) 0x473f60)(pThis, 0);
	}
	((void(__fastcall*)(void*)) (*(void**) ((char*) pThis + 4)))(pThis);
}
