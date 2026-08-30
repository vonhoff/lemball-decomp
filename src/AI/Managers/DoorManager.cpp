#include "DoorManager.h"

#include "../Objects/Door.h"

// 68K 0x10606c72 __ct__12CDoorManagerFP3CAIi
// STUB: LEMBALL 0x0040df30
DoorManager::DoorManager(Ai* p_arg0, int p_arg1)
{
}

// 68K 0x10606cfc Restart__12CDoorManagerFv
// FUNCTION: LEMBALL 0x0040df90
void DoorManager::Restart()
{
	if (m_doors != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_doors[i].Restart();
		}
	}
}

// 68K 0x10606d5e Initialise__12CDoorManagerFi
// STUB: LEMBALL 0x0040dfc0
void DoorManager::Initialise(int p_capacity)
{
}

// 68K 0x10606eaa GetViewData__12CDoorManagerFP9CViewData
// STUB: LEMBALL 0x0040e080
int DoorManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10606f2a Add__12CDoorManagerFUs11eObjectTypeUsiii
// STUB: LEMBALL 0x0040e0c0
int DoorManager::Add(unsigned short p_id,
					 eObjectType p_objectType,
					 unsigned short p_doorType,
					 int p_x,
					 int p_y,
					 int p_z)
{
	return 0;
}

// 68K 0x10606fe4 Open__12CDoorManagerFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0040e500
int DoorManager::Open(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x10607066 Process__12CDoorManagerFv
// STUB: LEMBALL 0x0040e550
void DoorManager::Process()
{
}

// 68K 0x10607100 Switch__12CDoorManagerF9swMessagei
// STUB: LEMBALL 0x0040e5a0
void DoorManager::Switch(swMessage p_message, int p_id)
{
}

// 68K 0x10607194 Id__12CDoorManagerFi
// STUB: LEMBALL 0x0040e600
unsigned short DoorManager::Id(int p_index)
{
	return 0;
}

// 68K 0x106071e4 LoadLevel__12CDoorManagerFPUciUc
// STUB: LEMBALL 0x0040e630
void DoorManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
}

// 68K 0x10606e32 __dt__12CDoorManagerFv
DoorManager::~DoorManager()
{
}
