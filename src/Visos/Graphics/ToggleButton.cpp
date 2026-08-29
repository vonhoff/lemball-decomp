#include "ToggleButton.h"

// 68K 0x1011c9d0 OnReleased__13CToggleButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00443830
void ToggleButton::OnReleased(int p_flags)
{
	m_toggled = m_toggled ^ 1;
	m_enabled = m_toggled;
}

// 68K 0x1011ca18 OnPressed__13CToggleButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00443850
void ToggleButton::OnPressed(int p_flags)
{
	m_enabled = m_pressed ^ m_toggled;
}

// 68K 0x1011ca64 OnEnterButton__13CToggleButtonFv
// FUNCTION: LEMBALL 0x00443870
void ToggleButton::OnEnterButton()
{
	m_enabled = m_toggled ^ m_pressed;
}

// 68K 0x1011caa6 OnExitButton__13CToggleButtonFv
// FUNCTION: LEMBALL 0x00443890
void ToggleButton::OnExitButton()
{
	m_enabled = m_pressed ^ m_toggled;
}

// 68K 0x1011c7f6 __dt__13CToggleButtonFv
ToggleButton::~ToggleButton()
{
}
