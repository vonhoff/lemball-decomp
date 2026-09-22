#ifndef LEMBALL_VIEWS_DISPLAY_C2D_H
#define LEMBALL_VIEWS_DISPLAY_C2D_H

#include "../../AI/Base/AiCoord.h"                      // complete type
#include "../../AI/Objects/CViewData.h"                 // complete type
#include "../../Visos/Foundation/CBaseQueueHandler.h"   // complete type
#include "../../Visos/Foundation/CVsPoint.h"            // complete type
#include "../../Visos/Foundation/CVsRect.h"             // complete type
#include "../../Visos/Graphics/CBigBitmap.h"            // complete type
#include "../../Visos/Graphics/CBitmap.h"               // complete type
#include "../../Visos/Graphics/CCopyColourToBackBuff.h" // complete type
#include "../../Visos/Graphics/CCopyToBackBuff.h"       // complete type
#include "../../Visos/Graphics/CDrawer.h"               // complete type
#include "../../Visos/Graphics/CDrawingMark.h"          // complete type
#include "../../Visos/Graphics/CHotAreaHandler.h"       // complete type
#include "../../Visos/Graphics/CLine.h"                 // complete type
#include "../../Visos/Graphics/CPopActive.h"            // complete type
#include "../../Visos/Graphics/CPushActive.h"           // complete type
#include "../../Visos/Graphics/CReceiveWindowState.h"   // complete type
#include "../../Visos/Graphics/CScreenScroll.h"         // complete type
#include "../../Visos/Graphics/CSolidRect.h"            // complete type
#include "../Pause/CPauseWindow.h"
#include "AI/Base/ObjectTypes.h"

