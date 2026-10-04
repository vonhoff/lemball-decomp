#include "CMineManager.h"

#include "../Base/CGameObject.h"
#include "../Base/tCoord3d.h"
#include "../Navigation/CAI.h"
#include "../Objects/CMine.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/LevelVersions.h"
#include "AI/Managers/CBaseObjectManager.h"
#include "Visos/Math/FixedPoint.h"

// FUNCTION: LEMBALL 0x00424020
CMineManager::CMineManager(CAI* p_ai, int p_capacity)
	: CBaseObjectManager(NETWORK_OBJECT_MANAGER_MESSAGE_ID_BASE + OBJECT_MANAGER_TRANSPORT_MINES,
						 OBJECT_MANAGER_TRANSPORT_MINES)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_mines = NULL;
	m_positions = NULL;
}

// FUNCTION: LEMBALL 0x00424080
void CMineManager::Restart()
{
	if (m_mines != NULL) {
		for (int i = 0; i < m_capacity; i++) {
			m_mines[i].Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x004240b0
void CMineManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_mines = NULL;
		return;
	}
	if (m_mines == NULL) {
		m_mines = new CMine[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			CMine* mine = &m_mines[i];
			mine->m_managerIndex = i;
			mine->m_manager = this;
			m_mines[i].Restart();
		}
		m_positions = new tCoord3d[m_capacity];
	}
}

// FUNCTION: LEMBALL 0x00424170
CMineManager::~CMineManager()
{
	if (m_mines != NULL) {
		delete[] m_mines;
		delete[] m_positions;
	}
}

// FUNCTION: LEMBALL 0x004241a0
void CMineManager::RemoveMine(CMine* p_mine)
{
	int index = 0;
	if (index < m_count) {
		do {
			if (&m_mines[index] == p_mine) {
				m_mines[index].SetId(INVALID_OBJECT_ID);
				for (int next = index + 1; next < m_count; next++) {
					m_mines[next - 1] = m_mines[next];
					m_positions[next - 1] = m_positions[next];
				}
				m_count--;
				return;
			}
			index++;
		} while (index < m_count);
	}
}

// FUNCTION: LEMBALL 0x00424560
void CMineManager::Triggered(CMine* p_mine)
{
	Trigger(p_mine->m_managerIndex, p_mine->m_triggerDelay);
}

// FUNCTION: LEMBALL 0x00424580
void CMineManager::Trigger(int p_index, int p_delay)
{
	enum {
		CHAIN_TRIGGER_DISTANCE_SQUARED = 0x800,
		CHAIN_TRIGGER_DELAY = 6
	};
	int x = m_positions[p_index].m_x;
	int y = m_positions[p_index].m_y;
	int z = m_positions[p_index].m_z;
	int i = 0;
	if (0 < m_count) {
		int positionOffset = 0;
		do {
			if (p_index != i && m_mines[i].m_action == ACTION_READY) {
				tCoord3d* position = &m_positions[positionOffset];
				int dx = position->m_x - x;
				int dy = position->m_y - y;
				int dz = position->m_z - z;
				if (dz * dz + dy * dy + dx * dx <= CHAIN_TRIGGER_DISTANCE_SQUARED) {
					m_mines[i].Trigger(p_delay + CHAIN_TRIGGER_DELAY);
				}
			}
			positionOffset++;
			i++;
		} while (i < m_count);
	}
}

// FUNCTION: LEMBALL 0x00424630
void CMineManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	enum {

		STEP_ON_HALF_WIDTH = 8,
		STEP_ON_BOUND_SPAN = 15
	};
	int xMin = (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - STEP_ON_HALF_WIDTH;
	int yMin = (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - STEP_ON_HALF_WIDTH;
	int zMin = (p_position.m_zFixed >> FIXED_POINT_FRACTION_BITS) - STEP_ON_HALF_WIDTH;
	int xMax = xMin + STEP_ON_BOUND_SPAN;
	int yMax = yMin + STEP_ON_BOUND_SPAN;
	int i = 0;
	if (m_count <= 0) {
		return;
	}
	do {
		if (m_mines[i].m_enabled != 0 && m_mines[i].m_activated == 0) {
			int pz;
			int px;
			int py = m_positions[i].m_y;
			px = m_positions[i].m_x;
			pz = m_positions[i].m_z;
			if (xMin < px && px < xMax && yMin < py && py < yMax && zMin < pz && pz < yMax) {
				m_mines[i].StepOn(p_object);
				Trigger(i, 0);
				return;
			}
		}
		i++;
	} while (i < m_count);
}

// FUNCTION: LEMBALL 0x00424710
void CMineManager::Add(unsigned short p_id, AICOORD p_position)
{
	if (p_id != INVALID_OBJECT_ID && m_count < m_capacity) {
		m_mines[m_count].SetId(p_id);
		m_mines[m_count].Set(p_position);
		m_positions[m_count].m_x = (short) (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS);
		m_positions[m_count].m_y = (short) (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS);
		m_positions[m_count].m_z = (short) (p_position.m_zFixed >> FIXED_POINT_FRACTION_BITS);
		m_count = m_count + 1;
	}
}

// FUNCTION: LEMBALL 0x004247b0
void CMineManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_mines[i].OnGround();
		m_mines[i].m_requestEnabled = 1;
		if (m_mines[i].m_enabled != 0) {
			m_mines[i].Process();
		}
	}
}

// FUNCTION: LEMBALL 0x00424800
int CMineManager::GetViewData(CViewData* p_viewData)
{
	int count = 0;
	int i = 0;
	if (0 < m_count) {
		CViewData* viewData = p_viewData;
		do {
			m_mines[i].GetViewData(*viewData);
			viewData++;
			count++;
			i++;
		} while (i < m_count);
	}
	return count;
}

// FUNCTION: LEMBALL 0x00424850
void CMineManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count;
	unsigned short* data = (unsigned short*) p_data;
	count = *data++;
	Initialise(count);
	if (count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			unsigned int x = *data++;
			unsigned int y = *data++;
			unsigned int z = *data++;
			AICOORD position(x << FIXED_POINT_FRACTION_BITS,
							 y << FIXED_POINT_FRACTION_BITS,
							 z << FIXED_POINT_FRACTION_BITS);
			Add(id, position);
			remaining--;
		} while (remaining != 0);
	}
}
