#ifndef LEMBALL_VISOS_GRAPHICS_CPVBUTTON_H
#define LEMBALL_VISOS_GRAPHICS_CPVBUTTON_H

#include "CGWnd.h"
#include "Engine/Graphics/Primitives/CClipRect.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Queues/tagMESSAGE.h"

class CBaseQueue;
class CPrimitive;
class CPVGWnd;
class CVSRect;

// SIZE 0x104
// VTABLE: LEMBALL 0x00499658 CGWnd
// VTABLE: LEMBALL 0x00499638 CHotAreaHandler
class CPVButton : public CGWnd, public CHotAreaHandler {
public:
	CPVButton(CPVGWnd* p_ownerWindow);
	CPVButton(const CVSRect& p_bounds, CPVGWnd* p_ownerWindow);
	BUTTON_FLAGS ConvertDoubleClick(BUTTON_FLAGS p_flags);
	virtual unsigned int GetStyle();                                        // vtable+0x64
	virtual void OnPaint(const CVSRect& p_rect);                            // vtable+0xa8
	virtual void Destroy();                                                 // vtable+0x74
	virtual void _DrawButton();                                             // vtable+0xb8
	virtual void DrawButton() = 0;                                          // vtable+0xbc
	virtual void OnReleased(BUTTON_FLAGS p_flags) = 0;                      // vtable+0xc0
	virtual void OnPressed(BUTTON_FLAGS p_flags) = 0;                       // vtable+0xc4
	virtual void OnEnterButton() = 0;                                       // vtable+0xc8
	virtual void OnExitButton() = 0;                                        // vtable+0xcc
	virtual void Move(const CVSPoint& p_point);                             // vtable+0x38
	virtual void OnButtonUp(const CVSPoint& p_point, BUTTON_FLAGS p_flags); // vtable+0x04
	virtual void OnButtonDown(const CVSPoint& p_point, BUTTON_FLAGS p_flags);
	virtual void OnEnter();                                                         // vtable+0x10
	virtual void OnExit();                                                          // vtable+0x14
	virtual void OnExternalButtonUp(const CVSPoint& p_point, BUTTON_FLAGS p_flags); // vtable+0x0c
	virtual void OnVisibilityChange();                                              // vtable+0x80
	virtual ~CPVButton();                                                           // vtable+0x00
	void CheckForceDraw();
	void Draw(unsigned int p_force);
	void Initialise();
	void SetAutoDraw(unsigned int p_enabled);
	void _OnReleased(BUTTON_FLAGS p_flags);
	void _OnPressed(BUTTON_FLAGS p_flags);
	void _OnEnterButton();
	void _OnExitButton();

	friend class CToggleButton;
	friend class CGraphicButton;
	friend class CGunButton;
	friend class CDepressedButton;
	friend class CFramedButton;
	friend class CTextButton;
	friend class CInputTextButton;
	friend class CStateTextButton;
	friend class CMessageTextButton;
	friend class CGunButtons;
	friend class CHiliteButtons;
	friend class CGunController;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CPanelButton;
	friend class CTrackerButton;

private:
	CPVGWnd* m_ownerWindow;        // 0xc8
	unsigned int m_controlMessage; // 0xcc
	bool m_pressed;                // 0xd0
	bool m_lastDrawnPressed;       // 0xd4
	unsigned int m_forceDrawCount; // 0xd8
	CVSPoint m_buttonPosition;     // 0xdc
	CPrimitive* m_primitive;       // 0xe0
	CClipRect m_clipRect[1];       // 0xe4
	CBaseQueue* m_messageQueue;    // 0xf4
	bool m_autoDraw;               // 0xf8
	bool m_drawCompleted;          // 0xfc
	CVSPoint m_clickPosition;      // 0x100
};

// SYNTHETIC: LEMBALL 0x00469880
// CPVButton::`scalar deleting destructor'

#endif
