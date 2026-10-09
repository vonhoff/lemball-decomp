#ifndef LEMBALL_FRONTEND_CONTROLS_CHILITECONTROLLER_H
#define LEMBALL_FRONTEND_CONTROLS_CHILITECONTROLLER_H

#include "../../Engine/Animation/CAnimsManager.h"
#include "../../Engine/Animation/CStaticAnim.h"
#include "Engine/Graphics/Primitives/CClipRect.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Queues/tagMESSAGE.h"
// SIZE 0x10
struct HiliteControllerJunction {
	unsigned int m_present;        // 0x00
	int m_x;                       // 0x04
	int m_y;                       // 0x08
	unsigned int m_controlMessage; // 0x0c
};

class CGWnd;
class CGDI;
class CHiliteButtons;
class CHiliteWindow;

enum eHiliteButtonMode {
	HILITE_BUTTON_MODE_VALUE = 0,
	HILITE_BUTTON_MODE_ACTION_MESSAGE = 1
};
// SIZE 0x14c
// VTABLE: LEMBALL 0x00498240 CBaseQueueHandler
// VTABLE: LEMBALL 0x0049823c CAnimsManager
class CHiliteController : public CBaseQueueHandler, public CAnimsManager {
public:
	CHiliteController(CGWnd* p_window,
					  CGDI* p_gdi,
					  int p_arg2,
					  unsigned int p_layoutMode,
					  unsigned int p_horizontalMode);
	virtual int ProcessMsg(tagMESSAGE* p_message); // vtable+0x08
	virtual ~CHiliteController();               // vtable+0x04
	void ActivateButtons(int p_active);
	void AddButton(int p_x,
				   int p_y,
				   unsigned long* p_animIds,
				   unsigned int p_mode,
				   int p_minimum,
				   int p_maximum,
				   int p_value,
				   void* p_binding,
				   unsigned long p_actionMessage);
	void AddHJunction(int p_x, int p_y, unsigned long p_controlMessage);
	void DrawButtons(int p_force);
	void DrawHiliteWindow();
	void MoveLeft();
	void MoveRight();
	void Process();
	void SetHilite(int p_buttonIndex);
	void SetHiliteWindow();
	void UpdateAnimIDs(unsigned long p_actionMessage);
	void PostSelectionMessage();
	void UpdateAllAnimIDs();

	friend class CNetworkOptionsDrawer;
	friend class CBaseFrontendDrawer;

private:
	int m_buttonCount;                       // 0x80
	tagMESSAGE m_navigationState;            // 0x84
	int m_currentX;                          // 0x98
	int m_currentY;                          // 0x9c
	int m_targetX;                           // 0xa0
	int m_targetY;                           // 0xa4
	int m_currentButton;                     // 0xa8
	CHiliteButtons* m_buttons[4];            // 0xac
	HiliteControllerJunction m_junctions[4]; // 0xbc
	CClipRect m_hiliteRect;                  // 0xfc
	CGDI* m_gdi;                             // 0x10c
	CGWnd* m_window;                         // 0x110
	unsigned int m_nextControlMessage;       // 0x114
	CStaticAnim m_hiliteAnim;                // 0x118
	CHiliteWindow* m_hiliteWindow;           // 0x128
	void* m_hiliteSurface;                   // 0x12c
	unsigned int m_layoutMode;               // 0x130
	unsigned int m_horizontalMode;           // 0x134
	unsigned int m_animationSet;             // 0x138
	unsigned long m_transitionStart;         // 0x13c
	unsigned long m_transitionEnd;           // 0x140
	unsigned int m_active;                   // 0x144
	unsigned int m_buttonsActive;            // 0x148
};

// SYNTHETIC: LEMBALL 0x0044fff0
// CHiliteController::`scalar deleting destructor'

#endif
