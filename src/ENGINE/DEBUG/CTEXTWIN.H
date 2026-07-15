#ifndef VISOS_CTEXTWINDOW_H
#define VISOS_CTEXTWINDOW_H

#include "ENGINE/CORE/WIN32.H"

// PRESERVED: class string at 0x004A2B70.
// SIZE 0x58
class CTextWindow {
public:
    CTextWindow *Construct(const char *pszTitle, unsigned int cchBuffer);

    // STRONG INFERENCE: preserved diagnostics identify these methods at the
    // listed executable addresses; implementations are not reconstructed yet.
    // 0x00473F80
    void RedrawLines(int nFirstLine, int nLastLine);
    // 0x00474520
    char *GetSelectionAsString(void);

public:
    void *m_pVTable;              // 0x00
    const char *m_pszTitle;       // 0x04
    unsigned int m_cchBuffer;     // 0x08
    char m_achReserved[0x4c];     // 0x0c
};

int __stdcall DebugMessageThreadMain(void);
CTextWindow *GetDebugTextWindow(void);

#endif
