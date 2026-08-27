#include "HiliteWindow.h"

#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"

// 68K 0x10804fb6 __ct__13CHiliteWindowFv
// STUB: LEMBALL 0x0044f400
HiliteWindow::HiliteWindow()
{
}

// 68K 0x10804ff8 OnCreate__13CHiliteWindowFv
// STUB: LEMBALL 0x0044f420
void HiliteWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}

// GLOBAL: LEMBALL 0x004a7b64
unsigned int g_dwHiliteAnimationId;
