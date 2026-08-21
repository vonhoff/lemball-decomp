#include "Platform/Windows/Entry.h"
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
static int g_fDebugTextWindowClassRegistered = 0;
static void* g_CTextWindowVTable[2] = {(void*) NetworkSafeVtableNoop, (void*) NetworkSafeVtableNoop};
extern HANDLE g_hDebugSyncEvent;
extern void __cdecl ShowFatalGetLastErrorMessageAndExit(const char* pszMessage);

// GLOBAL: LEMBALL 0x004a2b7c
int g_fDebugTextWindowActive = 0;

extern "C"
{
	HANDLE WINAPI GetDC(HWND hWnd);
	int WINAPI ReleaseDC(HWND hWnd, HANDLE hDC);
	HANDLE WINAPI SelectObject(HANDLE hdc, HANDLE h);
	BOOL WINAPI GetTextMetricsA(HANDLE hdc, void* pMetrics);
	HANDLE WINAPI CreateFontA(int nHeight,
							  int nWidth,
							  int nEscapement,
							  int nOrientation,
							  int fnWeight,
							  DWORD fdwItalic,
							  DWORD fdwUnderline,
							  DWORD fdwStrikeOut,
							  DWORD fdwCharSet,
							  DWORD fdwOutputPrecision,
							  DWORD fdwClipPrecision,
							  DWORD fdwQuality,
							  DWORD fdwPitchAndFamily,
							  LPCSTR lpszFace);
	int WINAPI SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw);
}

// FUNCTION: LEMBALL 0x00473A60
CTextWindow::CTextWindow(int cLines, const char* pszTitle)
{
	void** pBuffer;
	int* pLines;
	int i;
	WNDCLASSA windowClass;
	HWND hWindow;
	HANDLE hFont;
	HANDLE hdc;
	char textMetrics[60];
	int nWidth;
	int nHeight;

	*(void**) this = (void*) 0x4988d0;
	InitializeCriticalSection((char*) this + 4);
	*(void**) this = (void*) 0x49a4a0;
	pBuffer = (void**) AllocateVSMemBlock(12);
	if (pBuffer == 0) {
		*(void***) ((char*) this + 0x24) = 0;
	}
	else {
		pLines = (int*) AllocateVSMemBlock(cLines * 12 + 4);
		if (pLines == 0) {
			pBuffer[1] = 0;
		}
		else {
			*pLines = cLines;
			for (i = cLines - 1; i >= 0; --i) {
				((void(__fastcall*)(void*)) 0x4749b0)(pLines + 1 + i * 3);
			}
			pBuffer[1] = pLines + 1;
		}
		*(int*) pBuffer = cLines;
		pBuffer[2] = 0;
		*(void***) ((char*) this + 0x24) = pBuffer;
	}
	if (*(void***) ((char*) this + 0x24) == 0) {
		ShowFatalGetLastErrorMessageAndExit("Unable to allocate TextLineBuffer");
	}
	if (g_fDebugTextWindowClassRegistered == 0) {
		windowClass.style = 3;
		windowClass.lpfnWndProc = (WNDPROC) 0x474750;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 4;
		windowClass.hInstance = g_hApplicationInstance;
		windowClass.hIcon = LoadIconA(0, (LPCSTR) 0x7f00);
		windowClass.hCursor = LoadCursorA(0, (LPCSTR) 0x7f00);
		windowClass.hbrBackground = (HANDLE) GetStockObject(0);
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = "CTextWindow";
		if (RegisterClassA(&windowClass) == 0) {
			ShowFatalGetLastErrorMessageAndExit("Unable to register text window class");
		}
		g_fDebugTextWindowClassRegistered = 1;
		g_fDebugTextWindowActive = 1;
	}
	*(int*) ((char*) this + 0x44) = 0;
	nWidth = GetSystemMetrics(0) / 2;
	nHeight = GetSystemMetrics(1) / 2;
	hWindow = CreateWindowExA(0,
							  "CTextWindow",
							  pszTitle,
							  0xcf0000,
							  0x80000000,
							  0x80000000,
							  nWidth,
							  nHeight,
							  0,
							  0,
							  g_hApplicationInstance,
							  this);
	*(HWND*) ((char*) this + 0x1c) = hWindow;
	if (hWindow == 0) {
		ShowFatalGetLastErrorMessageAndExit("Unable to create text window");
	}
	hFont = CreateFontA(8, 6, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0x30, "Courier");
	*(HANDLE*) ((char*) this + 0x20) = hFont;
	if (hFont == 0) {
		ShowFatalGetLastErrorMessageAndExit("Unable to create font");
	}
	hdc = GetDC(hWindow);
	SelectObject(hdc, hFont);
	GetTextMetricsA(hdc, textMetrics);
	*(int*) ((char*) this + 0x3c) = *(int*) textMetrics + *(int*) (textMetrics + 16);
	ReleaseDC(hWindow, hdc);
	((void(__fastcall*)(void*)) 0x474050)(this);
	((void(__fastcall*)(void*)) 0x474000)(this);
	*(const char**) ((char*) this + 0x28) = pszTitle;
	*(int*) ((char*) this + 0x2c) = (*(int**) ((char*) this + 0x24))[2];
	*(int*) ((char*) this + 0x50) = -1;
	*(int*) ((char*) this + 0x54) = -1;
	*(int*) ((char*) this + 0x38) = 0;
	SetScrollPos(hWindow, 1, 0, 1);
	ShowWindow(hWindow, 5);
	UpdateWindow(hWindow);
}

class CTextWindowSyncInterface {
public:
	virtual void BeginUpdate(void);
	virtual void EndUpdate(void);
};

// FUNCTION: LEMBALL 0x00474520
char* CTextWindow::GetSelectionAsString(void)
{
	char* pszSelection;
	char* pszLine;
	char* pLines;
	int cchSelection;
	int i;

	((CTextWindowSyncInterface*) this)->BeginUpdate();
	cchSelection = 0;
	pLines = *(char**) (*(char**) ((char*) this + 0x24) + 4);
	for (i = *(int*) ((char*) this + 0x50); i <= *(int*) ((char*) this + 0x54); ++i) {
		pszLine = *(char**) (pLines + i * 12 + 4);
		cchSelection += strlen(pszLine) + 2;
	}
	pszSelection = (char*) malloc(cchSelection + 1);
	if (pszSelection == 0) {
		ShowFatalGetLastErrorMessageAndExit("GetSelectionAsString: Unable to allocate string");
	}
	*pszSelection = '\0';
	for (i = *(int*) ((char*) this + 0x50); i <= *(int*) ((char*) this + 0x54); ++i) {
		pszLine = *(char**) (pLines + i * 12 + 4);
		strcat(pszSelection, pszLine);
		strcat(pszSelection, "\x0d\x0a");
	}
	((CTextWindowSyncInterface*) this)->EndUpdate();
	return pszSelection;
}

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
