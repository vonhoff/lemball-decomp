#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/GDI/VSGDI.H"
#include "ENGINE/MEDIA/VSSOUND.H"
#include "SHELL/ENTRY.H"
#include "SHELL/SHELLUI.H"

#include <stdarg.h>
#include <stdio.h>

extern "C" int WINAPI IsBadReadPtr(const void* pAddress, unsigned int cbAddress);
#include <string.h>

static const char g_SHELLUI_RegistryKey[] = "SOFTWARE\\Visual Sciences\\Lemmings Paintball";
static const char g_SHELLUI_AboutBoxName[] = "AboutBox";
static const char g_SHELLUI_HelpError[] = "Couldn't help ya!\n";
static const char g_SHELLUI_WindowsNtFormat[] = "WindowsNT %u.%u (Build: %u)";
static const char g_SHELLUI_Win32sFormat[] = "Win32s %u.%u (Build: %u)";
static const char g_SHELLUI_Windows95Format[] = "Windows95 %u.%u";
static const char g_SHELLUI_SystemInfoFormat[] =
	"Operating System : %s\nBuild Number : %d\nViSOS Build Number : %d\nPhysical Memory Size : %lu bytes\nPercent Used "
	": %lu%%\nAvail Memory Free : %lu bytes\n";
static const char g_SHELLUI_SystemInfoBuffer[] = "System Information";

void LEMBALL_FASTCALL TogglePrimaryContextDisplayMode(void* pPrimaryContext);

enum {
	ShellUiMENU_EVENT_DISPLAY = 4,
	ShellUiMENU_EVENT_COMMAND = 15,
	ShellUiMENU_COMMAND_EXIT = 1,
	ShellUiMENU_COMMAND_HELP_INDEX = 2,
	ShellUiMENU_COMMAND_HELP_TOPIC = 3,
	ShellUiMENU_COMMAND_ABOUT = 4,
	ShellUiMENU_COMMAND_DISPLAY_MODE = 5,
	ShellUiMENU_COMMAND_HELP_ON_HELP = 6,
	ShellUiDIALOG_TEXT_CONTROL = 0x11b,
	ShellUiDIALOG_CLOSE_COMMAND_0 = 1,
	ShellUiDIALOG_CLOSE_COMMAND_1 = 2,
	ShellUiPRODUCT_BUILD_NUMBER = 0x33,
	ShellUiVISOS_BUILD_NUMBER = 0xc9,
};

static char g_AboutSystemInfoBuffer[1024];
static DWORD g_SHELLUI_PhysicalMemorySize = 16UL * 1024UL * 1024UL;
static DWORD g_SHELLUI_MemoryUsagePercent = 30;
static DWORD g_SHELLUI_AvailableMemorySize = 11UL * 1024UL * 1024UL;

static void CallNoArgVirtual(void* pObject, unsigned int nByteOffset)
{
	void** pVtable;

	if (pObject == 0 || IsBadReadPtr(pObject, sizeof(void*)) != 0) {
		return;
	}
	pVtable = *(void***) pObject;
	if (pVtable == 0 || IsBadReadPtr(pVtable + nByteOffset / sizeof(void*), sizeof(void*)) != 0) {
		return;
	}
	((void(LEMBALL_FASTCALL*)(void*, int)) pVtable[nByteOffset / sizeof(void*)])(pObject, 0);
}
static int FormatCStringV(char* pszBuffer, unsigned int cchBuffer, const char* pszFormat, va_list pArgs)
{
	int cchWritten;

	if (pszBuffer == 0 || cchBuffer == 0 || pszFormat == 0) {
		return 0;
	}

#if defined(_MSC_VER) && (_MSC_VER < 1400)
	cchWritten = _vsnprintf(pszBuffer, cchBuffer, pszFormat, pArgs);
	pszBuffer[cchBuffer - 1] = '\0';
#else
	cchWritten = vsnprintf(pszBuffer, cchBuffer, pszFormat, pArgs);
#endif

	return cchWritten;
}

