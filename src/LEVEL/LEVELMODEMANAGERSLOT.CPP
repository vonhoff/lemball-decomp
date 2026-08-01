#include "ENGINE/CORE/VSINIT.H"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;

struct ManagedEntityRegistryEntryView {
	unsigned char m_abReserved00[0x6a];
	unsigned short m_nRegistryIndex6A;
};

struct DoorChunkObjectManagerView {
	unsigned short GetObjectSlotId(int nIndex);
};

struct LevelModeChunkManagerView {
	unsigned char m_abReserved00[0x190];
	DoorChunkObjectManagerView* m_pDoorManager190;

	unsigned short GetDoorSlotId(int nIndex);
};

// FUNCTION: LEMBALL 0x00413000
unsigned short LevelModeChunkManagerView::GetDoorSlotId(int nIndex)
{
	return m_pDoorManager190->GetObjectSlotId(nIndex);
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
