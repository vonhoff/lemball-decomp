#ifndef LEMBALL_PLATFORM_WIN32_H
#define LEMBALL_PLATFORM_WIN32_H

#include "CORE/COMMON.H"

typedef void *HANDLE;
typedef void *HINSTANCE;
typedef void *HMODULE;
typedef void *HWND;
typedef void *HACCEL;
typedef void *HICON;
typedef void *HCURSOR;
typedef void *HBRUSH;
typedef void *HGDIOBJ;
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
typedef unsigned int MCIDEVICEID;
typedef unsigned int MCIERROR;
typedef unsigned int MMRESULT;
typedef void *LPVOID;
typedef void *HWAVEOUT;
typedef void *LPWAVEHDR;
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
    GWL_USERDATA = -21,
    WM_CREATE = 0x0001u,
    WM_DESTROY = 0x0002u,
    WM_INITDIALOG = 0x0110u,
    WM_COMMAND = 0x0111u,
    WM_QUIT = 0x0012u,
    WAIT_OBJECT_0 = 0x00000000u,
    INFINITE = 0xffffffffu,
    DRIVE_CDROM = 5u,
    KEY_ALL_ACCESS = 0x000f003fu,
    VK_LSHIFT = 0xa0u,
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

struct tagCREATESTRUCTA {
    LPVOID lpCreateParams;
    HINSTANCE hInstance;
    HMENU hMenu;
    HWND hwndParent;
    INT cy;
    INT cx;
    INT y;
    INT x;
    LONG style;
    LPCSTR lpszName;
    LPCSTR lpszClass;
    DWORD dwExStyle;
};

typedef tagCREATESTRUCTA CREATESTRUCTA;

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
extern "C" {
DWORD WINAPI GetLastError(void);
LSTATUS WINAPI RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, HKEY *phkResult);
LSTATUS WINAPI
RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD cbData);
LSTATUS WINAPI
RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, BYTE *lpData, LPDWORD lpcbData);
LSTATUS WINAPI RegCloseKey(HKEY hKey);
DWORD WINAPI GetLogicalDrives(void);
UINT WINAPI GetDriveTypeA(LPCSTR lpRootPathName);
UINT WINAPI GetModuleFileNameA(HINSTANCE hModule, LPSTR lpFilename, UINT nSize);
DWORD WINAPI GetCurrentDirectoryA(DWORD nBufferLength, LPSTR lpBuffer);
DWORD WINAPI GetVersion(void);
int WINAPI wsprintfA(LPSTR lpOut, LPCSTR lpFmt, ...);
__declspec(dllimport) HICON WINAPI LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName);
__declspec(dllimport) HCURSOR WINAPI LoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName);
__declspec(dllimport) HGLOBAL WINAPI GlobalAlloc(UINT uFlags, unsigned int dwBytes);
__declspec(dllimport) LPVOID WINAPI GlobalLock(HGLOBAL hMem);
__declspec(dllimport) BOOL WINAPI GlobalUnlock(HGLOBAL hMem);
__declspec(dllimport) HGLOBAL WINAPI GlobalFree(HGLOBAL hMem);
__declspec(dllimport) HGDIOBJ WINAPI GetStockObject(int fnObject);
__declspec(dllimport) HCURSOR WINAPI SetCursor(HCURSOR hCursor);
__declspec(dllimport) int WINAPI ShowCursor(BOOL bShow);
__declspec(dllimport) HMODULE WINAPI LoadLibraryA(LPCSTR lpLibFileName);
BOOL WINAPI FreeLibrary(HMODULE hLibModule);
__declspec(dllimport) FARPROC WINAPI GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
int WINAPI GetSystemMetrics(int nIndex);
void WINAPI InitializeCriticalSection(LPVOID lpCriticalSection);
void WINAPI DeleteCriticalSection(LPVOID lpCriticalSection);
void WINAPI EnterCriticalSection(LPVOID lpCriticalSection);
void WINAPI LeaveCriticalSection(LPVOID lpCriticalSection);
BOOL WINAPI WinHelpA(HWND hWndMain, LPCSTR lpszHelp, UINT uCommand, DWORD_PTR dwData);
int WINAPI
DialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
__declspec(dllimport) ATOM WINAPI RegisterClassA(const WNDCLASSA *lpWndClass);
LONG WINAPI GetWindowLongA(HWND hWnd, int nIndex);
LONG WINAPI SetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong);
LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
__declspec(dllimport) LRESULT WINAPI SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
__declspec(dllimport) BOOL WINAPI PostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI InvalidateRect(HWND hWnd, const void *lpRect, BOOL bErase);
BOOL WINAPI ClipCursor(const void *lpRect);
BOOL WINAPI ReleaseCapture(void);
HWND WINAPI SetCapture(HWND hWnd);
BOOL WINAPI SystemParametersInfoA(UINT uiAction, UINT uiParam, void *pvParam, UINT fWinIni);
DWORD WINAPI GetMessageTime(void);
__declspec(dllimport) HWND WINAPI CreateWindowExA(DWORD dwExStyle,
                            LPCSTR lpClassName,
                            LPCSTR lpWindowName,
                            DWORD dwStyle,
                            INT X,
                            INT Y,
                            INT nWidth,
                            INT nHeight,
                            HWND hWndParent,
                            HMENU hMenu,
                            HINSTANCE hInstance,
                            LPVOID lpParam);
