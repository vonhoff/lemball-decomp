#include "SpriteWindow.h"

#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"

// 68K 0x108030c2 __ct__13CSpriteWindowFv
// FUNCTION: LEMBALL 0x0044c830
SpriteWindow::SpriteWindow()
{
	m_gdiFlags = m_gdiFlags + 5;
}

// 68K 0x10803104 OnCreate__13CSpriteWindowFv
// FUNCTION: LEMBALL 0x0044c850
void SpriteWindow::Dummy3c()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}
