#define LEMBALL_C2D_LEMMING_FLY
#include "views/2d/C2DRender.h"

#include "AI/CAI.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Level/DRAWTEXT.H"
#include "Visos/Generic/CVSPoint.h"
#include "Visos/Generic/Memory.h"

extern void EmitLevelScreenVariantEntry(void* pObject,
										short x,
										short y,
										int nResourceId,
										int nFlags,
										void* pFrameSelector,
										int nValue);
extern void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);
extern void LEMBALL_FASTCALL SetLevelScreenActionPanelPauseActive(void* pObject, int nUnused, int fPaused);
extern int LEMBALL_FASTCALL IsPointInsideLevelScreenActionPanel(void* pObject, int nUnused, short* pPoint);
extern void* LEMBALL_FASTCALL ConstructLevelScreenActionPanelResources(void* pObject, int nUnused, void* pLevelScreen);
extern void* g_pLevelDemoPlaybackController;

class CAnimsManagerView {
public:
	void EmitLevelScreenVariantEntry(short x, short y, int nResourceId, int nFlags, void* pFrameSelector, int nValue);
};

struct CGround {
	short GetZ(int nLocalX, int nLocalY);
};

struct LevelViewRotationTransform {
	void* m_pVtable00;
	int m_nRotation04;
	int m_nReserved08;
	CGround* m_pGround0C;
	int m_nWidth10;
	int m_nHeight14;

	void GameToScreen(int* pX, int* pY);
	void ScreenToGame(int nScreenX, int nScreenY, int* pTileX, int* pTileY);
};

struct CViewDataFlyView {
	unsigned short m_nDirection00;
	unsigned short m_nReserved02;
	int m_nX04;
	int m_nY08;
	int m_nGroundHeight0C;
	unsigned short m_nGroundX10;
	unsigned short m_nGroundY12;
	unsigned char m_abReserved14[4];
	int m_nState18;
	short m_nVariant1C;
	unsigned short m_nReserved1E;
	int m_nFrame20;
	void* m_pFrameSelector24;
};

class CRenderQueueClientView {
public:
	virtual void Reserved(void) = 0;
	virtual void Reset(void* pRenderQueue) = 0;
};

// GLOBAL: LEMBALL 0x0049ef38
static const int g_anC2DBulletResources[8] = {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x5f};
// GLOBAL: LEMBALL 0x00497064
static const short g_anC2DAmmoOffsets[4] = {8, 0x10, 0x10, 0x10};
// GLOBAL: LEMBALL 0x00497070
static const short g_anC2DBulletOffsets[2] = {4, 4};
// GLOBAL: LEMBALL 0x00497054
static const short g_anC2DKeyOffsets[2] = {8, 0x20};
// GLOBAL: LEMBALL 0x00497084
static const short g_anC2DBonusOffsets[2] = {0x10, 0x10};
// GLOBAL: LEMBALL 0x00497088
static const short g_anC2DTimeBonusOffsets[2] = {0x10, 0x12};
// GLOBAL: LEMBALL 0x0049703c
static const short g_anC2DCrateOffsets[4] = {8, 0x18, 0x22, 0x32};
// GLOBAL: LEMBALL 0x0049704c
static const short g_anC2DMineOffsets[4] = {0x1e, 0x23, 2, 2};
// GLOBAL: LEMBALL 0x00497074
static const short g_anC2DBallOffsets[4] = {10, 0x0f, 0x0f, 0x11};
// GLOBAL: LEMBALL 0x00497080
static const short g_anC2DFlagOffsets[2] = {0x0f, 0x1c};
// GLOBAL: LEMBALL 0x00497098
static const short g_anC2DTrampolineOffsets[2] = {0x16, 0x16};
// GLOBAL: LEMBALL 0x004970a0
static const short g_anC2DTrapOffsets[8] = {0x0d, 0x19, 0x1e, 0x20, 0x1d, 0x1a, 0x0e, 0x22};

// GLOBAL: LEMBALL 0x0049709c
static const short g_anC2DPaintGunOffsets[2] = {0x19, 0x1b};
// GLOBAL: LEMBALL 0x0049706c
static const short g_anC2DSheepOffsets[2] = {9, 8};
// GLOBAL: LEMBALL 0x0049707c
static const short g_anC2DSwitchOffsets[2] = {5, 0x19};
// GLOBAL: LEMBALL 0x0049ef58
static const int g_adwC2DSheepResourcesMode2[8] = {0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa1};
// GLOBAL: LEMBALL 0x0049ef78
static const int g_adwC2DSheepResourcesMode014[8] = {0xa9, 0xa3, 0xaa, 0xa5, 0xab, 0xa7, 0xac, 0xa1};
// GLOBAL: LEMBALL 0x0049efa8
static const int g_anC2DLemmingExternalResources[8] = {0x90, 0x91, 0x91, 0x92, 0x92, 0x93, 0x93, 0x90};


extern int g_nLevelScreenTimedVariantResourceId;
extern int g_nLevelScreenMappedVariantResourceId0x209;
extern int g_nLevelScreenMultiPhaseSequenceResourceId;
extern int g_nLevelScreenCompositeSequencePrimaryResourceId;
extern int g_nLevelScreenCompositeSequenceSecondaryResourceId;
extern int g_nLevelScreenMultiStageSequencePrimaryResourceId;
extern int g_nLevelScreenMultiStageSequenceSecondaryResourceId;
extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;
extern "C" unsigned long __stdcall timeGetTime(void);

#pragma auto_inline(off)
void CAnimsManagerView::EmitLevelScreenVariantEntry(short x,
													short y,
													int nResourceId,
													int nFlags,
													void* pFrameSelector,
													int nValue)
{
	::EmitLevelScreenVariantEntry(this, x, y, nResourceId, nFlags, pFrameSelector, nValue);
}
#pragma auto_inline(on)

struct LevelScreenCursorMessage {
	short m_nType00;
	short m_nReserved02;
	int m_nReserved04;
	int m_nPosition08;
	int m_nHeight0C;
	int m_nReserved10;
};

// FUNCTION: LEMBALL 0x00437d00
void C2D::NewPauseWindow(int nMessage)
{
	char* pThis = (char*) this;
	*(int*) (pThis + 0x98c) = *(int*) (pThis + 0x988);
	*(int*) (pThis + 0x988) = nMessage;
	void* pWindow = *(void**) (pThis + 0x980);
	if (pWindow != 0) {
		void* pWindowBase = (char*) pWindow + 0x90;
		((void(__fastcall*)(void*, int, int)) (*(void***) pWindowBase)[1])(pWindowBase, 0, 1);
		*(void**) (pThis + 0x980) = 0;
	}
	if (*(int*) (pThis + 0x988) != 5) {
		pWindow = AllocateVSMemBlock(0x20c);
		if (pWindow == 0) {
			*(void**) (pThis + 0x980) = 0;
		}
		else {
			void* pOwner = pThis + 0x4c;
			if (this == 0) {
				pOwner = 0;
			}
			*(void**) (pThis + 0x980) =
				((void*(__fastcall*)(void*, int, void*, void*, int)) 0x402928)(
					pWindow, 0, pOwner, *(void**) (pThis + 0x978), *(int*) (pThis + 0x988));
		}
	}
	if (*(int*) (pThis + 0x988) == 3) {
		*(int*) (pThis + 0x984) = *(int*) (pThis + 0x50);
	}
}

// FUNCTION: LEMBALL 0x004380c0
void C2D::SendCursorMsg(void)
{
	char* pThis = (char*) this;
	LevelScreenCursorMessage Message;
	short Point[2];
	int nHeight;
	Message.m_nType00 = 1;
	Message.m_nReserved04 = 0;
	Message.m_nPosition08 = 0;
	Message.m_nHeight0C = 0;
	Message.m_nReserved10 = 0;
	Point[0] = (short) (m_nCameraOriginX0918 + *(short*) (pThis + 0x93c));
	Point[1] = (short) (*(short*) (pThis + 0x93e) + m_nCameraOriginY091C);
	if (((int(__fastcall*)(void*, int, int, int, short*, int*)) 0x401f5f)(
			this, 0, Point[0], Point[1], Point, &nHeight) == 0) {
		((void(__fastcall*)(void*, int, int, int, short*, int*)) 0x4021fd)(
			m_pLevel0914, 0, Point[0], Point[1], Point, &nHeight);
	}
	Message.m_nPosition08 = *(int*) Point;
	Message.m_nHeight0C = nHeight;
	void* pSink = *(void**) (pThis + 0x974);
	((void(__fastcall*)(void*, int, LevelScreenCursorMessage*)) (*(void***) pSink)[2])(pSink, 0, &Message);
}

