#include "ENGINE/CORE/COMMON.H"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;

struct LevelChunkTypeEnabledFlagsView {
	void Initialize(void);
	void SetEnabled(int nChunkType, int fEnabled);
};

// FUNCTION: LEMBALL 0x00412100
void LevelChunkTypeEnabledFlagsView::Initialize(void)
{
	int nChunkType;
	unsigned int i;
	unsigned int cManagedEntities;

	cManagedEntities = (unsigned int) g_GAME_ManagedEntityRegistryCount;
	for (i = 0; (int) i < (int) cManagedEntities; ++i) {
		if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
			nChunkType = *(int*) (g_GAME_ManagedEntityRegistryTable[i & 0xffff] + 100);
			if (nChunkType != 0xffff) {
				SetEnabled(nChunkType, 1);
			}
		}
	}
	if (*(int*) (*(int*) ((char*) this + 0x180) + 8) == 0) {
		SetEnabled(9, 0);
	}
	if (*(int*) (*(int*) ((char*) this + 0x190) + 0x34) == 0) {
		SetEnabled(0x1a, 0);
		SetEnabled(0x19, 0);
	}
	if (*(int*) (*(int*) ((char*) this + 0x1a0) + 0x34) == 0) {
		SetEnabled(0x2f, 0);
		SetEnabled(0x1e, 0);
	}
	if (*(int*) (*(int*) ((char*) this + 0x188) + 0x3c) == 0) {
		SetEnabled(0x13, 0);
	}
	if (*(int*) (*(int*) ((char*) this + 0x198) + 0x34) == 0) {
		SetEnabled(0x20, 0);
	}
	if (*(int*) (*(int*) ((char*) this + 0x1c8) + 0xc) == 0) {
		SetEnabled(0x35, 0);
	}
}

// FUNCTION: LEMBALL 0x004130a0
void LevelChunkTypeEnabledFlagsView::SetEnabled(int nChunkType, int fEnabled)
{
	if (nChunkType < 0x211 || 0x212 < nChunkType) {
		*(int*) (*(int*) ((char*) this + 0x50) + nChunkType * 4) = fEnabled;
	}
}
