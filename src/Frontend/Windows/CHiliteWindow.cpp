#include "CHiliteWindow.h"

#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x0044f400
CHiliteWindow::CHiliteWindow()
{
	m_gdiFlags = m_gdiFlags + 1;
}

// FUNCTION: LEMBALL 0x0044f420
void CHiliteWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}

// GLOBAL: LEMBALL 0x004a7b64
unsigned int g_dwHiliteAnimationId;

// GLOBAL: LEMBALL 0x004a7b68
char g_abPasswordLevelText[24];
