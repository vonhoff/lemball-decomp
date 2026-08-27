#ifndef LEMBALL_FRONTEND_BASE_BASEFRONTENDDRAWER_H
#define LEMBALL_FRONTEND_BASE_BASEFRONTENDDRAWER_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h"      // complete type
#include "../../Visos/Animation/StaticAnim.h"        // complete type
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/Prims.h"            // complete type
#include "../../Visos/Foundation/VsPoint.h"          // complete type
#include "../../Visos/Graphics/Drawer.h"             // complete type
#include "../Support/CoordPair.h"                    // complete type

// SIZE 0x398
// VTABLE: LEMBALL 0x004978a8
class BaseFrontendDrawer : public Drawer {
public:
	BaseFrontendDrawer(Main2DDisplay* p_arg0,
					   Gdi* p_arg1,
					   const VsRect& p_arg2,
					   eFlowProcesses p_arg3,
					   int p_arg4,
					   int p_arg5,
					   int p_arg6,
					   int p_arg7,
					   int p_arg8);
	bool ProcessMsg(Message* p_message);
	virtual ~BaseFrontendDrawer();                    // vtable+0x00
	virtual void Draw(const VsRect& p_rect);          // vtable+0x08
	virtual void OnSize(const VsRect& p_rect);        // vtable+0x10
	virtual void Process();                           // vtable+0x1c
	virtual void ResetPrimitives();                   // vtable+0x20
	virtual int GetReturnState();                     // vtable+0x28
	virtual bool QuitYet();                           // vtable+0x2c
	virtual void OnDriverChange();                    // vtable+0x30
	virtual void Processing();                        // vtable+0x38
	virtual bool ProcessMessages(Message* p_message); // vtable+0x3c
	virtual void Load() = 0;                         // vtable+0x40
	virtual void UnLoad() = 0;                       // vtable+0x44
	virtual void DrawAnims();                         // vtable+0x48
	virtual void DrawText();                          // vtable+0x4c
	virtual void DrawBackGround();                    // vtable+0x50
	virtual bool ConfirmedAction(int p_action);       // vtable+0x54
	void Action(int p_action, int p_stage);
	void _DrawAnims();
	void _DrawBackGround();
	void _Load();
	void _UnLoad();
	void DrawFrame(CoordPair p_start, CoordPair p_end);
	void DrawFrame(VsRect p_rect);
	void InitialiseBackBuffer();
	void LostConnection();
	void RemoteAction(int p_action, int p_stage);
	void ReplaceBackground();
	void Restart();
	void Setup();

	friend class NetworkOptionsDrawer;
	friend class IntroAnimDrawer;
	friend class MainOptions1Drawer;
	friend class Main2DDisplay;

private:
	BaseQueueHandler m_queueHandler;      // 0x04
	AnimsManager m_anims;                 // 0x14
	Main2DDisplay* m_display;             // 0x84
	Gdi* m_gdi;                           // 0x88
	unsigned int m_networkMode;           // 0x8c
	unsigned int m_actionPending;         // 0x90
	unsigned int m_startupPending;        // 0x94
	TextManager* m_textManager;           // 0x98
	unsigned int m_loaded;                // 0x9c
	int m_textCapacity;                   // 0xa0
	int m_textStyle;                      // 0xa4
	unsigned int m_drawBackground;        // 0xa8
	unsigned int m_drawFrame;             // 0xac
	unsigned int m_drawSolid;             // 0xb0
	unsigned int m_desiredPalette;        // 0xb4
	unsigned int m_activePalette;         // 0xb8
	unsigned int m_createdAt;             // 0xbc
	ResBitmap* m_backgroundBitmap;        // 0xc0
	ResBitmap* m_tileBitmap;              // 0xc4
	Prims m_primitiveBundle;              // 0xc8
	int m_primitiveBank;                  // 0x328
	int m_mode;                           // 0x32c
	unsigned int m_ready;                 // 0x330
	GunController* m_gunController;       // 0x334
	HiliteController* m_hiliteController; // 0x338
	unsigned int m_ambientDelay;          // 0x33c
	unsigned int m_ambientUpdatedAt;      // 0x340
	undefined m_unknown344[8];            // 0x344
	PlayThruAnim* m_ambientAnim;          // 0x34c
	StaticAnim m_staticAnim;              // 0x350
	unsigned int m_backBufferNeeded;      // 0x360
	unsigned int m_drawingBackBuffer;     // 0x364
	unsigned int m_backBufferReady;       // 0x368
	int m_framePrimitiveCount;            // 0x36c
	int m_flowProcess;                    // 0x370
	int m_returnState;                    // 0x374
	int m_quitYet;                        // 0x378
	short m_width;                        // 0x37c
	short m_height;                       // 0x37e
	VsPoint m_animPosition;               // 0x380
	undefined4 m_unknown384;              // 0x384
	unsigned int m_topFrameAnimId;        // 0x388
	unsigned int m_sideFrameAnimId;       // 0x38c
	unsigned int m_bottomFrameAnimId;     // 0x390
	unsigned int m_ambientAnimId;         // 0x394
};

extern int g_nPendingEffectsVolume;
extern int g_nPendingMusicVolume;
extern int g_nMusicVolume;
extern int g_nEffectsVolume;

// SYNTHETIC: LEMBALL 0x00447000
// BaseFrontendDrawer::`scalar deleting destructor'

#endif

