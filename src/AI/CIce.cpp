#include "AI/CIce.h"

#include "AI/CGameObject.h"
#include "AI/CInvisibleSwitch.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTick;

struct CGround {
	short GetZThunk(int nLocalX, int nLocalY);
};

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

typedef int(LEMBALL_FASTCALL* IceObjectPredicate)(CGameObject* pObject);
typedef void(LEMBALL_FASTCALL* IceObjectOneArgProc)(CGameObject* pObject, void* pUnusedEdx, int nArgument);
typedef void(LEMBALL_FASTCALL* IceObjectTwoArgProc)(CGameObject* pObject,
													void* pUnusedEdx,
													int nArgument1,
													int nArgument2);
typedef void(
	LEMBALL_FASTCALL* IceObjectAttachProc)(CGameObject* pObject, void* pUnusedEdx, int nMode, CIce* pIce, int nFlags);

// MACINTOSH: CIce::Set(unsigned short, const tCoord3d&, const tCoord3d&, int, int, unsigned char)
// FUNCTION: LEMBALL 0x0042cb00
void CIce::Set(unsigned short nSlot,
			   const tCoord3d& start,
			   const tCoord3d& end,
			   int nMoveX,
			   int nMoveY,
			   unsigned char fActive)
{
	char* pObject = (char*) this;
	char* pGrid = (char*) g_pLevelTileGrid;
	int nLeft = start.x;
	int nRight = end.x;
	int nTop = start.y;
	int nBottom = end.y;
	int x;
	int y;

	((CGameObject*) this)->SetId(nSlot);
	*(int*) (pObject + 0x13c) = 1;
	*(int*) (pObject + 0x150) = 0;
	*(int*) (pObject + 0xc8) = g_nLevelFrameClockTick;
	*(int*) (pObject + 0x144) = nMoveX;
	*(int*) (pObject + 0x148) = nMoveY;
	*(int*) (pObject + 0x14c) = fActive;
	*(int*) (pObject + 0x138) = fActive;
	if (nRight < nLeft) {
		x = nLeft;
		nLeft = nRight;
		nRight = x;
	}
	if (nBottom < nTop) {
		y = nTop;
		nTop = nBottom;
		nBottom = y;
	}
	*(short*) (pObject + 0x17c) = (short) nLeft;
	*(short*) (pObject + 0x17e) = (short) nTop;
	*(short*) (pObject + 0x182) = (short) nRight;
	*(short*) (pObject + 0x184) = (short) nBottom;
	if (nLeft < 0 || nTop < 0 || nLeft >> 4 >= *(int*) (pGrid + 0x10) || nTop >> 4 >= *(int*) (pGrid + 0x14)) {
		*(short*) (pObject + 0x180) = 0;
	}
	else {
		*(short*) (pObject + 0x180) =
			((CGround*) (*(char**) (pGrid + 0xc) + ((nTop >> 4) * *(int*) (pGrid + 0x10) + (nLeft >> 4)) * 0xc))
				->GetZThunk(nLeft & 0xf, nTop & 0xf);
	}
	if (nRight < 0 || nBottom < 0 || nRight >> 4 >= *(int*) (pGrid + 0x10) || nBottom >> 4 >= *(int*) (pGrid + 0x14)) {
		*(short*) (pObject + 0x186) = 0;
	}
	else {
		*(short*) (pObject + 0x186) =
			((CGround*) (*(char**) (pGrid + 0xc) + ((nBottom >> 4) * *(int*) (pGrid + 0x10) + (nRight >> 4)) * 0xc))
				->GetZThunk(nRight & 0xf, nBottom & 0xf);
	}
	*(int*) (pObject + 0x9c) = (int) start.x << 12;
	*(int*) (pObject + 0xa0) = (int) start.y << 12;
	*(int*) (pObject + 0xa4) = (int) start.z << 12;
	for (y = nTop; y <= nBottom; y += 0x10) {
		for (x = nLeft; x <= nRight; x += 0x10) {
			int nTileX = (x + (x >> 31 & 0xf)) >> 4;
			int nTileY = (y + (y >> 31 & 0xf)) >> 4;
			if (nTileX >= 0 && nTileY >= 0 && nTileX < *(int*) (pGrid + 0x10) && nTileY < *(int*) (pGrid + 0x14)) {
				unsigned char* pFlags =
					(unsigned char*) (*(char**) (pGrid + 0xc) + 7 + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0xc);
				*pFlags = (unsigned char) (*pFlags | 0x80);
			}
		}
	}
	if (*(int*) (pObject + 0x144) == 0 && *(int*) (pObject + 0x148) == 0) {
		*(int*) (pObject + 0x144) = 1;
		*(int*) (pObject + 0x148) = 1;
	}
}

