#include "AnimWnd.h"

#include "../../Platform/Windows/Entry.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsIOs.h"
#include "../Resources/ResMovie.h"
#include "../Target/TargetGraphicsDriver.h"
#include "../Target/TargetPlatformServices.h"
#include "../Target/TargetWinGDrawCodecState.h"
#include "GWnd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllimport) void* __cdecl MCIWndCreateA(void* p_hwndParent, void* p_instance, unsigned int p_style, const char* p_file);
extern "C" __declspec(dllimport) unsigned int __stdcall mciSendCommandA(unsigned int p_deviceId,
																		unsigned int p_message,
																		unsigned int p_flags,
																		void* p_param);

// GLOBAL: LEMBALL 0x004a20c0
AnimWnd* g_pAnimWnd = 0;

// GLOBAL: LEMBALL 0x004a2100
char g_szAnimWndError[] = "!!ERROR!!";

// GLOBAL: LEMBALL 0x004a210c
char g_szUnableToSupportMoreThanOneAnimWindow[] = "Unable to support more than one Anim Window";

// GLOBAL: LEMBALL 0x004a2138
char g_szMciError[] = "MCI ERROR";

// GLOBAL: LEMBALL 0x004a2144
char g_szUnableToSetMciDrawProcedure[] = "Unable to set MCI Draw Procedure";

// GLOBAL: LEMBALL 0x004a2168
char g_szPathSeparator[] = "\\";

// GLOBAL: LEMBALL 0x004a216c
char g_szAviSuffix[] = ".avi";

// 68K 0x10104782 OnSkip__8CAnimWndFi
// FUNCTION: LEMBALL 0x00447960
void AnimWnd::OnSkip(int p_position)
{
}

// 68K 0x101047a0 OnFrame__8CAnimWndFi
// FUNCTION: LEMBALL 0x00447970
void AnimWnd::OnFrame(int p_frame)
{
}

// 68K 0x101047c0 OnStart__8CAnimWndFv
// FUNCTION: LEMBALL 0x00447980
void AnimWnd::OnStart()
{
}

// 68K 0x10103c04 Initialise__8CAnimWndFv
// STUB: LEMBALL 0x0046dd60
void AnimWnd::Initialise()
{
	m_animSet = 0;
	m_playing = 0;
	m_paused = 0;
	m_animResourceId = 0;
	m_movieWindow = 0;
	if (g_pAnimWnd != 0) {
		MessageBoxA(0, g_szUnableToSupportMoreThanOneAnimWindow, g_szAnimWndError, 0x1000);
		VsExit(0xaaaa);
	}
	g_pAnimWnd = this;
}

// 68K 0x10103c50 __ct__8CAnimWndFv
// STUB: LEMBALL 0x0046ddc0
AnimWnd::AnimWnd()
{
	m_resolveMoviePath = 0;
	m_useMoviePrefix = 0;
	Initialise();
}

// 68K 0x10103cd2 __dt__8CAnimWndFv
// FUNCTION: LEMBALL 0x0046de70
AnimWnd::~AnimWnd()
{
	if (m_lifecycleRefs == 1) {
		Destroy();
	}
	g_pAnimWnd = 0;
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x10, 0, 0);
		m_movieWindow = 0;
	}
}

// 68K 0x10103df8 _OnCreate__8CAnimWndFv
// STUB: LEMBALL 0x0046ded0
void AnimWnd::OnCreate()
{
	GWnd::OnCreate();
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x10, 0, 0);
		m_movieWindow = 0;
	}
	m_movieWindow = MCIWndCreateA((HWND) m_nativeWindow, (HINSTANCE) g_pApplicationInstance, 0x50001f0a, 0);
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x499, 0, (LPARAM) m_moviePath.m_text);
	}
}

// 68K 0x101040d4 _OnDestroy__8CAnimWndFv
// FUNCTION: LEMBALL 0x0046df40
void AnimWnd::OnDestroy()
{
	Stop();
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x10, 0, 0);
		m_movieWindow = 0;
	}
	GWnd::OnDestroy();
	m_paused = 0;
	m_playing = 0;
}

// FUNCTION: LEMBALL 0x0046df80
void AnimWnd::OnNotifyMode(int p_mode)
{
	switch (p_mode) {
	case 0x20d:
		m_playing = 0;
		m_paused = 0;
		OnStop();
		break;
	case 0x20e:
		OnStart();
		break;
	case 0x20f:
	case 0x210:
	case 0x211:
	case 0x212:
		break;
	}
}

// 68K 0x10104516 SetMovieWindow__8CAnimWndFv
// STUB: LEMBALL 0x0046dfe0
void AnimWnd::SetMovieWindow()
{
	unsigned int mciId;
	unsigned int error;
	unsigned int params[7];

	params[0] = 0;
	params[1] = 0x8000;
	params[2] = (unsigned int) TargetWinGDrawCodec_DriverProc;
	params[3] = 0;
	params[4] = 0;
	params[5] = 0;
	params[6] = 0;
	mciId = (unsigned int) SendMessageA((HWND) m_movieWindow, 0x464, 0, 0);
	if (mciId != 0) {
		error = mciSendCommandA(mciId, 0x876, 0x1100000, params);
		if (error != 0) {
			MessageBoxA(0, g_szUnableToSetMciDrawProcedure, g_szMciError, 0x10);
		}
	}
}

