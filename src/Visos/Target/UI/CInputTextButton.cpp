#include "CInputTextButton.h"

#include <string.h>

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
