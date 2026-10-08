#include "CIntroAnimAnimWindow.h"

#include "Application/CGameStatus.h"
#include "CIntroAnimDrawer.h"
#include "Engine/Resources/Manifest.h"
#include "Level/CLevelLoader.h"
#include "Platform/Windows/Windowing/CAnimWnd.h"
#include "Platform/Windows/Windowing/CWnd.h"

// GLOBAL: LEMBALL 0x0049f194
char g_szMoviePrefix[] = "lemball";

// FUNCTION: LEMBALL 0x004477b0
void CIntroAnimAnimWindow::Initialise(CIntroAnimDrawer* p_owner,
									  CMain2DDisplay* p_display,
									  unsigned int p_completionSequence)
{
	m_completionSequence = p_completionSequence;
	m_owner = p_owner;
	m_display = p_display;
}

// FUNCTION: LEMBALL 0x004477e0
void CIntroAnimAnimWindow::SetAnim()
{
	if (m_completionSequence == 0) {
		CAnimWnd::SetAnim(RES_NEWFRONT_STRINGS_INTRONAME);
		return;
	}
	switch (g_pGameStatus->m_skill) {
	case SKILL_FUN:
	case SKILL_MAYHEM:
		CAnimWnd::SetAnim(RES_NEWFRONT_STRINGS_EXTRONAME);
		return;
	case SKILL_TRICKY:
	case SKILL_TAXING:
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
	return WINDOW_STYLE_CHILD | WINDOW_STYLE_SHOW_ON_CREATE;
}
