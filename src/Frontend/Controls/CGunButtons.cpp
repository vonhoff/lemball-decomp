#include "CGunButtons.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/CResAnim.h"
#include "../Windows/CTrackWindow.h"
#include "CGunButton.h"
#include "CTrackerButton.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Resources/CResBase.h"

#include <new.h>

class CPvGWnd;

// FUNCTION: LEMBALL 0x0044c270
CGunButtons::CGunButtons(CGWnd* p_arg0,
						 CGdi* p_arg1,
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
CGunButtons::CGunButtons(const CVsRect& p_rect,
						 CGWnd* p_window,
						 CGdi* p_gdi,
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
	const CVsPoint& position = p_rect;
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

// FUNCTION: LEMBALL 0x0044c410
CGunButtons::~CGunButtons()
{
	g_pMasterInputQueue->Detach(this, 0);
	UnLoadFaces();
}

// FUNCTION: LEMBALL 0x0044c440
bool CGunButtons::DrawBackBuffer()
{
	if (g_nGunButtonsRedrawPending != 0) {
		g_nGunButtonsRedrawPending = 0;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0044c460
int CGunButtons::ProcessMsg(Message* p_message)
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
void CGunButtons::Move(int p_x, int p_y)
{
	CVsPoint point((short) p_x, (short) p_y);
	if (m_graphicButton != 0) {
		m_graphicButton->Move(point);
	}
	if (m_trackerButton != 0) {
		m_trackerButton->Move(point);
	}
}

// FUNCTION: LEMBALL 0x0044c600
void CGunButtons::Draw(int p_firstState, int p_secondState)
{
	if (m_graphicButton != 0) {
		m_graphicButton->Draw(p_firstState);
	}
	if (m_trackerButton != 0) {
		m_trackerButton->Draw(p_firstState);
	}
}

// FUNCTION: LEMBALL 0x0044c630
void CGunButtons::LoadFaces(unsigned long* p_animIds)
{
	int i;
	void* storage;

	m_animIds = p_animIds;
	m_resources = (CResAnim**) operator new(m_valueCount * 4);
	i = 0;
	if (m_valueCount > i) {
		do {
			m_resources[i] = CResAnim::Load(m_animIds[i]);
			i = i + 1;
		} while (i < m_valueCount);
	}
	if (m_mode == 0) {
		storage = operator new(0x130);
		if (storage != 0) {
			CVsPoint position((short) m_x, (short) m_y);
			m_graphicButton =
				new (storage) CGunButton(position, (CPvGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
		}
		else {
			m_graphicButton = 0;
		}
		CSurface* surface = m_graphicButton->m_gdi->m_renderTarget;
		m_graphicButton->SetAutoDraw(0);
		surface->m_flag70 = 0;
		m_graphicButton->m_messageHandler = g_pMasterInputQueue;
		m_graphicButton->m_controlMessage = m_controlMessage;
		m_trackerButton = 0;
		return;
	}
	storage = operator new(0x138);
	if (storage != 0) {
		CVsPoint position((short) m_x, (short) m_y);
		m_trackerButton = new (storage) CTrackerButton(position, (CPvGWnd*) m_window, *m_animIds, m_trackRect, m_value);
	}
	else {
		m_trackerButton = 0;
	}
	CSurface* surface = m_trackerButton->m_gdi->m_renderTarget;
	m_trackerButton->SetAutoDraw(0);
	surface->m_flag70 = 0;
	m_trackerButton->m_messageHandler = g_pMasterInputQueue;
	m_trackerButton->m_controlMessage = m_controlMessage;
	m_graphicButton = 0;
}

// FUNCTION: LEMBALL 0x0044c7c0
void CGunButtons::UnLoadFaces()
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
			CResBase* resource = m_resources[i];
			i++;
			resource->UnLoad();
		} while (i < m_valueCount);
	}
	operator delete(m_resources);
	m_resources = 0;
}

// GLOBAL: LEMBALL 0x0049fa68
int g_nGunButtonsRedrawPending = 0;
