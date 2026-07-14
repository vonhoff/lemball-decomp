#ifndef VISOS_INVISIBLE_WINDOW_H
#define VISOS_INVISIBLE_WINDOW_H

#include "VISOS/WIN32.H"

// STRONG INFERENCE: generic ViSOS hidden-window infrastructure.
// SIZE 0x10
class VS_InvisibleWindow {
public:
    int Construct(LPCSTR pszClassName, int *pfClassRegistered);

public:
    void *m_pVTable;    // 0x00
    HWND m_hWnd;        // 0x04
    DWORD m_dwReserved0; // 0x08
    DWORD m_dwReserved1; // 0x0c
};

void __fastcall DestroyInvisibleMessageWindow(VS_InvisibleWindow *pWindow);

#endif