// FUNCTION: LEMBALL 0x0043c090
void C2D::DrawLemmingExternal(CViewData& ViewData, int nUseRemap)
{
	int nFrame;
	int nRemap;
	unsigned int nDirection;
	unsigned int nVariant;
	short nX;
	short nY;

	nFrame = ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f / 1000;
	if (nFrame < 0) {
		nFrame = 0;
	}
	nRemap = 0;
	if (nUseRemap != 0) {
		nRemap = m_nLemmingRemap0968;
	}
	nDirection = (ViewData.m_nDirection00 + m_nViewRotation090C * 2) & 7;
	nVariant = (unsigned short) ViewData.m_nVariant1C;
	nX = (short) ViewData.m_nX04;
	nY = (short) ViewData.m_nY08;
	switch (nVariant) {
	case 1:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x0f),
										  (short) (nY - 0x16),
										  g_anC2DLemmingExternalResources[nDirection],
										  nFrame % 4,
										  0,
										  nRemap);
		return;
	case 2:
		if (nFrame < 0x0f) {
			((CAnimsManagerView*) m_pAnimsManager0A40)
				->EmitLevelScreenVariantEntry((short) (nX - 7),
											  (short) (nY - 0x1c),
											  0x76,
											  nFrame,
											  0,
											  nRemap);
		}
		return;
	case 3:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x0f),
										  (short) (nY - 0x16),
										  0x3f,
										  nFrame % 8,
										  0,
										  nRemap);
		return;
	}
}

// MACINTOSH: C2D::DrawBullet(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c610
void C2D::DrawBullet(CViewData& ViewData, int)
{
	int nDirection = (ViewData.m_nDirection00 + m_nViewRotation090C * 2) & 7;
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DBulletOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DBulletOffsets[1]),
									  g_anC2DBulletResources[nDirection],
									  0,
									  ViewData.m_pFrameSelector24,
									  0);
}

// MACINTOSH: C2D::DrawAmmo(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c660
void C2D::DrawAmmo(CViewData& ViewData, int)
{
	if (ViewData.m_nState18 >= 0x18) {
		if (ViewData.m_nState18 <= 0x19) {
			((CAnimsManagerView*) m_pAnimsManager0A40)
				->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DAmmoOffsets[0]),
											  (short) (ViewData.m_nY08 - g_anC2DAmmoOffsets[1]),
											  0x7c,
											  0,
											  ViewData.m_pFrameSelector24,
											  0);
		}
		else {
			if (ViewData.m_nState18 != 0x1a) {
				return;
			}
			((CAnimsManagerView*) m_pAnimsManager0A40)
				->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DAmmoOffsets[2]),
											  (short) (ViewData.m_nY08 - g_anC2DAmmoOffsets[3]),
											  0x82,
											  ViewData.m_nFrame20,
											  ViewData.m_pFrameSelector24,
											  0);
		}
	}
}

// MACINTOSH: C2D::DrawHand(CViewData&)
// FUNCTION: LEMBALL 0x0043c7f0
void C2D::DrawHand(CViewData& ViewData)
{
	int nState = ViewData.m_nState18;
	int nX = ViewData.m_nX04 - 0x31;
	int nY = ViewData.m_nY08 - 0x14;
	int nRemap = 0;
	if (ViewData.m_nVariant1C != 0) {
		nRemap = m_nLemmingRemap0968;
	}
	switch (nState) {
	case 0x17:
	case 0x18:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) nX, (short) nY, g_nLevelScreenTimedVariantResourceId, 0, 0, 0);
		return;
	case 0x19:
	case 0x1a: {
		int nFrame = ((unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
		if (nFrame > 0x0b) {
			nFrame = 0x0b;
		}
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) nX,
										  (short) nY,
										  g_nLevelScreenTimedVariantResourceId,
										  nFrame,
										  0,
										  nRemap);
	} break;
	}
}

// MACINTOSH: C2D::DrawBalloonPost(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c9f0
void C2D::DrawBalloonPost(CViewData& ViewData, int nPlayer)
{
	int nRemap;
	if (nPlayer < 4) {
		nRemap = m_anPlayerRemaps0064[nPlayer];
	}
	else {
		nRemap = 0;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x10),
									  (short) (ViewData.m_nY08 - 0x40),
									  0x95,
									  0,
									  0,
									  nRemap);
}

// MACINTOSH: C2D::DrawTrampoline(CViewData&)
// FUNCTION: LEMBALL 0x0043ca30
void C2D::DrawTrampoline(CViewData& ViewData)
{
	int nX = ViewData.m_nX04 - g_anC2DTrampolineOffsets[0];
	int nY = ViewData.m_nY08 - g_anC2DTrampolineOffsets[1];
	if (ViewData.m_nState18 == 0x18) {
		((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry((short) nX, (short) nY, 0x97, 0, 0, 0);
		return;
	}
	if (ViewData.m_nState18 != 0x1b) {
		return;
	}
	unsigned int nFrame = ((unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
	if (nFrame > 0x0b) {
		nFrame = 0x0b;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry((short) nX, (short) nY, 0x97, nFrame, 0, 0);
}

// MACINTOSH: C2D::DrawGrenade(CViewData&)

// FUNCTION: LEMBALL 0x0043cad0
void C2D::DrawGrenade(CViewData& ViewData)
{
	unsigned short nFrame;
	switch (*(int*) ((char*) m_pLevelMode096C + 0x60)) {
	case 0:
		nFrame = 0x50;
		break;
	case 1:
		nFrame = 0x1a;
		break;
	case 2:
		nFrame = 0x52;
		break;
	case 3:
		nFrame = 0x38;
		break;
	}
	if (ViewData.m_nVariant1C == 0) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x10),
										  (short) (ViewData.m_nY08 - 0x1c),
										  g_nLevelScreenMappedVariantResourceId0x209,
										  nFrame,
										  0,
										  0);
		return;
	}
	if (ViewData.m_nVariant1C == 1) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x11),
										  (short) (ViewData.m_nY08 - 0x26),
										  0xa0,
										  (g_nLevelFrameClockTimeMs / 0x46) & 7,
										  0,
										  0);
	}
}

// FUNCTION: LEMBALL 0x0043c6e0
void C2D::DrawRocket(CViewData& ViewData)
{
	int nLeadFrame = -1;
	int nTimer = ((unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
	int nFrame;

	if (nTimer <= 7) {
		nFrame = nTimer;
		if (nTimer >= 4) {
			nFrame = 4;
		}
	}
	else if (nTimer >= 8 && nTimer <= 0x0d) {
		if (nTimer < 0x0b) {
			nFrame = nTimer - 3;
		}
		else {
			nFrame = 8;
		}
	}
	else if (nTimer >= 0x0e && nTimer <= 0x13) {
		nFrame = nTimer - 4;
		nLeadFrame = 9;
	}
	else if (nTimer >= 0x14 && nTimer <= 0x1f) {
		nFrame = (nTimer - 0x14) % 2 + 0x11;
		nLeadFrame = 0x10;
	}
	else if (nTimer >= 0x20 && nTimer <= 0x2a) {
		nFrame = nTimer - 0x0b;
		if (nFrame > 0x19) {
			nLeadFrame = 0x1a;
			++nFrame;
		}
	}
	else {
		nFrame = (nTimer & 1) + 0x21;
		nLeadFrame = 0x20;
	}

	if (nLeadFrame != -1) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x0d),
										  (short) (ViewData.m_nY08 - 0x49),
										  0x88,
										  nLeadFrame,
										  0,
										  0);
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x0d),
									  (short) (ViewData.m_nY08 - 0x49),
									  0x88,
									  nFrame,
									  0,
									  0);
}