class CAI;
class CBaseQueue;
class CBaseRemap;
class CGameObject;
class CGDI;
class CLemmingAnimsManager;
class CMain2DDisplay;
class CMap;
class CPadToButton;
class CPanel;
class CPlayerLemming;
class CTextManager;
struct AnimSpecialEntry;
struct ObjectClipGrid;
struct SpriteGroundLookup;
// SIZE 0x2428
// VTABLE: LEMBALL 0x00496df8 CDrawer
// VTABLE: LEMBALL 0x00496de8 CBaseQueueHandler
// VTABLE: LEMBALL 0x00496dc0 CHotAreaHandler
// VTABLE: LEMBALL 0x00496db0 CReceiveWindowState
class C2D : public CDrawer, public CBaseQueueHandler, public CHotAreaHandler, public CReceiveWindowState {
public:
	C2D(CMain2DDisplay* p_display, CAI* p_ai, CGDI* p_gdi, CMap* p_map, const CVsRect& p_rect);
	bool FindGameObject(const CVsPoint& p_point, int& p_index, int p_preferLemming);
	bool InGroupByObjectNo(int p_objectNo);
	bool IsInGrouping(CGameObject* p_object);
	bool ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY);
	unsigned short CalcZValue_Sprite(int p_index);
	unsigned long LemmingFly(CViewData& p_viewData, int& p_frame);
	unsigned short CalcGroundCode(eObjectType p_objectType, int p_x, int p_y, unsigned short p_z);
	void UseBalloon(int p_playerIndex);
	virtual bool GetPauser();                                        // vtable+0x04
	virtual bool QuitYet();                                          // vtable+0x2c
	virtual int GetReturnState();                                    // vtable+0x28
	virtual int ProcessMsg(Message* p_message);                      // vtable+0x08
	virtual void OnButtonDown(const CVsPoint& p_point, int p_flags); // vtable+0x04
	virtual void OnButtonUp(const CVsPoint& p_point, int p_flags);   // vtable+0x08
	virtual void OnDriverChange();                                   // vtable+0x30
	virtual void OnInside(const CVsPoint& p_point);                  // vtable+0x18
	virtual void OnSize(const CVsRect& p_rect);                      // vtable+0x10
	virtual void OnZoom(const CVsRect& p_rect);                      // vtable+0x0c
	virtual void Process();                                          // vtable+0x1c
	virtual void ShutDown();                                         // vtable+0x04
	virtual ~C2D();                                                  // vtable+0x00
	void AddObjectToGroup(int p_objectNo, int p_markSelection);
	void AddViewIndexToObjectClipGrid(int p_x, int p_y, int p_viewIndex, int p_groundHeight, int p_adjustForGround);
	void BuildObjectClipData(CViewData& p_viewData, int p_viewIndex);
	void CancelMoves();
	void CheckValidFormGroup();
	void CursorChangeType(int p_cursorType, int p_value);
	void DoButtons();
	void DoClipWidth(int p_mapX, int p_mapY, int p_count);
	void DoClipWidthSearch(int p_mapX, int p_mapY, int p_count);
	void Draw(const CVsRect& p_rect);
	void DrawAmmo(CViewData& p_viewData, int p_objectNo);
	void DrawBall(CViewData& p_viewData);
	void DrawBalloon(CViewData& p_viewData, int p_playerIndex);
	void DrawBalloonPost(CViewData& p_viewData, int p_playerIndex);
	void DrawBonus(CViewData& p_viewData);
	void DrawBullet(CViewData& p_viewData, int p_objectNo);
	void DrawCatapult(CViewData& p_viewData, int p_objectNo);
	void DrawCliff(int p_x, int p_y, int p_height, int p_count);
	int DrawClipData();
	int DrawClippedRectangle(const CVsRect& p_rect);
	void DrawCrate(CViewData& p_viewData, int p_objectNo);
	void DrawDemo();
	void DrawDoor(CViewData& p_viewData);
	void DrawDuplicator(CViewData& p_viewData);
	void DrawFlag(CViewData& p_viewData, eObjectType p_objectType);
	void DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame);
	void DrawHand(CViewData& p_viewData);
	void DrawKey(CViewData& p_viewData, int p_playerIndex);
	void DrawLaser(CViewData& p_viewData);
	void DrawLaserFire(CViewData& p_viewData);
	void DrawLemming(CViewData& p_viewData, int p_objectNo, unsigned int p_remapped);
	void DrawLemmingExternal(CViewData& p_viewData, unsigned int p_remapped);
	void DrawLemmingFall(CViewData& p_viewData, unsigned int p_remapped);
	void DrawLemmingFlyShadow(CViewData& p_viewData);
	void DrawLemmingJump(CViewData& p_viewData, unsigned int p_remapped);
	void DrawLemmingLanding(CViewData& p_viewData, unsigned int p_remapped);
	void DrawLemmingOnBalloon(CViewData& p_viewData, int p_balloonType, int p_remapped);
	void DrawLemmingOnConveyor(CViewData& p_viewData, int p_remapped);
	void DrawMine(CViewData& p_viewData);
	void DrawMover(CViewData& p_viewData);
	void DrawObject(CViewData& p_viewData);
	void DrawObjects();
	void DrawObjectsZBuff();
	void DrawPaintGun(CViewData& p_viewData);
	void DrawPaused();
	void DrawRocket(CViewData& p_viewData);
	void DrawScore();
	void DrawSheep(CViewData& p_viewData, int p_objectNo);
	void DrawSlinky(CViewData& p_viewData);
	void DrawSwitch(CViewData& p_viewData);
	void DrawTime();
	void DrawTimeBonus(CViewData& p_viewData);
	void DrawTrampoline(CViewData& p_viewData);
	void DrawTrapDoor(CViewData& p_viewData);
	void DrawZBuff_Anim(int p_index, unsigned short p_z);
	void DrawZBuff_Sprite(int p_index, unsigned short p_z);
	void FormGroup();
	void GroupingLeftClick(const CVsPoint& p_screenPoint, const CVsPoint& p_gamePoint, unsigned int p_alternate);
	void InitSpriteGroundLU();
	void KillRemapPalettes();
	void LeftClick(const CVsPoint& p_screenPoint,
				   const CVsPoint& p_gamePoint,
				   unsigned int p_cancelMoves,
				   unsigned int p_alternate);
	void MoveGroup(const CVsPoint& p_point);
	void NewPauseWindow(ePauseWindowMessages p_message);
	void NextGroup();
	void NoStateLeftClick(const CVsPoint& p_screenPoint,
						  const CVsPoint& p_gamePoint,
						  unsigned int p_cancelMoves,
						  unsigned int p_alternate);
	void NoStateRightClick(const CVsPoint& p_screenPoint, const CVsPoint& p_gamePoint);
	void OnLoaded();
	void PrevGroup();
	void RegisterRemaps();
	void RemoveFromGroupByObjectNo(int p_objectNo);
	void ReplaceBackground();
	void ResetPrimitives();
	void Restart();
	void RightClick(const CVsPoint& p_screenPoint, const CVsPoint& p_gamePoint);
	void SelectLemming(int p_playerIndex);
	void SelectObject(int p_viewIndex);
	void SendCursorMsg();
	void SetClipSize();
	void SetMouseShape();
	void SetOrigin();
	void SetPause(unsigned int p_paused);
	void SetUpRemapPalettes();
	void SortViewData();
	void TriggerPause(unsigned char p_paused);
	void UnRegisterRemaps();
	void UpdateSpriteGroundLookupRegions();
	void UseBalloon(CPlayerLemming* p_lemming);
	CVsRect* GetClipRectangle();
	friend class CPanel;
	friend class CPanelLemming;
	friend class CPanelPauseButton;
	friend class CPanelButton;

