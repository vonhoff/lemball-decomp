#ifndef LEMBALL_FRONTEND_DRAWERS_PASSWORDDRAWER_H
#define LEMBALL_FRONTEND_DRAWERS_PASSWORDDRAWER_H

#include "../../Common.h"
#include "../../Visos/Animation/StaticAnim.h" // complete type
#include "../../Visos/Graphics/SolidRect.h"   // complete type

// SIZE 0x494
// VTABLE: LEMBALL 0x00498408
class PasswordDrawer {
public:
	PasswordDrawer(Main2DDisplay* p_arg0, Gdi* p_arg1, const VsRect& p_arg2);
	unsigned int ButtonNumeric(int p_button);
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void DrawAnims();                         // vtable+0x48
	virtual void DrawBackGround();                    // vtable+0x50
	virtual void DrawText();                          // vtable+0x4c
	virtual void Load();                              // vtable+0x40
	virtual void Processing();                        // vtable+0x38
	virtual void UnLoad();                            // vtable+0x44
	virtual ~PasswordDrawer();                        // vtable+0x00
	void DrawButtons();
	void DrawHilite();
	void DrawPassword();
	void SetHiliteWindow();
	void ShiftHilite(int p_delta);

private:
	unsigned int m_animationId;           // 0x398
	undefined m_unknown39c[8];            // 0x39c
	PasswordHiliteWindow* m_hiliteWindow; // 0x3a4
	void* m_layout;                       // 0x3a8
	void* m_buttonAnimIds;                // 0x3ac
	char m_password[16];                  // 0x3b0
	int m_buttonOffsets[24];              // 0x3c0
	int m_passwordLength;                 // 0x420
	PvButton* m_buttons[12];              // 0x424
	SolidRect m_hiliteRect;               // 0x454
	StaticAnim m_hiliteAnim;              // 0x464
	void* m_hiliteSurface;                // 0x474
	int m_hiliteX;                        // 0x478
	int m_hiliteY;                        // 0x47c
	int m_selectedButton;                 // 0x480
	unsigned int m_passwordValid;         // 0x484
	unsigned int m_passwordSubmitted;     // 0x488
	unsigned int m_submitTime;            // 0x48c
	unsigned int m_returnDeadline;        // 0x490
};

#endif
