#include "shellui.h"

#include "main.h"
#include "vsgdi.h"
#include "vsinit.h"

#include <stdio.h>
#include <stdarg.h>

static const char g_SHELLUI_RegistryKey[] = "SOFTWARE\\Visual Sciences\\Lemmings Paintball";
static const char g_SHELLUI_AboutBoxName[] = "AboutBox";
static const char g_SHELLUI_HelpPath[] = "lemball\\lemball.hlp";
static const char g_SHELLUI_HelpError[] = "Couldn't help ya!\n";
static const char g_SHELLUI_WindowsNtFormat[] = "WindowsNT %u.%u (Build: %u)";
static const char g_SHELLUI_Win32sFormat[] = "Win32s %u.%u (Build: %u)";
static const char g_SHELLUI_Windows95Format[] = "Windows95 %u.%u";
static const char g_SHELLUI_SystemInfoFormat[] =
    "Operating System : %s\nBuild Number : %d\nViSOS Build Number : %d\nPhysical Memory Size : %lu bytes\nPercent Used "
    ": %lu%%\nAvail Memory Free : %lu bytes\n";
static const char g_SHELLUI_ModulePath[] = "LEMBALL.EXE";
static const char g_SHELLUI_SystemInfoBuffer[] = "System Information";

static void TogglePrimaryContextDisplayMode(SHELLUI_PrimaryContextShell *pShell);

enum {
    SHELLUI_MENU_EVENT_DISPLAY = 4,
    SHELLUI_MENU_EVENT_COMMAND = 15,
    SHELLUI_MENU_COMMAND_EXIT = 1,
    SHELLUI_MENU_COMMAND_HELP_INDEX = 2,
    SHELLUI_MENU_COMMAND_HELP_TOPIC = 3,
    SHELLUI_MENU_COMMAND_ABOUT = 4,
    SHELLUI_MENU_COMMAND_DISPLAY_MODE = 5,
    SHELLUI_MENU_COMMAND_HELP_ON_HELP = 6,
    SHELLUI_DIALOG_TEXT_CONTROL = 0x11b,
    SHELLUI_DIALOG_CLOSE_COMMAND_0 = 1,
    SHELLUI_DIALOG_CLOSE_COMMAND_1 = 2,
    SHELLUI_PRODUCT_BUILD_NUMBER = 0x33,
    SHELLUI_VISOS_BUILD_NUMBER = 0xc9,
};

static char g_AboutSystemInfoBuffer[1024];

static void CallNoArgVirtual(void *pObject, unsigned int nByteOffset) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (*)())pVtable[nByteOffset / sizeof(void *)])();
}

