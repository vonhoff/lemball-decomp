#ifndef LEMBALL_FRONTEND_CONTROLS_CGUNCONTROLLER_H
#define LEMBALL_FRONTEND_CONTROLS_CGUNCONTROLLER_H

#include "../../Engine/Animation/CAnimsManager.h"
#include "../../Engine/Animation/CStaticAnim.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Queues/Message.h"
#include "Engine/Graphics/Primitives/CClipRect.h"
#define GUN_SIDE_LEFT 0
#define GUN_SIDE_RIGHT 1
#define GUN_JUNCTION_LEFT GUN_SIDE_LEFT
#define GUN_JUNCTION_RIGHT GUN_SIDE_RIGHT
#define GUN_JUNCTION_BOTH 2
#define GUN_JUNCTION_UNASSIGNED 3

enum {
	GUN_JUNCTION_COORDINATE_UNASSIGNED = -1,
	GUN_JUNCTION_INDEX_NOT_FOUND = -1,
	GUN_CONTROLLER_ABOVE_TOP_BOUNDARY_Y = -1
};

// SIZE 0x20
struct GunControllerJunction {
	int m_leftX;                 // 0x00
	int m_y;                     // 0x04
	int m_rightX;                // 0x08
	int m_direction;             // 0x0c
	unsigned int m_leftMessage;  // 0x10
	unsigned int m_rightMessage; // 0x14
	void* m_leftBinding;         // 0x18
	void* m_rightBinding;        // 0x1c
};

class CGWnd;
class CGDI;
class CGunButtons;
class CPlayThruAnim;
class CSpriteWindow;
class CVSRect;

enum eGunSelectionState {
	GUN_SELECTION_IDLE = 0,
	GUN_SELECTION_TURNING = 1,
	GUN_SELECTION_AIMING = 2,
	GUN_SELECTION_FIRING = 3
};

enum eGunButtonPostAction {
	GUN_BUTTON_CYCLE_VALUE = 0U,
	GUN_BUTTON_POST_ACTION_MESSAGE = 1U
};

// SIZE 0x27c
// VTABLE: LEMBALL 0x00497f10 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497f0c CAnimsManager
class CGunController : public CBaseQueueHandler, public CAnimsManager {
public:
	CGunController(CGWnd* p_window, CGDI* p_gdi, int p_arg2, unsigned int p_mode);
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
						   const CVSRect& p_rect,
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
	eGunSelectionState m_selectionState;  // 0xa4
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
	CClipRect m_cursorRect[1];            // 0x200
	CGDI* m_gdi;                          // 0x210
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
	CGDI* m_spriteSurface;                // 0x26c
	unsigned int m_mode;                  // 0x270
	unsigned int m_alternateAssets;       // 0x274
	unsigned int m_buttonsActive;         // 0x278
};

// SYNTHETIC: LEMBALL 0x0044e690
// CGunController::`scalar deleting destructor'

#endif
