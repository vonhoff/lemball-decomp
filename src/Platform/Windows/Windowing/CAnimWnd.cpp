#include "CAnimWnd.h"

#include "CGWnd.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Resources/Types/CResMOVIE.h"
#include "Platform/Windows/CPlatformServices.h"
#include "Platform/Windows/Entry.h"
#include "Platform/Windows/Graphics/WinGDraw.h"

#define WIN32_LEAN_AND_MEAN
// clang-format off
#include <windows.h>
#include <mmsystem.h>
#include <stddef.h>

#include "Engine/Strings/CString.h"
#include "Engine/Resources/Types/CResSTRING.h"

#define NOAVIFILE
#include <vfw.h>
#include <digitalv.h>
#include <mciavi.h>
// clang-format on

// GLOBAL: LEMBALL 0x004a20c0
CAnimWnd* g_pAnimWnd = NULL;

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
	m_movieWindow = NULL;
	if (g_pAnimWnd != NULL) {
		MessageBoxA(NULL, g_szUnableToSupportMoreThanOneAnimWindow, g_szAnimWndError, MB_SYSTEMMODAL);
		_VSExit(0xaaaa);
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
	g_pAnimWnd = NULL;
	if (m_movieWindow != NULL) {
		SendMessageA((HWND) m_movieWindow, WM_CLOSE, 0, 0);
		m_movieWindow = NULL;
	}
}

// FUNCTION: LEMBALL 0x0046ded0
void CAnimWnd::_OnCreate()
{
	CGWnd::_OnCreate();
	if (m_movieWindow != NULL) {
		SendMessageA((HWND) m_movieWindow, WM_CLOSE, 0, 0);
		m_movieWindow = NULL;
	}
	m_movieWindow = MCIWndCreateA((HWND) m_nativeWindow,
								  (HINSTANCE) g_pApplicationInstance,
								  WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL,
								  NULL);
	SendMessageA((HWND) m_movieWindow, MCIWNDM_OPENA, 0, (LPARAM) m_moviePath.m_text);
}

// FUNCTION: LEMBALL 0x0046df40
void CAnimWnd::_OnDestroy()
{
	Stop();
	if (m_movieWindow != NULL) {
		SendMessageA((HWND) m_movieWindow, WM_CLOSE, 0, 0);
		m_movieWindow = NULL;
	}
	CGWnd::_OnDestroy();
	m_paused = 0;
	m_playing = 0;
}

// FUNCTION: LEMBALL 0x0046df80
void CAnimWnd::OnNotifyMode(int p_mode)
{
	switch (p_mode) {
	case MCI_MODE_STOP:
		m_playing = 0;
		m_paused = 0;
		OnStop();
		break;
	case MCI_MODE_PLAY:
		OnStart();
		break;
	case MCI_MODE_RECORD:
	case MCI_MODE_SEEK:
	case MCI_MODE_PAUSE:
	case MCI_MODE_OPEN:
		break;
	}
}

// FUNCTION: LEMBALL 0x0046dfe0
void CAnimWnd::SetMovieWindow(unsigned int p_lParam)
{
	unsigned int mciId;
	unsigned int error;
	MCI_DGV_SETVIDEO_PARMSA params;

	(void) p_lParam;
	params.dwValue = (DWORD) WinGDrawDriverProc;
	params.dwItem = MCI_AVI_SETVIDEO_DRAW_PROCEDURE;
	mciId = (unsigned int) SendMessageA((HWND) m_movieWindow, MCIWNDM_GETDEVICEID, 0, 0);
	if (mciId != 0) {
		error = mciSendCommandA(mciId, MCI_SETVIDEO, MCI_DGV_SETVIDEO_ITEM | MCI_DGV_SETVIDEO_VALUE, (DWORD) &params);
		if (error != 0) {
			MessageBoxA(NULL, g_szUnableToSetMciDrawProcedure, g_szMciError, MB_ICONHAND);
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
	case MCIWNDM_NOTIFYMODE:
		OnNotifyMode((int) p_lParam);
		return 0;
	case MCIWNDM_NOTIFYPOS:
		OnNotifyPos(0, 0);
		return 0;
	case MCIWNDM_NOTIFYSIZE:
		OnNotifySize(0, 0);
		return 0;
	case MCIWNDM_NOTIFYMEDIA:
		SetMovieWindow(p_lParam);
		return 0;
	case MCIWNDM_NOTIFYERROR:
		OnNotifyError((int) p_lParam);
		return 0;
	}
	if (m_nativeWindow != NULL) {
		return DefWindowProcA((HWND) m_nativeWindow, p_message, p_wParam, p_lParam);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046e120
void CAnimWnd::Refresh(CVSRect* p_rect)
{
}

// FUNCTION: LEMBALL 0x0046e130
void CAnimWnd::SetAnim(unsigned long p_resourceId)
{
	CResMOVIE* movie;
	char* fileName;
	char* cdDir;

	movie = NULL;
	fileName = "test";
	if (p_resourceId != 0) {
		m_animResourceId = p_resourceId;
		movie = CResMOVIE::Load(p_resourceId);
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
		if (relative.m_text[relative.getlength() - 1] != '\\') {
			relative += g_szPathSeparator;
		}
	}
	relative += fileName;
	relative += g_szAviSuffix;
	if (m_resolveMoviePath == 0) {
		m_moviePath = g_szCurrentDirectory;
		if (m_moviePath.m_text[m_moviePath.getlength() - 1] != '\\') {
			m_moviePath += "\\";
		}
	}
	else {
		cdDir = g_pTargetPlatformServices->GetCDDir(relative.m_text);
		if (cdDir != NULL) {
			m_moviePath = cdDir;
		}
		else {
			m_moviePath = g_szCurrentDirectory;
		}
		if (m_moviePath.m_text[m_moviePath.getlength() - 1] != '\\') {
			m_moviePath += "\\";
		}
	}
	m_moviePath += relative;
	if (p_resourceId != 0) {
		movie->m_directUseCount = movie->m_directUseCount - 1;
		movie->UnLoad();
	}
	if (m_movieWindow != NULL) {
		SendMessageA((HWND) m_movieWindow, MCIWNDM_OPENA, 0, (LPARAM) m_moviePath.m_text);
	}
	m_animSet = 1;
}

// FUNCTION: LEMBALL 0x0046e300
void CAnimWnd::Play()
{
	if (m_playing == 0) {
		SendMessageA((HWND) m_movieWindow, MCI_SEEK, 0, (LPARAM) -1);
		SendMessageA((HWND) m_movieWindow, MCI_PLAY, 0, 0);
		m_playing = 1;
	}
}

// FUNCTION: LEMBALL 0x0046e390
void CAnimWnd::Stop()
{
	if (m_paused == 0) {
		SendMessageA((HWND) m_movieWindow, MCI_PAUSE, 0, 0);
		m_paused = 1;
	}
}

// FUNCTION: LEMBALL 0x0046e3c0
void CAnimWnd::Resume()
{
	if (m_paused != 0) {
		SendMessageA((HWND) m_movieWindow, MCI_RESUME, 0, 0);
		m_paused = 0;
	}
}

// FUNCTION: LEMBALL 0x0046e400
void CAnimWnd::OnStop()
{
}
