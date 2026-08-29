#include "HiliteButtons.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/GraphicButton.h"
#include "../../Visos/Graphics/PvButton.h"
#include "../../Visos/Graphics/PvGWnd.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/ResAnim.h"

#include <new.h>

// 68K 0x10804ab2 __ct__14CHiliteButtonsFP5CGWndP4CGDIiiPUlUciiiUlPvUl
// FUNCTION: LEMBALL 0x0044f070
HiliteButtons::HiliteButtons(GWnd* p_arg0,
							 Gdi* p_arg1,
							 int p_arg2,
							 int p_arg3,
							 unsigned long* p_arg4,
							 unsigned char p_arg5,
							 int p_arg6,
							 int p_arg7,
							 int p_arg8,
							 unsigned long p_arg9,
							 void* p_arg10,
							 unsigned long p_arg11)
{
	int count;

	m_bounds.m_height = 0;
	m_bounds.m_width = 0;
	m_bounds.m_y = 0;
	m_window = p_arg0;
	m_bounds.m_x = 0;
	m_active = 1;
	m_gdi = p_arg1;
	count = (p_arg7 - p_arg6) + 1;
	m_valueCount = count;
	if (count < 2) {
		m_valueCount = 1;
	}
	m_minimum = p_arg6;
	m_controlMessage = p_arg9;
	m_maximum = p_arg7;
	m_x = p_arg2;
	m_y = p_arg3;
	if (p_arg10 == 0) {
		m_value = 0;
	}
	else {
		m_value = *(int*) p_arg10;
	}
	if (p_arg11 != 0xffffffff) {
		m_actionMessage = p_arg11;
	}
	m_binding = (int*) p_arg10;
	m_mode = p_arg5;
	g_pMasterInputQueue->Attach(this, 0);
	LoadFaces(p_arg4);
}

// 68K 0x10804c52 ProcessMsg__14CHiliteButtonsFP10tagMESSAGE
// STUB: LEMBALL 0x0044f160
int HiliteButtons::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10804d7a Draw__14CHiliteButtonsFUc
// FUNCTION: LEMBALL 0x0044f270
void HiliteButtons::Draw(unsigned char p_force)
{
	if (m_button != 0) {
		m_button->Draw(p_force);
	}
}

// 68K 0x10804db8 LoadFaces__14CHiliteButtonsFPUl
// FUNCTION: LEMBALL 0x0044f290
void HiliteButtons::LoadFaces(unsigned long* p_animIds)
{
	int index;
	void* storage;
	VsPoint position;

	m_animIds = p_animIds;
	m_resources = (ResAnim**) operator new(m_valueCount << 2);
	index = 0;
	while (index < m_valueCount) {
		m_resources[index] = ResAnim::Load(m_animIds[index]);
		index = index + 1;
	}
	storage = operator new(0x130);
	if (storage == 0) {
		m_button = 0;
	}
	else {
		position.m_x = (short) m_x;
		position.m_y = (short) m_y;
		m_button = new (storage) GraphicButton(position, (PvGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
	}
	m_button->SetAutoDraw(0);
	m_button->m_gdi->m_renderTarget->m_flag70 = 0;
	m_button->m_messageHandler = g_pMasterInputQueue;
	m_button->m_controlMessage = m_controlMessage;
}

// 68K 0x10804ebc UnLoadFaces__14CHiliteButtonsFv
// STUB: LEMBALL 0x0044f370
void HiliteButtons::UnLoadFaces()
{
}

// 68K 0x10804f42 UpdateAnimID__14CHiliteButtonsFv
// STUB: LEMBALL 0x0044f3d0
void HiliteButtons::UpdateAnimId()
{
}

// 68K 0x10804be0 __dt__14CHiliteButtonsFv
HiliteButtons::~HiliteButtons()
{
}

