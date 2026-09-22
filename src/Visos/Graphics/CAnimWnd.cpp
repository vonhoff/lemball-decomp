#include "CAnimWnd.h"

#include "../../Platform/Windows/Entry.h"
#include "../Foundation/CVsIOs.h"
#include "../Foundation/VsDebug.h"
#include "../Resources/CResMovie.h"
#include "../Target/CGraphicsDriver.h"
#include "../Target/CPlatformServices.h"
#include "../Target/WinGDrawState.h"
#include "CGWnd.h"

#define WIN32_LEAN_AND_MEAN
#include "Visos/Foundation/CString.h"
#include "Visos/Resources/CResString.h"

#include <windows.h>

extern "C" void* __cdecl MCIWndCreateA(void* p_hwndParent, void* p_instance, unsigned int p_style, const char* p_file);
extern "C" __declspec(dllimport) unsigned int __stdcall mciSendCommandA(unsigned int p_deviceId,
																		unsigned int p_message,
																		unsigned int p_flags,
																		void* p_param);

// GLOBAL: LEMBALL 0x004a20c0
CAnimWnd* g_pAnimWnd = 0;

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

// FUNCTION: LEMBALL 0x00447960
void CAnimWnd::OnSkip(int p_position)
{
}

// FUNCTION: LEMBALL 0x00447970
void CAnimWnd::OnFrame(int p_frame)
{
}

// FUNCTION: LEMBALL 0x00447980
void CAnimWnd::OnStart()
{
}

// FUNCTION: LEMBALL 0x0046dd60
void CAnimWnd::Initialise()
{
	m_animSet = 0;
	m_playing = 0;
	m_paused = 0;
	m_animResourceId = 0;
	m_movieWindow = 0;
	if (g_pAnimWnd != 0) {
		MessageBoxA(0, g_szUnableToSupportMoreThanOneAnimWindow, g_szAnimWndError, 0x1000);
		InternalVsExit(0xaaaa);
	}
	g_pAnimWnd = this;
}

// FUNCTION: LEMBALL 0x0046ddc0
CAnimWnd::CAnimWnd()
{
	Initialise();
}

// FUNCTION: LEMBALL 0x0046de70
CAnimWnd::~CAnimWnd()
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

// FUNCTION: LEMBALL 0x0046ded0
void CAnimWnd::InternalOnCreate()
{
	CGWnd::InternalOnCreate();
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x10, 0, 0);
		m_movieWindow = 0;
	}
	m_movieWindow = MCIWndCreateA((HWND) m_nativeWindow, (HINSTANCE) g_pApplicationInstance, 0x50001f0a, 0);
	SendMessageA((HWND) m_movieWindow, 0x499, 0, (LPARAM) m_moviePath.m_text);
}

// FUNCTION: LEMBALL 0x0046df40
void CAnimWnd::InternalOnDestroy()
{
	Stop();
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x10, 0, 0);
		m_movieWindow = 0;
	}
	CGWnd::InternalOnDestroy();
	m_paused = 0;
	m_playing = 0;
}

// FUNCTION: LEMBALL 0x0046df80
void CAnimWnd::OnNotifyMode(int p_mode)
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

// FUNCTION: LEMBALL 0x0046dfe0
void CAnimWnd::SetMovieWindow(unsigned int p_lParam)
{
	unsigned int mciId;
	unsigned int error;
	unsigned int params[7];

	(void) p_lParam;
	params[1] = 0x8000;
	params[2] = (unsigned int) WinGDrawDriverProc;
	mciId = (unsigned int) SendMessageA((HWND) m_movieWindow, 0x464, 0, 0);
	if (mciId != 0) {
		error = mciSendCommandA(mciId, 0x876, 0x1100000, params);
		if (error != 0) {
			MessageBoxA(0, g_szUnableToSetMciDrawProcedure, g_szMciError, 0x10);
		}
	}
}

// FUNCTION: LEMBALL 0x0046e050
void CAnimWnd::OnNotifyError(int p_error)
{
}

// FUNCTION: LEMBALL 0x0046e060
void CAnimWnd::OnNotifyPos(int p_position, int p_flags)
{
}

// FUNCTION: LEMBALL 0x0046e070
void CAnimWnd::OnNotifySize(int p_width, int p_height)
{
}

// FUNCTION: LEMBALL 0x0046e080
int CAnimWnd::ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
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
		SetMovieWindow(p_lParam);
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

// FUNCTION: LEMBALL 0x0046e120
void CAnimWnd::Refresh(CVsRect* p_rect)
{
}

// FUNCTION: LEMBALL 0x0046e130
void CAnimWnd::SetAnim(unsigned int p_resourceId)
{
	CResMovie* movie;
	char* fileName;
	char* cdDir;

	movie = 0;
	fileName = "test";
	if (p_resourceId != 0) {
		m_animResourceId = p_resourceId;
		movie = CResMovie::Load(p_resourceId);
		if (movie->m_loaded != 0) {
			movie->m_age = 0;
		}
		else {
			movie->LoadData();
		}
		movie->m_directUseCount = movie->m_directUseCount + 1;
		fileName = (char*) movie->m_movieEntries->m_data;
	}
	CString relative;
	if (m_useMoviePrefix != 0) {
		relative = m_moviePrefix;
		if (relative.m_text[relative.Getlength() - 1] != '\\') {
			relative += g_szPathSeparator;
		}
	}
	relative += fileName;
	relative += g_szAviSuffix;
	if (m_resolveMoviePath == 0) {
		m_moviePath = g_szCurrentDirectory;
		if (m_moviePath.m_text[m_moviePath.Getlength() - 1] != '\\') {
			m_moviePath += "\\";
		}
	}
	else {
		cdDir = g_pTargetPlatformServices->GetCdDir(relative.m_text);
		if (cdDir != 0) {
			m_moviePath = cdDir;
		}
		else {
			m_moviePath = g_szCurrentDirectory;
		}
		if (m_moviePath.m_text[m_moviePath.Getlength() - 1] != '\\') {
			m_moviePath += "\\";
		}
	}
	m_moviePath += relative;
	if (p_resourceId != 0) {
		movie->m_directUseCount = movie->m_directUseCount - 1;
		movie->UnLoad();
	}
	if (m_movieWindow != 0) {
		SendMessageA((HWND) m_movieWindow, 0x499, 0, (LPARAM) m_moviePath.m_text);
	}
	m_animSet = 1;
}

// FUNCTION: LEMBALL 0x0046e300
void CAnimWnd::Play()
{
	if (m_playing == 0) {
		SendMessageA((HWND) m_movieWindow, 0x807, 0, (LPARAM) -1);
		SendMessageA((HWND) m_movieWindow, 0x806, 0, 0);
		m_playing = 1;
	}
}

// FUNCTION: LEMBALL 0x0046e390
void CAnimWnd::Stop()
{
	if (m_paused == 0) {
		SendMessageA((HWND) m_movieWindow, 0x809, 0, 0);
		m_paused = 1;
	}
}

// FUNCTION: LEMBALL 0x0046e3c0
void CAnimWnd::Resume()
{
	if (m_paused != 0) {
		SendMessageA((HWND) m_movieWindow, 0x855, 0, 0);
		m_paused = 0;
	}
}

// FUNCTION: LEMBALL 0x0046e400
void CAnimWnd::OnStop()
{
}
