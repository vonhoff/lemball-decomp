#include "TargetTextButton.h"

#include "../Foundation/ChangeList.h"
#include "../Foundation/Text.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/VsGdi.h"
#include "../Resources/ResFont.h"

// FUNCTION: LEMBALL 0x00469120
void TargetTextButton::ExpandToFitText(const VsSize& p_textSize)
{
	if (m_horizontalMargin * m_verticalMargin != 0) {
		short width = (short) (p_textSize.m_width + 2 * m_horizontalMargin);
		short height = (short) (p_textSize.m_height + 2 * m_verticalMargin);
		if (m_width < width) {
			m_width = width;
		}
		if (m_height < height) {
			m_height = height;
		}
	}
}

// FUNCTION: LEMBALL 0x00469180
void TargetTextButton::AlignTextPosition(VsPoint& p_position, const VsSize& p_textSize)
{
	if ((m_alignmentFlags & 0x10) != 0) {
		p_position.m_x = (short) (m_width - p_textSize.m_width);
	}
	else if ((m_alignmentFlags & 4) != 0) {
		p_position.m_x = (short) ((m_width - p_textSize.m_width) / 2);
	}
	if ((m_alignmentFlags & 0x20) != 0) {
		p_position.m_y = (short) (m_height - p_textSize.m_height);
	}
	else if ((m_alignmentFlags & 8) != 0) {
		p_position.m_y = (short) ((m_height - p_textSize.m_height) / 2);
	}
	m_forceDrawCount = 1;
}

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

// FUNCTION: LEMBALL 0x00469440
TargetTextButton::~TargetTextButton()
{
	m_font->UnLoad();
	delete[] m_textPrimitive;
}

// FUNCTION: LEMBALL 0x00469480
void TargetTextButton::DrawButton()
{
	VsPoint position;
	char* text;
	bool depressed = m_pressed != 0 && HotAreaHandler::m_active != 0;
	if (!depressed) {
		position.m_x = m_normalTextX;
		text = m_normalText;
		position.m_y = m_normalTextY;
	}
	else {
		position.m_x = m_pressedTextX;
		text = m_pressedText;
		position.m_y = m_pressedTextY;
	}
	if (text != 0) {
		m_gdi->m_renderTarget->GetCurrDb();
		m_textPrimitive->Set(position, m_font, text, 0x20, m_remap);
		m_textPrimitive->Draw(m_gdi);
	}
}

// FUNCTION: LEMBALL 0x00469530
void TargetTextButton::OnPaint(const VsRect& p_rect)
{
	if (m_lastDrawnRemap != m_remap) {
		m_forceDrawCount = 1;
	}
	if (m_gdi->m_primitiveCount == 0 && (m_autoDraw != 0 || m_forceDrawCount != 0 || m_pressed != m_lastDrawnPressed)) {
		if (GetSizeStatus() != 0) {
			InternalDrawButton();
			TargetFramedButton::DrawButton();
			DrawButton();
		}
		ChangeList* changeList = m_gdi->m_renderTarget->GetChangeList();
		m_gdi->AddToList(m_primitive);
		changeList->Reset();
		m_drawCompleted = 1;
	}
}
