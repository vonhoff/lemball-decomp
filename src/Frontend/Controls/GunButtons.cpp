#include "GunButtons.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Graphics/GraphicButton.h"
#include "../../Visos/Resources/ResAnim.h"

#include <new.h>

// 68K 0x10802a2e __ct__11CGunButtonsFP5CGWndP4CGDIiiPUlUciiiUlPvUl
// STUB: LEMBALL 0x0044c270
GunButtons::GunButtons(GWnd* p_arg0,
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
	int range;

	m_trackRect.m_height = 0;
	m_mode = 0;
	m_trackRect.m_width = 0;
	m_trackRect.m_y = 0;
	m_trackRect.m_x = 0;
	m_window = p_arg0;
	m_gdi = p_arg1;
	m_active = 1;
	range = (p_arg7 - p_arg6) + 1;
	m_valueCount = range;
	if (range < 2) {
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
	m_postAction = p_arg5;
	m_graphicButton = 0;
	m_trackerButton = 0;
	m_resources = 0;
	if (g_pMasterInputQueue != 0) {
		g_pMasterInputQueue->Attach(this, 0);
	}
	LoadFaces(p_arg4);
}

// 68K 0x10802c2c DrawBackBuffer__11CGunButtonsFv
// STUB: LEMBALL 0x0044c440
bool GunButtons::DrawBackBuffer()
{
	if (g_nGunButtonsRedrawPending != 0) {
		g_nGunButtonsRedrawPending = 0;
		return 1;
	}
	return 0;
}

// 68K 0x10802c6a ProcessMsg__11CGunButtonsFP10tagMESSAGE
// STUB: LEMBALL 0x0044c460
int GunButtons::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10802dfc Draw__11CGunButtonsFUcUc
// STUB: LEMBALL 0x0044c600
void GunButtons::Draw(unsigned char p_firstState, unsigned char p_secondState)
{
	if (m_graphicButton != 0) {
		m_graphicButton->Draw(p_firstState);
	}
}

// 68K 0x10802e54 LoadFaces__11CGunButtonsFPUl
// STUB: LEMBALL 0x0044c630
void GunButtons::LoadFaces(unsigned long* p_animIds)
{
	VsPoint position;
	void* storage;
	int i;

	m_animIds = p_animIds;
	m_resources = 0;
	if (m_valueCount > 0) {
		m_resources = (ResAnim**) operator new(m_valueCount * 4);
		i = 0;
		while (i < m_valueCount) {
			m_resources[i] = ResAnim::Load(p_animIds[i]);
			i = i + 1;
		}
	}
	storage = operator new(0x130);
	if (storage == 0) {
		m_graphicButton = 0;
	}
	else {
		position.m_x = (short) m_x;
		position.m_y = (short) m_y;
		m_graphicButton = new (storage) GraphicButton(position, (PvGWnd*) m_window, p_animIds[m_value - m_minimum], 3);
	}
	if (m_graphicButton != 0) {
		m_graphicButton->SetAutoDraw(0);
		m_graphicButton->m_messageHandler = g_pMasterInputQueue;
		m_graphicButton->m_controlMessage = m_controlMessage;
	}
	m_trackerButton = 0;
}

// 68K 0x1080301a UnLoadFaces__11CGunButtonsFv
// STUB: LEMBALL 0x0044c7c0
void GunButtons::UnLoadFaces()
{
}

// 68K 0x10802b54 __dt__11CGunButtonsFv
GunButtons::~GunButtons()
{
}

// GLOBAL: LEMBALL 0x0049fa68
int g_nGunButtonsRedrawPending = 0;
