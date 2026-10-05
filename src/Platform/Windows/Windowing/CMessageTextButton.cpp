#include "CMessageTextButton.h"

#include "Engine/Queues/CBaseQueue.h"

// FUNCTION: LEMBALL 0x004697c0
CMessageTextButton::CMessageTextButton(unsigned int p_controlMessage,
									   const CVSRect& p_rect,
									   CPVGWnd* p_parent,
									   unsigned int p_fontResourceId,
									   unsigned int p_alignmentFlags)
	: CTextButton(p_rect, p_parent, p_fontResourceId, p_alignmentFlags)
{
	Initialize();
	m_controlMessage = p_controlMessage;
}

// FUNCTION: LEMBALL 0x00469860
void CMessageTextButton::Initialize()
{
	m_messageQueue = g_pMasterInputQueue;
}
