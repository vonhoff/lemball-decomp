#include "AI/CLift.h"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"

extern int g_nLevelFrameClockTick;
extern int g_nNetworkFrameClockTick;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pLiftTileGrid;

struct ManagedEntityActionStateView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateId(int nStateId);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
};

struct CGround {
	short GetZ(int nLocalX, int nLocalY);
};

// Split from CHUNKOBJVT.CPP to preserve MSVC 4.20 code generation.

// FUNCTION: LEMBALL 0x00425100
int CLift::Process(void)
{
	void* pObject = this;
	ManagedEntityActionStateView* pActionState = (ManagedEntityActionStateView*) pObject;
	int* pFields = (int*) pObject;
	int nTick;
	short nNow;
	short nHeight;
	int nTileHeight;
	int nX;
	int nY;

	if (g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) == 0) {
		nTick = g_nNetworkFrameClockTick;
	}
	else {
		nTick = g_nLevelFrameClockTick;
	}
	nNow = (short) nTick;
	switch (pFields[0x2e]) {
	case 8:
		pFields[0x59] = 0;
		pFields[0x2e] = 0x18;
		break;
	case 0x19:
		pFields[0x59] = 1;
		pFields[0x5b] = 1;
		pActionState->SetActionState(0x2c);
		if (pFields[0x59] != 0 &&
			(g_nSelectedNetworkLobbyPeerId == 0 || *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) != 0)) {
			pFields[0x25] = nTick;
			if (*(short*) ((char*) pObject + 0x154) == 1) {
				pActionState->SetManagedEntityStateId(0x1f);
			}
			else {
				pActionState->SetManagedEntityStateId(0x23);
			}
		}
		break;
	case 0x1f:
		nNow -= (short) pFields[0x25];
		nHeight = *(short*) ((char*) pObject + 0x13e);
		nNow += nHeight;
		pFields[0x54] = nHeight;
		*(short*) ((char*) pObject + 0x154) = 1;
		pFields[0x59] = 1;
		pFields[0x2e] = 0x20;
		*(short*) ((char*) pObject + 0x13e) = nNow;
		break;
	case 0x20:
		nHeight = (short) pFields[0x54] - (short) pFields[0x25] + nNow;
		*(short*) ((char*) pObject + 0x13e) = nHeight;
		if (pFields[0x53] <= nHeight) {
			int fActive;
			*(short*) ((char*) pObject + 0x13e) = (short) pFields[0x53];
			fActive = pFields[0x5a];
			pFields[0x59] = fActive;
			*(short*) ((char*) pObject + 0x154) = -1;
			if (fActive == 0 ||
				(g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) == 0)) {
				pFields[0x2e] = 0x18;
			}
			else {
				pFields[0x25] = nTick;
				pActionState->SetManagedEntityStateId(0x23);
			}
		}
		break;
	case 0x22:
		nHeight = (short) pFields[0x54] - nNow + (short) pFields[0x25];
		*(short*) ((char*) pObject + 0x13e) = nHeight;
		if (nHeight <= pFields[0x52]) {
			int fActive;
			*(short*) ((char*) pObject + 0x13e) = (short) pFields[0x52];
			fActive = pFields[0x5a];
			pFields[0x59] = fActive;
			*(short*) ((char*) pObject + 0x154) = 1;
			if (fActive == 0 ||
				(g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) == 0)) {
				pFields[0x2e] = 0x18;
			}
			else {
				pFields[0x25] = nTick;
				pActionState->SetManagedEntityStateId(0x1f);
			}
		}
		break;
	case 0x23:
		*(short*) ((char*) pObject + 0x154) = -1;
		pFields[0x54] = *(short*) ((char*) pObject + 0x13e);
		pFields[0x2e] = 0x22;
		pFields[0x59] = 1;
		*(short*) ((char*) pObject + 0x13e) += (short) pFields[0x25] - nNow;
		break;
	}

	nTileHeight = *(short*) ((char*) pObject + 0x13e);
	for (nY = *(short*) ((char*) pObject + 0x13c); nY <= *(short*) ((char*) pObject + 0x142); nY += 0x10) {
		char* pTile;
		short nStartX = *(short*) ((char*) pObject + 0x13a);
		pTile = (char*) *(void**) ((char*) g_pLiftTileGrid + 0x0c) +
				((nY / 0x10) * *(int*) ((char*) g_pLiftTileGrid + 0x10) + nStartX / 0x10) * 0x0c;
		pFields[0x58] = (int) pTile;
		for (nX = nStartX; nX <= *(short*) ((char*) pObject + 0x140); nX += 0x10) {
			*(short*) (pFields[0x58] + 8) = (short) nTileHeight;
			*(short*) (pFields[0x58] + 10) = (short) ((nTileHeight + 0x0f) / 0x10);
			pFields[0x58] += 0x0c;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00425440
void CLift::CheckObjects(void)
{
	struct ObjectView {};
	typedef int (ObjectView::*ObjectPredicate)(void);
	typedef int (ObjectView::*ObjectBoundsPredicate)(tCoord3d* pStart, tCoord3d* pEnd);
	char* pObject;
	void** pPassenger;
	void* pChild;
	ObjectPredicate predicate;
	ObjectBoundsPredicate boundsPredicate;
	int i;

	pObject = (char*) this;
	pPassenger = (void**) (pObject + 0x170);
	for (i = 0; i < 8; ++i, ++pPassenger) {
		pChild = *pPassenger;
		if (pChild != 0) {
			*(void**) &predicate = (*(void***) pChild)[0x38];
			*(void**) &boundsPredicate = (*(void***) pChild)[0x28];
			if ((((ObjectView*) pChild)->*predicate)() != 0 ||
				(((ObjectView*) pChild)->*boundsPredicate)((tCoord3d*) (pObject + 0x13a),
														   (tCoord3d*) (pObject + 0x140)) == 0) {
				*(int*) ((char*) pChild + 0x110) = 0xffff;
				*pPassenger = 0;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x004254a0
int CLift::StepOn(const AICOORD& position, CGameObject* pObject)
{
	char* pLift;
	char* pChild;
	char* pGrid;
	CGround* pTile;
	unsigned short nLiftId;
	unsigned short nHeight;
	int nStartX;
	int nStartY;
	int nPositionX;
	int nPositionY;
	int nPositionZ;
	int i;

	pLift = (char*) this;
	pChild = (char*) pObject;
	nLiftId = m_nLiftObjectId138;
	if (nLiftId == *(unsigned int*) (pChild + 0x110)) {
		return 1;
	}
	nStartX = *(short*) (pLift + 0x13a) - 8;
	nStartY = *(short*) (pLift + 0x13c) - 8;
	nPositionX = position.x >> 12;
	nPositionY = position.y >> 12;
	if (nPositionX >= nStartX && nPositionX <= *(short*) (pLift + 0x140) + 7 && nPositionY >= nStartY &&
		nPositionY <= *(short*) (pLift + 0x142) + 7) {
		nPositionZ = position.z >> 12;
		pGrid = (char*) g_pLiftTileGrid;
		if (nStartX < 0 || nStartY < 0 || nStartX >> 4 >= *(int*) (pGrid + 0x10) ||
			nStartY >> 4 >= *(int*) (pGrid + 0x14)) {
			nHeight = 0;
		}
		else {
			pTile =
				(CGround*) (*(char**) (pGrid + 0x0c) + ((nStartY >> 4) * *(int*) (pGrid + 0x10) + (nStartX >> 4)) * 12);
			nHeight = pTile->GetZ(nStartX & 0x0f, nStartY & 0x0f);
		}
		if ((int) nHeight - 2 <= nPositionZ && nPositionZ <= (int) nHeight + 4) {
			for (i = 0; i < 8; ++i) {
				if (*(void**) (pLift + 0x170 + i * 4) == 0) {
					*(void**) (pLift + 0x170 + i * 4) = pObject;
					*(unsigned int*) (pChild + 0x110) = nLiftId;
					if (*(int*) (pLift + 0x15c) == 1) {
						Activate();
						return 1;
					}
					if (*(int*) (pLift + 0x15c) == 4 && *(int*) (pLift + 0x16c) != 1) {
						Activate();
					}
					return 1;
				}
			}
		}
	}
	if (nLiftId == *(unsigned int*) (pChild + 0x110)) {
		*(unsigned int*) (pChild + 0x110) = 0xffff;
	}
	return 0;
}
