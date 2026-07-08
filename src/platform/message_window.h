#ifndef LEMBALL_MESSAGE_WINDOW_H
#define LEMBALL_MESSAGE_WINDOW_H

#include "../platform/win32.h"

class PLATFORM_InvisibleMessageWindow {
public:
    int Construct(LPCSTR pszClassName, int *pfClassRegistered);

public:
    void *m_pVTable;
    HWND m_hWnd;
    DWORD m_dwReserved0;
    DWORD m_dwReserved1;
};

class PLATFORM_DebugTextWindow {
public:
    PLATFORM_DebugTextWindow *Construct(const char *pszTitle, unsigned int cchBuffer);

public:
    void *m_pVTable;
    const char *m_pszTitle;
    unsigned int m_cchBuffer;
    char m_achReserved[0x4c];
};

void DestroyInvisibleMessageWindow(PLATFORM_InvisibleMessageWindow *pWindow);
int DebugMessageThreadMain(void);
PLATFORM_DebugTextWindow *GetDebugTextWindow(void);

#endif
