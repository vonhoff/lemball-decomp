#ifndef LEMBALL_VISOS_GRAPHICS_CPVBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_CPVBUTTON_H

#include "CClipRect.h"       // complete type
#include "CGWnd.h"           // complete type
#include "CHotAreaHandler.h" // complete type
#include "CSolidRect.h"
class CPrimitive;
class CPVGWnd;
class CVsRect;

// SIZE 0x104
// VTABLE: LEMBALL 0x00499658 CGWnd
// VTABLE: LEMBALL 0x00499638 CHotAreaHandler
class CPVButton : public CGWnd, public CHotAreaHandler {
public:
	CPVButton(CPVGWnd* p_arg0);
	CPVButton(const CVsRect& p_arg0, CPVGWnd* p_arg1);
	int ConvertDoubleClick(int p_flags);
	virtual unsigned int GetStyle();                                       // vtable+0x64
	virtual void OnPaint(const CVsRect& p_rect);                           // vtable+0xa8
	virtual void Destroy();                                                // vtable+0x74
	virtual void _DrawButton();                                            // vtable+0xb8
	virtual void DrawButton() = 0;                                         // vtable+0xbc
	virtual void OnReleased(int p_flags) = 0;                              // vtable+0xc0
	virtual void OnPressed(int p_flags) = 0;                               // vtable+0xc4
	virtual void OnEnterButton() = 0;                                      // vtable+0xc8
	virtual void OnExitButton() = 0;                                       // vtable+0xcc
	virtual void Move(const CVsPoint& p_point);                            // vtable+0x38
	virtual void OnButtonUp(const CVsPoint& p_point, int p_flags);         // vtable+0x04
	virtual void OnButtonDown(const CVsPoint& p_point, int p_flags);       // hot-area vtable+0x08
	virtual void OnEnter();                                                // vtable+0x10
	virtual void OnExit();                                                 // vtable+0x14
	virtual void OnExternalButtonUp(const CVsPoint& p_point, int p_flags); // vtable+0x0c
	virtual void OnVisibilityChange();                                     // vtable+0x80
	virtual ~CPVButton();                                                  // vtable+0x00
	void CheckForceDraw();
	void Draw(unsigned int p_force);
	void Initialise();
	void SetAutoDraw(unsigned int p_enabled);
	void _OnReleased(int p_flags);
	void _OnPressed(int p_flags);
	void _OnEnterButton();
	void _OnExitButton();

	friend class CToggleButton;
	friend class CGraphicButton;
	friend class CGunButton;
	friend class CDepressedButton;
	friend class CFramedButton;
	friend class CTextButton;
	friend class CGunButtons;
	friend class CHiliteButtons;
	friend class CGunController;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CPanelButton;
	friend class CTrackerButton;

private:
	CPVGWnd* m_ownerWindow;          // 0xc8
	unsigned int m_controlMessage;   // 0xcc
	unsigned int m_pressed;          // 0xd0
	unsigned int m_lastDrawnPressed; // 0xd4
	unsigned int m_forceDrawCount;   // 0xd8
	short m_buttonX;                 // 0xdc
	short m_buttonY;                 // 0xde
	CPrimitive* m_primitive;         // 0xe0
	CSolidRect m_clipRect[1];        // 0xe4
	void* m_messageHandler;          // 0xf4
	unsigned int m_autoDraw;         // 0xf8
	unsigned int m_drawCompleted;    // 0xfc
	short m_clickX;                  // 0x100
	short m_clickY;                  // 0x102
};

// SYNTHETIC: LEMBALL 0x00469880
// CPVButton::`scalar deleting destructor'

#endif
