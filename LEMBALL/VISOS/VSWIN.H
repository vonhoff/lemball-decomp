#ifndef LEMBALL_VSWIN_H
#define LEMBALL_VSWIN_H

#include "WIN32.H"

class VSWIN_InvisibleMessageWindow {
public:
    VSWIN_InvisibleMessageWindow(void);

    int Construct(LPCSTR pszClassName, int *pfClassRegistered);
    HWND GetHwnd(void) const;

private:
    HWND m_hWnd;
    DWORD m_dwReserved0;
    DWORD m_dwReserved1;
};

class VSWIN_DebugTextWindow {
public:
    VSWIN_DebugTextWindow(void);

    VSWIN_DebugTextWindow *Construct(const char *pszTitle, unsigned int cchBuffer);
    const char *GetTitle(void) const;
    unsigned int GetBufferSize(void) const;

private:
    const char *m_pszTitle;
    unsigned int m_cchBuffer;
    char m_achReserved[0x50];
};

int ConstructInvisibleMessageWindow(VSWIN_InvisibleMessageWindow *pWindow, LPCSTR pszClassName, int *pfClassRegistered);
int DebugMessageThreadMain(void);
VSWIN_DebugTextWindow *GetDebugTextWindow(void);

#endif
