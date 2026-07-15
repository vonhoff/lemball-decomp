#ifndef LEMBALL_SHELL_UI_H
#define LEMBALL_SHELL_UI_H

#include "GAME.H"

class ShellUiPrimaryContextShell {
public:
    ShellUiPrimaryContextShell(void);

    void ToggleDisplayMode(void);
    void RequestQuit(void);

public:
    HWND m_hMainWindow;
    int m_fQuitRequested;
    int m_fWideDisplay;
};

struct ShellUiMenuCommandMessage {
    short m_wMessage;
    short m_wReserved0;
    short m_wReserved1;
    short m_wReserved2;
    int m_nCommand;
};

void InitializePrimaryContextShell(ShellUiPrimaryContextShell *pShell);
char *LEMBALL_STDCALL BuildSystemInformationReportString(void);
int LEMBALL_FASTCALL HandlePrimaryContextMenuCommand(
    ShellUiPrimaryContextShell *pShell, int nUnused,
    const ShellUiMenuCommandMessage *pMessage);
int LEMBALL_FASTCALL HandlePrimaryContextMenuCommandThunk(
    ShellUiPrimaryContextShell *pShell, int nUnused,
    const ShellUiMenuCommandMessage *pMessage);
int CALLBACK AboutBoxProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam);

#endif