// MACINTOSH: C2D::DrawBoobyTrap(CViewData&)
// FUNCTION: LEMBALL 0x0043cc10
void C2D::DrawBoobyTrap(CViewData& ViewData)
{
	unsigned int nVariant = (unsigned short) ViewData.m_nVariant1C;
	short nX = (short) (ViewData.m_nX04 - g_anC2DTrapOffsets[nVariant * 2]);
	short nY = (short) (ViewData.m_nY08 - g_anC2DTrapOffsets[nVariant * 2 + 1]);
	int nResource;
	switch (nVariant) {
	case 0:
		nResource = 0x9c;
		break;
	case 1:
		nResource = 0x9e;
		break;
	case 2:
		nResource = 0x9d;
		break;
	case 3:
		nResource = 0x9f;
		break;
	default:
		return;
	}
	int nFrame = 0;
	if (ViewData.m_nState18 != 0x18) {
		if (ViewData.m_nState18 != 0x1b) {
			return;
		}
		nFrame = (((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
		if (nFrame > 0x0c) {
			nFrame = 0x0c;
		}
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry(nX, nY, nResource, nFrame, 0, 0);
}

// MACINTOSH: C2D::DrawCrusher(CViewData&)
// FUNCTION: LEMBALL 0x0043ccf0
void C2D::DrawCrusher(CViewData& ViewData)
{
	unsigned int nVariant = (unsigned short) ViewData.m_nVariant1C;
	short nX = (short) (ViewData.m_nX04 - g_anC2DTrapOffsets[nVariant * 2]);
	short nY = (short) (ViewData.m_nY08 - g_anC2DTrapOffsets[nVariant * 2 + 1]);
	int nResource;
	switch (nVariant) {
	case 0:
		nResource = 0x9c;
		break;
	case 1:
		nResource = 0x9e;
		break;
	case 2:
		nResource = 0x9d;
		break;
	case 3:
		nResource = 0x9f;
		break;
	default:
		return;
	}
	int nFrame = 0;
	if (ViewData.m_nState18 != 0x18) {
		if (ViewData.m_nState18 != 0x1b) {
			return;
		}
		nFrame = (((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
		if (nFrame > 0x0c) {
			nFrame = 0x0c;
		}
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry(nX, nY, nResource, nFrame, 0, 0);
}

// MACINTOSH: C2D::DrawBurner(CViewData&)

// MACINTOSH: C2D::DrawPaintGun(CViewData&)
// FUNCTION: LEMBALL 0x0043cd50
void C2D::DrawPaintGun(CViewData& ViewData)
{
	short nX = (short) (ViewData.m_nX04 - g_anC2DPaintGunOffsets[0]);
	int nY = ViewData.m_nY08 - g_anC2DPaintGunOffsets[1];
	if (ViewData.m_nState18 != 3 && ViewData.m_nState18 != 0x1b) {
		return;
	}
	unsigned int nFrame = ((unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 8) / 1000;
	if (nFrame > 0x39) {
		nFrame = 0;
	}
	if (nFrame < 0x0c) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, (short) nY, g_nLevelScreenMultiPhaseSequenceResourceId, nFrame, 0, 0);
	}
	else if (nFrame < 0x2f) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, (short) nY, g_nLevelScreenMultiPhaseSequenceResourceId, 0x0b, 0, 0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, (short) nY, 0x98, nFrame - 0x0c, 0, 0);
	}
	if (nFrame > 0x2e) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX,
										  (short) (nY + nFrame - 0x2f),
										  g_nLevelScreenMultiPhaseSequenceResourceId,
										  0x0b,
										  0,
										  0);
	}
}
// FUNCTION: LEMBALL 0x0043cd80
void C2D::DrawBurner(CViewData& ViewData)
{
	short nX = (short) (ViewData.m_nX04 - 0x19);
	int nY = ViewData.m_nY08 - 0x1b;
	if (ViewData.m_nState18 != 3 && ViewData.m_nState18 != 0x1b) {
		return;
	}
	unsigned int nFrame = ((unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 8) / 1000;
	if (nFrame > 0x39) {
		nFrame = 0;
	}
	if (nFrame < 0x0c) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, (short) nY, g_nLevelScreenMultiPhaseSequenceResourceId, nFrame, 0, 0);
	}
	else if (nFrame < 0x2f) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, (short) nY, g_nLevelScreenMultiPhaseSequenceResourceId, 0x0b, 0, 0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, (short) nY, 0x98, nFrame - 0x0c, 0, 0);
	}
	if (nFrame > 0x2e) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX,
										  (short) (nY + nFrame - 0x2f),
										  g_nLevelScreenMultiPhaseSequenceResourceId,
										  0x0b,
										  0,
										  0);
	}
}

// MACINTOSH: C2D::DrawLaserFire(CViewData&)
// FUNCTION: LEMBALL 0x0043ce30
void C2D::DrawLaserFire(CViewData& ViewData)
{
	if (ViewData.m_nObjectType28 == 0x26) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x0d),
										  (short) (ViewData.m_nY08 - 9),
										  0x8f,
										  0,
										  0,
										  0);
		return;
	}
	if (ViewData.m_nObjectType28 == 0x32) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x16),
										  (short) (ViewData.m_nY08 - 0x0f),
										  0x8d,
										  0,
										  0,
										  0);
	}
}

// MACINTOSH: C2D::DrawTeleporter(CViewData&)

// FUNCTION: LEMBALL 0x0043ced0
void C2D::DrawTeleporter(CViewData& ViewData)
{
	int nResource;
	int nX;
	int nY;
	switch (ViewData.m_nObjectType28) {
	case 0x1e:
	case 0x30:
		nResource = 0x8c;
		nX = ViewData.m_nX04 - 0x14;
		nY = ViewData.m_nY08 - 10;
		break;
	case 0x2f:
	case 0x31:
		nResource = 0x8e;
		nX = ViewData.m_nX04 - 0x2e;
		nY = ViewData.m_nY08 - 10;
		break;
	default:
		return;
	}
	switch (ViewData.m_nState18) {
	case 0x17:
	case 0x18:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) nX, (short) nY, nResource, 0, 0, 0);
		return;
	case 0x19:
	case 0x1a: {
		unsigned int nFrame = ((unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 0x0f) / 1000;
		if (nFrame > 0x11) {
			nFrame = 0x11;
		}
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) nX, (short) nY, nResource, nFrame, 0, 0);
	} break;
	}
}

// MACINTOSH: C2D::DrawBalloon(CViewData&)

// FUNCTION: LEMBALL 0x0043cfc0
void C2D::DrawBalloon(CViewData& ViewData)
{
	int nNow = (int) ViewData.m_pFrameSelector24;
	int nState = ViewData.m_nState18;
	int nStart = ViewData.m_nFrame20;
	short nX = (short) (ViewData.m_nX04 - 0x1c);
	short nY = (short) (ViewData.m_nY08 - 0x3f);
	int nRemap = ViewData.m_nVariant1C != 0 ? m_nLemmingRemap0968 : 0;
	((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry(nX, nY, 0x89, 0, 0, nRemap);
	if (nState == 0x18) {
		((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry(nX, nY, 0x89, 0x3f, 0, nRemap);
		return;
	}
	if (nState != 0x1a) {
		return;
	}
	unsigned int nFrame = ((unsigned int) (nNow - nStart) * 0x0f) / 1000;
	if (nFrame > 0x3e) {
		nFrame = 0x3e;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)->EmitLevelScreenVariantEntry(nX, nY, 0x89, nFrame + 1, 0, nRemap);
}

// MACINTOSH: C2D::DrawCrate(CViewData&, int)
// FUNCTION: LEMBALL 0x0043d070
void C2D::DrawCrate(CViewData& ViewData, int)
{
	if (ViewData.m_nState18 == 0x18) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DCrateOffsets[0]),
										  (short) (ViewData.m_nY08 - g_anC2DCrateOffsets[1]),
										  0x86,
										  ViewData.m_nFrame20,
										  ViewData.m_pFrameSelector24,
										  0);
		return;
	}
	if (ViewData.m_nState18 >= 0x19 && ViewData.m_nState18 <= 0x1a) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DCrateOffsets[2]),
										  (short) (ViewData.m_nY08 - g_anC2DCrateOffsets[3]),
										  0x87,
										  ViewData.m_nFrame20,
										  ViewData.m_pFrameSelector24,
										  0);
	}
}

// MACINTOSH: C2D::DrawTimeBonus(CViewData&)
// FUNCTION: LEMBALL 0x0043d0f0
void C2D::DrawTimeBonus(CViewData& ViewData)
{
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DTimeBonusOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DTimeBonusOffsets[1]),
									  0x85,
									  ViewData.m_nFrame20,
									  ViewData.m_pFrameSelector24,
									  0);
}

// MACINTOSH: C2D::DrawExit(CViewData&)
// FUNCTION: LEMBALL 0x0043d270
void C2D::DrawExit(CViewData& ViewData)
{
	int nRemap = ViewData.m_nVariant1C != 0 ? m_nLemmingRemap0968 : 0;
	int nFrame = ViewData.m_nFrame20;
	short nX = (short) ViewData.m_nX04;
	short nY = (short) ViewData.m_nY08;
	switch (ViewData.m_nState18) {
	case 0x18:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  0,
										  0,
										  0);
		return;
	case 0x19:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  0,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x30),
										  (short) (nY - 0x3c),
										  0x74,
										  nFrame,
										  ViewData.m_pFrameSelector24,
										  nRemap);
		return;
	case 0x1a:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequenceSecondaryResourceId,
										  nFrame + 0x640,
										  ViewData.m_pFrameSelector24,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x30),
										  (short) (nY - 0x3c),
										  0x74,
										  nFrame,
										  ViewData.m_pFrameSelector24,
										  nRemap);
		return;
	case 0x1b:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequenceSecondaryResourceId,
										  nFrame + 0x640,
										  ViewData.m_pFrameSelector24,
										  0);
	}
}

// MACINTOSH: C2D::DrawBall(CViewData&)

