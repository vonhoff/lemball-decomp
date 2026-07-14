#ifndef LEMBALL_MAIN_H
#define LEMBALL_MAIN_H

#include "VISOS/WIN32.H"

extern HINSTANCE g_hApplicationInstance;
extern HICON g_hApplicationIcon;

#ifdef __cplusplus
extern "C" {
#endif
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#ifdef __cplusplus
}
#endif

#endif
