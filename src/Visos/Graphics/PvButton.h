#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_PVBUTTON_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_PVBUTTON_H

#include "../../Common.h"

// SIZE 0x104
// VTABLE: LEMBALL 0x00499658 primary
// VTABLE: LEMBALL 0x00499638 adjusted hot-area subobject at +0x90
class PvButton {
public:
	PvButton(PvGWnd* p_arg0);
	PvButton(const VsRect& p_arg0, PvGWnd* p_arg1);
	static int ConvertDoubleClick(int p_flags);
	virtual unsigned int GetStyle();                                      // vtable+0x64
	virtual unsigned int OnPaint(const VsRect& p_rect);                   // vtable+0xa8
	virtual void Destroy();                                               // vtable+0x74
	virtual void DrawButton();                                            // vtable+0xb8
	virtual void Move(const VsPoint& p_point);                            // vtable+0x38
	virtual void OnButtonDown(const VsPoint& p_point, int p_flags);       // vtable+0x04
	virtual void OnButtonUp(const VsPoint& p_point, int p_flags);         // vtable+0x08
	virtual void OnEnter();                                               // vtable+0x10
	virtual void OnEnterButton();                                         // vtable+0xc8
	virtual void OnExit();                                                // vtable+0x14
	virtual void OnExitButton();                                          // vtable+0xcc
	virtual void OnExternalButtonUp(const VsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnPressed(int p_flags);                                  // vtable+0xc4
	virtual void OnReleased(int p_flags);                                 // vtable+0xc0
	virtual void OnVisibilityChange();                                    // vtable+0x80
	virtual ~PvButton();                                                  // vtable+0x00
	void CheckForceDraw();
	void Draw(unsigned char p_force);
	void Initialise();
	void SetAutoDraw(unsigned char p_enabled);
	PvButton();

private:
	unsigned int m_pressed;          // 0xd0
	unsigned int m_lastDrawnPressed; // 0xd4
	unsigned int m_forceDrawCount;   // 0xd8
	short m_buttonX;                 // 0xdc
	short m_buttonY;                 // 0xde
	Primitive* m_primitive;          // 0xe0
	undefined m_drawState[0x10];     // 0xe4
	void* m_messageHandler;          // 0xf4
	unsigned int m_autoDraw;         // 0xf8
	unsigned int m_drawCompleted;    // 0xfc
	short m_clickX;                  // 0x100
	short m_clickY;                  // 0x102
};

#endif
