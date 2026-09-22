#ifndef LEMBALL_VISOS_TARGET_TARGETINPUTTEXTBUTTON_H
#define LEMBALL_VISOS_TARGET_TARGETINPUTTEXTBUTTON_H

#include "TargetTextButton.h" // complete type

class CGWnd;
class CHotAreaHandler;
// MINIMUM SIZE 0x150
// VTABLE: LEMBALL 0x00497108 CGWnd
// VTABLE: LEMBALL 0x004970e0 CHotAreaHandler
class TargetInputTextButton : public TargetTextButton {
public:
	virtual ~TargetInputTextButton();
	void SetOwnedText(char* p_text);

private:
	char* m_ownedText; // 0x14c
};

// SYNTHETIC: LEMBALL 0x0043a450
// TargetInputTextButton::`scalar deleting destructor'

#endif
