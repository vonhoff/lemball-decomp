#define LEMBALL_CBALL_HARDTAIL_METHODS
#include "AI/CBall.h"

extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x004216c0
void CBall::Set(AICOORD oCurrent, AICOORD oTarget, int nSpeed)
{
	m_nPositionX9C = oCurrent.x;
	m_nPositionYA0 = oCurrent.y;
	m_nPositionZA4 = oCurrent.z;
	m_nSavedPosX40 = oCurrent.x;
	m_nSavedPosY44 = oCurrent.y;
	m_nSavedPosZ48 = oCurrent.z;
	m_nTargetX124 = oTarget.x;
	m_nTargetY128 = oTarget.y;
	m_nTargetZ12C = oTarget.z;
	m_nStateB8 = 0x25;
	m_wDirectionBC = 0;
	if (*(unsigned short*) ((char*) g_pActiveManagedEntityOwner + 0x54) < 7) {
		m_wSpeed130 = (short) *(int*) (m_nObjectType64 * 4 + 0x49d070);
	}
	else {
		m_wSpeed130 = (short) nSpeed;
	}
	if (*(unsigned short*) ((char*) this + 0x130) > 1) {
		m_nActive134 = 1;
	}
	else {
		m_wSpeed130 = (short) *(int*) (m_nObjectType64 * 4 + 0x49d070);
		m_nActive134 = 1;
	}
	m_nActive134 = 1;
}

