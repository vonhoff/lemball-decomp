#include "ENGINE/CORE/VSINIT.H"

extern int g_nLevelFrameClockTick;
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct MineChunkObjectRuntimeView {
	unsigned char m_abReserved00[0xb8];
	int m_nStateB8;
	unsigned char m_abReservedBC[0x10];
	int m_nFrameTickCC;
	unsigned char m_abReservedD0[0x68];
	int m_anRuntimeState138[4];
};

// GLOBAL: LEMBALL 0x004a7840
short g_LEVEL_MineTileVariantCodes[4];

void LEMBALL_FASTCALL ResetMineChunkObjectRuntimeState(MineChunkObjectRuntimeView* pObject);

// FUNCTION: LEMBALL 0x00423c30
void LEMBALL_FASTCALL ResetMineChunkObject(MineChunkObjectRuntimeView* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetMineChunkObjectRuntimeState(pObject);
}

// FUNCTION: LEMBALL 0x00423c50
void LEMBALL_FASTCALL ResetMineChunkObjectRuntimeState(MineChunkObjectRuntimeView* pObject)
{
	pObject->m_nStateB8 = 0x18;
	pObject->m_anRuntimeState138[0] = 0;
	pObject->m_anRuntimeState138[1] = 0;
	pObject->m_anRuntimeState138[2] = 0;
	pObject->m_anRuntimeState138[3] = 0;
	pObject->m_nFrameTickCC = g_nLevelFrameClockTick;
	g_LEVEL_MineTileVariantCodes[0] = 10;
	g_LEVEL_MineTileVariantCodes[1] = 0x11;
	g_LEVEL_MineTileVariantCodes[2] = 0x30;
	g_LEVEL_MineTileVariantCodes[3] = 8;
}
