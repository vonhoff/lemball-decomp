#ifndef LEMBALL_WIN32_H
#define LEMBALL_WIN32_H

#include "COMMON.H"

typedef void *HANDLE;
typedef void *HINSTANCE;
typedef void *HMODULE;
typedef void *HWND;
typedef void *HACCEL;
typedef void *HICON;
typedef void *HCURSOR;
typedef void *HBRUSH;
typedef void *HMENU;
typedef void *HKEY;
typedef const char *LPCSTR;
typedef char *LPSTR;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned long WPARAM;
typedef long LPARAM;
typedef long LRESULT;
typedef unsigned short ATOM;
typedef int LSTATUS;
typedef int BOOL;
typedef int INT;
typedef void *FARPROC;
typedef DWORD *LPDWORD;

#ifndef PASCAL
#define PASCAL
#endif

#ifndef WINAPI
#define WINAPI
#endif

#ifndef CALLBACK
#define CALLBACK WINAPI
#endif

typedef int (CALLBACK *DLGPROC)(HWND, UINT, WPARAM, LPARAM);

enum {
    MB_OK = 0x00000000u,
    MB_ICONERROR = 0x00000010u,
    CS_DBLCLKS = 0x0008u,
    CW_USEDEFAULT = 0x80000000u,
    WM_INITDIALOG = 0x0110u,
    WM_COMMAND = 0x0111u,
    WM_QUIT = 0x0012u,
    WAIT_OBJECT_0 = 0x00000000u,
    INFINITE = 0xffffffffu,
};

struct tagPOINT {
    LONG x;
    LONG y;
};

struct tagMSG {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    tagPOINT pt;
};

typedef tagMSG MSG;

typedef LRESULT (WINAPI *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

struct tagWNDCLASSA {
    UINT style;
    WNDPROC lpfnWndProc;
    INT cbClsExtra;
    INT cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCSTR lpszMenuName;
    LPCSTR lpszClassName;
};

typedef tagWNDCLASSA WNDCLASSA;

struct tagMEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORD dwTotalPhys;
    DWORD dwAvailPhys;
    DWORD dwTotalPageFile;
    DWORD dwAvailPageFile;
    DWORD dwTotalVirtual;
    DWORD dwAvailVirtual;
};

typedef tagMEMORYSTATUS MEMORYSTATUS;

typedef struct LemballImportMap {
    const char *library;
    const char *name;
    u32 iat_rva;
} LemballImportMap;

BOOL lemball_imports_ready(void);
void lemball_imports_reset(void);
const LemballImportMap *lemball_get_import_map(size_t *count);
int lemball_platform_show_error(const char *title, const char *message);

#endif
