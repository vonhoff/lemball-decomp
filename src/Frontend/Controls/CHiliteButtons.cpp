#include "CHiliteButtons.h"

#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CMasterInput.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/CSurface.h"
#include "../../Visos/Resources/CResANIM.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"

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
	if (binding != 0) {
		// The original retains this comparison before the identical binding reads.
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
	if (p_actionMessage != 0xffffffff) {
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
	posted.type = 0xc;
	posted.time = CurrentQueueTimer();
	posted.code = 0;
	posted.payload = 0;
	posted.source = 0;

	if (p_message->code != (int) m_controlMessage) {
		return 0;
	}
	switch ((int) p_message->type) {
	default:
		return 0;
	case 0xb:
		g_pSoundView->PlayEffect(SFX_DRUM1);
		return 0;
	case 0xc:
		if (m_mode == 1) {
			posted.code = (int) m_actionMessage;
			g_pMasterInputQueue->Post(posted);
			return 0;
		}
		else {
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
			m_button->SetAnimID(m_animIds[m_value - m_minimum]);
			return 0;
		}
	}
}

// FUNCTION: LEMBALL 0x0044f240
void CHiliteButtons::MoveCurrentButton(int p_x, int p_y)
{
	CVsPoint point(p_x, p_y);
	if (m_button != 0) {
		m_button->Move(point);
	}
}

// FUNCTION: LEMBALL 0x0044f270
void CHiliteButtons::Draw(int p_force)
{
	if (m_button != 0) {
		m_button->Draw(p_force);
	}
}

// FUNCTION: LEMBALL 0x0044f290
void CHiliteButtons::LoadFaces(unsigned long* p_animIds)
{
	int index;

	m_animIds = p_animIds;
	m_resources = (CResANIM**) operator new(m_valueCount << 2);
	index = 0;
	while (index < m_valueCount) {
		m_resources[index] = CResANIM::Load(m_animIds[index]);
		index = index + 1;
	}
	m_button =
		new CGraphicButton(CVsPoint((short) m_x, (short) m_y), (CPVGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
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

	if (m_button != 0) {
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
	m_resources = 0;
}

// FUNCTION: LEMBALL 0x0044f3d0
void CHiliteButtons::UpdateAnimID()
{
	if (m_binding != 0) {
		if (m_valueCount == 1) {
			m_value = *m_binding;
		}
		else {
			m_value = *m_binding;
		}
	}
	m_button->SetAnimID(m_animIds[m_value - m_minimum]);
}