static int FormatCString(char* pszBuffer, unsigned int cchBuffer, const char* pszFormat, ...)
{
	int cchWritten;
	va_list Args;

	va_start(Args, pszFormat);
	cchWritten = FormatCStringV(pszBuffer, cchBuffer, pszFormat, Args);
	va_end(Args);

	return cchWritten;
}

static void CopyCString(char* pszTarget, unsigned int cchTarget, const char* pszSource)
{
	unsigned int i;

	if (pszTarget == 0 || cchTarget == 0) {
		return;
	}

	if (pszSource == 0) {
		pszTarget[0] = '\0';
		return;
	}

	i = 0;
	while (pszSource[i] != '\0' && i + 1 < cchTarget) {
		pszTarget[i] = pszSource[i];
		++i;
	}
	pszTarget[i] = '\0';
}

static void AppendCString(char* pszTarget, unsigned int cchTarget, const char* pszSuffix)
{
	unsigned int cchCurrent;

	if (pszTarget == 0 || cchTarget == 0) {
		return;
	}

	cchCurrent = 0;
	while (cchCurrent < cchTarget && pszTarget[cchCurrent] != '\0') {
		++cchCurrent;
	}

	if (cchCurrent < cchTarget) {
		CopyCString(pszTarget + cchCurrent, cchTarget - cchCurrent, pszSuffix);
	}
}

static int HostGetDlgItemTextA(HWND hDlg, int nControlId, char* pszText, int cchText)
{
	(void) hDlg;
	(void) nControlId;

	if (pszText == 0 || cchText <= 0) {
		return 0;
	}

	pszText[0] = '\0';
	return 0;
}
static int HostSetDlgItemTextA(HWND hDlg, int nControlId, const char* pszText)
{
	(void) hDlg;
	(void) nControlId;
	(void) pszText;
	return 1;
}

static HWND HostGetDlgItem(HWND hDlg, int nControlId)
{
	(void) hDlg;
	(void) nControlId;
	return (HWND) g_SHELLUI_SystemInfoBuffer;
}

static LRESULT HostSendMessageA(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	(void) hWnd;
	(void) uMessage;
	(void) wParam;
	(void) lParam;
	return 0;
}
static int HostEndDialog(HWND hDlg, int nResult)
{
	(void) hDlg;
	(void) nResult;
	return 1;
}

static char* GetPrimaryContextFromShell(ShellUiPrimaryContextShell* pShell)
{
	return (char*) pShell - 0x90;
}

static HWND GetPrimaryContextMainWindow(ShellUiPrimaryContextShell* pShell)
{
	return *(HWND*) (GetPrimaryContextFromShell(pShell) + 0x44);
}

ShellUiPrimaryContextShell::ShellUiPrimaryContextShell(void)
{
	m_hMainWindow = 0;
	m_fQuitRequested = 0;
	m_fWideDisplay = 0;
}

void ShellUiPrimaryContextShell::ToggleDisplayMode(void)
{
	TogglePrimaryContextDisplayMode(GetPrimaryContextFromShell(this));
}

void ShellUiPrimaryContextShell::RequestQuit(void)
{
	*(int*) ((char*) this + 0x38) = 1;
}

// STUB: LEMBALL 0x0040292d
int LEMBALL_FASTCALL HandlePrimaryContextMenuCommandThunk(ShellUiPrimaryContextShell* pShell,
														  int,
														  const ShellUiMenuCommandMessage* pMessage)