static int FormatCStringV(char *pszBuffer, unsigned int cchBuffer, const char *pszFormat, va_list pArgs) {
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

static int FormatCString(char *pszBuffer, unsigned int cchBuffer, const char *pszFormat, ...) {
    int cchWritten;
    va_list Args;

    va_start(Args, pszFormat);
    cchWritten = FormatCStringV(pszBuffer, cchBuffer, pszFormat, Args);
    va_end(Args);

    return cchWritten;
}

static void CopyCString(char *pszTarget, unsigned int cchTarget, const char *pszSource) {
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

static void AppendCString(char *pszTarget, unsigned int cchTarget, const char *pszSuffix) {
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

static DWORD HostGetVersion(void) {
    return 0x80000004u;
}

static void HostGlobalMemoryStatus(MEMORYSTATUS *pStatus) {
    if (pStatus == 0) {
        return;
    }

    pStatus->dwLength = sizeof(*pStatus);
    pStatus->dwMemoryLoad = 30;
    pStatus->dwTotalPhys = 16UL * 1024UL * 1024UL;
    pStatus->dwAvailPhys = 11UL * 1024UL * 1024UL;
    pStatus->dwTotalPageFile = 0;
    pStatus->dwAvailPageFile = 0;
    pStatus->dwTotalVirtual = 0;
    pStatus->dwAvailVirtual = 0;
}

static int HostGetDlgItemTextA(HWND hDlg, int nControlId, char *pszText, int cchText) {
    (void)hDlg;
    (void)nControlId;

    if (pszText == 0 || cchText <= 0) {
        return 0;
    }

    pszText[0] = '\0';
    return 0;
}

static int HostSetDlgItemTextA(HWND hDlg, int nControlId, const char *pszText) {
    (void)hDlg;
    (void)nControlId;
    (void)pszText;
    return 1;
}

static HWND HostGetDlgItem(HWND hDlg, int nControlId) {
    (void)hDlg;
    (void)nControlId;
    return (HWND)g_SHELLUI_SystemInfoBuffer;
}

static LRESULT HostSendMessageA(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    (void)hWnd;
    (void)uMessage;
    (void)wParam;
    (void)lParam;
    return 0;
}

static int HostEndDialog(HWND hDlg, int nResult) {
    (void)hDlg;
    (void)nResult;
    return 1;
}

static char *GetPrimaryContextFromShell(SHELLUI_PrimaryContextShell *pShell) {
    return (char *)pShell - 0x90;
}

static HWND GetPrimaryContextMainWindow(SHELLUI_PrimaryContextShell *pShell) {
    return *(HWND *)(GetPrimaryContextFromShell(pShell) + 0x44);
}

SHELLUI_PrimaryContextShell::SHELLUI_PrimaryContextShell(void) {
    m_hMainWindow = 0;
    m_fQuitRequested = 0;
    m_fWideDisplay = 0;
}

void SHELLUI_PrimaryContextShell::ToggleDisplayMode(void) {
    TogglePrimaryContextDisplayMode(this);
}

void SHELLUI_PrimaryContextShell::RequestQuit(void) {
    *(int *)((char *)this + 0x38) = 1;
}

// FUNCTION: LEMBALL 0x00431C90
static void TogglePrimaryContextDisplayMode(SHELLUI_PrimaryContextShell *pShell) {
    char *pPrimaryContext;
    int nDriver;

    pPrimaryContext = GetPrimaryContextFromShell(pShell);
    if (*(void **)(pPrimaryContext + 0xd4) != 0) {
        CallNoArgVirtual(*(void **)(pPrimaryContext + 0xd4), 0x18);
    }

    nDriver = GetSelectedGraphicsDriverId();
    if (nDriver != VSGDI_DRIVER_METRICS) {
        if (nDriver != VSGDI_DRIVER_DIB_640_480) {
            return;
        }
        InitializeSelectedGraphicsDriver(VSGDI_DRIVER_METRICS);
        return;
    }
    InitializeSelectedGraphicsDriver(VSGDI_DRIVER_DIB_640_480);
}

void InitializePrimaryContextShell(SHELLUI_PrimaryContextShell *pShell) {
    if (pShell == 0) {
        return;
    }

    *pShell = SHELLUI_PrimaryContextShell();
}

// FUNCTION: LEMBALL 0x00455FF0
char *BuildSystemInformationReportString(void) {
    MEMORYSTATUS MemoryStatus;
    DWORD dwVersion;
    unsigned int uMajorVersion;
    unsigned int uMinorVersion;
    unsigned int uBuildVersion;
    char szOperatingSystem[256];
    char szModulePath[256];

    MemoryStatus.dwLength = sizeof(MemoryStatus);
    HostGlobalMemoryStatus(&MemoryStatus);

    dwVersion = HostGetVersion();
    uMajorVersion = (unsigned int)(dwVersion & 0xff);
    uMinorVersion = (unsigned int)((dwVersion >> 8) & 0xff);
    uBuildVersion = (unsigned int)((dwVersion >> 16) & 0x7fff);

    if ((dwVersion & 0x80000000u) == 0) {
        FormatCString(szOperatingSystem,
                      sizeof(szOperatingSystem),
                      g_SHELLUI_WindowsNtFormat,
                      uMajorVersion,
                      uMinorVersion,
                      uBuildVersion);
    } else if (uMajorVersion < 4) {
        FormatCString(szOperatingSystem,
                      sizeof(szOperatingSystem),
                      g_SHELLUI_Win32sFormat,
                      uMajorVersion,
                      uMinorVersion,
                      uBuildVersion);
    } else {
        FormatCString(
            szOperatingSystem, sizeof(szOperatingSystem), g_SHELLUI_Windows95Format, uMajorVersion, uMinorVersion);
    }

    FormatCString(g_AboutSystemInfoBuffer,
                  sizeof(g_AboutSystemInfoBuffer),
                  g_SHELLUI_SystemInfoFormat,
                  szOperatingSystem,
                  SHELLUI_PRODUCT_BUILD_NUMBER,
                  SHELLUI_VISOS_BUILD_NUMBER,
                  MemoryStatus.dwTotalPhys,
                  MemoryStatus.dwMemoryLoad,
                  MemoryStatus.dwAvailPhys);

    if (GetModuleFileNameA(g_hApplicationInstance, szModulePath, sizeof(szModulePath)) == 0) {
        CopyCString(szModulePath, sizeof(szModulePath), g_SHELLUI_ModulePath);
    }
    AppendCString(g_AboutSystemInfoBuffer, sizeof(g_AboutSystemInfoBuffer), szModulePath);
    AppendCString(g_AboutSystemInfoBuffer, sizeof(g_AboutSystemInfoBuffer), "\n");
    AppendCString(g_AboutSystemInfoBuffer, sizeof(g_AboutSystemInfoBuffer), g_SHELLUI_RegistryKey);
    AppendCString(g_AboutSystemInfoBuffer, sizeof(g_AboutSystemInfoBuffer), "\n");

    return g_AboutSystemInfoBuffer;
}

int CALLBACK AboutBoxProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    char szDialogText[256];

    (void)lParam;

    if (uMessage == WM_INITDIALOG) {
        HostSetDlgItemTextA(hDlg, SHELLUI_DIALOG_TEXT_CONTROL, BuildSystemInformationReportString());
        HostGetDlgItemTextA(hDlg, SHELLUI_DIALOG_TEXT_CONTROL, szDialogText, sizeof(szDialogText));
        HostSendMessageA(HostGetDlgItem(hDlg, SHELLUI_DIALOG_TEXT_CONTROL), 0x30, 1, 0);
        return 1;
    }

    if (uMessage == WM_COMMAND) {
        if ((WORD)wParam == SHELLUI_DIALOG_CLOSE_COMMAND_0 || (WORD)wParam == SHELLUI_DIALOG_CLOSE_COMMAND_1) {
            HostEndDialog(hDlg, 1);
            return 1;
        }
    }

    return 0;
}

// FUNCTION: LEMBALL 0x00431CD0
int HandlePrimaryContextMenuCommand(SHELLUI_PrimaryContextShell *pShell, const SHELLUI_MenuCommandMessage *pMessage) {
    char szHelpPath[256];
    char *pszCdromPath;

    if (pShell == 0 || pMessage == 0) {
        return 0;
    }

    if (pMessage->m_wMessage == SHELLUI_MENU_EVENT_DISPLAY) {
        if (pMessage->m_nCommand != 0x25) {
            return 0;
        }

        TogglePrimaryContextDisplayMode(pShell);
        return 1;
    }

    if (pMessage->m_wMessage != SHELLUI_MENU_EVENT_COMMAND) {
        return 0;
    }

    switch (pMessage->m_nCommand) {
    case SHELLUI_MENU_COMMAND_EXIT:
        pShell->RequestQuit();
        return 0;
    case SHELLUI_MENU_COMMAND_HELP_INDEX:
        szHelpPath[0] = '\0';
        pszCdromPath = FindCdromFilePathBySuffix(g_SHELLUI_HelpPath);
        if (pszCdromPath != 0) {
            CopyCString(szHelpPath, sizeof(szHelpPath), pszCdromPath);
        }
        if (!WinHelpA(GetPrimaryContextMainWindow(pShell), szHelpPath, 0x101, 0x49e7cc)) {
            AppendErrorCString(g_SHELLUI_HelpError);
        }
        return 0;
    case SHELLUI_MENU_COMMAND_HELP_TOPIC:
        szHelpPath[0] = '\0';
        pszCdromPath = FindCdromFilePathBySuffix(g_SHELLUI_HelpPath);
        if (pszCdromPath != 0) {
            CopyCString(szHelpPath, sizeof(szHelpPath), pszCdromPath);
        }
        if (!WinHelpA(GetPrimaryContextMainWindow(pShell), szHelpPath, 0x105, 0x49e814)) {
            AppendErrorCString(g_SHELLUI_HelpError);
        }
        return 0;
    case SHELLUI_MENU_COMMAND_ABOUT:
        DialogBoxParamA(
            g_hApplicationInstance, g_SHELLUI_AboutBoxName, GetPrimaryContextMainWindow(pShell), AboutBoxProc, 0);
        return 0;
    case SHELLUI_MENU_COMMAND_DISPLAY_MODE:
        TogglePrimaryContextDisplayMode(pShell);
        return 0;
    case SHELLUI_MENU_COMMAND_HELP_ON_HELP:
        if (!WinHelpA(GetPrimaryContextMainWindow(pShell), 0, 4, 0)) {
            AppendErrorCString(g_SHELLUI_HelpError);
        }
        return 0;
    default:
        break;
    }

    return 1;
}
