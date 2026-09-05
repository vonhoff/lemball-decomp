#include "LiftManager.h"

#include "../Base/Coord3d.h"
#include "../Navigation/Ai.h"
#include "../Objects/Lift.h"

// GLOBAL: LEMBALL 0x0049e1c0
unsigned short g_wMovingLiftCount = 0;

// 68K 0x1061508e __ct__12CLiftManagerFP3CAIi
// FUNCTION: LEMBALL 0x00425680
LiftManager::LiftManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x12, 7)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_lifts = 0;
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
// FUNCTION: LEMBALL 0x00425720
void LiftManager::Initialise(int p_capacity)
{
	m_count = 0;
	if (p_capacity == 0) {
		m_lifts = 0;
		return;
	}
	m_capacity = p_capacity;
	if (m_lifts == 0) {
		m_lifts = new Lift[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_lifts[i].m_manager = this;
			m_lifts[i].Restart();
		}
	}
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
// FUNCTION: LEMBALL 0x00425fc0
void LiftManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	unsigned short count;

	data = (unsigned short*) p_data;
	count = *data++;
	Initialise(count);
	if (m_ai->m_levelVersion >= 3 && count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short id;
			unsigned int initialActive;
			eLiftActivateType activateType;
			int lowHeight;
			int highHeight;
			short direction;

			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) GameObject::NextId();
			}

			if (m_ai->m_levelVersion >= 5) {
				Coord3d start;
				Coord3d end;

				initialActive = *data++;
				activateType = (eLiftActivateType) *data++;
				start.m_x = (short) *data++;
				start.m_y = (short) *data++;
				start.m_z = (short) *data++;
				end.m_x = (short) *data++;
				end.m_y = (short) *data++;
				end.m_z = (short) *data++;
				lowHeight = (short) *data++;
				highHeight = (short) *data++;
				direction = (short) *data++;
				m_lifts[m_count].SetId(id);
				m_lifts[m_count]
					.Set(start, end, direction, lowHeight, highHeight, activateType, (unsigned char) initialActive);
			}
			else {
				short x;
				short y;
				short z;

				initialActive = *data++;
				activateType = (eLiftActivateType) *data++;
				x = (short) *data++;
				y = (short) *data++;
				z = (short) *data++;
				lowHeight = (short) *data++;
				highHeight = (short) *data++;
				direction = (short) *data++;
				m_lifts[m_count].SetId(id);
				m_lifts[m_count]
					.Set(x, y, z, direction, lowHeight, highHeight, activateType, (unsigned char) initialActive);
			}

			m_count++;
			remaining--;
		} while (remaining != 0);
	}
}

// 68K 0x1061524c __dt__12CLiftManagerFv
LiftManager::~LiftManager()
{
}
