#ifndef LEMBALL_VIEWS_DISPLAY_C2D_H
#define LEMBALL_VIEWS_DISPLAY_C2D_H

#include "../../AI/Base/AiCoord.h" // complete type
#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/VsPoint.h"          // complete type
#include "../../Visos/Foundation/VsRect.h"           // complete type
#include "../../Visos/Graphics/Drawer.h"             // complete type
#include "../../Visos/Graphics/HotAreaHandler.h"     // complete type
#include "../../Visos/Graphics/ReceiveWindowState.h" // complete type

// SIZE 0x2428
// VTABLE: LEMBALL 0x00496df8 26-entry primary
// VTABLE: LEMBALL 0x00496de8 adjusted BaseQueueHandler at +0x04
// VTABLE: LEMBALL 0x00496dc0 adjusted HotAreaHandler at +0x14
// VTABLE: LEMBALL 0x00496db0 adjusted ReceiveWindowState at +0x4c
class C2D : public Drawer, public BaseQueueHandler, public HotAreaHandler, public ReceiveWindowState {
public:
	C2D(Main2DDisplay* p_arg0, Ai* p_arg1, Gdi* p_arg2, Map* p_arg3, const VsRect& p_arg4);
	bool FindGameObject(const VsPoint& p_point, int& p_index, unsigned char p_preferLemming);
	bool InGroupByObjectNo(int p_objectNo);
	bool IsInGrouping(GameObject* p_object);
	bool ScreenToGame(int p_screenX, int p_screenY, int& p_gameX, int& p_gameY);
	int CalcZValueSprite(int p_index);
	unsigned long LemmingFly(ViewData& p_viewData, int& p_frame);
	unsigned short CalcGroundCode(eObjectType p_objectType, int p_x, int p_y, unsigned short p_z);
	void UseBalloon(int p_playerIndex);
	virtual bool GetPauser();                                                        // vtable+0x04
	virtual bool QuitYet();                                                          // vtable+0x2c
	virtual int GetReturnState();                                                    // vtable+0x28
	virtual int ProcessMsg(Message* p_message);                                      // vtable+0x08
	virtual unsigned int OnButtonDown(const VsPoint& p_point, unsigned int p_flags); // vtable+0x04
	virtual void OnButtonUp(const VsPoint& p_point, unsigned int p_flags);           // vtable+0x08
	virtual void OnDriverChange();                                                   // vtable+0x30
	virtual void OnInside(const VsPoint& p_point);                                   // vtable+0x18
	virtual void OnSize(const VsRect& p_rect);                                       // vtable+0x10
	virtual void OnZoom(const VsRect& p_rect);                                       // vtable+0x0c
	virtual void Process();                                                          // vtable+0x1c
	virtual void ShutDown();                                                         // vtable+0x04
	virtual ~C2D();                                                                  // vtable+0x00
	void AddObjectToGroup(int p_objectNo, unsigned char p_markSelection);
	void CancelMoves();
	void CheckValidFormGroup();
	void CursorChangeType(int p_cursorType, int p_value);
	void DoButtons();
	void DoClipWidth(int p_mapX, int p_mapY, int p_count);
	void DoClipWidthSearch(int p_mapX, int p_mapY, int p_count);
	void Draw(const VsRect& p_rect);
	void DrawAmmo(ViewData& p_viewData, int p_objectNo);
	void DrawBall(ViewData& p_viewData);
	void DrawBalloon(ViewData& p_viewData, int p_playerIndex);
	void DrawBalloonPost(ViewData& p_viewData, int p_playerIndex);
	void DrawBonus(ViewData& p_viewData);
	void DrawBullet(ViewData& p_viewData, int p_objectNo);
	void DrawCatapult(ViewData& p_viewData, int p_objectNo);
	void DrawCliff(int p_x, int p_y, int p_height, int p_count);
	void DrawClipData();
	void DrawClippedRectangle(const VsRect& p_rect);
	void DrawCrate(ViewData& p_viewData, int p_objectNo);
	void DrawDemo();
	void DrawDoor(ViewData& p_viewData);
	void DrawDuplicator(ViewData& p_viewData);
	void DrawFlag(ViewData& p_viewData, eObjectType p_objectType);
	void DrawGround(int p_x, int p_y, eObjectType p_groundType, unsigned short p_frame);
	void DrawHand(ViewData& p_viewData);
	void DrawKey(ViewData& p_viewData, int p_playerIndex);
	void DrawLaser(ViewData& p_viewData);
	void DrawLaserFire(ViewData& p_viewData);
	void DrawLemming(ViewData& p_viewData, int p_objectNo, unsigned char p_remapped);
	void DrawLemmingExternal(ViewData& p_viewData, unsigned char p_remapped);
	void DrawLemmingFall(ViewData& p_viewData, unsigned char p_remapped);
	void DrawLemmingFlyShadow(ViewData& p_viewData);
	void DrawLemmingJump(ViewData& p_viewData, unsigned char p_remapped);
	void DrawLemmingLanding(ViewData& p_viewData, unsigned char p_remapped);
	void DrawLemmingOnBalloon(ViewData& p_viewData, int p_balloonType, unsigned char p_remapped);
	void DrawLemmingOnConveyor(ViewData& p_viewData, unsigned char p_remapped);
	void DrawMine(ViewData& p_viewData);
	void DrawMover(ViewData& p_viewData);
	void DrawObject(ViewData& p_viewData);
	void DrawObjects();
	void DrawObjectsZBuff();
	void DrawPaintGun(ViewData& p_viewData);
	void DrawPaused();
	void DrawRocket(ViewData& p_viewData);
	void DrawScore();
	void DrawSheep(ViewData& p_viewData, int p_objectNo);
	void DrawSlinky(ViewData& p_viewData);
	void DrawSwitch(ViewData& p_viewData);
	void DrawTime();
	void DrawTimeBonus(ViewData& p_viewData);
	void DrawTrampoline(ViewData& p_viewData);
	void DrawTrapDoor(ViewData& p_viewData);
	void DrawZBuffAnim(int p_index, unsigned short p_z);
	void DrawZBuffSprite(int p_index, unsigned short p_z);
	void FormGroup();
	void GroupingLeftClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint, unsigned char p_alternate);
	void InitSpriteGroundLu();
	void KillRemapPalettes();
	void LeftClick(const VsPoint& p_screenPoint,
				   const VsPoint& p_gamePoint,
				   unsigned char p_commitMoves,
				   unsigned char p_alternate);
	void MoveGroup(const VsPoint& p_point);
	void NewPauseWindow(int p_message);
	void NextGroup();
	void NoStateLeftClick(const VsPoint& p_screenPoint,
						  const VsPoint& p_gamePoint,
						  unsigned char p_commitMoves,
						  unsigned char p_alternate);
	void NoStateRightClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint);
	void OnLoaded();
	void PrevGroup();
	void RegisterRemaps();
	void RemoveFromGroupByObjectNo(int p_objectNo);
	void ReplaceBackground();
	void ResetPrimitives();
	void Restart();
	void RightClick(const VsPoint& p_screenPoint, const VsPoint& p_gamePoint);
	void SelectLemming(int p_playerIndex);
	void SelectObject(int p_viewIndex);
	void SendCursorMsg();
	void SetClipSize();
	void SetMouseShape();
	void SetOrigin();
	void SetPause(unsigned char p_paused);
	void SetUpRemapPalettes();
	void SortViewData();
	void TriggerPause(unsigned char p_paused);
	void UnRegisterRemaps();
	void UseBalloon(PlayerLemming* p_lemming);

