#include "CLiftManager.h"

#include "Gameplay/Geometry/tCoord3d.h"
#include "Gameplay/Simulation/CAI.h"
#include "CLift.h"
#include "LiftEndpointRecord.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CBaseObjectManager.h"
#include "SwitchEntry.h"

enum {
	LIFT_MANAGER_DEFAULT_UPPER_HEIGHT_PIXELS = 48
};

// GLOBAL: LEMBALL 0x0049e1c0
unsigned short g_wMovingLiftCount = 0;

// FUNCTION: LEMBALL 0x00425680
CLiftManager::CLiftManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_LIFTS,
						 OBJECT_MANAGER_TRANSPORT_LIFTS)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_lifts = NULL;
}

// FUNCTION: LEMBALL 0x004256e0
void CLiftManager::Restart()
{
	g_wMovingLiftCount = 0;
	if (m_lifts != NULL) {
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
		m_lifts = NULL;
		return;
	}
	m_capacity = p_capacity;
	if (m_lifts == NULL) {
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

// FUNCTION: LEMBALL 0x004257e0
int CLiftManager::ExportLiftStartCoordinates(tCoord3d* p_records)
{
	for (int index = 0; index < m_count; ++index) {
		p_records[index] = m_lifts[index].m_start;
	}
	return m_count;
}

// FUNCTION: LEMBALL 0x00425830
int CLiftManager::ExportEndpoints(LiftEndpointRecord* p_records)
{
	for (int i = 0; i < m_count; i++) {
		CLift* lift = &m_lifts[i];
		p_records->m_start = lift->m_start;
		p_records->m_end = lift->m_end;
		p_records++;
	}
	return m_count;
}

// FUNCTION: LEMBALL 0x00425890
void CLiftManager::RemoveLift(CLift* p_lift)
{
	int i = 0;
	int count = m_count;
	if (i < count) {
		CLift* lift = m_lifts;
		do {
			if (lift == p_lift) {
				m_lifts[i].SetId(INVALID_OBJECT_ID);
				for (int next = i + 1; next < m_count; next++) {
					m_lifts[next - 1] = m_lifts[next];
				}
				m_count--;
				return;
			}
			lift++;
			i++;
		} while (i < count);
	}
}

// FUNCTION: LEMBALL 0x00425c80
void CLiftManager::AddLiftFromXyz(unsigned short p_id, int p_x, int p_y, int p_z)
{
	if (m_count < m_capacity) {
		m_lifts[m_count].SetId(p_id);
		m_lifts[m_count]
			.Set(p_x, p_y, p_z, 1, -1, LIFT_MANAGER_DEFAULT_UPPER_HEIGHT_PIXELS, LIFT_ACTIVATE_CONTINUOUS, 1);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00425ce0
void CLiftManager::AddLiftFromEndpoints(unsigned short p_id, tCoord3d& p_start, tCoord3d& p_end)
{
	if (m_count < m_capacity) {
		m_lifts[m_count].SetId(p_id);
		m_lifts[m_count]
			.Set(p_start, p_end, 1, -1, LIFT_MANAGER_DEFAULT_UPPER_HEIGHT_PIXELS, LIFT_ACTIVATE_CONTINUOUS, 1);
		m_count++;
	}
}

// FUNCTION: LEMBALL 0x00425dc0
void CLiftManager::CalculateAllLiftCliffs()
{
	for (int index = 0; index < m_count; ++index) {
		m_lifts[index].CalculateCliff();
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
		if (p_message == SW_LIFT) {
			CLift* lift = &m_lifts[i];
			if (lift->m_activateType != LIFT_ACTIVATE_SWITCH_ONCE) {
				if (lift->m_activateType == LIFT_ACTIVATE_SWITCH_TOGGLE) {
					lift->ActivateDeactivate();
				}
				return;
			}
			if (lift->m_activationLatched == LIFT_ACTIVATION_NOT_LATCHED) {
				lift->Activate();
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00425f90
unsigned short CLiftManager::Id(int p_index)
{
	if (p_index >= m_count) {
		return INVALID_OBJECT_ID;
	}
	return m_lifts[p_index].GetId();
}
