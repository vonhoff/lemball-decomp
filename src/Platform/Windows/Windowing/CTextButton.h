#ifndef LEMBALL_VISOS_TARGET_UI_CTEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_UI_CTEXTBUTTON_H

#include "CFramedButton.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSSize.h"

class CRemap;
class CResFONT;
class CText;
class CVSRect;
class CPVGWnd;

enum eTextButtonAlignmentFlags {
	TEXT_BUTTON_ALIGN_HORIZONTAL_CENTER = 0x04,
	TEXT_BUTTON_ALIGN_VERTICAL_CENTER = 0x08,
	TEXT_BUTTON_ALIGN_RIGHT = 0x10,
	TEXT_BUTTON_ALIGN_BOTTOM = 0x20
};
// SIZE 0x14c
// VTABLE: LEMBALL 0x00499a18 CGWnd
// VTABLE: LEMBALL 0x004999f8 CHotAreaHandler
class CTextButton : public CFramedButton {
public:
	CTextButton(const CVSRect& p_rect, CPVGWnd* p_parent, unsigned int p_fontResourceId, unsigned int p_alignmentFlags);
	void ExpandToFitText(const CVSSize& p_textSize);
	void AlignTextPosition(CVSPoint& p_position, const CVSSize& p_textSize);
	void SetText(char* p_normalText, char* p_pressedText);
	void Initialize();
	virtual ~CTextButton();
	virtual void DrawButton();
	virtual void OnPaint(const CVSRect& p_rect);

private:
	char* m_normalText;             // 0x118
	char* m_pressedText;            // 0x11c
	unsigned int m_reserved120;     // 0x120
	CText* m_textPrimitive;         // 0x124
	CVSPoint m_pressedTextPosition; // 0x128
	CVSPoint m_normalTextPosition;  // 0x12c
	unsigned int m_fontResourceId;  // 0x130
	CResFONT* m_font;               // 0x134
	unsigned int m_alignmentFlags;  // 0x138
	bool m_nativeButtonCreated;     // 0x13c
	CVSSize m_textMargins;          // 0x140
	CRemap* m_remap;                // 0x144
	CRemap* m_lastDrawnRemap;       // 0x148
};

// SYNTHETIC: LEMBALL 0x004699d0
// CTextButton::`scalar deleting destructor'

#endif
