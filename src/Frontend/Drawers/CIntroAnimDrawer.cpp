#include "CIntroAnimDrawer.h"

#include "../../Views/Display/CMain2DDisplay.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CString.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Frontend/Windows/CIntroAnimAnimWindow.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/Message.h"

#include <stddef.h>

enum {
	INTRO_ANIMATION_COUNTDOWN_COMPLETE = -1
};

// GLOBAL: LEMBALL 0x0049f19c
char g_szPaintBallIntroSequence[] = "PaintBall Intro Sequence";

extern char g_szMoviePrefix[];

// FUNCTION: LEMBALL 0x00447410
CIntroAnimDrawer::CIntroAnimDrawer(CMain2DDisplay* p_display,
								   CGDI* p_gdi,
								   const CVSRect& p_rect,
								   unsigned int p_completionSequence)
	: CBaseFrontendDrawer(p_display, p_gdi, p_rect, FLOW_INTRO_ANIM, 0, 0, 0, 0, 0)
{
	m_completionSequence = p_completionSequence;
	m_nextFlow = FLOW_SUCCESS;
	if (p_completionSequence == 0) {
		m_nextFlow = FLOW_MAIN_OPTIONS_1;
	}
	m_started = 0;
	m_startCountdown = 10;
	m_display->Clear(-1);
	m_animWindow.Initialise(this, m_display, m_completionSequence);
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
	switch ((unsigned int) p_message->m_type) {
	case MESSAGE_KEY_DOWN:
		switch (p_message->m_code) {
		case INPUT_KEY_SPACE:
		case INPUT_KEY_ACTIVATE:
		case INPUT_KEY_ESCAPE:
		case INPUT_KEY_RETURN:
			EndPhase();
			return true;
		default:
			return false;
		}
	case MESSAGE_MOUSE_BUTTON_DOWN:
		EndPhase();
		return true;
	default:
		m_processedCount++;
		return false;
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
		CVSSize displaySize(m_display->m_rect);
		CVSRect introRect(0, 0, displaySize.m_width, displaySize.m_height);
		introRect.m_x = (short) (introRect.m_width - 320) / 2;
		short height = introRect.m_height;
		introRect.m_width = 320;
		introRect.m_height = 240;
		introRect.m_y = (short) (height - 240) / 2;
		if (m_started == 0) {
			g_pSoundView->ChangeState(SOUND_STATE_INTRO, NULL);
			m_animWindow.Create(introRect, m_display, g_szPaintBallIntroSequence);
			m_animWindow.Play();
			m_started = 1;
		}
		m_animWindow.Resume();
		m_startCountdown = INTRO_ANIMATION_COUNTDOWN_COMPLETE;
	}
}
