#include "TargetTextButton.h"

#include "../Foundation/ChangeList.h"
#include "../Foundation/Text.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/HotAreaList.h"
#include "../Graphics/PvGWnd.h"
#include "../Graphics/VsGdi.h"
#include "../Resources/ResFont.h"

extern char g_szButton[];

// FUNCTION: LEMBALL 0x00469120
void TargetTextButton::ExpandToFitText(const VsSize& p_textSize)
{
	if (m_textMargins.m_width * m_textMargins.m_height != 0) {
		short width = (short) (p_textSize.m_width + 2 * m_textMargins.m_width);
		short height = (short) (p_textSize.m_height + 2 * m_textMargins.m_height);
		if (m_bounds.m_width < width) {
			m_bounds.m_width = width;
		}
		if (m_bounds.m_height < height) {
			m_bounds.m_height = height;
		}
	}
}

// FUNCTION: LEMBALL 0x00469180
void TargetTextButton::AlignTextPosition(VsPoint& p_position, const VsSize& p_textSize)
{
	if ((m_alignmentFlags & 0x10) != 0) {
		p_position.m_x = (short) (m_bounds.m_width - p_textSize.m_width);
	}
	else if ((m_alignmentFlags & 4) != 0) {
		p_position.m_x = (short) ((m_bounds.m_width - p_textSize.m_width) / 2);
	}
	if ((m_alignmentFlags & 0x20) != 0) {
		p_position.m_y = (short) (m_bounds.m_height - p_textSize.m_height);
	}
	else if ((m_alignmentFlags & 8) != 0) {
		p_position.m_y = (short) ((m_bounds.m_height - p_textSize.m_height) / 2);
	}
	m_forceDrawCount = 1;
}

// FUNCTION: LEMBALL 0x00469210
void TargetTextButton::SetText(char* p_normalText, char* p_pressedText)
{
	m_pressedText = p_pressedText;
	m_normalText = p_normalText;
	short normalSizeStorage[2];
	VsSize* normalSize = (VsSize*) normalSizeStorage;
	m_font->GetSize(normalSize, p_normalText, 0x20);
	ExpandToFitText(*normalSize);
	VsSize pressedSize(*normalSize);
	if (m_pressedText != 0) {
		short sizeStorage[2];
		VsSize* size = m_font->GetSize((VsSize*) sizeStorage, m_pressedText, 0x20);
		pressedSize.m_width = size->m_width;
		pressedSize.m_height = size->m_height;
		ExpandToFitText(pressedSize);
	}
	AlignTextPosition(m_normalTextPosition, *normalSize);
	if (m_pressedText != 0) {
		AlignTextPosition(m_pressedTextPosition, pressedSize);
	}
	else {
		m_pressedText = m_normalText;
		m_pressedTextPosition.m_x = m_normalTextPosition.m_x;
		m_pressedTextPosition.m_y = m_normalTextPosition.m_y;
	}
	m_pressedTextPosition.m_x++;
	m_pressedTextPosition.m_y++;
	if (m_nativeButtonCreated == 0) {
		VsRect rect(m_buttonX, m_buttonY, m_bounds.m_width, m_bounds.m_height);
		Create(rect, m_ownerWindow, g_szButton);
		m_bounds.m_x += m_relativeTopLeft.m_x;
		m_bounds.m_y += m_relativeTopLeft.m_y;
		HotAreaHandler* area = this;
		m_ownerWindow->m_hotAreaList->AddToList(area);
		SetActive(1);
		m_nativeButtonCreated = 1;
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
		position.m_x = m_normalTextPosition.m_x;
		text = m_normalText;
		position.m_y = m_normalTextPosition.m_y;
	}
	else {
		position.m_x = m_pressedTextPosition.m_x;
		text = m_pressedText;
		position.m_y = m_pressedTextPosition.m_y;
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
