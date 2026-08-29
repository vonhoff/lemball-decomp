#include "PasswordHiliteWindow.h"

#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"

// 68K 0x1080d510 __ct__21CPasswordHiliteWindowFv
// FUNCTION: LEMBALL 0x00451fb0
PasswordHiliteWindow::PasswordHiliteWindow()
{
	m_gdiFlags = m_gdiFlags + 10;
}

// 68K 0x1080d55c OnCreate__21CPasswordHiliteWindowFv
// FUNCTION: LEMBALL 0x00451fd0
void PasswordHiliteWindow::Dummy3c()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}
