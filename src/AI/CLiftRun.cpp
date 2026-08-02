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

// Split from CHUNKOBJVT.CPP to preserve MSVC 4.20 code generation.

// MACINTOSH: CLift::Process()
// FUNCTION: LEMBALL 0x00425100
int LEMBALL_FASTCALL AdvanceLiftChunkObjectStateMachine(void* pObject)
{
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