// FUNCTION: LEMBALL 0x0046e050
void AnimWnd::OnNotifyError(int p_error)
{
}

// FUNCTION: LEMBALL 0x0046e060
void AnimWnd::OnNotifyPos(int p_position, int p_flags)
{
}

// FUNCTION: LEMBALL 0x0046e070
void AnimWnd::OnNotifySize(int p_width, int p_height)
{
}

// FUNCTION: LEMBALL 0x0046e080
int AnimWnd::ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	switch (p_message) {
	case 0x4c8:
		OnNotifyMode((int) p_lParam);
		return 0;
	case 0x4c9:
		OnNotifyPos(0, 0);
		return 0;
	case 0x4ca:
		OnNotifySize(0, 0);
		return 0;
	case 0x4cb:
		SetMovieWindow();
		return 0;
	case 0x4cc:
		break;
	case 0x4cd:
		OnNotifyError((int) p_lParam);
		return 0;
	}
	if (m_nativeWindow != 0) {
		return DefWindowProcA((HWND) m_nativeWindow, p_message, p_wParam, p_lParam);
	}
	return 0;
}

// 68K 0x1010412e Refresh__8CAnimWndFv
// FUNCTION: LEMBALL 0x0046e120
void AnimWnd::Refresh(VsRect* p_rect)
{
}

// 68K 0x1010414e SetAnim__8CAnimWndFUl
// STUB: LEMBALL 0x0046e130
void AnimWnd::SetAnim(unsigned int p_resourceId)
{
	ResMovie* movie;
	char* fileName;
	char* cdDir;
	String relative;

	movie = 0;
	fileName = "";
	if (p_resourceId != 0) {
		m_animResourceId = p_resourceId;
		movie = ResMovie::Load(p_resourceId);
		if (movie != 0) {
			if (movie->m_loaded == 0) {
				movie->LoadData();
			}
			else {
				movie->m_age = 0;
			}
			movie->m_directUseCount = movie->m_directUseCount + 1;
			if (movie->m_movieEntries != 0) {
				fileName = (char*) movie->m_movieEntries->m_data;
				if (fileName == 0) {
					fileName = "";
				}
			}
		}
	}
	relative = "";
	if (m_useMoviePrefix != 0) {
		relative = m_moviePrefix;
		if (relative.Getlength() > 0 && relative.m_text[relative.Getlength() - 1] != '\\') {
			relative += g_szPathSeparator;
		}
	}
	relative += fileName;
	relative += g_szAviSuffix;
	if (m_resolveMoviePath == 0) {
		m_moviePath = g_szCurrentDirectory;
		if (m_moviePath.Getlength() > 0 && m_moviePath.m_text[m_moviePath.Getlength() - 1] != '\\') {
			m_moviePath += g_szPathSeparator;
		}
	}
	else {
		cdDir = 0;
		if (g_pTargetPlatformServices != 0) {
			cdDir = g_pTargetPlatformServices->GetCDDir(relative.m_text);
		}
		if (cdDir == 0) {
			cdDir = g_szCurrentDirectory;
		}
		m_moviePath = cdDir;
		if (m_moviePath.Getlength() > 0 && m_moviePath.m_text[m_moviePath.Getlength() - 1] != '\\') {
			m_moviePath += g_szPathSeparator;
		}
	}
	m_moviePath += relative;
	if (movie != 0) {
		movie->m_directUseCount = movie->m_directUseCount - 1;
		movie->UnLoad();
	}
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x499, 0, (LPARAM) m_moviePath.m_text);
	}
	m_animSet = 1;
}

// 68K 0x1010469c Play__8CAnimWndFv
// STUB: LEMBALL 0x0046e300
void AnimWnd::Play()
{
	if (m_playing == 0 && m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x807, 0, (LPARAM) -1);
		SendMessageA((HWND) m_movieWindow, 0x806, 0, 0);
		m_playing = 1;
	}
}

// 68K 0x101046d8 Stop__8CAnimWndFv
// STUB: LEMBALL 0x0046e390
void AnimWnd::Stop()
{
	if (m_paused == 0) {
		SendMessageA((HWND) m_movieWindow, 0x809, 0, 0);
		m_paused = 1;
	}
}

// 68K 0x10104716 Resume__8CAnimWndFv
// STUB: LEMBALL 0x0046e3c0
void AnimWnd::Resume()
{
	if (m_paused != 0 && m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x855, 0, 0);
		m_paused = 0;
	}
}

// 68K 0x1010467e OnStop__8CAnimWndFv
// FUNCTION: LEMBALL 0x0046e400
void AnimWnd::OnStop()
{
}
