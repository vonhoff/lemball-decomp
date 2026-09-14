#include "HiliteButtons.h"

#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/MasterInput.h"
#include "../../Visos/Foundation/VsPoint.h"
#include "../../Visos/Foundation/VsTime.h"
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

// 68K 0x10804be0 __dt__14CHiliteButtonsFv
// FUNCTION: LEMBALL 0x0044f130
HiliteButtons::~HiliteButtons()
{
	g_pMasterInputQueue->Detach(this, 0);
	UnLoadFaces();
}

// 68K 0x10804c52 ProcessMsg__14CHiliteButtonsFP10tagMESSAGE
// FUNCTION: LEMBALL 0x0044f160
int HiliteButtons::ProcessMsg(Message* p_message)
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
		g_pSoundView->PlayEffect((eSoundEffect) 0x25);
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

// 68K 0x10804d7a Draw__14CHiliteButtonsFUc
// FUNCTION: LEMBALL 0x0044f270
void HiliteButtons::Draw(int p_force)
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

	m_animIds = p_animIds;
	m_resources = (ResAnim**) operator new(m_valueCount << 2);
	index = 0;
	while (index < m_valueCount) {
		m_resources[index] = ResAnim::Load(m_animIds[index]);
		index = index + 1;
	}
	m_button =
		new GraphicButton(VsPoint((short) m_x, (short) m_y), (PvGWnd*) m_window, m_animIds[m_value - m_minimum], 3);
	Surface* surface = m_button->m_gdi->m_renderTarget;
	m_button->SetAutoDraw(0);
	surface->m_flag70 = 0;
	m_button->m_messageHandler = g_pMasterInputQueue;
	m_button->m_controlMessage = m_controlMessage;
}

// 68K 0x10804ebc UnLoadFaces__14CHiliteButtonsFv
// FUNCTION: LEMBALL 0x0044f370
void HiliteButtons::UnLoadFaces()
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

// 68K 0x10804f42 UpdateAnimID__14CHiliteButtonsFv
// FUNCTION: LEMBALL 0x0044f3d0
void HiliteButtons::UpdateAnimId()
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
