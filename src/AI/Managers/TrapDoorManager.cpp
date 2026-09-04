#include "TrapDoorManager.h"

#include "../Objects/TrapDoor.h"

// 68K 0x1062169e __ct__16CTrapDoorManagerFv
// FUNCTION: LEMBALL 0x0040c750
TrapDoorManager::TrapDoorManager() : BaseObjectManager(0x1e, 0x13)
{
	m_count = 0;
	for (int i = 0; i < 8; i++) {
		m_doors[i] = 0;
	}
}

// 68K 0x1062172c Restart__16CTrapDoorManagerFv
// FUNCTION: LEMBALL 0x0040c7b0
void TrapDoorManager::Restart()
{
	for (int i = 0; i < m_count; i++) {
		m_doors[i]->Restart();
	}
}

// 68K 0x10621816 AddNewDoor__16CTrapDoorManagerFUsR7AICOORDUcUl
// STUB: LEMBALL 0x0040c810
void TrapDoorManager::AddNewDoor(unsigned short p_id,
								 const AiCoord& p_position,
								 unsigned char p_mode,
								 unsigned long p_deadline)
{
}

// 68K 0x106218d6 GetViewData__16CTrapDoorManagerFP9CViewData
// STUB: LEMBALL 0x0040c890
int TrapDoorManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10621960 Process__16CTrapDoorManagerFv
// STUB: LEMBALL 0x0040c8f0
void TrapDoorManager::Process()
{
}

// 68K 0x106219d8 LoadLevel__16CTrapDoorManagerFPUciUc
// STUB: LEMBALL 0x0040ca40
void TrapDoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
}

// 68K 0x1062177e __dt__16CTrapDoorManagerFv
TrapDoorManager::~TrapDoorManager()
{
}
