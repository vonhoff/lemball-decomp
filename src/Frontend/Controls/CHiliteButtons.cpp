#include "CHiliteButtons.h"

#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/CMasterInput.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CGraphicButton.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "../../Visos/Resources/CResAnim.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"

class CPvGWnd;

// FUNCTION: LEMBALL 0x0044f070
CHiliteButtons::CHiliteButtons(CGWnd* p_arg0,
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
	int count;

	m_window = p_arg0;
	m_active = 1;
	m_gdi = p_arg1;
	count = (p_arg7 - p_arg6) + 1;
	m_valueCount = count;
	if (count <= 1) {
		m_valueCount = 1;
	}
	m_minimum = p_arg6;
	m_controlMessage = p_arg9;
	m_maximum = p_arg7;
	m_x = p_arg2;
	m_y = p_arg3;
	int* binding = (int*) p_arg10;
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
	if (p_arg11 != 0xffffffff) {
		m_actionMessage = p_arg11;
	}
	m_binding = binding;
	m_mode = p_arg5;
	g_pMasterInputQueue->Attach(this, 0);
	LoadFaces(p_arg4);
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
			m_button->SetAnimId(m_animIds[m_value - m_minimum]);
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
	m_resources = (CResAnim**) operator new(m_valueCount << 2);
	index = 0;
	while (index < m_valueCount) {
		m_resources[index] = CResAnim::Load(m_animIds[index]);
		index = index + 1;
	}
	m_button =
		new CGraphicButton(CVsPoint((short) m_x, (short) m_y), (CPvGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
	CSurface* surface = m_button->m_gdi->m_renderTarget;
	m_button->SetAutoDraw(0);
	surface->m_flag70 = 0;
	m_button->m_messageHandler = g_pMasterInputQueue;
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
void CHiliteButtons::UpdateAnimId()
{
	if (m_binding != 0) {
		if (m_valueCount == 1) {
			m_value = *m_binding;
		}
		else {
			m_value = *m_binding;
		}
	}
	m_button->SetAnimId(m_animIds[m_value - m_minimum]);
}
