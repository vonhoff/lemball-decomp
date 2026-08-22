#ifndef LEMBALL_VISOS_TARGET_TARGETTEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETTEXTBUTTON_H

#include "../../Common.h"
#include "TargetFramedButton.h" // complete type

// SIZE 0x14c
// VTABLE: LEMBALL 0x00499a18 primary
// VTABLE: LEMBALL 0x004999f8 hot-area view at +0x90
class TargetTextButton : public TargetFramedButton {
public:
	// No functions mapped yet.

private:
	char* m_normalText;                 // 0x118
	char* m_pressedText;                // 0x11c
	unsigned int m_reserved120;         // 0x120
	Text* m_textPrimitive;              // 0x124
	short m_pressedTextX;               // 0x128
	short m_pressedTextY;               // 0x12a
	short m_normalTextX;                // 0x12c
	short m_normalTextY;                // 0x12e
	unsigned int m_fontResourceId;      // 0x130
	ResFont* m_font;                    // 0x134
	unsigned int m_alignmentFlags;      // 0x138
	unsigned int m_nativeButtonCreated; // 0x13c
	short m_horizontalMargin;           // 0x140
	short m_verticalMargin;             // 0x142
	Remap* m_remap;                     // 0x144
	Remap* m_lastDrawnRemap;            // 0x148
};

#endif
