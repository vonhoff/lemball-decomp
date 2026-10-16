#ifndef LEMBALL_FRONTEND_SUPPORT_ABOUTDIALOG_H
#define LEMBALL_FRONTEND_SUPPORT_ABOUTDIALOG_H

#include "Platform/Windows/WindowsTypes.h"

enum AboutDialogControls {
	IDC_ABOUT_COMPANY_NAME = 1024,
	IDC_ABOUT_FILE_DESCRIPTION = 1025,
	IDC_ABOUT_PRODUCT_VERSION = 1026,
	IDC_ABOUT_COPYRIGHT = 1027,
	IDC_ABOUT_LEGAL_TRADEMARKS = 1028,
	IDC_ABOUT_SYSTEM_INFO = 283
};

void CenterWindowOnParent(HWND p_window, HWND p_parent);
char* BuildAboutSystemInfo();
BOOL __stdcall AboutDialogProc(HWND p_dlg, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);

extern int g_nVisosBuildNumber;
extern char g_szAboutBox[12];
extern char g_szCouldntHelpYa[20];
extern char g_szLemballHelpFile[20];
extern char g_szHelpContentsKey[12];

#endif