private:
	unsigned char* m_remapTables[4];              // 0x54
	CBaseRemap* m_remaps[5];                      // 0x64
	CTextManager* m_textManager;                  // 0x78
	CViewData m_unk0x7c;                          // 0x7c
	CViewData m_unk0xc8;                          // 0xc8
	CViewData m_unk0x114;                         // 0x114
	unsigned short m_zoom;                        // 0x160
	unsigned short m_groundWidth;                 // 0x162
	unsigned short m_groundHeight;                // 0x164
	short m_groundAnimationFrame;                 // 0x166
	char m_pad0x168[8];                           // 0x168
	int m_clipMapStepX;                           // 0x170
	int m_clipMapStepY;                           // 0x174
	char m_pad0x178[0x0c];                        // 0x178
	int m_clipScreenX;                            // 0x184
	int m_clipScreenY;                            // 0x188
	unsigned int m_mouseButtonDown;               // 0x18c
	unsigned int m_frameCount;                    // 0x190
	unsigned int m_frameTime;                     // 0x194
	unsigned int m_levelTestFrame;                // 0x198
	char m_pad0x19c[8];                           // 0x19c
	unsigned int m_cursorState;                   // 0x1a4
	unsigned int m_cursorTimestamp;               // 0x1a8
	unsigned short m_cursorBlinkPhase;            // 0x1ac
	char m_pad0x1ae[0x71a];                       // 0x1ae
	unsigned int m_pad0x8c8;                      // 0x8c8
	unsigned int m_pad0x8cc;                      // 0x8cc
	unsigned short m_clipSearchHeight;            // 0x8d0
	CVsRect m_spriteGroundLookupRectA;            // 0x8d2
	CVsRect m_spriteGroundLookupRectB;            // 0x8da
	CVsRect m_demoTextRect;                       // 0x8e2
	char m_pad0x8ea[2];                           // 0x8ea
	unsigned int m_zBufferEnabled;                // 0x8ec
	int m_score;                                  // 0x8f0
	int m_levelScore;                             // 0x8f4
	unsigned int m_scoreTimestamp;                // 0x8f8
	int m_returnState;                            // 0x8fc
	CPushActive m_pushActive;                     // 0x900
	CPopActive m_popActive;                       // 0x908
	unsigned int m_viewOrientation;               // 0x90c
	unsigned int m_pad0x910;                      // 0x910
	CMap* m_map;                                  // 0x914
	int m_viewOriginX;                            // 0x918
	int m_viewOriginY;                            // 0x91c
	unsigned int m_pad0x920;                      // 0x920
	char m_pad0x924[8];                           // 0x924
	CPadToButton* m_padToButton;                  // 0x92c
	SpriteGroundLookup* m_spriteGroundLookup;     // 0x930
	ObjectClipGrid* m_objectClipGrid;             // 0x934
	CVsPoint m_spriteGroundTranslationPoint;      // 0x938
	CVsPoint m_cursorGamePoint;                   // 0x93c
	CVsPoint m_viewSize;                          // 0x940
	AiCoord m_originPosition;                     // 0x944
	int m_clipOffsetX;                            // 0x950
	int m_clipOffsetY;                            // 0x954
	CVsPoint m_clipSize;                          // 0x958
	CViewData* m_viewData;                        // 0x95c
	unsigned char* m_zBuffer;                     // 0x960
	unsigned short m_viewDataCount;               // 0x964
	CBaseRemap* m_paletteRemap;                   // 0x968
	CAI* m_ai;                                    // 0x96c
	CGDI* m_gdi;                                  // 0x970
	CBaseQueue* m_lemmingManager;                 // 0x974
	CMain2DDisplay* m_display;                    // 0x978
	CPanel* m_panel;                              // 0x97c
	CPauseWindow* m_pauseWindow;                  // 0x980
	unsigned int m_pauseSelection;                // 0x984
	ePauseWindowMessages m_pauseMessage;          // 0x988
	ePauseWindowMessages m_previousPauseMessage;  // 0x98c
	CDrawingMark m_drawingMark;                   // 0x990
	unsigned int m_clipConfigured;                // 0x994
	CLine m_lineAt998;                            // 0x998
	CLine m_lineAt9a8;                            // 0x9a8
	CVsRect m_spriteGroundTranslatedPointRect;    // 0x9b8
	CLine m_lines[4];                             // 0x9c0
	CBigBitmap m_bigBitmaps[4];                   // 0xa00
	CLemmingAnimsManager* m_lemmingAnims;         // 0xa40
	char m_pad0xa44[4];                           // 0xa44
	unsigned int m_groupingActive;                // 0xa48
	unsigned short m_groupCount;                  // 0xa4c
	unsigned short m_groupSelectionCount;         // 0xa4e
	unsigned short m_groupObjectIds[20];          // 0xa50
	unsigned int m_quitRequested;                 // 0xa78
	unsigned int m_paused;                        // 0xa7c
	unsigned int m_pauser;                        // 0xa80
	unsigned int m_connectionTimeoutActive;       // 0xa84
	unsigned int m_connectionTimeoutStart;        // 0xa88
	char m_pad0xa8c[0x204];                       // 0xa8c
	unsigned int m_unk0xc90;                      // 0xc90
	char m_pad0xc94[0x7d0];                       // 0xc94
	unsigned int m_unk0x1464;                     // 0x1464
	CSolidRect m_solidRects[210];                 // 0x1468
	unsigned int m_primitiveCount;                // 0x2188
	unsigned short m_groundHitMode;               // 0x218c
	char m_pad0x218e[2];                          // 0x218e
	CSolidRect m_solidRect[1];                    // 0x2190
	CScreenScroll m_screenScroll;                 // 0x21a0
	CCopyColourToBackBuff m_copyColourToBackBuff; // 0x21b0
	CCopyToBackBuff m_copyToBackBuff;             // 0x21c0
	CCopyToBackBuff m_scrollCopyToBackBuffs[2];   // 0x21d0
	unsigned int m_bitmapCount;                   // 0x21f0
	CBitmap m_bitmaps[2];                         // 0x21f4
	unsigned int m_redrawPending;                 // 0x2214
	unsigned int m_scrollPending;                 // 0x2218
	short m_scrollDeltaX;                         // 0x221c
	short m_scrollDeltaY;                         // 0x221e
	AnimSpecialEntry* m_zBufferAnimations;        // 0x2220
	int m_zBufferAnimationCount;                  // 0x2224
	char m_pad0x2228[0x200];                      // 0x2228
};

// SYNTHETIC: LEMBALL 0x004369a0
// $E2

// SYNTHETIC: LEMBALL 0x00439490
// ??_EC2D@@UAEPAXI@Z

// SYNTHETIC: LEMBALL 0x00439840
// ??_EC2D@@W3AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00439850
// ??_EC2D@@WBE@AEPAXI@Z

// GLOBAL: LEMBALL 0x004a78a8
// ?$S1@?1??GetClipRectangle@C2D@@QAEPAVCVsRect@@XZ@4EA

// GLOBAL: LEMBALL 0x004a78b0
// clipRectangle

#endif