{
	return HandlePrimaryContextMenuCommand(pShell, 0, pMessage);
}
// FUNCTION: LEMBALL 0x00431c90
void LEMBALL_FASTCALL TogglePrimaryContextDisplayMode(void* pPrimaryContext)
{
	void* pScreen;
	int nDriver;

	pScreen = *(void**) ((char*) pPrimaryContext + 0xd4);
	if (pScreen != 0) {
		((void(LEMBALL_FASTCALL*)(void*))(*(void***) pScreen)[0x18 / sizeof(void*)])(pScreen);
	}

	nDriver = ((VsGdiSelectedGraphicsDriverRuntime*) g_pSelectedGraphicsDriverRuntime)->m_nSelectedDriver;
	switch (nDriver) {
	case VsGdiDRIVER_METRICS:
		((VsGdiSelectedGraphicsDriverRuntime*) g_pSelectedGraphicsDriverRuntime)
			->InitializeSelectedGraphicsDriver(VsGdiDRIVER_DIB_640_480);
		return;
	case VsGdiDRIVER_DIB_640_480:
		((VsGdiSelectedGraphicsDriverRuntime*) g_pSelectedGraphicsDriverRuntime)
			->InitializeSelectedGraphicsDriver(VsGdiDRIVER_METRICS);
		return;
	}
}

void InitializePrimaryContextShell(ShellUiPrimaryContextShell* pShell)
{
	if (pShell == 0) {
		return;
	}

	*pShell = ShellUiPrimaryContextShell();
}

// FUNCTION: LEMBALL 0x00431cd0
int LEMBALL_FASTCALL HandlePrimaryContextMenuCommand(ShellUiPrimaryContextShell* pShell,
													 int,
													 const ShellUiMenuCommandMessage* pMessage)
{
	char szHelpPath[256];

	if (pMessage->m_wMessage == ShellUiMENU_EVENT_DISPLAY) {
		if (pMessage->m_nCommand != 0x25) {
			return 0;
		}

		TogglePrimaryContextDisplayMode((char*) pShell - 0x90);
		return 1;
	}

	if (pMessage->m_wMessage != ShellUiMENU_EVENT_COMMAND) {
		return 0;
	}

	switch (pMessage->m_nCommand) {
	case ShellUiMENU_COMMAND_EXIT:
		*(int*) ((char*) pShell + 0x38) = 1;
		return 0;
	case ShellUiMENU_COMMAND_HELP_INDEX:
		szHelpPath[0] = '\0';
		strcpy(szHelpPath, FindCdromFilePathBySuffix("lemball\\lemball.hlp"));
		strcat(szHelpPath, "lemball\\lemball.hlp");
		if (!WinHelpA(*(HWND*) ((char*) pShell - 0x4c), szHelpPath, 0x101, 0x49e7cc)) {
			g_pErrorOutputStream->AppendCStringToStream(g_SHELLUI_HelpError);
		}
		return 0;
	case ShellUiMENU_COMMAND_HELP_TOPIC:
		szHelpPath[0] = '\0';
		strcpy(szHelpPath, FindCdromFilePathBySuffix("lemball\\lemball.hlp"));
		strcat(szHelpPath, "lemball\\lemball.hlp");
		if (!WinHelpA(*(HWND*) ((char*) pShell - 0x4c), szHelpPath, 0x105, 0x49e814)) {
			g_pErrorOutputStream->AppendCStringToStream(g_SHELLUI_HelpError);
		}
		return 0;
	case ShellUiMENU_COMMAND_ABOUT:
		DialogBoxParamA(g_hApplicationInstance,
						g_SHELLUI_AboutBoxName,
						*(HWND*) ((char*) pShell - 0x4c),
						AboutBoxProc,
						0);
		return 0;
	case ShellUiMENU_COMMAND_DISPLAY_MODE:
		TogglePrimaryContextDisplayMode((char*) pShell - 0x90);
		return 0;
	case ShellUiMENU_COMMAND_HELP_ON_HELP:
		if (!WinHelpA(*(HWND*) ((char*) pShell - 0x4c), 0, 4, 0)) {
			g_pErrorOutputStream->AppendCStringToStream(g_SHELLUI_HelpError);
		}
		return 0;
	default:
		break;
	}

	return 1;
}