// MACINTOSH: C2D::DrawSheep(CViewData&, int)
// FUNCTION: LEMBALL 0x0043d370
void C2D::DrawSheep(CViewData& ViewData, int nUnused)
{
	unsigned int uRot = (ViewData.m_nDirection00 + m_nViewRotation090C * 2) & 7;
	int nState = ViewData.m_nState18;
	int nFrameStart = ViewData.m_nFrame20;
	void* pFrameSel = ViewData.m_pFrameSelector24;
	if (nState == 2) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DSheepOffsets[0]),
										  (short) (ViewData.m_nY08 - g_anC2DSheepOffsets[1]),
										  g_adwC2DSheepResourcesMode2[uRot],
										  nFrameStart,
										  pFrameSel,
										  0);
		return;
	}
	if (nState == 3) {
		return;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DSheepOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DSheepOffsets[1]),
									  g_adwC2DSheepResourcesMode014[uRot],
									  nFrameStart,
									  pFrameSel,
									  0);
}
// FUNCTION: LEMBALL 0x0043d420
void C2D::DrawBall(CViewData& ViewData)
{
	int nX = ViewData.m_nX04;
	int nY = ViewData.m_nY08;
	switch (ViewData.m_nState18) {
	case 0x25:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - g_anC2DBallOffsets[0]),
										  (short) (nY - g_anC2DBallOffsets[1]),
										  0x7d,
										  0,
										  ViewData.m_pFrameSelector24,
										  0);
		return;
	case 0x26:
		break;
	default:
		return;
	}
	int nFrame = ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) / 0x40;
	if (nFrame > 8) {
		nFrame = 8;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (nX - g_anC2DBallOffsets[2]),
									  (short) (nY - g_anC2DBallOffsets[3]),
									  0x7e,
									  nFrame,
									  0,
									  0);
}

// MACINTOSH: C2D::DrawKey(CViewData&, int)
// FUNCTION: LEMBALL 0x0043d4b0
void C2D::DrawKey(CViewData& ViewData, int nPlayer)
{
	int nRemap;
	if (nPlayer < 4) {
		nRemap = m_anPlayerRemaps0064[nPlayer];
	}
	else {
		nRemap = 0;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DKeyOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DKeyOffsets[1]),
									  0xb2,
									  0,
									  0,
									  nRemap);
}

// MACINTOSH: C2D::DrawMine(CViewData&)
// FUNCTION: LEMBALL 0x0043d500
void C2D::DrawMine(CViewData& ViewData)
{
	int nX = ViewData.m_nX04;
	int nY = ViewData.m_nY08;
	int nFrame = ViewData.m_nFrame20;
	if (ViewData.m_nState18 < 0x18) {
		return;
	}
	if (ViewData.m_nState18 <= 0x1a) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - g_anC2DMineOffsets[2]),
										  (short) (nY - g_anC2DMineOffsets[3]),
										  0x84,
										  0,
										  0,
										  0);
		return;
	}
	if (ViewData.m_nState18 == 0x1b) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - g_anC2DMineOffsets[0]),
										  (short) (nY - g_anC2DMineOffsets[1]),
										  0x83,
										  nFrame,
										  ViewData.m_pFrameSelector24,
										  0);
	}
}

// MACINTOSH: C2D::DrawFlag(CViewData&, eObjectType)

// MACINTOSH: C2D::DrawSwitch(CViewData&)
// FUNCTION: LEMBALL 0x0043d7e0
void C2D::DrawSwitch(CViewData& ViewData)
{
	short nX = (short) (ViewData.m_nX04 - g_anC2DSwitchOffsets[0]);
	short nY = (short) (ViewData.m_nY08 - g_anC2DSwitchOffsets[1]);
	int nFrame = ViewData.m_nFrame20;
	int nState = ViewData.m_nState18;
	if (nState == 7 || nState == 0x18) {
		if ((ViewData.m_nVariant1C == 0 || ViewData.m_nVariant1C == 1)) {
			((CAnimsManagerView*) m_pAnimsManager0A40)
				->EmitLevelScreenVariantEntry(nX, nY, 0xb1, 0, 0, 0);
		}
		return;
	}
	if (nState != 0x1a) {
		return;
	}
	if (ViewData.m_nVariant1C == 0 || ViewData.m_nVariant1C == 1) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, nY, 0xb0, nFrame, ViewData.m_pFrameSelector24, 0);
	}
}
// FUNCTION: LEMBALL 0x0043d8d0
void C2D::DrawFlag(CViewData& ViewData, int nObjectType)
{
	int nX = ViewData.m_nX04 - g_anC2DFlagOffsets[0];
	int nY = ViewData.m_nY08 - g_anC2DFlagOffsets[1];
	if (nObjectType == 0x0b) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) nX,
										  (short) nY,
										  0x80,
										  0,
										  ViewData.m_pFrameSelector24,
										  m_anPlayerRemaps0064[3]);
		return;
	}
	if (nObjectType == 0x0c) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) nX,
										  (short) nY,
										  0x80,
										  0,
										  ViewData.m_pFrameSelector24,
										  m_anPlayerRemaps0064[1]);
	}
}

// MACINTOSH: C2D::DrawBonus(CViewData&)
// FUNCTION: LEMBALL 0x0043d950
void C2D::DrawBonus(CViewData& ViewData)
{
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DBonusOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DBonusOffsets[1]),
									  0x81,
									  0,
									  ViewData.m_pFrameSelector24,
									  0);
}


// MACINTOSH: C2D::DrawLemmingOnBalloon(CViewData&, int, unsigned char)
// FUNCTION: LEMBALL 0x0043c8a0
void C2D::DrawLemmingOnBalloon(CViewData& ViewData, int nPlayer, unsigned char fUseRemap)
{
	int nRemap = fUseRemap ? m_nLemmingRemap0968 : 0;
	((void (__fastcall*)(void*, int, void*)) 0x401f4b)(this, 0, &ViewData);
	int nPlayerRemap = nPlayer < 4 ? m_anPlayerRemaps0064[nPlayer] : 0;
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x10),
									  (short) (ViewData.m_nY08 - 0x40),
									  0x94,
									  0,
									  0,
									  nPlayerRemap);
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - 0x16),
									  (short) (ViewData.m_nY08 - 0x1e),
									  0x75,
									  0,
									  0,
									  nRemap);
}

// FUNCTION: LEMBALL 0x0043fce0
void C2D::DrawDemo(void)
{
	unsigned char* pFlags = (unsigned char*) 0x4a78c8;
	unsigned long* pLastUpdate = (unsigned long*) 0x4a78c4;
	int* pVisible = (int*) 0x49efc8;
	if ((*pFlags & 1) == 0) {
		*pFlags |= 1;
		*pLastUpdate = timeGetTime();
	}
	if (timeGetTime() - *pLastUpdate > 500) {
		*pVisible = *pVisible == 0;
		*pLastUpdate = timeGetTime();
	}
	if (*pVisible != 0) {
		const short* pPoint = (const short*) ((char*) this + 0x8e6);
		short RelativePoint[2] = {0, 0};
		if ((int) this == -0x8e2) {
			pPoint = 0;
		}
		((CTextManager*) *(void**) ((char*) this + 0x78))
			->DrawString(m_pRenderQueue0970,
						 pPoint,
						 RelativePoint,
						 0xf8,
						 *(void**) 0x49ee70,
						 0x20,
						 *(int*) ((char*) this + 0x74));
	}
}

// FUNCTION: LEMBALL 0x0043fd80
void C2D::DrawTime(void)
{
	char* pMode = (char*) m_pLevelMode096C;
	unsigned short nBaseTime = *(unsigned short*) (pMode + 0xe4);
	short nTime = (short) (*(int*) (pMode + 0xe8) + nBaseTime);
	if (nTime < 0) {
		nTime = 0;
	}
	if (nTime >= 600) {
		if (nBaseTime >= 600) {
			return;
		}
		if (nTime >= 600) {
			nTime = 599;
		}
	}
	if (nTime != *(int*) 0x49efcc) {
		int nSeconds = nTime % 60;
		*(char*) 0x4a78bc = (char) (nTime / 60 + '0');
		*(char*) 0x4a78bd = ':';
		*(char*) 0x4a78be = (char) (nSeconds / 10 + '0');
		*(char*) 0x4a78bf = (char) (nSeconds % 10 + '0');
		*(char*) 0x4a78c0 = 0;
		*(int*) 0x49efcc = nTime;
	}
	const short* pPoint = (const short*) ((char*) this + 0x8d6);
	short RelativePoint[2] = {-4, 0};
	if ((int) this == -0x8d2) {
		pPoint = 0;
	}
	((CTextManager*) *(void**) ((char*) this + 0x78))
		->DrawString(m_pRenderQueue0970, pPoint, RelativePoint, 0x115, (void*) 0x4a78bc, 0x20, 0);
}