// FUNCTION: LEMBALL 0x00421b40
void CBall::SetHeightCorrect(void)
{
	int xq = m_nPositionX9C >> 12;
	int yq = m_nPositionYA0 >> 12;
	int nTileX = m_nPositionX9C >> 16;
	int nTileY = m_nPositionYA0 >> 16;
	unsigned int z = 0;
	if (!(xq < 0 || yq < 0 || *(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
		  *(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) (((nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc) +
					 *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			xq & 0xf,
			yq & 0xf);
	}
	*(unsigned int*) ((char*) this + 0xa4) = (z & 0xffff) << 12;
}

extern int g_nLevelFrameClockTick;
extern int Distance2DIntPixels(int x1, int y1, int x2, int y2);

// FUNCTION: LEMBALL 0x00421770
void CBall::StartMovement(int nUseTarget)
{
	AICOORD oCurrent;
	AICOORD oTarget;
	int nTargetX;
	int nTargetY;
	int nDistance;
	int nDuration;

	*(int*) ((char*) this + 0x138) = nUseTarget;
	if (nUseTarget == 0) {
		nTargetX = m_nSavedPosX40;
		nTargetY = m_nSavedPosY44;
	}
	else {
		nTargetX = m_nTargetX124;
		nTargetY = m_nTargetY128;
	}
	nDistance = Distance2DIntPixels(m_nPositionX9C >> 12, m_nPositionYA0 >> 12, nTargetX >> 12, nTargetY >> 12);
	*(int*) ((char*) this + 0xc8) = g_nLevelFrameClockTick;
	nDuration = (int) ((unsigned short) m_wSpeed130 * nDistance) / 50;
	*(int*) ((char*) this + 0x88) = nDuration;
	if (nDuration == 0) {
		*(int*) ((char*) this + 0x88) = 1;
	}
	*(int*) ((char*) this + 0xcc) = *(int*) ((char*) this + 0x88) + g_nLevelFrameClockTick;
	oCurrent.x = m_nPositionX9C;
	oCurrent.y = m_nPositionYA0;
	oTarget.x = nTargetX;
	oTarget.y = nTargetY;
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x40326f)((char*) this + 0x78,
																   0,
																   oTarget.x,
																   oTarget.y,
																   oCurrent.x,
																   oCurrent.y);
}

// FUNCTION: LEMBALL 0x00421da0
void CBall::LoadLevel(void* pStreamCursor)
{
	AICOORD oCurrent;
	AICOORD oTarget;
	unsigned short** ppCursor;
	unsigned short* pCursor;
	unsigned short nSpeed;

	oCurrent.x = (int) 0xaa55aa55;
	oCurrent.y = (int) 0xaa55aa55;
	oCurrent.z = (int) 0xaa55aa55;
	oTarget.x = (int) 0xaa55aa55;
	oTarget.y = (int) 0xaa55aa55;
	oTarget.z = (int) 0xaa55aa55;
	ppCursor = (unsigned short**) pStreamCursor;
	if (*(unsigned short*) ((char*) g_pActiveManagedEntityOwner + 0x54) > 1) {
		pCursor = *ppCursor;
		((void(__fastcall*)(void*, int, int)) 0x402293)(this, 0, (unsigned int) *pCursor);
		*ppCursor = pCursor + 1;
	}
	pCursor = *ppCursor;
	oCurrent.x = (unsigned int) pCursor[0] << 12;
	*ppCursor = pCursor + 1;
	oCurrent.y = (unsigned int) pCursor[1] << 12;
	*ppCursor = pCursor + 2;
	oCurrent.z = (unsigned int) pCursor[2] << 12;
	*ppCursor = pCursor + 3;
	oTarget.x = (unsigned int) pCursor[3] << 12;
	*ppCursor = pCursor + 4;
	oTarget.y = (unsigned int) pCursor[4] << 12;
	*ppCursor = pCursor + 5;
	oTarget.z = (unsigned int) pCursor[5] << 12;
	*ppCursor = pCursor + 6;
	nSpeed = pCursor[6];
	*ppCursor = pCursor + 7;
	Set(oCurrent, oTarget, nSpeed);
}

// FUNCTION: LEMBALL 0x00421bc0
int CBall::Process(void)
{
	char* pThis;
	unsigned short nPhase;

	pThis = (char*) this;
	if (m_nStateB8 != 0x25) {
		if (m_nStateB8 != 0x26) {
			return 1;
		}
		if ((unsigned int) *(int*) (pThis + 0xcc) >= (unsigned int) g_nLevelFrameClockTick) {
			return 1;
		}
		((void(__fastcall*)(void*, int, void*)) 0x40302b)(*(void**) 0x49e1bc, 0, this);
		return 0;
	}
	nPhase = *(unsigned short*) (pThis + 0xbc);
	switch (nPhase) {
	case 0:
		*(int*) (pThis + 0xcc) = g_nLevelFrameClockTick;
		m_nPositionX9C = m_nSavedPosX40;
		m_nPositionYA0 = m_nSavedPosY44;
		m_nPositionZA4 = m_nSavedPosZ48;
		SetHeightCorrect();
		*(unsigned short*) (pThis + 0xbc) = 1;
		break;
	case 1:
		if ((unsigned int) *(int*) (pThis + 0xcc) < (unsigned int) g_nLevelFrameClockTick) {
			*(unsigned short*) (pThis + 0xbc) = 2;
			StartMovement(1);
		}
		break;
	case 2:
		if ((unsigned int) *(int*) (pThis + 0xcc) < (unsigned int) g_nLevelFrameClockTick) {
			m_nPositionX9C = m_nTargetX124;
			m_nPositionYA0 = m_nTargetY128;
			m_nPositionZA4 = m_nTargetZ12C;
			SetHeightCorrect();
			*(unsigned short*) (pThis + 0xbc) = 3;
			break;
		}
		((void(__fastcall*)(void*, int))(*(void***) this)[0x11])(this, 0);
		break;
	case 3:
		*(int*) (pThis + 0xcc) = g_nLevelFrameClockTick;
		m_nPositionX9C = m_nTargetX124;
		m_nPositionYA0 = m_nTargetY128;
		m_nPositionZA4 = m_nTargetZ12C;
		SetHeightCorrect();
		*(unsigned short*) (pThis + 0xbc) = 4;
		break;
	case 4:
		if ((unsigned int) *(int*) (pThis + 0xcc) < (unsigned int) g_nLevelFrameClockTick) {
			*(unsigned short*) (pThis + 0xbc) = 5;
			StartMovement(0);
		}
		break;
	case 5:
		if ((unsigned int) *(int*) (pThis + 0xcc) < (unsigned int) g_nLevelFrameClockTick) {
			m_nPositionX9C = m_nSavedPosX40;
			m_nPositionYA0 = m_nSavedPosY44;
			m_nPositionZA4 = m_nSavedPosZ48;
			SetHeightCorrect();
			*(unsigned short*) (pThis + 0xbc) = 0;
			break;
		}
		((void(__fastcall*)(void*, int))(*(void***) this)[0x11])(this, 0);
	}
	((void(__fastcall*)(void*, int)) 0x401500)(this, 0);
	return 1;
}