__declspec(dllimport) int WINAPI ShowWindow(HWND hWnd, int nCmdShow);
__declspec(dllimport) BOOL WINAPI UpdateWindow(HWND hWnd);
__declspec(dllimport) BOOL WINAPI SetForegroundWindow(HWND hWnd);
BOOL WINAPI PeekMessageA(MSG *lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL WINAPI GetMessageA(MSG *lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI TranslateMessage(const MSG *lpMsg);
LRESULT WINAPI DispatchMessageA(const MSG *lpMsg);
BOOL WINAPI DestroyWindow(HWND hWnd);
BOOL WINAPI ClientToScreen(HWND hWnd, tagPOINT *lpPoint);
__declspec(dllimport) BOOL WINAPI GetCursorPos(tagPOINT *lpPoint);
__declspec(dllimport) short WINAPI GetKeyState(int nVirtKey);
HANDLE WINAPI CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes,
                           BOOL bManualReset,
                           BOOL bInitialState,
                           LPCSTR lpName);
BOOL WINAPI SetEvent(HANDLE hEvent);
HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
                           DWORD dwStackSize,
                           LPTHREAD_START_ROUTINE lpStartAddress,
                           LPVOID lpParameter,
                           DWORD dwCreationFlags,
                           LPDWORD lpThreadId);
BOOL WINAPI SetThreadPriority(HANDLE hThread, int nPriority);
DWORD WINAPI WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
BOOL WINAPI TerminateThread(HANDLE hThread, DWORD dwExitCode);
void WINAPI ExitProcess(UINT uExitCode);
int WINAPI TranslateAcceleratorA(HWND hWnd, HACCEL hAccTable, MSG *lpMsg);
MCIERROR WINAPI mciSendCommandA(MCIDEVICEID mciId, UINT uMsg, DWORD_PTR fdwCommand, DWORD_PTR dwParam);
BOOL WINAPI mciGetErrorStringA(MCIERROR mcierr, LPSTR pszText, UINT cchText);
MMRESULT WINAPI waveOutReset(HWAVEOUT hWaveOut);
MMRESULT WINAPI waveOutClose(HWAVEOUT hWaveOut);
MMRESULT WINAPI waveOutWrite(HWAVEOUT hWaveOut, LPWAVEHDR pWaveOutHdr, UINT cbWaveOutHdr);
MMRESULT WINAPI waveOutOpen(HWAVEOUT *phwo,
                            UINT uDeviceId,
                            const void *pWaveFormat,
                            DWORD_PTR dwCallback,
                            DWORD_PTR dwInstance,
                            DWORD fdwOpen);
MMRESULT WINAPI waveOutGetErrorTextA(MMRESULT mmrError, LPSTR pszText, UINT cchText);
MMRESULT WINAPI waveOutGetVolume(HWAVEOUT hWaveOut, LPDWORD pdwVolume);
MMRESULT WINAPI waveOutSetVolume(HWAVEOUT hWaveOut, DWORD dwVolume);
UINT WINAPI waveOutGetNumDevs(void);
MMRESULT WINAPI waveOutGetDevCapsA(UINT uDeviceID, void *pWaveOutCaps, UINT cbWaveOutCaps);
}

#endif
