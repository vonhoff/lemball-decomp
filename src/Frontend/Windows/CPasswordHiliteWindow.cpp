#include "CPasswordHiliteWindow.h"

#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x00451fb0
CPasswordHiliteWindow::CPasswordHiliteWindow()
{
	m_gdiFlags = m_gdiFlags + 10;
}

// FUNCTION: LEMBALL 0x00451fd0
void CPasswordHiliteWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}
