#include "CMineManager.h"

#include "../Base/CGameObject.h"
#include "../Base/Coord3d.h"
#include "../Navigation/CAI.h"
#include "../Objects/CMine.h"
#include "AI/Base/AiCoord.h"
#include "AI/Managers/CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x00424020
CMineManager::CMineManager(CAI* p_ai, int p_capacity) : CBaseObjectManager(0xd, 2)
{
	m_ai = p_ai;
	m_capacity = p_capacity;
	m_mines = 0;
	m_positions = 0;
}

// FUNCTION: LEMBALL 0x00424080
void CMineManager::Restart()
{
	if (m_mines != 0) {
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
		m_mines = 0;
		return;
	}
	if (m_mines == 0) {
		m_mines = new CMine[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			CMine* mine = &m_mines[i];
			mine->m_managerIndex = i;
			mine->m_manager = this;
			m_mines[i].Restart();
		}
		m_positions = new Coord3d[m_capacity];
	}
}

// FUNCTION: LEMBALL 0x00424170
CMineManager::~CMineManager()
{
	if (m_mines != 0) {
		delete[] m_mines;
		delete[] m_positions;
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
	int x = m_positions[p_index].m_x;
	int y = m_positions[p_index].m_y;
	int z = m_positions[p_index].m_z;
	int i = 0;
	if (0 < m_count) {
		int positionOffset = 0;
		do {
			if (p_index != i && m_mines[i].m_action == 0x18) {
				Coord3d* position = &m_positions[positionOffset];
				int dx = position->m_x - x;
				int dy = position->m_y - y;
				int dz = position->m_z - z;
				if (dz * dz + dy * dy + dx * dx <= 0x800) {
					m_mines[i].Trigger(p_delay + 6);
				}
			}
			positionOffset++;
			i++;
		} while (i < m_count);
	}
}

// FUNCTION: LEMBALL 0x00424630
void CMineManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	int x = p_position.m_xFixed >> 0xc;
	int y = p_position.m_yFixed >> 0xc;
	int xMin = x - 8;
	int yMin = y - 8;
	int zMin = (p_position.m_zFixed >> 0xc) - 8;
	int xMax = xMin + 15;
	int yMax = yMin + 15;
	int i = 0;
	if (m_count <= 0) {
		return;
	}
	do {
		if (m_mines[i].m_enabled != 0 && m_mines[i].m_activated == 0) {
			int py = m_positions[i].m_y;
			int px = m_positions[i].m_x;
			int pz = m_positions[i].m_z;
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
void CMineManager::Add(unsigned short p_id, AiCoord p_position)
{
	if (p_id != 0xffff && m_count < m_capacity) {
		m_mines[m_count].SetId(p_id);
		m_mines[m_count].Set(p_position);
		m_positions[m_count].m_x = (short) (p_position.m_xFixed >> 0xc);
		m_positions[m_count].m_y = (short) (p_position.m_yFixed >> 0xc);
		m_positions[m_count].m_z = (short) (p_position.m_zFixed >> 0xc);
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
	int i = 0;
	int count = 0;
	if (0 < m_count) {
		do {
			m_mines[i].GetViewData(*p_viewData);
			p_viewData++;
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
			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) CGameObject::NextId();
			}
			unsigned int x = *data++;
			unsigned int y = *data++;
			unsigned int z = *data++;
			AiCoord position(x << 0xc, y << 0xc, z << 0xc);
			Add(id, position);
			remaining--;
		} while (remaining != 0);
	}
}
