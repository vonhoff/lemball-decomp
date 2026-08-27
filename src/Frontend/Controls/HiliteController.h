#ifndef LEMBALL_FRONTEND_CONTROLS_HILITECONTROLLER_H
#define LEMBALL_FRONTEND_CONTROLS_HILITECONTROLLER_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h"      // complete type
#include "../../Visos/Animation/StaticAnim.h"        // complete type
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Graphics/SolidRect.h"          // complete type
#include "HiliteControllerJunction.h"                // complete type

// SIZE 0x14c
// VTABLE: LEMBALL 0x00498240
class HiliteController : public BaseQueueHandler {
public:
	HiliteController(GWnd* p_arg0, Gdi* p_arg1, int p_arg2, unsigned char p_arg3, unsigned char p_arg4);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~HiliteController();                // vtable+0x04
	void ActivateButtons(unsigned char p_active);
	void AddButton(int p_x,
				   int p_y,
				   unsigned long* p_animIds,
				   unsigned char p_mode,
				   int p_minimum,
				   int p_maximum,
				   int p_value,
				   void* p_binding,
				   unsigned long p_actionMessage);
	void AddHJunction(int p_x, int p_y, unsigned long p_controlMessage);
	void DrawButtons(unsigned char p_force);
	void DrawHiliteWindow();
	void MoveLeft();
	void MoveRight();
	void Process();
	void SetHilite(int p_buttonIndex);
	void SetHiliteWindow();
	void UpdateAnimIDs(unsigned long p_actionMessage);

	friend class NetworkOptionsDrawer;
	friend class BaseFrontendDrawer;

private:
	AnimsManager m_anims;                    // 0x10
	int m_buttonCount;                       // 0x80
	undefined m_navigationState[0x14];       // 0x84
	int m_currentX;                          // 0x98
	int m_currentY;                          // 0x9c
	int m_targetX;                           // 0xa0
	int m_targetY;                           // 0xa4
	int m_currentButton;                     // 0xa8
	HiliteButtons* m_buttons[4];             // 0xac
	HiliteControllerJunction m_junctions[4]; // 0xbc
	SolidRect m_hiliteRect;                  // 0xfc
	Gdi* m_gdi;                              // 0x10c
	GWnd* m_window;                          // 0x110
	unsigned int m_nextControlMessage;       // 0x114
	StaticAnim m_hiliteAnim;                 // 0x118
	HiliteWindow* m_hiliteWindow;            // 0x128
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
// HiliteController::`scalar deleting destructor'


#endif

