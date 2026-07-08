#ifndef LEMBALL_SHELL_UI_H
#define LEMBALL_SHELL_UI_H

#include "game/game_app.h"

class SHELLUI_PrimaryContextShell {
public:
    SHELLUI_PrimaryContextShell(void);

    void ToggleDisplayMode(void);
    void RequestQuit(void);

public:
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
