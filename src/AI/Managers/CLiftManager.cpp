#include "CLiftManager.h"

#include "../Base/Coord3d.h"
#include "../Navigation/CAi.h"
#include "../Objects/CLift.h"
#include "AI/Base/CGameObject.h"
#include "AI/Managers/CBaseObjectManager.h"
#include "AI/Objects/SwitchEntry.h"

// GLOBAL: LEMBALL 0x0049e1c0
unsigned short g_wMovingLiftCount = 0;

// FUNCTION: LEMBALL 0x00425680
CLiftManager::CLiftManager(CAi* p_arg0, int p_arg1) : CBaseObjectManager(0x12, 7)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_lifts = 0;
}

// FUNCTION: LEMBALL 0x004256e0
void CLiftManager::Restart()
{
	g_wMovingLiftCount = 0;
	if (m_lifts != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_lifts[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x00425720
void CLiftManager::Initialise(int p_capacity)
{
	m_count = 0;
	if (p_capacity == 0) {
		m_lifts = 0;
		return;
	}
	m_capacity = p_capacity;
	if (m_lifts == 0) {
		m_lifts = new CLift[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_lifts[i].m_manager = this;
			m_lifts[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x004257c0
CLiftManager::~CLiftManager()
{
	delete[] m_lifts;
}

// FUNCTION: LEMBALL 0x00425830
int CLiftManager::ExportEndpoints(Coord3d p_records[][2])
{
	for (int i = 0; i < m_count; i++) {
		CLift* lift = &m_lifts[i];
		(*p_records)[0] = lift->m_start;
		(*p_records)[1] = lift->m_end;
		p_records++;
	}
	return m_count;
}

// FUNCTION: LEMBALL 0x00425c80
void CLiftManager::AddLiftFromXyz(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		m_lifts[m_count].SetId(p_id);
		m_lifts[m_count].Set(p_x, p_y, p_z, 1, -1, 0x30, LIFT_ACTIVATE_CONTINUOUS, 1);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00425ce0
void CLiftManager::AddLiftFromEndpoints(unsigned short p_id, const Coord3d& p_start, const Coord3d& p_end)
{
	if (m_count < m_capacity) {
		m_lifts[m_count].SetId(p_id);
		m_lifts[m_count].Set(p_start, p_end, 1, -1, 0x30, LIFT_ACTIVATE_CONTINUOUS, 1);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00425d30
void CLiftManager::Process()
{
	CLiftManager* self = this;
	for (int i = 0; i < self->m_count; i++) {
		self->m_lifts[i].m_requestEnabled = 1;
		self->m_lifts[i].Process();
		self->m_lifts[i].CheckObjects();
	}
}

// FUNCTION: LEMBALL 0x00425d80
void CLiftManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	for (int i = 0; i < m_count; i++) {
		m_lifts[i].StepOn(p_position, p_object);
	}
}

// FUNCTION: LEMBALL 0x00425df0
int CLiftManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	while (i < m_count) {
		m_lifts[i].CGameObject::GetViewData(*p_viewData++);
		count++;
		i++;
	}
	return count;
}

// FUNCTION: LEMBALL 0x00425f10
void CLiftManager::Switch(swMessage p_message, int p_id, int p_legacyA, int p_legacyB)
{
	int i = 0;
	if (0 < m_count) {
		while (true) {
			if ((unsigned short) m_lifts[i].GetId() == p_id) {
				break;
			}
			i++;
			if (m_count <= i) {
				return;
			}
		}
		if (p_message == 1) {
			CLift* lift = &m_lifts[i];
			if (lift->m_activateType != LIFT_ACTIVATE_SWITCH_ONCE) {
				if (lift->m_activateType == LIFT_ACTIVATE_SWITCH_TOGGLE) {
					lift->ActivateDeactivate();
				}
				return;
			}
			if (lift->m_activationLatched == 0) {
				lift->Activate();
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00425f90
unsigned short CLiftManager::Id(int p_index)
{
	if (p_index >= m_count) {
		return 0xffff;
	}
	return m_lifts[p_index].GetId();
}

// FUNCTION: LEMBALL 0x00425fc0
void CLiftManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	Coord3d start;
	Coord3d end;
	Coord3d position;
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
				id = (unsigned short) CGameObject::NextId();
			}

			if (m_ai->m_levelVersion >= 5) {

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
				m_lifts[m_count].Set(start, end, direction, lowHeight, highHeight, activateType, initialActive);
			}
			else {

				initialActive = *data++;
				activateType = (eLiftActivateType) *data++;
				position.m_x = (short) *data++;
				position.m_y = (short) *data++;
				position.m_z = (short) *data++;
				lowHeight = (short) *data++;
				highHeight = (short) *data++;
				direction = (short) *data++;
				m_lifts[m_count].SetId(id);
				m_lifts[m_count].Set(position.m_x,
									 position.m_y,
									 position.m_z,
									 direction,
									 lowHeight,
									 highHeight,
									 activateType,
									 initialActive);
			}

			m_count++;
			remaining--;
		} while (remaining != 0);
	}
}
