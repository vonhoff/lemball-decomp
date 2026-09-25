#include "Frontend/Drawers/CSuccFailDrawer.h"

#include "Control/Game/CGameStatus.h"
#include "Control/Game/GameMain.h"
#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Frontend/Windows/CSuccFailAnimWnd.h"
#include "Visos/Foundation/VsTime.h"

extern char g_szSuccFailMoviePrefix[];

// FUNCTION: LEMBALL 0x00450020
CSuccFailDrawer::CSuccFailDrawer(CMain2DDisplay* p_display, CGDI* p_gdi, const CVsRect& p_rect, unsigned int p_success)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_SUCCESS, 0x32, 200, 0, 0x28, 0x30)
{
	m_success = p_success;
	m_animationsEnabled = (unsigned int) (g_nAnimationsDisabled == 0);
	m_animWindow.Initialise(this, m_display, p_success);
	m_animWindow.m_resolveMoviePath = 1;
	m_animWindow.m_moviePrefix = g_szSuccFailMoviePrefix;
	m_animWindow.m_useMoviePrefix = 1;
	m_animStarted = 0;
	m_soundStarted = 0;
	m_soundStopped = 0;
	m_animStartDeadline = CurrentMilliTimer() + 0x28;
	m_drawBackground = 1;
	m_drawFrame = 1;
	m_drawSolid = 1;
	m_password = g_pGameStatus->EncodePassword();
	Setup();
}
