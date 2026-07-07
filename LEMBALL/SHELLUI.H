#ifndef LEMBALL_SHELLUI_H
#define LEMBALL_SHELLUI_H

#include "GAME.H"

struct SHELLUI_PrimaryContextShell {
    HWND m_hMainWindow;
    int m_fQuitRequested;
    int m_fWideDisplay;
};

struct SHELLUI_MenuCommandMessage {
    short m_wMessage;
    short m_wReserved0;
    short m_wReserved1;
    short m_wReserved2;
    int m_nCommand;
};

void InitializePrimaryContextShell(SHELLUI_PrimaryContextShell *pShell);
char *BuildSystemInformationReportString(void);
int HandlePrimaryContextMenuCommand(SHELLUI_PrimaryContextShell *pShell, const SHELLUI_MenuCommandMessage *pMessage);
int CALLBACK AboutBoxProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam);

#endif
