#include "CSpriteWindow.h"

#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/VsGdi.h"

// FUNCTION: LEMBALL 0x0044c830
CSpriteWindow::CSpriteWindow()
{
	m_gdiFlags = m_gdiFlags + 5;
}

// FUNCTION: LEMBALL 0x0044c850
void CSpriteWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}
