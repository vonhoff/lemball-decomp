#include "CIntroAnimDrawer.h"

#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CString.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Windows/CIntroAnimAnimWindow.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"

// GLOBAL: LEMBALL 0x0049f19c
char g_szPaintBallIntroSequence[] = "PaintBall Intro Sequence";

extern char g_szMoviePrefix[];

// FUNCTION: LEMBALL 0x00447410
CIntroAnimDrawer::CIntroAnimDrawer(CMain2DDisplay* p_arg0, CGdi* p_arg1, const CVsRect& p_arg2, unsigned int p_arg3)
	: CBaseFrontendDrawer(p_arg0, p_arg1, p_arg2, FLOW_INTRO_ANIM, 0, 0, 0, 0, 0)
{
	m_variant = p_arg3;
	m_nextFlow = 0xe;
	if (p_arg3 == 0) {
		m_nextFlow = 2;
	}
	m_started = 0;
	m_startCountdown = 10;
	m_display->Clear(-1);
	m_animWindow.Initialise(this, m_display, m_variant);
	m_animWindow.m_resolveMoviePath = 1;
	m_animWindow.m_moviePrefix = g_szMoviePrefix;
	m_animWindow.m_useMoviePrefix = 1;
	m_drawBackground = 0;
	m_drawFrame = 0;
	m_drawSolid = 0;
	Setup();
}

// FUNCTION: LEMBALL 0x00447530
void CIntroAnimDrawer::Load()
{
	m_animWindow.SetAnim();
}

// FUNCTION: LEMBALL 0x00447540
void CIntroAnimDrawer::UnLoad()
{
}

// FUNCTION: LEMBALL 0x00447550
CIntroAnimDrawer::~CIntroAnimDrawer()
{
	if (m_loaded != 0) {
		UnLoad();
	}
	m_display->Clear(-1);
}

// FUNCTION: LEMBALL 0x004475a0
void CIntroAnimDrawer::DestroyDrawer()
{
	if (m_started != 0 && m_animWindow.m_lifecycleRefs == 1) {
		m_animWindow.Destroy();
		m_startCountdown = 10;
		m_started = 0;
	}
}

// FUNCTION: LEMBALL 0x004475e0
void CIntroAnimDrawer::EndPhase()
{
	m_quitYet = 1;
	m_returnState = m_nextFlow;
	if (m_animWindow.m_lifecycleRefs == 1) {
		m_animWindow.Destroy();
	}
}

// FUNCTION: LEMBALL 0x00447610
bool CIntroAnimDrawer::ProcessMessages(Message* p_message)
{
	switch ((unsigned int) p_message->type) {
	case 4:
		switch (p_message->code) {
		case 0x1f:
		case 0x22:
		case 0x23:
		case 0x4c:
			EndPhase();
			return 1;
		default:
			return 0;
		}
	case 6:
		EndPhase();
		return 1;
	default:
		m_processedCount++;
		return 0;
	}
}

// FUNCTION: LEMBALL 0x004476b0
void CIntroAnimDrawer::Processing()
{
	if (m_startCountdown > 0) {
		m_startCountdown--;
	}
	if (m_display->IsWindowValid() == 0) {
		EndPhase();
		return;
	}
	if (m_startCountdown == 0) {
		CVsRect introRect;
		introRect.m_x = (short) (m_display->m_rect.m_width - 320) / 2;
		introRect.m_width = 320;
		introRect.m_height = 240;
		introRect.m_y = (short) (m_display->m_rect.m_height - 240) / 2;
		if (m_started == 0) {
			g_pSoundView->ChangeState(1, 0);
			m_animWindow.Create(introRect, m_display, g_szPaintBallIntroSequence);
			m_animWindow.Play();
			m_started = 1;
		}
		m_animWindow.Resume();
		m_startCountdown = -1;
	}
}
