#include "GunButtons.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/GraphicButton.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/ResAnim.h"
#include "../Windows/TrackWindow.h"
#include "GunButton.h"
#include "TrackerButton.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/VsPoint.h"
#include "Visos/Foundation/VsRect.h"
#include "Visos/Graphics/GWnd.h"
#include "Visos/Resources/ResBase.h"

#include <new.h>

class PvGWnd;

// 68K 0x10802a2e __ct__11CGunButtonsFP5CGWndP4CGDIiiPUlUciiiUlPvUl
// FUNCTION: LEMBALL 0x0044c270
GunButtons::GunButtons(GWnd* p_arg0,
					   Gdi* p_arg1,
					   int p_arg2,
					   int p_arg3,
					   unsigned long* p_arg4,
					   unsigned int p_arg5,
					   int p_arg6,
					   int p_arg7,
					   int p_arg8,
					   unsigned long p_arg9,
					   void* p_arg10,
					   unsigned long p_arg11)
{
	int range;

	m_mode = 0;
	m_window = p_arg0;
	m_gdi = p_arg1;
	m_active = 1;
	range = (p_arg7 - p_arg6) + 1;
	m_valueCount = range;
	if (range <= 1) {
		m_valueCount = 1;
	}
	m_minimum = p_arg6;
	m_controlMessage = p_arg9;
	m_maximum = p_arg7;
	m_x = p_arg2;
	m_y = p_arg3;
	int* binding = (int*) p_arg10;
	if (binding != 0) {
		// The original tests the range even though both paths read the same binding.
		if (m_valueCount == 1) {
			m_value = *binding;
		}
		else {
			m_value = *binding;
		}
	}
	else {
		m_value = 0;
	}
	if (p_arg11 != 0xffffffff) {
		m_actionMessage = p_arg11;
	}
	m_binding = binding;
	m_postAction = p_arg5;
	g_pMasterInputQueue->Attach(this, 0);
	LoadFaces(p_arg4);
}

// FUNCTION: LEMBALL 0x0044c330
GunButtons::GunButtons(const VsRect& p_rect,
					   GWnd* p_window,
					   Gdi* p_gdi,
					   int p_x,
					   int p_y,
					   unsigned long* p_animIds,
					   unsigned int p_postAction,
					   int p_value,
					   unsigned int p_controlMessage,
					   int* p_binding,
					   int p_actionMessage)
{
	m_mode = 1;
	m_trackRect.m_width = p_rect.m_width;
	m_trackRect.m_height = p_rect.m_height;
	const VsPoint& position = p_rect;
	m_trackRect.m_x = position.m_x;
	m_trackRect.m_y = position.m_y;
	m_window = p_window;
	m_gdi = p_gdi;
	m_controlMessage = p_controlMessage;
	m_x = p_x;
	m_y = p_y;
	m_value = p_value;
	m_valueCount = 1;
	m_minimum = 0;
	m_maximum = 100;
	if (p_actionMessage != -1) {
		m_actionMessage = p_actionMessage;
	}
	m_binding = p_binding;
	m_postAction = p_postAction;
	g_pMasterInputQueue->Attach(this, 0);
	LoadFaces(p_animIds);
}

// 68K 0x10802b54 __dt__11CGunButtonsFv
// FUNCTION: LEMBALL 0x0044c410
GunButtons::~GunButtons()
{
	g_pMasterInputQueue->Detach(this, 0);
	UnLoadFaces();
}

// 68K 0x10802c2c DrawBackBuffer__11CGunButtonsFv
// FUNCTION: LEMBALL 0x0044c440
bool GunButtons::DrawBackBuffer()
{
	if (g_nGunButtonsRedrawPending != 0) {
		g_nGunButtonsRedrawPending = 0;
		return 1;
	}
	return 0;
}