// MACINTOSH: CIce::StepOn(const AICOORD&, CGameObject*)
// FUNCTION: LEMBALL 0x0042d380
int CIce::StepOn(const AICOORD& position, CGameObject* pObject)
{
	char* pIce = (char*) this;
	void** pVtable;
	int fAlreadyAttached;
	int nX;
	int nY;

	if (*(int*) (pIce + 0x138) == 0) {
		return 0;
	}
	pVtable = (void**) pObject->m_pVtable00;
	if (pObject->m_nEntityType64 == 2) {
		fAlreadyAttached = ((IceObjectPredicate) pVtable[0xe4 / 4])(pObject);
	}
	else {
		fAlreadyAttached = pObject->m_nStateB8 == 0xf && pObject->m_nActionPhaseBC == 3;
	}
	if (fAlreadyAttached != 0) {
		return 0;
	}
	nX = position.x >> 12;
	nY = position.y >> 12;
	if (*(short*) (pIce + 0x17c) - 8 <= nX && nX <= *(short*) (pIce + 0x182) + 7 &&
		*(short*) (pIce + 0x17e) - 8 <= nY && nY <= *(short*) (pIce + 0x184) + 7) {
		int nCount = *(int*) (pIce + 0x150);
		if (nCount < 10) {
			*(CGameObject**) (pIce + 0x154 + nCount * 4) = pObject;
			*(int*) (pIce + 0x150) = nCount + 1;
			pObject->ResetInstructions();
			pObject->m_nFrameTime94 = g_nLevelFrameClockTick * 50;
			pObject->m_nNextUpdateTickCC = g_nLevelFrameClockTick + 1000;
			if (pObject->m_nEntityType64 == 2) {
				((IceObjectOneArgProc) pVtable[0x08 / 4])(pObject, 0, 0x16);
				((IceObjectOneArgProc) pVtable[0x34 / 4])(pObject, 0, 0x2d);
				((IceObjectAttachProc) pVtable[0xe8 / 4])(pObject, 0, 1, this, 0);
				return 1;
			}
			((IceObjectTwoArgProc) pVtable[0x04 / 4])(pObject, 0, 0xf, 3);
			((IceObjectOneArgProc) pVtable[0x34 / 4])(pObject, 0, 0x2d);
		}
		return 1;
	}
	return 0;
}

// MACINTOSH: CIce::Leave(CPlayerLemming*)
// FUNCTION: LEMBALL 0x0042d4d0
void CIce::Leave(CPlayerLemming* pLemming)
{
	char* pIce = (char*) this;
	unsigned short nSlot = ((CGameObject*) pLemming)->GetId();
	int i = 0;

	if (*(int*) (pIce + 0x150) > 0) {
		CGameObject** ppObject = (CGameObject**) (pIce + 0x154);
		while ((*ppObject)->GetId() != nSlot) {
			++ppObject;
			++i;
			if (*(int*) (pIce + 0x150) <= i) {
				return;
			}
		}
		++i;
		while (i < *(int*) (pIce + 0x150)) {
			ppObject[0] = ppObject[1];
			++ppObject;
			++i;
		}
		--*(int*) (pIce + 0x150);
	}
}

