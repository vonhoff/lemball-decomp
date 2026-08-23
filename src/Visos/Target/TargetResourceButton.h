#ifndef LEMBALL_VISOS_TARGET_TARGETRESOURCEBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETRESOURCEBUTTON_H

#include "../../Common.h"
#include "TargetFramedButton.h" // complete type

// MINIMUM SIZE 0x11c
// VTABLE: LEMBALL 0x00499928 primary
// VTABLE: LEMBALL 0x00499908 hot-area view at +0x90
class TargetResourceButton : public TargetFramedButton {
private:
	void* m_resourceInterface; // 0x118
};

#endif