// FUNCTION: LEMBALL 0x0043fe80
void C2D::DrawScore(void)
{
	char* pMode = (char*) m_pLevelMode096C;
	int nTargetScore = *(int*) (pMode + 0xf0);
	int nScore = *(int*) ((char*) this + 0x8f0);
	if (*(unsigned int*) ((char*) this + 0x8f8) <= (unsigned int) g_nLevelFrameClockTick) {
		if (nScore != nTargetScore) {
			if (*(int*) (pMode + 0x108) == 2) {
				nScore += 10;
			}
			else {
				nScore += 100;
			}
			if (nScore >= nTargetScore) {
				nScore = nTargetScore;
			}
			*(int*) ((char*) this + 0x8f0) = nScore;
		}
		*(unsigned int*) ((char*) this + 0x8f8) = g_nLevelFrameClockTick + 1;
	}
	if (nScore > 9999999) {
		nScore = 9999999;
	}
	char Text[8];
	Text[7] = 0;
	for (int i = 1; i < 8; ++i) {
		Text[7 - i] = (char) (nScore % 10 + '0');
		nScore /= 10;
	}
	const short* pPoint = (const short*) ((char*) this + 0x8de);
	short RelativePoint[2] = {-4, 0};
	if ((int) this == -0x8da) {
		pPoint = 0;
	}
	((CTextManager*) *(void**) ((char*) this + 0x78))
		->DrawString(m_pRenderQueue0970, pPoint, RelativePoint, 0x115, Text, 0x20, 0);
}

// MACINTOSH: C2D::ResetPrimitives()
// FUNCTION: LEMBALL 0x00440400
void C2D::ResetPrimitives(void)
{
	typedef int(LEMBALL_FASTCALL * GetUploadStateProc)(void*);
	void* pContext = *(void**) ((char*) m_pRenderQueue0970 + 0x0c);
	int nUploadState = ((GetUploadStateProc) (*(void***) pContext)[2])(pContext);
	((CRenderQueueClientView*) m_RenderQueueClient0990)->Reset(m_pRenderQueue0970);
	InitializeHelperUploadStatePending(nUploadState);
}

// MACINTOSH: C2D::DrawZBuff_Sprite(int, ushort)
// FUNCTION: LEMBALL 0x00440460
void C2D::DrawZBuff_Sprite(int nViewDataIndex, unsigned short nValue)
{
	*(unsigned short*) ((char*) m_pAnimsManager0A40 + 0x68) = nValue;
	((void(__fastcall*)(void*, unsigned short*)) 0x4033eb)(this, (unsigned short*) ((char*) m_pViewData095C + nViewDataIndex * 0x4c));
}

// MACINTOSH: C2D::DrawLemmingJump(ushort*, int) - shared directional-timed emit host
// FUNCTION: LEMBALL 0x0043bee0
void C2D::DrawLemmingJump(unsigned short* pViewData, int nFrameIndex)
{
	unsigned int uRotated;
	short sRotX;
	short sRotY;
	int iDelta;
	int nSize;

	uRotated = (*pViewData + m_nViewRotation090C * 2) & 7;
	iDelta = uRotated * 4;
	sRotX = (short) *(int*) (pViewData + 2) - *(short*) (0x497018 + iDelta);
	sRotY = (short) *(int*) (pViewData + 4) - *(short*) (0x49701a + iDelta);
	if (pViewData[0xe] == 0) {
		iDelta = (*(int*) (pViewData + 0x12) - *(int*) (pViewData + 0x10)) * 0xf;
		nSize = (int) (iDelta + (iDelta >> 0x1f & 0x3ffU)) >> 10;
		if (6 < nSize) {
			nSize = 6;
		}
	}
	else if (pViewData[0xe] == 1) {
		iDelta = (*(int*) (pViewData + 0x12) - *(int*) (pViewData + 0x10)) * 0xf;
		nSize = ((int) (iDelta + (iDelta >> 0x1f & 0x3ffU)) >> 10) + 7;
		if (0xc < nSize) {
			nSize = 0xc;
		}
	}
	if (nFrameIndex == 0) {
		EmitLevelScreenVariantEntry(m_pAnimsManager0A40, sRotX, sRotY, *(int*) (0x49eef8 + uRotated * 4), nSize, 0, 0);
		return;
	}
	EmitLevelScreenVariantEntry(m_pAnimsManager0A40, sRotX, sRotY, *(int*) (0x49eef8 + uRotated * 4), nSize, 0, m_nLemmingRemap0968);
}

// MACINTOSH: C2D::DrawLemmingFall(ushort*, int)
// FUNCTION: LEMBALL 0x0043c070
void C2D::DrawLemmingFall(unsigned short* pViewData, int nUnused)
{
	DrawLemmingJump(pViewData, nUnused);
}

// MACINTOSH: C2D::DrawLemmingLanding(ushort*, int)
// FUNCTION: LEMBALL 0x0043bfc0
void C2D::DrawLemmingLanding(unsigned short* pViewData, int nFrameIndex)
{
	unsigned int uRotated;
	short sRotX;
	short sRotY;
	int iDelta;
	int nSize;

	uRotated = (*pViewData + m_nViewRotation090C * 2) & 7;
	iDelta = uRotated * 4;
	sRotX = (short) *(int*) (pViewData + 2) - *(short*) (0x497018 + iDelta);
	sRotY = (short) *(int*) (pViewData + 4) - *(short*) (0x49701a + iDelta);
	iDelta = (*(int*) (pViewData + 0x12) - *(int*) (pViewData + 0x10)) * 0xf;
	nSize = ((int) (iDelta + (iDelta >> 0x1f & 0x3ffU)) >> 10) + 7;
	if (0xc < nSize) {
		nSize = 0xc;
	}
	if (nFrameIndex == 0) {
		EmitLevelScreenVariantEntry(m_pAnimsManager0A40, sRotX, sRotY, *(int*) (0x49eef8 + uRotated * 4), nSize, 0, 0);
		return;
	}
	EmitLevelScreenVariantEntry(m_pAnimsManager0A40, sRotX, sRotY, *(int*) (0x49eef8 + uRotated * 4), nSize, 0, m_nLemmingRemap0968);
}

