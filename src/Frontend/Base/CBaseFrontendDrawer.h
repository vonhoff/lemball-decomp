#ifndef LEMBALL_FRONTEND_BASE_CBASEFRONTENDDRAWER_H
#define LEMBALL_FRONTEND_BASE_CBASEFRONTENDDRAWER_H

#include "../../Visos/Animation/CAnimsManager.h"
#include "../../Visos/Animation/CStaticAnim.h"
#include "../../Visos/Foundation/CBaseQueueHandler.h"
#include "../../Visos/Foundation/CVsPoint.h"
#include "../../Visos/Foundation/tagPRIMS.h"
#include "../../Visos/Graphics/CDrawer.h"
#include "../Support/CUserActionMessage.h"
#include "../Support/CoordPair.h"
#include "FlowProcesses.h"

class CGDI;
class CGunController;
class CHiliteController;
class CMain2DDisplay;
class CPlayThruAnim;
class CResBITMAP;
class CTextManager;
class CVsRect;
struct Message;
// SIZE 0x398
// VTABLE: LEMBALL 0x004978a8 CDrawer
// VTABLE: LEMBALL 0x00497898 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497894 CAnimsManager
class CBaseFrontendDrawer : public CDrawer, public CBaseQueueHandler, public CAnimsManager {
public:
	CBaseFrontendDrawer(CMain2DDisplay* p_display,
						CGDI* p_gdi,
						const CVsRect& p_rect,
						eFlowProcesses p_flowProcess,
						int p_resourceCapacity,
						int p_animCapacity,
						int p_zrleCapacity,
						int p_textPrimitiveCapacity,
						int p_maxStringLen);
	virtual int ProcessMsg(Message* p_message);          // vtable+0x08
	virtual ~CBaseFrontendDrawer();                      // vtable+0x00
	virtual void Draw(const CVsRect& p_rect);            // vtable+0x08
	virtual void OnSize(const CVsRect& p_rect);          // vtable+0x10
	virtual void Process();                              // vtable+0x1c
	virtual void ResetPrimitives();                      // vtable+0x20
	virtual int GetReturnState();                        // vtable+0x28
	virtual bool QuitYet();                              // vtable+0x2c
	virtual void OnDriverChange();                       // vtable+0x30
	virtual void Processing();                           // vtable+0x38
	virtual bool ProcessMessages(Message* p_message);    // vtable+0x3c
	virtual void Load() = 0;                             // vtable+0x40
	virtual void UnLoad() = 0;                           // vtable+0x44
	virtual void DrawAnims();                            // vtable+0x48
	virtual void DrawText();                             // vtable+0x4c
	virtual void DrawBackGround();                       // vtable+0x50
	virtual bool ConfirmedAction(eUserActions p_action); // vtable+0x54
	void Action(eUserActions p_action, eUserActionStages p_stage);
	void _DrawAnims();
	void _DrawBackGround();
	void _Load();
	void _UnLoad();
	void DrawFrame(CoordPair p_start, CoordPair p_end);
	void DrawFrame(CVsRect p_rect);
	void InitialiseBackBuffer();
	void LostConnection();
	void RemoteAction(eUserActions p_action, eUserActionStages p_stage);
	void ReplaceBackground();
	void Restart();
	void Setup();

	friend class CNetworkOptionsDrawer;
	friend class CIntroAnimDrawer;
	friend class CMainOptions1Drawer;
	friend class CMainOptions2Drawer;
	friend class CMain2DDisplay;
	friend class CPasswordDrawer;
	friend class CSuccFailDrawer;
	friend class CPreviewDrawer;

protected:
	CMain2DDisplay* m_display;             // 0x84
	CGDI* m_gdi;                           // 0x88
	unsigned int m_networkMode;            // 0x8c
	unsigned int m_actionPending;          // 0x90
	unsigned int m_startupPending;         // 0x94
	CTextManager* m_textManager;           // 0x98
	unsigned int m_loaded;                 // 0x9c
	int m_textPrimitiveCapacity;           // 0xa0
	int m_maxStringLen;                    // 0xa4
	unsigned int m_drawBackground;         // 0xa8
	unsigned int m_drawFrame;              // 0xac
	unsigned int m_drawSolid;              // 0xb0
	unsigned int m_desiredPalette;         // 0xb4
	unsigned int m_activePalette;          // 0xb8
	unsigned int m_createdAt;              // 0xbc
	CResBITMAP* m_backgroundBitmap;        // 0xc0
	CResBITMAP* m_tileBitmap;              // 0xc4
	tagPRIMS m_primitiveBundle[1];         // 0xc8
	int m_primitiveBank;                   // 0x328
	int m_mode;                            // 0x32c
	unsigned int m_ready;                  // 0x330
	CGunController* m_gunController;       // 0x334
	CHiliteController* m_hiliteController; // 0x338
	unsigned int m_ambientDelay;           // 0x33c
	unsigned int m_ambientUpdatedAt;       // 0x340
	char m_unknown344[8];                  // 0x344
	CPlayThruAnim* m_ambientAnim;          // 0x34c
	CStaticAnim m_staticAnim;              // 0x350
	unsigned int m_backBufferNeeded;       // 0x360
	unsigned int m_drawingBackBuffer;      // 0x364
	unsigned int m_backBufferReady;        // 0x368
	int m_framePrimitiveCount;             // 0x36c
	int m_flowProcess;                     // 0x370
	int m_returnState;                     // 0x374
	int m_quitYet;                         // 0x378
	short m_width;                         // 0x37c
	short m_height;                        // 0x37e
	CVsPoint m_animPosition;               // 0x380
	unsigned int m_chalkFontId;            // 0x384
	unsigned int m_topFrameAnimId;         // 0x388
	unsigned int m_sideFrameAnimId;        // 0x38c
	unsigned int m_bottomFrameAnimId;      // 0x390
	unsigned int m_ambientAnimId;          // 0x394
};

extern int g_nPendingEffectsVolume;
extern int g_nPendingMusicVolume;
extern int g_nMusicVolume;
extern int g_nEffectsVolume;
extern CBaseFrontendDrawer* g_pBaseFrontendDrawer;

// SYNTHETIC: LEMBALL 0x00447000
// CBaseFrontendDrawer::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004472f0
// CBaseFrontendDrawer::`vector deleting destructor'

#endif
