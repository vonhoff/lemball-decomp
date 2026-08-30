#include "IntroAnimAnimWindow.h"

#include "../../Control/Game/GameStatus.h"
#include "../Drawers/IntroAnimDrawer.h"

// GLOBAL: LEMBALL 0x0049f194
char g_szMoviePrefix[] = "lemball";

// 68K 0x108061a2 Initialise__20CIntroAnimAnimWindowFP16CIntroAnimDrawerP14CMain2DDisplayUc
// FUNCTION: LEMBALL 0x004477b0
void IntroAnimAnimWindow::Initialise(IntroAnimDrawer* p_owner, Main2DDisplay* p_display, unsigned int p_variant)
{
	m_variant = p_variant;
	m_owner = p_owner;
	m_display = p_display;
}

// 68K 0x1080620e SetAnim__20CIntroAnimAnimWindowFv
// FUNCTION: LEMBALL 0x004477e0
void IntroAnimAnimWindow::SetAnim()
{
	if (m_variant == 0) {
		AnimWnd::SetAnim(0x116);
		return;
	}
	switch (g_pGameStatus->m_skill) {
	case 0:
	case 3:
		AnimWnd::SetAnim(0x117);
		return;
	case 1:
	case 2:
		AnimWnd::SetAnim(0x118);
		return;
	default:
		return;
	}
}

// 68K 0x1080629c OnStop__20CIntroAnimAnimWindowFv
// FUNCTION: LEMBALL 0x00447830
void IntroAnimAnimWindow::OnStop()
{
	if (m_owner->QuitYet() != 0) {
		return;
	}
	Destroy();
	m_owner->EndPhase();
}

// 68K 0x1011c32c GetStyle__20CIntroAnimAnimWindowFv
// FUNCTION: LEMBALL 0x00447990
unsigned int IntroAnimAnimWindow::GetStyle()
{
	return 1073741825;
}