// FUNCTION: LEMBALL 0x0043bce0
int C2D::LemmingFly(CViewData& RawViewData, int& nFrame)
{
	CViewDataFlyView& ViewData = *(CViewDataFlyView*) &RawViewData;
	unsigned int nDirection = (ViewData.m_nDirection00 + m_nViewRotation090C * 2) & 7;
	int nDelta = (int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20;
	nFrame = 0;
	if (nDelta >= 0) {
		LevelViewRotationTransform* pLevel = (LevelViewRotationTransform*) m_pLevel0914;
		int nWorldX = ViewData.m_nGroundX10;
		int nWorldY = ViewData.m_nGroundY12;
		unsigned short nHeight = 0;
		int nTileX = nWorldX >> 4;
		int nTileY = nWorldY >> 4;
		if (nWorldX >= 0 && nWorldY >= 0 && nTileX < pLevel->m_nWidth10 && nTileY < pLevel->m_nHeight14) {
			CGround* pGround = pLevel->m_pGround0C + nTileY * pLevel->m_nWidth10 + nTileX;
			nHeight = pGround->GetZ(nWorldX & 0x0f, nWorldY & 0x0f);
		}
		nDelta *= 0x0f;
		if ((int) nHeight < ViewData.m_nGroundHeight0C) {
			nFrame = nDelta / 1000;
			if (nFrame > 6) {
				nFrame = 6;
			}
		}
		else {
			nFrame = nDelta / 1000 + 7;
			if (nFrame > 0x0c) {
				nFrame = 0x0c;
			}
		}
	}
	return *(int*) (0x49eef8 + nDirection * 4);
}
struct PaletteRemapPointerTableMemberView {
	void** m_ppItems;
	int m_nCursor;
	void ReleasePaletteRemapVariant(void* pVariant);
};

// FUNCTION: LEMBALL 0x00436480
void C2D::UnRegisterRemaps(void)
{
	void* pObject = this;
	int i;
	for (i = 0; i < 5; i++) {
		((PaletteRemapPointerTableMemberView*) *(void**) 0x4a2000)->ReleasePaletteRemapVariant((void*) *(int*) ((char*) pObject + 0x64 + i * 4));
	}
}

// FUNCTION: LEMBALL 0x00437da0
void C2D::TriggerPause(int param_1)
{
	void* pObject = this;
	if (param_1 == 0) {
		((void(__fastcall*)(void*, int)) 0x4021a8)(pObject, 0);
	}
	else {
		int nPauseState = *(int*) (*(int*) ((char*) pObject + 0x96c) + 0x108);
		if (nPauseState >= 1 && nPauseState <= 2) {
			((void(__fastcall*)(void*, int)) 0x4013ed)(*(void**) ((char*) pObject + 0x96c), 1);
		}
	}
}

// FUNCTION: LEMBALL 0x004381c0
void C2D::OnButtonUp(void* param_2)
{
	void* pObject = this;
	*(int*) ((char*) pObject + 0x178) = 0;
	if (*(int*) ((char*) pObject + 0xa68) == 0) {
		if ((g_pLevelDemoPlaybackController == 0 || *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) &&
			((int(__fastcall*)(void*))(*(void***) *(void**) ((char*) pObject + 0x964) + 0x60 / 4))(
				*(void**) ((char*) pObject + 0x964)) == 0) {
			return;
		}
		((C2D*) ((char*) pObject - 0x14))->SendCursorMsg();
	}
}

// FUNCTION: LEMBALL 0x00438210
void C2D::OnButtonDown(const CVSPoint& Point, int nButtonFlags)
{
	LevelViewRotationTransform* pTransform;
	CVSPoint ScreenPoint;
	CVSPoint TilePoint;
	void* pInputView;
	void* pMode;
	int nScreenX;
	int nScreenY;
	int fMaskedCell;

	*(int*) ((char*) this + 0x178) = 1;
	if (*(int*) ((char*) this + 0xa68) != 0) {
		return;
	}
	pMode = *(void**) ((char*) this + 0x964);
	if ((g_pLevelDemoPlaybackController == 0 ||
		 *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) &&
		((int(__fastcall*)(void*)) (*(void***) pMode)[0x60 / 4])(pMode) == 0) {
		return;
	}
	ScreenPoint.m_nX = (short) (*(int*) ((char*) this + 0x904) + Point.m_nX);
	ScreenPoint.m_nY = (short) (Point.m_nY + *(short*) ((char*) this + 0x908));
	nScreenX = ScreenPoint.m_nX;
	nScreenY = ScreenPoint.m_nY;
	pInputView = (char*) this - 0x14;
	fMaskedCell = ((int(__fastcall*)(void*, int, int, int, int*, int*)) 0x00401f5f)(
		pInputView, 0, nScreenX, nScreenY, (int*) 0x0049ea14, (int*) 0x0049ea18);
	*(int*) 0x0049ea1c = fMaskedCell;
	if (fMaskedCell == 0) {
		pTransform = (LevelViewRotationTransform*) *(void**) ((char*) this + 0x900);
		pTransform->ScreenToGame(nScreenX, nScreenY, (int*) 0x0049ea14, (int*) 0x0049ea18);
	}
	TilePoint.m_nX = (short) *(int*) 0x0049ea14;
	TilePoint.m_nY = (short) *(int*) 0x0049ea18;
	switch (nButtonFlags) {
	case 0:
	case 3:
		((void(__fastcall*)(void*, int, CVSPoint*, CVSPoint*, int, int)) 0x00401e42)(
			pInputView, 0, &ScreenPoint, &TilePoint, 1, 0);
		return;
	case 1:
	case 4:
		((void(__fastcall*)(void*, int, CVSPoint*, CVSPoint*)) 0x00401fcd)(
			pInputView, 0, &ScreenPoint, &TilePoint);
		return;
	}
}

// FUNCTION: LEMBALL 0x00438170
void C2D::OnInside(unsigned short* param_1)
{
	void* pThis = this;
	if ((g_pLevelDemoPlaybackController == 0 || *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) &&
		((int(__fastcall*)(void*))(*(void***) *(void**) ((char*) pThis + 0x964) + 0x60 / 4))(
			*(void**) ((char*) pThis + 0x964)) == 0) {
		return;
	}
	*(unsigned short*) ((char*) pThis + 0x928) = param_1[0];
	*(unsigned short*) ((char*) pThis + 0x92a) = param_1[1];
	((C2D*) ((char*) pThis - 0x14))->SendCursorMsg();
}

// FUNCTION: LEMBALL 0x00436850
void C2D::Restart(void)
{
	void* pThis = this;
	*(int*) ((char*) pThis + 0x918) = 0;
	*(int*) ((char*) pThis + 0x91c) = 0;
	*(int*) ((char*) pThis + 0x2214) = 1;
	SetLevelScreenActionPanelPauseActive(*(void**) ((char*) pThis + 0x97c), 0, 0);
	((void(__fastcall*)(void*)) 0x4021ee)(*(void**) ((char*) pThis + 0x96c));
	((void(__fastcall*)(void*, int, int)) 0x465aa0)(*(void**) ((char*) pThis + 0x978), 0, 0);
	*(int*) ((char*) pThis + 0x8f8) = *(int*) 0x49ce04;
	*(int*) (*(int*) 0x49cb68 + 4) = *(int*) ((char*) pThis + 0x8f4);
	*(int*) (*(int*) ((char*) pThis + 0x96c) + 0xf0) = *(int*) ((char*) pThis + 0x8f4);
	((CAI*) *(void**) ((char*) pThis + 0x96c))->Start();
	*(int*) ((char*) pThis + 0x8f0) = *(int*) (*(int*) ((char*) pThis + 0x96c) + 0xf0);
}

// FUNCTION: LEMBALL 0x0043cbb0
void C2D::DrawSlinky(CViewData& ViewData)
{
	static const int anResources[4] = {0x9c, 0x9e, 0x9d, 0x9f};
	unsigned int nVariant;
	int nFrame;
	short nX;
	short nY;

	nVariant = (unsigned short) ViewData.m_nVariant1C;
	nX = (short) (ViewData.m_nX04 - g_anC2DTrapOffsets[nVariant * 2]);
	nY = (short) (ViewData.m_nY08 - g_anC2DTrapOffsets[nVariant * 2 + 1]);
	if (ViewData.m_nState18 == 0x18) {
		if (nVariant < 4) {
			((CAnimsManagerView*) m_pAnimsManager0A40)
				->EmitLevelScreenVariantEntry(nX, nY, anResources[nVariant], 0, 0, 0);
		}
		return;
	}
	if (ViewData.m_nState18 != 0x1b) {
		return;
	}
	nFrame = ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) * 15 / 1000;
	if (nFrame > 12) {
		nFrame = 12;
	}
	if (nVariant < 4) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry(nX, nY, anResources[nVariant], nFrame, 0, 0);
	}
}

// FUNCTION: LEMBALL 0x0043d130
void C2D::DrawCatapult(CViewData& ViewData)
{
	int nRemap = ViewData.m_nVariant1C != 0 ? m_nLemmingRemap0968 : 0;
	int nFrame = ViewData.m_nFrame20;
	short nX = (short) ViewData.m_nX04;
	short nY = (short) ViewData.m_nY08;
	switch (ViewData.m_nState18) {
	case 0x18:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  0,
										  0,
										  0);
		return;
	case 0x19:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  0,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x30),
										  (short) (nY - 0x3c),
										  0x74,
										  nFrame,
										  ViewData.m_pFrameSelector24,
										  nRemap);
		return;
	case 0x1a:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequenceSecondaryResourceId,
										  nFrame + 0x640,
										  ViewData.m_pFrameSelector24,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x30),
										  (short) (nY - 0x3c),
										  0x74,
										  nFrame,
										  ViewData.m_pFrameSelector24,
										  nRemap);
		return;
	case 0x1b:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequencePrimaryResourceId,
										  1,
										  0,
										  0);
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - 0x28),
										  (short) (nY - 0x3c),
										  g_nLevelScreenCompositeSequenceSecondaryResourceId,
										  nFrame + 0x640,
										  ViewData.m_pFrameSelector24,
										  0);
	}
}

// FUNCTION: LEMBALL 0x0043d590
void C2D::DrawDoor(CViewData& ViewData)
{
	CAnimsManagerView* pManager;
	int nElapsed;
	int nResource;
	int nFrame;
	int nRemap;
	short nX;
	short nY;

	pManager = (CAnimsManagerView*) m_pAnimsManager0A40;
	nX = (short) ViewData.m_nX04;
	nY = (short) ViewData.m_nY08;
	nElapsed = (int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20;
	if (ViewData.m_nObjectType28 == 0x19) {
		nResource = 0x29;
	}
	else if (ViewData.m_nObjectType28 == 0x1a) {
		nResource = 0x28;
	}
	switch (ViewData.m_nState18) {
	case 0x1c:
		nRemap = -1;
		switch (ViewData.m_nVariant1C) {
		case 0x15:
			nRemap = 3;
			break;
		case 0x16:
			nRemap = 1;
			break;
		case 0x17:
			nRemap = 4;
			break;
		}
		if (nRemap >= 0) {
			nRemap = nRemap < 4 ? m_anPlayerRemaps0064[nRemap] : 0;
			pManager->EmitLevelScreenVariantEntry((short) (nX - 10), (short) (nY - 0x2c), 0xb2, 0, 0, nRemap);
		}
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 0, 0, 0);
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 1, 0, 0);
		return;
	case 0x1d:
	case 0x1e:
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 0, 0, 0);
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 1, 0, 0);
		return;
	case 0x20:
	case 0x22:
		nFrame = nElapsed * 15;
		nFrame = (nFrame + ((nFrame >> 31) & 0x3ff)) >> 10;
		if (nFrame > 7) {
			nFrame = 7;
		}
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 0, 0, 0);
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a),
											  (short) (nY - 0x18),
											  nResource,
											  ViewData.m_nState18 == 0x20 ? nFrame + 1 : 8 - nFrame,
											  0,
											  0);
		return;
	case 0x21:
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 0, 0, 0);
		pManager->EmitLevelScreenVariantEntry((short) (nX - 0x1a), (short) (nY - 0x18), nResource, 8, 0, 0);
	}
}

