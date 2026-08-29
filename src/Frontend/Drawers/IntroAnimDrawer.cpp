#include "IntroAnimDrawer.h"

#include "../../Views/Display/Main2DDisplay.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/String.h"
#include "../../Visos/Graphics/PvGWnd.h"

// GLOBAL: LEMBALL 0x0049f19c
char g_szPaintBallIntroSequence[] = "PaintBall Intro Sequence";

extern char g_szMoviePrefix[];

// 68K 0x10805c12 __ct__16CIntroAnimDrawerFP14CMain2DDisplayP4CGDIRC7CVSRectUc
// FUNCTION: LEMBALL 0x00447410
IntroAnimDrawer::IntroAnimDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2, unsigned char p_arg3)
	: BaseFrontendDrawer(p_arg0, p_arg1, p_arg2, 1, 0, 0, 0, 0, 0)
{
	m_variant = p_arg3;
	m_nextFlow = 0xe;
	if (p_arg3 == 0) {
		m_nextFlow = 2;
	}
	m_started = 0;
	m_startCountdown = 10;
	m_display->Clear(-1);
	m_animWindow.Initialise(this, m_display, p_arg3);
	m_animWindow.m_resolveMoviePath = 1;
	m_animWindow.m_moviePrefix = g_szMoviePrefix;
	m_animWindow.m_useMoviePrefix = 1;
	m_drawBackground = 0;
	m_drawFrame = 0;
	m_drawSolid = 0;
	Setup();
}

// 68K 0x10805d7e Load__16CIntroAnimDrawerFv
// FUNCTION: LEMBALL 0x00447530
void IntroAnimDrawer::Load()
{
	m_animWindow.SetAnim();
}

// 68K 0x10805db2 UnLoad__16CIntroAnimDrawerFv
// FUNCTION: LEMBALL 0x00447540
void IntroAnimDrawer::UnLoad()
{
}

// 68K 0x10805eb8 DestroyDrawer__16CIntroAnimDrawerFv
// FUNCTION: LEMBALL 0x004475a0
void IntroAnimDrawer::DestroyDrawer()
{
	if (m_started != 0 && m_animWindow.m_lifecycleRefs == 1) {
		m_animWindow.Destroy();
		m_startCountdown = 10;
		m_started = 0;
	}
}

// 68K 0x10805f22 EndPhase__16CIntroAnimDrawerFv
// FUNCTION: LEMBALL 0x004475e0
void IntroAnimDrawer::EndPhase()
{
	m_quitYet = 1;
	m_returnState = m_nextFlow;
	if (m_animWindow.m_lifecycleRefs == 1) {
		m_animWindow.Destroy();
	}
}

// 68K 0x10805f82 ProcessMessages__16CIntroAnimDrawerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00447610
bool IntroAnimDrawer::ProcessMessages(Message* p_message)
{
	if (p_message == 0) {
		return 0;
	}
	if (p_message->type == 4) {
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
	}
	if (p_message->type == 6) {
		EndPhase();
		return 1;
	}
	return 0;
}

// 68K 0x1080601e Processing__16CIntroAnimDrawerFv
// FUNCTION: LEMBALL 0x004476b0
void IntroAnimDrawer::Processing()
{
	VsRect introRect;

	if (0 < m_startCountdown) {
		m_startCountdown = m_startCountdown - 1;
	}
	if (m_display == 0 || m_display->IsWindowValid() == 0) {
		EndPhase();
		return;
	}
	if (m_startCountdown == 0) {
		introRect.m_width = 0x140;
		introRect.m_height = 0xf0;
		introRect.m_x = (short) ((m_display->m_rect.m_width - 0x140) / 2);
		introRect.m_y = (short) ((m_display->m_rect.m_height - 0xf0) / 2);
		if (m_started == 0) {
			if (g_pSoundView != 0) {
				g_pSoundView->ChangeState(1, 0);
			}
			m_animWindow.Create(introRect, m_display, g_szPaintBallIntroSequence);
			m_animWindow.Play();
			m_started = 1;
		}
		m_animWindow.Resume();
		m_startCountdown = -1;
	}
}

// 68K 0x10805dda __dt__16CIntroAnimDrawerFv
IntroAnimDrawer::~IntroAnimDrawer()
{
}
