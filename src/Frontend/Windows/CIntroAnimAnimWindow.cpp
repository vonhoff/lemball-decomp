#include "CIntroAnimAnimWindow.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Visos/Resources/Manifest.h"
#include "../Drawers/CIntroAnimDrawer.h"
#include "Visos/Graphics/CAnimWnd.h"

// GLOBAL: LEMBALL 0x0049f194
char g_szMoviePrefix[] = "lemball";

// FUNCTION: LEMBALL 0x004477b0
void CIntroAnimAnimWindow::Initialise(CIntroAnimDrawer* p_owner, CMain2DDisplay* p_display, unsigned int p_variant)
{
	m_variant = p_variant;
	m_owner = p_owner;
	m_display = p_display;
}

// FUNCTION: LEMBALL 0x004477e0
void CIntroAnimAnimWindow::SetAnim()
{
	if (m_variant == 0) {
		CAnimWnd::SetAnim(RES_NEWFRONT_STRINGS_INTRONAME);
		return;
	}
	switch (g_pGameStatus->m_skill) {
	case 0:
	case 3:
		CAnimWnd::SetAnim(RES_NEWFRONT_STRINGS_EXTRONAME);
		return;
	case 1:
	case 2:
		CAnimWnd::SetAnim(RES_NEWFRONT_STRINGS_SUCCFAIL);
		return;
	default:
		return;
	}
}

// FUNCTION: LEMBALL 0x00447830
void CIntroAnimAnimWindow::OnStop()
{
	Destroy();
	m_owner->EndPhase();
}

// FUNCTION: LEMBALL 0x00447990
unsigned int CIntroAnimAnimWindow::GetStyle()
{
	return 1073741825;
}
