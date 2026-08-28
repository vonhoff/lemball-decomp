#ifndef LEMBALL_FRONTEND_SUPPORT_ABOUTDIALOG_H
#define LEMBALL_FRONTEND_SUPPORT_ABOUTDIALOG_H

#include "../../Common.h"

void CenterWindowOnParent(void* p_window, void* p_parent);
char* BuildAboutSystemInfo();
int __stdcall AboutDialogProc(void* p_dlg, unsigned int p_msg, unsigned int p_wParam, long p_lParam);

extern int g_nVisosBuildNumber;
extern char g_szAboutBox[12];
extern char g_szCouldntHelpYa[20];
extern char g_szLemballHelpFile[20];
extern char g_szHelpContentsKey[12];

#endif
