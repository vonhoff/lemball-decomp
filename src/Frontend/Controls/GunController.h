#ifndef LEMBALL_FRONTEND_CONTROLS_GUNCONTROLLER_H
#define LEMBALL_FRONTEND_CONTROLS_GUNCONTROLLER_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h"      // complete type
#include "../../Visos/Animation/StaticAnim.h"        // complete type
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Graphics/SolidRect.h"          // complete type
#include "GunControllerJunction.h"                   // complete type

// SIZE 0x27c
// VTABLE: LEMBALL 0x00497f10
class GunController : public BaseQueueHandler {
public:
	GunController(GWnd* p_arg0, Gdi* p_arg1, int p_arg2, unsigned char p_arg3);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~GunController();                   // vtable+0x04
	void ActivateButtons(unsigned char p_active);
	void AddButton(int p_x,
				   int p_y,
				   unsigned long* p_animIds,
				   unsigned char p_postAction,
				   int p_minimum,
				   int p_maximum,
				   int p_value,
				   void* p_binding,
				   unsigned long p_actionMessage);
	void AddJunction(int p_x, int p_y, unsigned char p_side, unsigned long p_message);
	void DrawButtons(unsigned char p_firstState, unsigned char p_secondState);
	void DrawSpriteWindow();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void Process();
	void SelectOption();
	void SetGun(int p_junction);
	void SetSpriteWindow();

	friend class BaseFrontendDrawer;

private:
	AnimsManager m_anims;                 // 0x10
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
	GunButtons* m_buttons[8];             // 0x1e0
	SolidRect m_cursorRect;               // 0x200
	Gdi* m_gdi;                           // 0x210
	GWnd* m_window;                       // 0x214
	unsigned int m_nextMessageId;         // 0x218
	unsigned int m_moveStartTime;         // 0x21c
	unsigned int m_moveEndTime;           // 0x220
	unsigned int m_sideStartTime;         // 0x224
	unsigned int m_sideEndTime;           // 0x228
	unsigned int m_selectStartTime;       // 0x22c
	unsigned int m_selectEndTime;         // 0x230
	undefined4 m_reserved234;             // 0x234
	undefined4 m_reserved238;             // 0x238
	unsigned int m_fireStartTime;         // 0x23c
	unsigned int m_fireEndTime;           // 0x240
	PlayThruAnim* m_sideAnim;             // 0x244
	PlayThruAnim* m_leftShotAnim;         // 0x248
	PlayThruAnim* m_cursorAnim;           // 0x24c
	PlayThruAnim* m_rightShotAnim;        // 0x250
	PlayThruAnim* m_hitAnim;              // 0x254
	StaticAnim m_staticAnim;              // 0x258
	SpriteWindow* m_spriteWindow;         // 0x268
	Gdi* m_spriteSurface;                 // 0x26c
	unsigned int m_mode;                  // 0x270
	unsigned int m_alternateAssets;       // 0x274
	unsigned int m_buttonsActive;         // 0x278
};

// SYNTHETIC: LEMBALL 0x0044e690
// GunController::`scalar deleting destructor'

#endif
