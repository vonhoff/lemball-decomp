#ifndef LEMBALL_WIN32_H
#define LEMBALL_WIN32_H

#include "common.h"

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
typedef unsigned long DWORD_PTR;
typedef long LONG;
typedef unsigned char BYTE;
typedef void *LPVOID;
typedef unsigned long WPARAM;
typedef long LPARAM;
typedef long LRESULT;
typedef unsigned short ATOM;
typedef int LSTATUS;
typedef int BOOL;
typedef int INT;
typedef unsigned long REGSAM;
typedef void *FARPROC;
typedef DWORD *LPDWORD;
typedef void *HGLOBAL;
typedef void *LPSECURITY_ATTRIBUTES;

#ifndef PASCAL
#if defined(_MSC_VER) || defined(__WATCOMC__)
#define PASCAL __stdcall
#else
#define PASCAL
#endif
#endif

#ifndef WINAPI
#if defined(_MSC_VER) || defined(__WATCOMC__)
#define WINAPI __stdcall
#else
#define WINAPI
#endif
#endif

#ifndef CALLBACK
#define CALLBACK WINAPI
#endif

typedef DWORD(WINAPI *LPTHREAD_START_ROUTINE)(LPVOID);
typedef int(CALLBACK *DLGPROC)(HWND, UINT, WPARAM, LPARAM);

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
    DRIVE_CDROM = 5u,
    KEY_ALL_ACCESS = 0x000f003fu,
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

typedef LRESULT(WINAPI *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

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

extern "C" {
LSTATUS WINAPI RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, HKEY *phkResult);
LSTATUS WINAPI
RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD cbData);
LSTATUS WINAPI
RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, BYTE *lpData, LPDWORD lpcbData);
LSTATUS WINAPI RegCloseKey(HKEY hKey);
DWORD WINAPI GetLogicalDrives(void);
UINT WINAPI GetDriveTypeA(LPCSTR lpRootPathName);
UINT WINAPI GetModuleFileNameA(HINSTANCE hModule, LPSTR lpFilename, UINT nSize);
HICON WINAPI LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName);
BOOL WINAPI WinHelpA(HWND hWndMain, LPCSTR lpszHelp, UINT uCommand, DWORD_PTR dwData);
int WINAPI
DialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
}

#endif