// 68K 0x10802c6a ProcessMsg__11CGunButtonsFP10tagMESSAGE
// FUNCTION: LEMBALL 0x0044c460
int GunButtons::ProcessMsg(Message* p_message)
{
	Message posted;
	int nextValue;
	unsigned long animId;

	posted.type = 0xc;
	posted.time = CurrentQueueTimer();
	posted.code = 0;
	posted.payload = 0;
	posted.source = 0;
	if (p_message->code == (int) m_controlMessage) {
		switch ((unsigned int) p_message->type) {
		case 0xc:
			switch (m_mode) {
			case 0:
				if (m_postAction == 1) {
					posted.code = (int) m_actionMessage;
					g_pMasterInputQueue->Post(posted);
					return 0;
				}
				nextValue = m_value + 1;
				m_value = nextValue;
				if (m_maximum < nextValue) {
					m_value = m_minimum;
				}
				if (m_binding != 0) {
					if (m_valueCount == 1) {
						if (*m_binding == 0) {
							*m_binding = 1;
						}
						else {
							*m_binding = 0;
						}
					}
					else {
						*m_binding = m_value;
					}
				}
				if (m_graphicButton != 0) {
					animId = m_animIds[m_value - m_minimum];
					m_graphicButton->SetAnimId(animId);
				}
				else {
					animId = *m_animIds;
					m_trackerButton->SetAnimId(animId);
				}
				g_nGunButtonsRedrawPending = 1;
				return 0;
			case 1: {
				int value = m_trackerButton->m_trackWindow->m_value;
				int maximum = m_maximum;
				if (value == maximum) {
					value = 0;
				}
				else {
					value += maximum / 4;
					if (value > maximum) {
						value = maximum;
					}
				}
				*m_binding = value;
				m_trackerButton->m_trackWindow->SetButtonValue(value);
				break;
			}
			}
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0044c5b0
void GunButtons::Move(int p_x, int p_y)
{
	VsPoint point((short) p_x, (short) p_y);
	if (m_graphicButton != 0) {
		m_graphicButton->Move(point);
	}
	if (m_trackerButton != 0) {
		m_trackerButton->Move(point);
	}
}

// 68K 0x10802dfc Draw__11CGunButtonsFUcUc
// FUNCTION: LEMBALL 0x0044c600
void GunButtons::Draw(int p_firstState, int p_secondState)
{
	if (m_graphicButton != 0) {
		m_graphicButton->Draw(p_firstState);
	}
	if (m_trackerButton != 0) {
		m_trackerButton->Draw(p_firstState);
	}
}

// 68K 0x10802e54 LoadFaces__11CGunButtonsFPUl
// FUNCTION: LEMBALL 0x0044c630
void GunButtons::LoadFaces(unsigned long* p_animIds)
{
	int i;
	void* storage;

	m_animIds = p_animIds;
	m_resources = (ResAnim**) operator new(m_valueCount * 4);
	i = 0;
	if (m_valueCount > i) {
		do {
			m_resources[i] = ResAnim::Load(m_animIds[i]);
			i = i + 1;
		} while (i < m_valueCount);
	}
	if (m_mode == 0) {
		storage = operator new(0x130);
		if (storage != 0) {
			VsPoint position((short) m_x, (short) m_y);
			m_graphicButton = new (storage) GunButton(position, (PvGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
		}
		else {
			m_graphicButton = 0;
		}
		Surface* surface = m_graphicButton->m_gdi->m_renderTarget;
		m_graphicButton->SetAutoDraw(0);
		surface->m_flag70 = 0;
		m_graphicButton->m_messageHandler = g_pMasterInputQueue;
		m_graphicButton->m_controlMessage = m_controlMessage;
		m_trackerButton = 0;
		return;
	}
	storage = operator new(0x138);
	if (storage != 0) {
		VsPoint position((short) m_x, (short) m_y);
		m_trackerButton = new (storage) TrackerButton(position, (PvGWnd*) m_window, *m_animIds, m_trackRect, m_value);
	}
	else {
		m_trackerButton = 0;
	}
	Surface* surface = m_trackerButton->m_gdi->m_renderTarget;
	m_trackerButton->SetAutoDraw(0);
	surface->m_flag70 = 0;
	m_trackerButton->m_messageHandler = g_pMasterInputQueue;
	m_trackerButton->m_controlMessage = m_controlMessage;
	m_graphicButton = 0;
}

// 68K 0x1080301a UnLoadFaces__11CGunButtonsFv
// FUNCTION: LEMBALL 0x0044c7c0
void GunButtons::UnLoadFaces()
{
	int i;

	if (m_graphicButton != 0) {
		delete m_graphicButton;
	}
	if (m_trackerButton != 0) {
		delete m_trackerButton;
	}
	i = 0;
	if (0 < m_valueCount) {
		do {
			ResBase* resource = m_resources[i];
			i++;
			resource->UnLoad();
		} while (i < m_valueCount);
	}
	operator delete(m_resources);
	m_resources = 0;
}

// GLOBAL: LEMBALL 0x0049fa68
int g_nGunButtonsRedrawPending = 0;
