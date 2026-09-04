#include "IceManager.h"

#include "../Objects/Ice.h"

// 68K 0x10612448 __ct__11CIceManagerFP3CAIi
// FUNCTION: LEMBALL 0x0042d7a0
IceManager::IceManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x19, 0xe)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_ice = 0;
}

// 68K 0x106124d2 Restart__11CIceManagerFv
// FUNCTION: LEMBALL 0x0042d800
void IceManager::Restart()
{
	if (m_ice != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_ice[i].Restart();
		}
	}
}

// 68K 0x10612534 Initialise__11CIceManagerFi
// FUNCTION: LEMBALL 0x0042d830
void IceManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_ice = 0;
		return;
	}
	if (m_ice == 0) {
		m_ice = new Ice[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_ice[i].Restart();
			m_ice[i].m_manager = this;
		}
	}
}

// 68K 0x10612672 Process__11CIceManagerFv
// STUB: LEMBALL 0x0042dd00
void IceManager::Process()
{
}

// 68K 0x106126ce GetViewData__11CIceManagerFP9CViewData
// FUNCTION: LEMBALL 0x0042dd30
int IceManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10612708 StepOn__11CIceManagerFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0042dd40
bool IceManager::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x1061278a Switch__11CIceManagerF9swMessagei
// STUB: LEMBALL 0x0042dd90
void IceManager::Switch(int p_message, int p_id)
{
}

// 68K 0x1061281e Add__11CIceManagerFUsRC8tCoord3dRC8tCoord3diiUc
// STUB: LEMBALL 0x0042ddf0
void IceManager::Add(unsigned short p_id,
					 const Coord3d& p_cornerA,
					 const Coord3d& p_cornerB,
					 int p_velocityX,
					 int p_velocityY,
					 unsigned char p_initialSwitched)
{
}

// 68K 0x106128a4 LoadLevel__11CIceManagerFPUciUc
// STUB: LEMBALL 0x0042dea0
void IceManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
}

// 68K 0x106125fe __dt__11CIceManagerFv
IceManager::~IceManager()
{
}
