#include "views/2d/C2DRender.h"

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

extern int g_nLevelScreenTimedVariantResourceId;
extern int g_nLevelScreenMappedVariantResourceId0x209;
extern int g_nLevelScreenMultiPhaseSequenceResourceId;
extern int g_nLevelScreenCompositeSequencePrimaryResourceId;
extern int g_nLevelScreenCompositeSequenceSecondaryResourceId;
extern int g_nLevelFrameClockTimeMs;

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

// MACINTOSH: C2D::DrawRocket(CViewData&)
// FUNCTION: LEMBALL 0x0043cb70
void C2D::DrawRocket(CViewData& ViewData)
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