private:
	unsigned char* m_remapTables[4];                // 0x54
	BaseRemap* m_remaps[5];                         // 0x64
	TextManager* m_textManager;                     // 0x78
	undefined m_pad0x7c[0x856];                     // 0x7c
	VsRect m_spriteGroundLookupRectA;               // 0x8d2
	VsRect m_spriteGroundLookupRectB;               // 0x8da
	undefined m_pad0x8e2[0x1a];                     // 0x8e2
	int m_returnState;                              // 0x8fc
	undefined m_pad0x900[0x10];                     // 0x900
	Map* m_map;                                     // 0x910
	int m_viewOriginX;                              // 0x918
	int m_viewOriginY;                              // 0x91c
	undefined m_pad0x920[0xc];                      // 0x920
	PadToButton* m_padToButton;                     // 0x92c
	TargetSpriteGroundLookup* m_spriteGroundLookup; // 0x930
	TargetObjectClipGrid* m_objectClipGrid;         // 0x934
	VsPoint m_spriteGroundTranslationPoint;         // 0x938
	VsPoint m_cursorGamePoint;                      // 0x93c
	VsPoint m_viewSize;                             // 0x940
	AiCoord m_originPosition;                       // 0x944
	int m_clipOffsetX;                              // 0x950
	int m_clipOffsetY;                              // 0x954
	VsPoint m_clipSize;                             // 0x958
	ViewData* m_viewData;                           // 0x95c
	unsigned char* m_zBuffer;                       // 0x960
	unsigned short m_viewDataCount;                 // 0x964
	BaseRemap* m_paletteRemap;                      // 0x968
	Ai* m_ai;                                       // 0x96c
	Gdi* m_gdi;                                     // 0x970
	void* m_lemmingManager;                         // 0x974
	Main2DDisplay* m_display;                       // 0x978
	Panel* m_panel;                                 // 0x97c
	PauseWindow* m_pauseWindow;                     // 0x980
	unsigned int m_pauseSelection;                  // 0x984
	int m_pauseMessage;                             // 0x988
	int m_previousPauseMessage;                     // 0x98c
	undefined m_pad0x990[4];                        // 0x990
	unsigned int m_clipConfigured;                  // 0x994
	undefined m_pad0x998[0x20];                     // 0x998
	VsRect m_spriteGroundTranslatedPointRect;       // 0x9b8
	undefined m_pad0x9c0[0x80];                     // 0x9c0
	LemmingAnimsManager* m_lemmingAnims;            // 0xa40
	undefined m_pad0xa44[8];                        // 0xa44
	unsigned int m_groupingActive;                  // 0xa48
	unsigned short m_groupCount;                    // 0xa4c
	unsigned short m_groupSelectionCount;           // 0xa4e
	unsigned short m_groupObjectIds[20];            // 0xa50
	unsigned int m_quitRequested;                   // 0xa78
	unsigned int m_paused;                          // 0xa7c
	unsigned int m_pauser;                          // 0xa80
	unsigned int m_primitiveCount;                  // 0x2188
	unsigned short m_groundHitMode;                 // 0x218c
	unsigned int m_redrawPending;                   // 0x2214
	unsigned int m_scrollPending;                   // 0x2218
	short m_scrollDeltaX;                           // 0x221c
	short m_scrollDeltaY;                           // 0x221e
	void* m_zBufferAnimations;                      // 0x2220
	unsigned int m_zBufferAnimationCount;           // 0x2224
};

#endif
