#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTBUTTON_H

#include "TargetFramedButton.h" // complete type

class GWnd;
class HotAreaHandler;
class Remap;
class ResFont;
class Text;
class VsRect;
struct VsPoint;
struct VsSize;
// SIZE 0x14c
// VTABLE: LEMBALL 0x00499a18 GWnd
// VTABLE: LEMBALL 0x004999f8 HotAreaHandler
class TargetTextButton : public TargetFramedButton {
public:
	void ExpandToFitText(const VsSize& p_textSize);
	void AlignTextPosition(VsPoint& p_position, const VsSize& p_textSize);
	void SetText(char* p_normalText, char* p_pressedText);
	void Initialize();
	virtual ~TargetTextButton();
	virtual void DrawButton();
	virtual void OnPaint(const VsRect& p_rect);

private:
	char* m_normalText;                 // 0x118
	char* m_pressedText;                // 0x11c
	unsigned int m_reserved120;         // 0x120
	Text* m_textPrimitive;              // 0x124
	VsPoint m_pressedTextPosition;      // 0x128
	VsPoint m_normalTextPosition;       // 0x12c
	unsigned int m_fontResourceId;      // 0x130
	ResFont* m_font;                    // 0x134
	unsigned int m_alignmentFlags;      // 0x138
	unsigned int m_nativeButtonCreated; // 0x13c
	VsSize m_textMargins;               // 0x140
	Remap* m_remap;                     // 0x144
	Remap* m_lastDrawnRemap;            // 0x148
};

// SYNTHETIC: LEMBALL 0x004699d0
// TargetTextButton::`scalar deleting destructor'

#endif
