#include "CStateTextButton.h"

#include "Engine/Queues/CBaseQueue.h"

// FUNCTION: LEMBALL 0x004695d0
CStateTextButton::CStateTextButton(unsigned int p_controlMessage,
								   const CVSRect& p_rect,
								   CPVGWnd* p_parent,
								   unsigned int p_fontResourceId,
								   unsigned int p_alignmentFlags)
	: CTextButton(p_rect, p_parent, p_fontResourceId, p_alignmentFlags)
{
	InitializeState();
	m_controlMessage = p_controlMessage;
}

// FUNCTION: LEMBALL 0x00469670
void CStateTextButton::InitializeState()
{
	m_state = 0;
	m_messageQueue = g_pMasterInputQueue;
}
