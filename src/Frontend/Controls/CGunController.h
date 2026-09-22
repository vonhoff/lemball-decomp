#ifndef LEMBALL_FRONTEND_CONTROLS_CGUNCONTROLLER_H
#define LEMBALL_FRONTEND_CONTROLS_CGUNCONTROLLER_H

#include "../../Visos/Animation/CAnimsManager.h"      // complete type
#include "../../Visos/Animation/CStaticAnim.h"        // complete type
#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/Message.h"           // complete type
#include "../../Visos/Graphics/CSolidRect.h"          // complete type
#include "GunControllerJunction.h"                    // complete type

class CGWnd;
class CGdi;
class CGunButtons;
class CPlayThruAnim;
class CSpriteWindow;
class CVsRect;
// SIZE 0x27c
// VTABLE: LEMBALL 0x00497f10 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497f0c CAnimsManager
class CGunController : public CBaseQueueHandler, public CAnimsManager {
public:
	CGunController(CGWnd* p_arg0, CGdi* p_arg1, int p_arg2, unsigned int p_arg3);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~CGunController();                  // vtable+0x04
	void ActivateButtons(int p_active);
	void AddButton(int p_x,
				   int p_y,
				   unsigned long* p_animIds,
				   unsigned int p_postAction,
				   int p_minimum,
				   int p_maximum,
				   int p_value,
				   void* p_binding,
				   unsigned long p_actionMessage);
	void AddJunction(int p_x, int p_y, unsigned int p_side, unsigned long p_message);
	void DrawButtons(int p_firstState, int p_secondState);
	void DrawSpriteWindow();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void Process();
	void SelectOption();
	void SetGun(int p_junction);
	void SetGunPosition(int p_x, int p_y, int p_side);
	void SetSpriteWindow();
	void AddButtonWithRect(int p_x,
						   int p_y,
						   unsigned long* p_animIds,
						   unsigned int p_postAction,
						   unsigned int p_unusedFirst,
						   unsigned int p_unusedSecond,
						   int p_value,
						   int* p_binding,
						   const CVsRect& p_rect,
						   int p_actionMessage,
						   int p_context);

	friend class CBaseFrontendDrawer;

private:
	int m_buttonCount;                    // 0x80
	unsigned int m_controllerActive;      // 0x84
	int m_gunX;                           // 0x88
	int m_gunY;                           // 0x8c
	int m_moveStartY;                     // 0x90
	int m_currentSide;                    // 0x94
	int m_selectionStartX;                // 0x98
	int m_targetY;                        // 0x9c
	int m_targetSide;                     // 0xa0
	int m_selectionState;                 // 0xa4
	int m_projectileX;                    // 0xa8
	int m_projectileY;                    // 0xac
	int m_projectileTargetX;              // 0xb0
	unsigned int m_messageSent;           // 0xb4
	Message m_selectionMessage;           // 0xb8
	int m_projectileEndX;                 // 0xcc
	int m_projectileEndY;                 // 0xd0
	unsigned int m_inputReadyTime;        // 0xd4
	unsigned int m_selectedMessage;       // 0xd8
	unsigned int m_verticalMoving;        // 0xdc
	GunControllerJunction m_junctions[8]; // 0xe0
	CGunButtons* m_buttons[8];            // 0x1e0
	CSolidRect m_cursorRect[1];           // 0x200
	CGdi* m_gdi;                          // 0x210
	CGWnd* m_window;                      // 0x214
	unsigned int m_nextMessageId;         // 0x218
	unsigned int m_moveStartTime;         // 0x21c
	unsigned int m_moveEndTime;           // 0x220
	unsigned int m_sideStartTime;         // 0x224
	unsigned int m_sideEndTime;           // 0x228
	unsigned int m_selectStartTime;       // 0x22c
	unsigned int m_selectEndTime;         // 0x230
	unsigned int m_reserved234;           // 0x234
	unsigned int m_reserved238;           // 0x238
	unsigned int m_fireStartTime;         // 0x23c
	unsigned int m_fireEndTime;           // 0x240
	CPlayThruAnim* m_sideAnim;            // 0x244
	CPlayThruAnim* m_leftShotAnim;        // 0x248
	CPlayThruAnim* m_cursorAnim;          // 0x24c
	CPlayThruAnim* m_rightShotAnim;       // 0x250
	CPlayThruAnim* m_hitAnim;             // 0x254
	CStaticAnim m_staticAnim;             // 0x258
	CSpriteWindow* m_spriteWindow;        // 0x268
	CGdi* m_spriteSurface;                // 0x26c
	unsigned int m_mode;                  // 0x270
	unsigned int m_alternateAssets;       // 0x274
	unsigned int m_buttonsActive;         // 0x278
};

// SYNTHETIC: LEMBALL 0x0044e690
// CGunController::`scalar deleting destructor'

#endif
