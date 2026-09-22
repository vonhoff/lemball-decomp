#include "CToggleButton.h"

// FUNCTION: LEMBALL 0x00443830
void CToggleButton::OnReleased(int p_flags)
{
	m_toggled = m_toggled ^ 1;
	m_enabled = m_toggled;
}

// FUNCTION: LEMBALL 0x00443850
void CToggleButton::OnPressed(int p_flags)
{
	m_enabled = m_pressed ^ m_toggled;
}

// FUNCTION: LEMBALL 0x00443870
void CToggleButton::OnEnterButton()
{
	m_enabled = m_toggled ^ m_pressed;
}

// FUNCTION: LEMBALL 0x00443890
void CToggleButton::OnExitButton()
{
	m_enabled = m_pressed ^ m_toggled;
}
