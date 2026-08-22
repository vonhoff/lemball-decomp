#ifndef LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETFRAMEDBUTTON_H
#define LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETFRAMEDBUTTON_H

#include "../../Common.h"
#include "../Graphics/DepressedButton.h" // complete type

// SIZE 0x118
// VTABLE: LEMBALL 0x00499838 primary
// VTABLE: LEMBALL 0x00499818 hot-area view at +0x90
class TargetFramedButton : public DepressedButton {
public:
	// No functions mapped yet.

private:
	Line* m_frameLine;         // 0x10c
	ClipRect* m_frameRects;    // 0x110
	unsigned int m_frameColor; // 0x114
};

#endif
