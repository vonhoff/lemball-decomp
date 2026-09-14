#include "TargetTextButton.h"

#include "../Foundation/Text.h"
#include "../Resources/ResFont.h"

// FUNCTION: LEMBALL 0x004693b0
void TargetTextButton::Initialize()
{
	m_textPrimitive = new Text[1];
	m_gdiFlags++;
	m_pressedText = 0;
	m_normalText = 0;
	m_reserved120 = 0;
	m_lastDrawnRemap = 0;
	m_remap = 0;
	m_font = ResFont::Load(m_fontResourceId);
	m_nativeButtonCreated = 0;
}