// FUNCTION: LEMBALL 0x0043d990
void C2D::DrawTrapDoor(CViewData& ViewData)
{
	CAnimsManagerView* pManager;
	unsigned int nFrame;
	unsigned int nTopFrame;
	int nResource;
	short nX;
	short nY;
	short nFxX;
	short nFxY;

	pManager = (CAnimsManagerView*) m_pAnimsManager0A40;
	nX = (short) (ViewData.m_nX04 - 0x30);
	nY = (short) (ViewData.m_nY08 - 0x28);
	nFxX = (short) (ViewData.m_nX04 - 0x20);
	nFxY = (short) (ViewData.m_nY08 + 0x26);
	nFrame = (unsigned int) ((int) ViewData.m_pFrameSelector24 - ViewData.m_nFrame20) / 0x42;
	switch (ViewData.m_nState18) {
	case 0x1f:
		if (nFrame > 0x28) {
			nFrame = 0x28;
		}
		nTopFrame = nFrame;
		if (nFrame > 0x20) {
			pManager
				->EmitLevelScreenVariantEntry(nX, nY, g_nLevelScreenMultiStageSequencePrimaryResourceId, 0x21, 0, 0);
			nTopFrame = nFrame + 1;
		}
		pManager
			->EmitLevelScreenVariantEntry(nX, nY, g_nLevelScreenMultiStageSequencePrimaryResourceId, nTopFrame, 0, 0);
		if (nFrame > 0x17) {
			if (nFrame > 0x21) {
				nFrame = 0x21;
			}
			pManager->EmitLevelScreenVariantEntry(nFxX, nFxY, 0x1f, nFrame - 0x17, 0, 0);
		}
		return;
	case 0x20:
	case 0x21:
	case 0x22:
		if (ViewData.m_nState18 == 0x20) {
			if (nFrame > 0xe) {
				nFrame = 0xe;
			}
			nTopFrame = nFrame + 1;
		}
		else if (ViewData.m_nState18 == 0x21) {
			nTopFrame = 0xf;
		}
		else {
			if (nFrame > 7) {
				nFrame = 7;
			}
			nTopFrame = 8 - nFrame;
		}
		pManager->EmitLevelScreenVariantEntry(nX, nY, g_nLevelScreenMultiStageSequenceSecondaryResourceId, 0, 0, 0);
		pManager
			->EmitLevelScreenVariantEntry(nX, nY, g_nLevelScreenMultiStageSequenceSecondaryResourceId, nTopFrame, 0, 0);
		pManager->EmitLevelScreenVariantEntry(nFxX, nFxY, 0x1f, 10, 0, 0);
		return;
	case 0x23:
		if (nFrame > 0x28) {
			nFrame = 0x28;
		}
		if ((int) (0x28 - nFrame) < 0x21) {
			nResource = 0x28;
		}
		else {
			pManager
				->EmitLevelScreenVariantEntry(nX, nY, g_nLevelScreenMultiStageSequencePrimaryResourceId, 0x21, 0, 0);
			nResource = 0x29;
		}
		pManager->EmitLevelScreenVariantEntry(nX,
											  nY,
											  g_nLevelScreenMultiStageSequencePrimaryResourceId,
											  nResource - nFrame,
											  0,
											  0);
		if (nFrame < 0xd) {
			pManager->EmitLevelScreenVariantEntry(nFxX, nFxY, 0x1f, 10, 0, 0);
		}
		else if (nFrame < 0x17) {
			pManager->EmitLevelScreenVariantEntry(nFxX, nFxY, 0x1f, 0x17 - nFrame, 0, 0);
		}
	}
}

