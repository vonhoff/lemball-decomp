#include "CHiliteButtons.h"

#include "../../Views/Sound/CSoundView.h"
#include "Visos/Queues/CBaseQueue.h"
#include "Visos/Input/CMasterInput.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Time/VsTime.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Controls/CGraphicButton.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Resources/Types/CResANIM.h"
#include "CHiliteController.h"
#include "Frontend/Controls/ButtonActionMessages.h"
#include "Game/SoundEffects.h"
#include "Visos/Queues/Message.h"
#include "Platform/Windows/Windowing/CGWnd.h"

class CPVGWnd;

// FUNCTION: LEMBALL 0x0044f070
CHiliteButtons::CHiliteButtons(CGWnd* p_window,
							   CGDI* p_gdi,
							   int p_x,
							   int p_y,
							   unsigned long* p_animIds,
							   unsigned int p_mode,
							   int p_minimum,
							   int p_maximum,
							   int p_arg8,
							   unsigned long p_controlMessage,
							   void* p_binding,
							   unsigned long p_actionMessage)
{
	int count;

	m_window = p_window;
	m_active = 1;
	m_gdi = p_gdi;
	count = (p_maximum - p_minimum) + 1;
	m_valueCount = count;
	if (count <= 1) {
		m_valueCount = 1;
	}
	m_minimum = p_minimum;
	m_controlMessage = p_controlMessage;
	m_maximum = p_maximum;
	m_x = p_x;
	m_y = p_y;
	int* binding = (int*) p_binding;
	if (binding != NULL) {
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
	if (p_actionMessage != BUTTON_ACTION_MESSAGE_UNASSIGNED) {
		m_actionMessage = p_actionMessage;
	}
	m_binding = binding;
	m_mode = p_mode;
	g_pMasterInputQueue->Attach(this, 0);
	LoadFaces(p_animIds);
}

// FUNCTION: LEMBALL 0x0044f130
CHiliteButtons::~CHiliteButtons()
{
	g_pMasterInputQueue->Detach(this, 0);
	UnLoadFaces();
}

// FUNCTION: LEMBALL 0x0044f160
int CHiliteButtons::ProcessMsg(Message* p_message)
{
	Message posted;
	int nextValue;
	posted.m_type = MESSAGE_BUTTON_RELEASED;
	posted.m_time = CurrentQueueTimer();
	posted.m_code = 0;
	posted.m_payload = NULL;
	posted.m_source = NULL;

	if (p_message->m_code != (int) m_controlMessage) {
		return 0;
	}
	switch ((int) p_message->m_type) {
	default:
		return 0;
	case MESSAGE_BUTTON_PRESSED:
		g_pSoundView->PlayEffect(SFX_DRUM1);
		return 0;
	case MESSAGE_BUTTON_RELEASED:
		if (m_mode == HILITE_BUTTON_MODE_ACTION_MESSAGE) {
			posted.m_code = (int) m_actionMessage;
			g_pMasterInputQueue->Post(posted);
			return 0;
		}
		else {
			nextValue = m_value + 1;
			m_value = nextValue;
			if (m_maximum < nextValue) {
				m_value = m_minimum;
			}
			if (m_binding != NULL) {
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
			m_button->SetAnimID(m_animIds[m_value - m_minimum]);
			return 0;
		}
	}
}

// FUNCTION: LEMBALL 0x0044f240
void CHiliteButtons::MoveCurrentButton(int p_x, int p_y)
{
	CVSPoint point(p_x, p_y);
	if (m_button != NULL) {
		m_button->Move(point);
	}
}

// FUNCTION: LEMBALL 0x0044f270
void CHiliteButtons::Draw(int p_force)
{
	if (m_button != NULL) {
		m_button->Draw(p_force);
	}
}

// FUNCTION: LEMBALL 0x0044f290
void CHiliteButtons::LoadFaces(unsigned long* p_animIds)
{
	int index;

	m_animIds = p_animIds;
	m_resources = (CResANIM**) operator new(m_valueCount * sizeof(*m_resources));
	index = 0;
	while (index < m_valueCount) {
		m_resources[index] = CResANIM::Load(m_animIds[index]);
		index = index + 1;
	}
	m_button =
		new CGraphicButton(CVSPoint((short) m_x, (short) m_y), (CPVGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
	CSurface* surface = m_button->m_gdi->m_renderTarget;
	m_button->SetAutoDraw(0);
	surface->m_flag70 = 0;
	m_button->m_messageQueue = g_pMasterInputQueue;
	m_button->m_controlMessage = m_controlMessage;
}

// FUNCTION: LEMBALL 0x0044f370
void CHiliteButtons::UnLoadFaces()
{
	int index;

	if (m_button != NULL) {
		delete m_button;
	}
	index = 0;
	if (0 < m_valueCount) {
		do {
			m_resources[index]->UnLoad();
			index = index + 1;
		} while (index < m_valueCount);
	}
	operator delete(m_resources);
	m_resources = NULL;
}

// FUNCTION: LEMBALL 0x0044f3d0
void CHiliteButtons::UpdateAnimID()
{
	if (m_binding != NULL) {
		if (m_valueCount == 1) {
			m_value = *m_binding;
		}
		else {
			m_value = *m_binding;
		}
	}
	m_button->SetAnimID(m_animIds[m_value - m_minimum]);
}