// FUNCTION: LEMBALL 0x0042d550
void CIce::Switch(void)
{
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
}

// FUNCTION: LEMBALL 0x0042d560
void CIce::Switched(void)
{
	char* pIce;
	char* pGrid;
	CGameObject* pObject;
	void** pVtable;
	int aPosition[3];
	int aVelocity[3];
	int nTileX;
	int nTileY;
	int nHeight;
	int i;
	int j;

	pIce = (char*) this;
	if (*(int*) (pIce + 0x138) != 0) {
		*(int*) (pIce + 0xc8) = g_nLevelFrameClockTick;
		return;
	}
	i = 0;
	while (i < *(int*) (pIce + 0x150)) {
		pObject = *(CGameObject**) (pIce + 0x154 + i * 4);
		pVtable = (void**) pObject->m_pVtable00;
		aPosition[0] = pObject->m_WorldPosition9C.x;
		aPosition[1] = pObject->m_WorldPosition9C.y;
		aPosition[2] = pObject->m_WorldPosition9C.z;
		*(int*) ((char*) pObject + 0xc0) = 0;
		pObject->m_nStateB8 = 0;
		pObject->m_nNextUpdateTickCC = g_nLevelFrameClockTick;
		pGrid = (char*) g_pLevelTileGrid;
		if (pObject->m_nEntityType64 == 2) {
			((IceObjectOneArgProc) pVtable[0x84 / 4])(pObject, 0, 0xfa0);
			((IceObjectTwoArgProc) pVtable[0xe8 / 4])(pObject, 0, 0, 0);
			nTileX = (aPosition[0] >> 12) >> 4;
			nTileY = (aPosition[1] >> 12) >> 4;
			if ((aPosition[0] >> 12) < 0 || (aPosition[1] >> 12) < 0 || nTileX >= *(int*) (pGrid + 0x10) ||
				nTileY >= *(int*) (pGrid + 0x14)) {
				nHeight = 0;
			}
			else {
				nHeight = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
					*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0x0c,
					0,
					(aPosition[0] >> 12) & 0xf,
					(aPosition[1] >> 12) & 0xf);
			}
			if (nHeight < (aPosition[2] >> 12)) {
				aVelocity[0] = (*(int*) (pIce + 0x144) << 12) / 6;
				aVelocity[1] = (*(int*) (pIce + 0x148) << 12) / 6;
				aVelocity[2] = 0;
				((void(__fastcall*)(void*, int, int*, int)) pVtable[0x20 / 4])(pObject, 0, aVelocity, 0);
			}
		}
		for (j = i + 1; j < *(int*) (pIce + 0x150); ++j) {
			*(void**) (pIce + 0x150 + j * 4) = *(void**) (pIce + 0x154 + j * 4);
		}
		--*(int*) (pIce + 0x150);
		--i;
		nTileX = (aPosition[0] >> 12) >> 4;
		nTileY = (aPosition[1] >> 12) >> 4;
		if ((aPosition[0] >> 12) < 0 || (aPosition[1] >> 12) < 0 || nTileX >= *(int*) (pGrid + 0x10) ||
			nTileY >= *(int*) (pGrid + 0x14)) {
			nHeight = 0;
		}
		else {
			nHeight = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
				*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0x0c,
				0,
				(aPosition[0] >> 12) & 0xf,
				(aPosition[1] >> 12) & 0xf);
		}
		if ((aPosition[2] >> 12) <= nHeight) {
			((void(__fastcall*)(void*, int, int*, void*, unsigned short)) 0x40341d)(*(void**) 0x4a74b0,
																					0,
																					aPosition,
																					pObject,
																					pObject->m_nBehaviourFlags68);
		}
		++i;
	}
}
