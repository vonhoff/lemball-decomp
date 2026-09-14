#include "TargetInputTextButton.h"

// FUNCTION: LEMBALL 0x0043a190
TargetInputTextButton::~TargetInputTextButton()
{
	if (m_ownedText != 0) {
		delete[] m_ownedText;
	}
}
