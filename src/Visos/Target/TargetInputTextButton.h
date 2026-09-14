#ifndef LEMBALL_VISOS_TARGET_TARGETINPUTTEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETINPUTTEXTBUTTON_H

#include "../../Common.h"
#include "TargetTextButton.h" // complete type

// MINIMUM SIZE 0x150
// VTABLE: LEMBALL 0x00497108 GWnd
// VTABLE: LEMBALL 0x004970e0 HotAreaHandler
class TargetInputTextButton : public TargetTextButton {
public:
	virtual ~TargetInputTextButton();

private:
	char* m_ownedText; // 0x14c
};

// SYNTHETIC: LEMBALL 0x0043a450
// TargetInputTextButton::`scalar deleting destructor'

#endif
