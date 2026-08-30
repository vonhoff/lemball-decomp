#include "LiftManager.h"

#include "../Objects/Lift.h"

// GLOBAL: LEMBALL 0x0049e1c0
unsigned short g_wMovingLiftCount = 0;

// 68K 0x1061508e __ct__12CLiftManagerFP3CAIi
// STUB: LEMBALL 0x00425680
LiftManager::LiftManager(Ai* p_arg0, int p_arg1)
{
}

// 68K 0x10615118 Restart__12CLiftManagerFv
// FUNCTION: LEMBALL 0x004256e0
void LiftManager::Restart()
{
	g_wMovingLiftCount = 0;
	if (m_lifts != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_lifts[i].Restart();
		}
	}
}

// 68K 0x10615180 Initialise__12CLiftManagerFi
// STUB: LEMBALL 0x00425720
void LiftManager::Initialise(int p_capacity)
{
}

// 68K 0x106152bc Process__12CLiftManagerFv
// STUB: LEMBALL 0x00425d30
void LiftManager::Process()
{
}

// 68K 0x10615344 StepOn__12CLiftManagerFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x00425d80
void LiftManager::StepOn(const AiCoord& p_position, GameObject* p_object)
{
}

// 68K 0x106153be GetViewData__12CLiftManagerFP9CViewData
// STUB: LEMBALL 0x00425df0
int LiftManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x10615516 Switch__12CLiftManagerF9swMessageiii
// STUB: LEMBALL 0x00425f10
void LiftManager::Switch(swMessage p_message, int p_id, int p_legacyA, int p_legacyB)
{
}

// 68K 0x1061561a Id__12CLiftManagerFi
// STUB: LEMBALL 0x00425f90
unsigned short LiftManager::Id(int p_index)
{
	return 0;
}

// 68K 0x10615668 LoadLevel__12CLiftManagerFPUciUc
// STUB: LEMBALL 0x00425fc0
void LiftManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
}

// 68K 0x1061524c __dt__12CLiftManagerFv
LiftManager::~LiftManager()
{
}
