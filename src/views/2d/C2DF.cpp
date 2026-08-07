#include "views/2d/C2DF.h"

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern void EmitLevelScreenVariantEntry(void* pObject,
										short x,
										short y,
										int nResourceId,
										int nFlags,
										void* pFrameSelector,
										int nValue);
extern void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);

class CAnimsManagerView {
public:
	void EmitLevelScreenVariantEntry(short x, short y, int nResourceId, int nFlags, void* pFrameSelector, int nValue);
};

class CRenderQueueClientView {
public:
	virtual void Reserved(void) = 0;
	virtual void Reset(void* pRenderQueue) = 0;
};

// GLOBAL: LEMBALL 0x0049ef38
static const int g_anC2DFBulletResources[8] = {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x5f};
// GLOBAL: LEMBALL 0x00497064
static const short g_anC2DFAmmoOffsets[4] = {8, 0x10, 0x10, 0x10};
// GLOBAL: LEMBALL 0x00497070
static const short g_anC2DFBulletOffsets[2] = {4, 4};
// GLOBAL: LEMBALL 0x00497054
static const short g_anC2DFKeyOffsets[2] = {8, 0x20};
// GLOBAL: LEMBALL 0x00497084
static const short g_anC2DFBonusOffsets[2] = {0x10, 0x10};
// GLOBAL: LEMBALL 0x00497088
static const short g_anC2DFTimeBonusOffsets[2] = {0x10, 0x12};
// GLOBAL: LEMBALL 0x0049703c
static const short g_anC2DFCrateOffsets[4] = {8, 0x18, 0x22, 0x32};
// GLOBAL: LEMBALL 0x0049704c
static const short g_anC2DFMineOffsets[4] = {0x1e, 0x23, 2, 2};
// GLOBAL: LEMBALL 0x00497074
static const short g_anC2DFBallOffsets[4] = {10, 0x0f, 0x0f, 0x11};
// GLOBAL: LEMBALL 0x00497080
static const short g_anC2DFFlagOffsets[2] = {0x0f, 0x1c};
// GLOBAL: LEMBALL 0x00497098
static const short g_anC2DFTrampolineOffsets[2] = {0x16, 0x16};

extern int g_nLevelScreenTimedVariantResourceId;

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

// MACINTOSH: C2DF::DrawBullet(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c610
void C2DF::DrawBullet(CViewData& ViewData, int)
{
	int nDirection = (ViewData.m_nDirection00 + m_nViewRotation090C * 2) & 7;
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFBulletOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DFBulletOffsets[1]),
									  g_anC2DFBulletResources[nDirection],
									  0,
									  ViewData.m_pFrameSelector24,
									  0);
}

// MACINTOSH: C2DF::DrawAmmo(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c660
void C2DF::DrawAmmo(CViewData& ViewData, int)
{
	if (ViewData.m_nState18 >= 0x18) {
		if (ViewData.m_nState18 <= 0x19) {
			((CAnimsManagerView*) m_pAnimsManager0A40)
				->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFAmmoOffsets[0]),
											  (short) (ViewData.m_nY08 - g_anC2DFAmmoOffsets[1]),
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
				->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFAmmoOffsets[2]),
											  (short) (ViewData.m_nY08 - g_anC2DFAmmoOffsets[3]),
											  0x82,
											  ViewData.m_nFrame20,
											  ViewData.m_pFrameSelector24,
											  0);
		}
	}
}

// MACINTOSH: C2DF::DrawHand(CViewData&)
// FUNCTION: LEMBALL 0x0043c7f0
void C2DF::DrawHand(CViewData& ViewData)
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

// MACINTOSH: C2DF::DrawBalloonPost(CViewData&, int)
// FUNCTION: LEMBALL 0x0043c9f0
void C2DF::DrawBalloonPost(CViewData& ViewData, int nPlayer)
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

// MACINTOSH: C2DF::DrawTrampoline(CViewData&)
// FUNCTION: LEMBALL 0x0043ca30
void C2DF::DrawTrampoline(CViewData& ViewData)
{
	int nX = ViewData.m_nX04 - g_anC2DFTrampolineOffsets[0];
	int nY = ViewData.m_nY08 - g_anC2DFTrampolineOffsets[1];
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

// MACINTOSH: C2DF::DrawLaserFire(CViewData&)
// FUNCTION: LEMBALL 0x0043ce30
void C2DF::DrawLaserFire(CViewData& ViewData)
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

// MACINTOSH: C2DF::DrawCrate(CViewData&, int)
// FUNCTION: LEMBALL 0x0043d070
void C2DF::DrawCrate(CViewData& ViewData, int)
{
	if (ViewData.m_nState18 == 0x18) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFCrateOffsets[0]),
										  (short) (ViewData.m_nY08 - g_anC2DFCrateOffsets[1]),
										  0x86,
										  ViewData.m_nFrame20,
										  ViewData.m_pFrameSelector24,
										  0);
		return;
	}
	if (ViewData.m_nState18 >= 0x19 && ViewData.m_nState18 <= 0x1a) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFCrateOffsets[2]),
										  (short) (ViewData.m_nY08 - g_anC2DFCrateOffsets[3]),
										  0x87,
										  ViewData.m_nFrame20,
										  ViewData.m_pFrameSelector24,
										  0);
	}
}

