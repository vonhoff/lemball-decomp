#ifndef LEMBALL_FRONTEND_CONTROLS_CHILITECONTROLLER_H
#define LEMBALL_FRONTEND_CONTROLS_CHILITECONTROLLER_H

#include "../../Visos/Animation/CAnimsManager.h"      // complete type
#include "../../Visos/Animation/CStaticAnim.h"        // complete type
#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/Message.h"           // complete type
#include "../../Visos/Graphics/CSolidRect.h"          // complete type
#include "HiliteControllerJunction.h"                 // complete type

class CGWnd;
class CGDI;
class CHiliteButtons;
class CHiliteWindow;
// SIZE 0x14c
// VTABLE: LEMBALL 0x00498240 CBaseQueueHandler
// VTABLE: LEMBALL 0x0049823c CAnimsManager
class CHiliteController : public CBaseQueueHandler, public CAnimsManager {
public:
	CHiliteController(CGWnd* p_arg0, CGDI* p_arg1, int p_arg2, unsigned int p_arg3, unsigned int p_arg4);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
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
	Message m_navigationState;               // 0x84
	int m_currentX;                          // 0x98
	int m_currentY;                          // 0x9c
	int m_targetX;                           // 0xa0
	int m_targetY;                           // 0xa4
	int m_currentButton;                     // 0xa8
	CHiliteButtons* m_buttons[4];            // 0xac
	HiliteControllerJunction m_junctions[4]; // 0xbc
	CSolidRect m_hiliteRect;                 // 0xfc
	CGDI* m_gdi;                             // 0x10c
	CGWnd* m_window;                         // 0x110
	unsigned int m_nextControlMessage;       // 0x114
	CStaticAnim m_hiliteAnim;                // 0x118
	CHiliteWindow* m_hiliteWindow;           // 0x128
	void* m_hiliteSurface;                   // 0x12c
	unsigned int m_layoutMode;               // 0x130
	unsigned int m_horizontalMode;           // 0x134
	unsigned int m_animationSet;             // 0x138
	unsigned int m_transitionStart;          // 0x13c
	unsigned int m_transitionEnd;            // 0x140
	unsigned int m_active;                   // 0x144
	unsigned int m_buttonsActive;            // 0x148
};

// SYNTHETIC: LEMBALL 0x0044fff0
// CHiliteController::`scalar deleting destructor'

#endif