// FUNCTION: LEMBALL 0x00437970
int C2D::ScreenToGame(int nX, int nY, int* pTileX, int* pTileY)
{
	char* pThis;
	char* pLevel;
	int nMaxX;
	int nMaxY;
	int nScanY;
	int nStartX;
	int nEndX;
	int nScanX;
	int nCellX;
	int nCellY;
	int nScreenX;
	int nScreenY;
	int nPixelX;
	int nPixelY;

	pThis = (char*) this;
	pLevel = (char*) m_pLevel0914;
	nMaxX = *(int*) (pLevel + 0x10) * 0x10 - 1;
	nMaxY = *(int*) (pLevel + 0x14) * 0x10 - 1;
	nScanY = nY + 0x50;
	nStartX = nX - 0x20;
	nEndX = nX + 0x20;
	while (nScanY >= nY) {
		nScanX = nStartX;
		while (nScanX <= nEndX) {
			((LevelViewRotationTransform*) m_pLevel0914)->ScreenToGame(nScanX, nScanY, &nCellX, &nCellY);
			if (nCellX >= 0 && nCellX <= nMaxX && nCellY >= 0 && nCellY <= nMaxY) {
				nCellX = (nCellX + ((nCellX >> 31) & 0x0f)) >> 4;
				nCellY = (nCellY + ((nCellY >> 31) & 0x0f)) >> 4;
				nScreenX = nCellX << 4;
				nScreenY = nCellY << 4;
				((LevelViewRotationTransform*) m_pLevel0914)->GameToScreen(&nScreenX, &nScreenY);
				pLevel = (char*) m_pLevel0914;
				nScreenY -= *(unsigned short*) (*(char**) (pLevel + 0x0c) +
												(*(int*) (pLevel + 0x10) * nCellY + nCellX) * 0x0c + 8);
				if (nX >= nScreenX - 0x10 && nX <= nScreenX + 0x0f && nY >= nScreenY - 0x10 && nY <= nScreenY + 0x0f) {
					nPixelX = nX - (nScreenX - 0x10);
					nPixelY = nY - (nScreenY - 0x10);
					if (nPixelX >= 0 && nPixelY >= 0 && nPixelX < 0x20 && nPixelY < 0x20 &&
						((int(__fastcall*)(void*, int, int, int, int)) 0x40159b)(
							*(char**) (pLevel + 0x0c) + (*(int*) (pLevel + 0x10) * nCellY + nCellX) * 0x0c,
							0,
							nPixelX,
							nPixelY,
							*(short*) (pThis + 0x218c) != 0)) {
						*pTileX = nCellX * 0x10 + 8;
						*pTileY = nCellY * 0x10 + 8;
						return 1;
					}
				}
			}
			nScanX += 0x10;
		}
		nScanY -= 8;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00440560
void C2D::DrawObjectsZBuff(void)
{
	char* pThis;
	char* pEntry;
	char* pStatic;
	int nDynamic;
	int nStatic;
	int nOffset;
	int nDynamicKeyValid;
	int nStaticKeyValid;
	unsigned short nDynamicKey;
	unsigned short nStaticKey;
	int Point[3];

	pThis = (char*) this;
	nStatic = m_nStaticEntryCount2188++;
	pEntry = pThis + 0x1468 + nStatic * 0x10;
	*(short*) (pEntry + 4) = *(short*) (pThis + 0x958);
	*(short*) (pEntry + 6) = *(short*) (pThis + 0x95a);
	*(short*) (pEntry + 8) = 0;
	*(short*) (pEntry + 10) = 0;
	*(int*) (pEntry + 12) = 0;
	((void(__fastcall*)(void*, int, void*))(*(void***) pEntry)[1])(pEntry, 0, m_pRenderQueue0970);
	for (nDynamic = 0, nOffset = 0; nDynamic < (int) m_nViewDataCount0964; ++nDynamic, nOffset += 0x4c) {
		pEntry = (char*) m_pViewData095C + nOffset;
		*(short*) (pEntry + 0x10) = (short) *(int*) (pEntry + 4);
		*(short*) (pEntry + 0x12) = (short) *(int*) (pEntry + 8);
		Point[0] = *(int*) (pEntry + 4);
		Point[1] = *(int*) (pEntry + 8);
		Point[2] = *(int*) (pEntry + 0x0c);
		((void(__fastcall*)(void*, int, int*, int*)) 0x40199c)(m_pLevel0914, 0, &Point[0], &Point[1]);
		Point[0] -= m_nCameraOriginX0918;
		Point[1] = Point[1] - Point[2] - m_nCameraOriginY091C;
		*(int*) (pEntry + 4) = Point[0];
		*(int*) (pEntry + 8) = Point[1];
		*(int*) (pEntry + 0x0c) = Point[2];
	}
	((void(__fastcall*)(void*, int)) 0x40226b)(this, 0);
	*(int*) ((char*) m_pAnimsManager0A40 + 0xac) = 0x80000;
	pStatic = *(char**) ((char*) m_pLevelMode096C + 0x8c);
	*(char**) (pThis + 0x2220) = *(char**) pStatic;
	*(int*) (pThis + 0x2224) = *(int*) (pStatic + 4);
	nDynamic = 0;
	nStatic = 0;
	nOffset = 0;
	nDynamicKeyValid = 0;
	nStaticKeyValid = 0;
	while (nDynamic < (int) m_nViewDataCount0964 && nStatic < *(int*) (pThis + 0x2224)) {
		if (!nDynamicKeyValid) {
			nDynamicKey = ((unsigned short(__fastcall*)(void*, int, int)) 0x4032b5)(this, 0, nDynamic);
			nDynamicKeyValid = 1;
		}
		if (!nStaticKeyValid) {
			pStatic = *(char**) (pThis + 0x2220) + nOffset;
			nStaticKey = *(short*) (*(char**) (pStatic + 8) + 8) + *(short*) (pStatic + 4);
			nStaticKeyValid = 1;
		}
		if (nStaticKey < nDynamicKey) {
			((void(__fastcall*)(void*, int, int, unsigned short)) 0x40319d)(this, 0, nStatic, nStaticKey);
			nOffset += 0x0c;
			++nStatic;
			nStaticKeyValid = 0;
		}
		else {
			((void(__fastcall*)(void*, int, int, unsigned short)) 0x401f46)(this, 0, nDynamic, nDynamicKey);
			++nDynamic;
			nDynamicKeyValid = 0;
		}
	}
	while (nDynamic < (int) m_nViewDataCount0964) {
		nDynamicKey = ((unsigned short(__fastcall*)(void*, int, int)) 0x4032b5)(this, 0, nDynamic);
		((void(__fastcall*)(void*, int, int, unsigned short)) 0x401f46)(this, 0, nDynamic, nDynamicKey);
		++nDynamic;
	}
	while (nStatic < *(int*) (pThis + 0x2224)) {
		pStatic = *(char**) (pThis + 0x2220) + nStatic * 0x0c;
		nStaticKey = *(short*) (*(char**) (pStatic + 8) + 8) + *(short*) (pStatic + 4);
		((void(__fastcall*)(void*, int, int, unsigned short)) 0x40319d)(this, 0, nStatic, nStaticKey);
		++nStatic;
	}
}

// FUNCTION: LEMBALL 0x004364d0
void C2D::OnLoaded(void)
{
	char* pThis;
	char* pWindow;
	char* pObject;
	void* pPanel;
	unsigned short nOldScale;
	unsigned int nScale;
	short Rect[4];

	pThis = (char*) this;
	((void(__fastcall*)(void*, int, int, int)) 0x403585)(this, 0, 1, 0);
	nScale = 2;
	if (*(int*) 0x4a630c || *(int*) 0x4a6304 || *(int*) 0x4a6288) {
		nScale = 1;
	}
	nOldScale = *(unsigned short*) (pThis + 0x160);
	*(short*) (pThis + 0x160) = (short) nScale;
	pWindow = *(char**) (pThis + 0x978);
	*(short*) (pThis + 0x940) = *(short*) (pWindow + 8);
	*(short*) (pThis + 0x942) = *(short*) (pWindow + 10);
	*(short*) (pThis + 0x940) = (short) (*(short*) (pThis + 0x940) / (int) nScale);
	*(short*) (pThis + 0x942) = (short) (*(short*) (pThis + 0x942) / (int) nScale);
	((void(__fastcall*)(void*, int)) 0x4036ca)(this, 0);
	if (*(short*) (pThis + 0x940) != *(short*) (pThis + 0x958) ||
		*(short*) (pThis + 0x942) != *(short*) (pThis + 0x95a)) {
		Rect[0] = *(short*) (pThis + 0x958);
		Rect[1] = *(short*) (pThis + 0x95a);
		Rect[2] = *(short*) (pThis + 0x950);
		Rect[3] = *(short*) (pThis + 0x954);
		((void(__fastcall*)(void*, int, short*))(*(void***) pWindow)[2])(pWindow, 0, Rect);
	}
	pPanel = AllocateVSMemBlock(0x58);
	if (pPanel) {
		pPanel = ConstructLevelScreenActionPanelResources(pPanel, 0, this);
	}
	*(void**) (pThis + 0x97c) = pPanel;
	if (nOldScale == nScale) {
		((void(__fastcall*)(void*, int, void*))(*(void***) this)[4])(this, 0, pWindow + 8);
	}
	else {
		((void(__fastcall*)(void*, int, unsigned int)) 0x4662b0)(pWindow, 0, *(unsigned short*) (pThis + 0x160));
	}
	pObject = *(char**) (*(char**) (pWindow + 0x4c) + 0x0c);
	pObject = (char*) *(int*) (*(char**) (pObject + 0x40) + 4) + (int) pObject + 0x40;
	((void(__fastcall*)(void*, int, int))(*(void***) pObject)[0x11])(pObject, 0, 3000);
	pObject = *(char**) (*(char**) ((char*) m_pRenderQueue0970 + 0x0c) + 0x44);
	((void(__fastcall*)(void*, int, int)) 0x466840)(pObject, 0, 1);
	*(int*) (pThis + 0x2214) = 1;
	*(int*) (pThis + 0x2218) = 0;
	if (*(void**) 0x4a6408) {
		*(void**) (*(char**) 0x4a6408 + 0x34) = pWindow;
	}
	((void(__fastcall*)(void*, int, int)) 0x465aa0)(pWindow, 0, 0);
	((void(__fastcall*)(void*, int)) 0x4029aa)(m_pLevelMode096C, 0);
}

// FUNCTION: LEMBALL 0x00437e90
void C2D::SetMouseShape(void)
{
	char* pThis;
	char* pWindow;
	char* pOrigin;
	short Cursor[2];
	short Point[2];
	int nHotspot;
	int nScale;
	int nHit;
	unsigned short nBlink;

	pThis = (char*) this;
	if (*(int*) (pThis + 0xa7c)) {
		return;
	}
	pWindow = *(char**) (pThis + 0x978);
	pOrigin = pWindow + 0x0c;
	nScale = *(int*) (pWindow + 0x38);
	Cursor[0] = (short) ((*(short*) (*(char**) 0x4a9bf4 + 0x10) - *(short*) pOrigin) / nScale);
	Cursor[1] = (short) ((*(short*) (*(char**) 0x4a9bf4 + 0x12) - *(short*) (pOrigin + 2)) / nScale);
	if (IsPointInsideLevelScreenActionPanel(*(void**) (pThis + 0x97c), 0, Cursor)) {
		*(int*) (pThis + 0x1a4) = 3;
		return;
	}
	Point[0] = (short) (m_nCameraOriginX0918 + *(short*) (pThis + 0x93c));
	Point[1] = (short) (m_nCameraOriginY091C + *(short*) (pThis + 0x93e));
	if (Cursor[0] < *(short*) (pThis + 0x48) ||
		Cursor[0] >= (short) (*(short*) (pThis + 0x44) + *(short*) (pThis + 0x48)) ||
		Cursor[1] < *(short*) (pThis + 0x4a) ||
		Cursor[1] >= (short) (*(short*) (pThis + 0x46) + *(short*) (pThis + 0x4a))) {
		((void(__fastcall*)(void*, int, int, int)) 0x403585)(this, 0, 1, 0);
		return;
	}
	nHit = ((int(__fastcall*)(void*, int, short*, int*, int)) 0x4025ef)(this, 0, Point, &nHotspot, 0);
	if (nHit) {
		if (*(int*) (pThis + 0x1a4) != 2) {
			*(int*) (pThis + 0x1a4) = 2;
			*(int*) (pThis + 0x1a8) = g_nLevelFrameClockTimeMs;
			((void(__fastcall*)(void*, int, int, int)) 0x403585)(this, 0, 1, 4);
		}
		return;
	}
	nHit = ScreenToGame(Point[0], Point[1], (int*) 0x49ea14, (int*) 0x49ea18);
	*(int*) 0x49ea1c = nHit;
	if (nHit) {
		*(int*) (pThis + 0x1a4) = 0;
		*(int*) (pThis + 0x1a8) = g_nLevelFrameClockTimeMs;
		((void(__fastcall*)(void*, int, int, int)) 0x403585)(this, 0, 1, *(int*) (pThis + 0x18c) ? 1 : 0);
		return;
	}
	if (*(int*) (pThis + 0x1a4) != 1) {
		*(int*) (pThis + 0x1a4) = 1;
		*(int*) (pThis + 0x1a8) = g_nLevelFrameClockTimeMs;
		((void(__fastcall*)(void*, int, int, int)) 0x403585)(this, 0, 1, 2);
		return;
	}
	if ((unsigned int) (*(int*) (pThis + 0x1a8) + 100) < (unsigned int) g_nLevelFrameClockTimeMs) {
		nBlink = *(unsigned short*) (pThis + 0x1ac) ^ 1;
		*(unsigned short*) (pThis + 0x1ac) = nBlink;
		*(int*) (pThis + 0x1a8) = g_nLevelFrameClockTimeMs;
		((void(__fastcall*)(void*, int, int, int)) 0x403585)(this, 0, 1, nBlink + 2);
	}
}