// MACINTOSH: C2DF::DrawTimeBonus(CViewData&)
// FUNCTION: LEMBALL 0x0043d0f0
void C2DF::DrawTimeBonus(CViewData& ViewData)
{
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFTimeBonusOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DFTimeBonusOffsets[1]),
									  0x85,
									  ViewData.m_nFrame20,
									  ViewData.m_pFrameSelector24,
									  0);
}

// MACINTOSH: C2DF::DrawBall(CViewData&)
// FUNCTION: LEMBALL 0x0043d420
void C2DF::DrawBall(CViewData& ViewData)
{
	int nX = ViewData.m_nX04;
	int nY = ViewData.m_nY08;
	switch (ViewData.m_nState18) {
	case 0x25:
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - g_anC2DFBallOffsets[0]),
										  (short) (nY - g_anC2DFBallOffsets[1]),
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
		->EmitLevelScreenVariantEntry((short) (nX - g_anC2DFBallOffsets[2]),
									  (short) (nY - g_anC2DFBallOffsets[3]),
									  0x7e,
									  nFrame,
									  0,
									  0);
}

// MACINTOSH: C2DF::DrawKey(CViewData&, int)
// FUNCTION: LEMBALL 0x0043d4b0
void C2DF::DrawKey(CViewData& ViewData, int nPlayer)
{
	int nRemap;
	if (nPlayer < 4) {
		nRemap = m_anPlayerRemaps0064[nPlayer];
	}
	else {
		nRemap = 0;
	}
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFKeyOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DFKeyOffsets[1]),
									  0xb2,
									  0,
									  0,
									  nRemap);
}

// MACINTOSH: C2DF::DrawMine(CViewData&)
// FUNCTION: LEMBALL 0x0043d500
void C2DF::DrawMine(CViewData& ViewData)
{
	int nX = ViewData.m_nX04;
	int nY = ViewData.m_nY08;
	int nFrame = ViewData.m_nFrame20;
	if (ViewData.m_nState18 < 0x18) {
		return;
	}
	if (ViewData.m_nState18 <= 0x1a) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - g_anC2DFMineOffsets[2]),
										  (short) (nY - g_anC2DFMineOffsets[3]),
										  0x84,
										  0,
										  0,
										  0);
		return;
	}
	if (ViewData.m_nState18 == 0x1b) {
		((CAnimsManagerView*) m_pAnimsManager0A40)
			->EmitLevelScreenVariantEntry((short) (nX - g_anC2DFMineOffsets[0]),
										  (short) (nY - g_anC2DFMineOffsets[1]),
										  0x83,
										  nFrame,
										  ViewData.m_pFrameSelector24,
										  0);
	}
}

// MACINTOSH: C2DF::DrawFlag(CViewData&, eObjectType)
// FUNCTION: LEMBALL 0x0043d8d0
void C2DF::DrawFlag(CViewData& ViewData, int nObjectType)
{
	int nX = ViewData.m_nX04 - g_anC2DFFlagOffsets[0];
	int nY = ViewData.m_nY08 - g_anC2DFFlagOffsets[1];
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

// MACINTOSH: C2DF::DrawBonus(CViewData&)
// FUNCTION: LEMBALL 0x0043d950
void C2DF::DrawBonus(CViewData& ViewData)
{
	((CAnimsManagerView*) m_pAnimsManager0A40)
		->EmitLevelScreenVariantEntry((short) (ViewData.m_nX04 - g_anC2DFBonusOffsets[0]),
									  (short) (ViewData.m_nY08 - g_anC2DFBonusOffsets[1]),
									  0x81,
									  0,
									  ViewData.m_pFrameSelector24,
									  0);
}

// MACINTOSH: C2DF::ResetPrimitives()
// FUNCTION: LEMBALL 0x00440400
void C2DF::ResetPrimitives(void)
{
	typedef int(LEMBALL_FASTCALL * GetUploadStateProc)(void*);
	void* pContext = *(void**) ((char*) m_pRenderQueue0970 + 0x0c);
	int nUploadState = ((GetUploadStateProc) (*(void***) pContext)[2])(pContext);
	((CRenderQueueClientView*) m_RenderQueueClient0990)->Reset(m_pRenderQueue0970);
	InitializeHelperUploadStatePending(nUploadState);
}
