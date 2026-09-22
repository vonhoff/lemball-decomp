#include "CInputTextButton.h"

#include "Visos/Foundation/CBaseQueue.h"
#include "Visos/Resources/Manifest.h"

#include <string.h>

// FUNCTION: LEMBALL 0x0043a130
CInputTextButton::CInputTextButton(const CVsRect& p_rect,
								   CPVGWnd* p_parent,
								   unsigned int p_controlMessage,
								   char* p_text)
	: CTextButton(p_rect, p_parent, RES_GAME_FONT1, 0x0c)
{
	m_ownedText = 0;
	SetOwnedText(p_text);
	m_messageHandler = g_pMasterInputQueue;
	m_controlMessage = p_controlMessage;
}

// FUNCTION: LEMBALL 0x0043a190
CInputTextButton::~CInputTextButton()
{
	if (m_ownedText != 0) {
		delete[] m_ownedText;
	}
}

// FUNCTION: LEMBALL 0x0043a1d0
void CInputTextButton::SetOwnedText(char* p_text)
{
	if (m_ownedText != 0) {
		delete[] m_ownedText;
	}
	m_ownedText = new char[strlen(p_text) + 1];
	strcpy(m_ownedText, p_text);
	SetText(m_ownedText, 0);
}