// FUNCTION: LEMBALL 0x00455ff0
char* LEMBALL_STDCALL BuildSystemInformationReportString(void)
{
	char chTerminator;
	DWORD dwVersion;
	unsigned char bMinorVersion;
	unsigned int cchAudioDescription;
	int cchBuffer;
	char* pszAudioDescription;
	char* pszAudioSource;
	char* pszAudioTarget;
	char* pszBuffer;
	char* pszTarget;
	char szOperatingSystem[256];

	dwVersion = GetVersion();
	bMinorVersion = (unsigned char) ((dwVersion & 0xffff) >> 8);

	if (dwVersion < 0x80000000u) {
		wsprintfA(szOperatingSystem,
				  g_SHELLUI_WindowsNtFormat,
				  dwVersion & 0xff,
				  (unsigned int) bMinorVersion,
				  (unsigned int) (dwVersion >> 16));
	}
	else if ((unsigned char) (dwVersion & 0xff) < 4) {
		wsprintfA(szOperatingSystem,
				  g_SHELLUI_Win32sFormat,
				  dwVersion & 0xff,
				  (unsigned int) bMinorVersion,
				  (unsigned int) ((dwVersion >> 16) & 0x7fff));
	}
	else {
		wsprintfA(szOperatingSystem, g_SHELLUI_Windows95Format, dwVersion & 0xff, (unsigned int) bMinorVersion);
	}

	wsprintfA(g_AboutSystemInfoBuffer,
			  g_SHELLUI_SystemInfoFormat,
			  szOperatingSystem,
			  ShellUiPRODUCT_BUILD_NUMBER,
			  ShellUiVISOS_BUILD_NUMBER,
			  g_SHELLUI_PhysicalMemorySize,
			  g_SHELLUI_MemoryUsagePercent,
			  g_SHELLUI_AvailableMemorySize);

	pszAudioDescription = BuildAudioManagerDescriptionString(g_pAudioManager);
	cchAudioDescription = 0xffffffffu;
	do {
		pszAudioTarget = pszAudioDescription;
		if (cchAudioDescription == 0) {
			break;
		}
		--cchAudioDescription;
		pszAudioTarget = pszAudioDescription + 1;
		chTerminator = *pszAudioDescription;
		pszAudioDescription = pszAudioTarget;
	} while (chTerminator != '\0');

	cchAudioDescription = ~cchAudioDescription;
	cchBuffer = -1;
	pszBuffer = g_AboutSystemInfoBuffer;
	do {
		pszTarget = pszBuffer;
		if (cchBuffer == 0) {
			break;
		}
		cchBuffer = cchBuffer + -1;
		pszTarget = pszBuffer + 1;
		chTerminator = *pszBuffer;
		pszBuffer = pszTarget;
	} while (chTerminator != '\0');

	pszAudioSource = pszAudioTarget - cchAudioDescription;
	pszTarget = pszTarget - 1;
	memcpy(pszTarget, pszAudioSource, cchAudioDescription);

	return g_AboutSystemInfoBuffer;
}

int CALLBACK AboutBoxProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	char szDialogText[256];

	(void) lParam;

	if (uMessage == WM_INITDIALOG) {
		HostSetDlgItemTextA(hDlg, ShellUiDIALOG_TEXT_CONTROL, BuildSystemInformationReportString());
		HostGetDlgItemTextA(hDlg, ShellUiDIALOG_TEXT_CONTROL, szDialogText, sizeof(szDialogText));
		HostSendMessageA(HostGetDlgItem(hDlg, ShellUiDIALOG_TEXT_CONTROL), 0x30, 1, 0);
		return 1;
	}

	if (uMessage == WM_COMMAND) {
		if ((WORD) wParam == ShellUiDIALOG_CLOSE_COMMAND_0 || (WORD) wParam == ShellUiDIALOG_CLOSE_COMMAND_1) {
			HostEndDialog(hDlg, 1);
			return 1;
		}
	}

	return 0;
}
