#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/LevelModeTripletArrayView.h"
#include "AI/CDoorManager.h"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;

struct ManagedEntityRegistryEntryView {
	unsigned char m_abReserved00[0x6a];
	unsigned short m_nRegistryIndex6A;
};

struct LevelModeChunkEntryArrayView {
	void CopyTripletToFixedPointThunk(int* pOutput, int nIndex);
};

struct LevelModePositionOwnerView {
	unsigned char m_abReserved00[0x15c];
	LevelModeChunkEntryArrayView* m_pEntries15C;

	void CopyProfilePositionFixed(int* pOutput, int nIndex);
};

;

struct LevelModeChunkManagerView {
	unsigned char m_abReserved00[0x190];
	CDoorManager* m_pDoorManager190;

	unsigned short GetDoorSlotId(int nIndex);
};

// MACINTOSH: CAI::DoorId(int)
// FUNCTION: LEMBALL 0x00413000
unsigned short LevelModeChunkManagerView::GetDoorSlotId(int nIndex)
{
	return m_pDoorManager190->Id(nIndex);
}

// FUNCTION: LEMBALL 0x00413050
void SynchronizeManagedEntityRegistrySlotIds(void)
{
	int i;
	int cManagedEntities;
	ManagedEntityRegistryEntryView* pEntity;

	cManagedEntities = (unsigned int) g_GAME_ManagedEntityRegistryCount;
	for (i = 0; i < cManagedEntities; ++i) {
		pEntity =
			(ManagedEntityRegistryEntryView*) (unsigned long) g_GAME_ManagedEntityRegistryTable[(unsigned short) i];
		if (pEntity != 0 && pEntity->m_nRegistryIndex6A != i) {
			pEntity->m_nRegistryIndex6A = (unsigned short) i;
		}
	}
}

// FUNCTION: LEMBALL 0x00413100
void LevelModePositionOwnerView::CopyProfilePositionFixed(int* pOutput, int nIndex)
{
	m_pEntries15C->CopyTripletToFixedPointThunk(pOutput, nIndex);
}

// MACINTOSH: CAI::AddANetworkStart(int, int, int, int)
// FUNCTION: LEMBALL 0x00413180
void LevelModeTripletArrayView::SetTripletEntry(int nX, int nY, int nZ, int nIndex)
{
	m_anX90[nIndex] = nX;
	m_anYA0[nIndex] = nY;
	m_anZB0[nIndex] = nZ;
}
