#include "MineManager.h"

#include "../Base/Coord3d.h"
#include "../Base/GameObject.h"
#include "../Navigation/Ai.h"
#include "../Objects/Mine.h"
#include "../Objects/ViewData.h"

// 68K 0x10616764 __ct__12CMineManagerFP3CAIi
// FUNCTION: LEMBALL 0x00424020
MineManager::MineManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0xd, 2)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_mines = 0;
	m_positions = 0;
}

// 68K 0x106167f2 Restart__12CMineManagerFv
// FUNCTION: LEMBALL 0x00424080
void MineManager::Restart()
{
	if (m_mines != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_mines[i].Restart();
		}
	}
}

// 68K 0x10616854 Initialise__12CMineManagerFi
// FUNCTION: LEMBALL 0x004240b0
void MineManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_mines = 0;
		return;
	}
	if (m_mines == 0) {
		m_mines = new Mine[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_mines[i].m_managerIndex = i;
			m_mines[i].m_manager = this;
			m_mines[i].Restart();
		}
		m_positions = new Coord3d[m_capacity];
	}
}

// 68K 0x1061693e __dt__12CMineManagerFv
// FUNCTION: LEMBALL 0x00424170
MineManager::~MineManager()
{
	if (m_mines != 0) {
		delete[] m_mines;
		delete[] m_positions;
	}
}

// 68K 0x106169c2 Triggered__12CMineManagerFP5CMine
// FUNCTION: LEMBALL 0x00424560
void MineManager::Triggered(Mine* p_mine)
{
	Trigger(p_mine->m_managerIndex, p_mine->m_triggerDelay);
}

// 68K 0x10616a0a Trigger__12CMineManagerFii
// FUNCTION: LEMBALL 0x00424580
void MineManager::Trigger(int p_index, int p_delay)
{
	int x = m_positions[p_index].m_x;
	int y = m_positions[p_index].m_y;
	int z = m_positions[p_index].m_z;
	if (0 < m_count) {
		for (int i = 0; i < m_count; i++) {
			if (p_index != i && m_mines[i].m_action == 0x18) {
				int dx = m_positions[i].m_x - x;
				int dy = m_positions[i].m_y - y;
				int dz = m_positions[i].m_z - z;
				if (dz * dz + dy * dy + dx * dx < 0x801) {
					m_mines[i].Trigger(p_delay + 6);
				}
			}
		}
	}
}

// 68K 0x10616b24 StepOn__12CMineManagerFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x00424630
void MineManager::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	int x = p_position.m_xFixed >> 0xc;
	int y = p_position.m_yFixed >> 0xc;
	int yMax = y + 7;
	if (m_count < 1) {
		return;
	}
	for (int i = 0; i < m_count; i++) {
		if (m_mines[i].m_enabled != 0 && m_mines[i].m_activated == 0) {
			int py = m_positions[i].m_y;
			int px = m_positions[i].m_x;
			int pz = m_positions[i].m_z;
			if (x - 8 < px && px < x + 7 && y - 8 < py && py < yMax && (p_position.m_zFixed >> 0xc) - 8 < pz &&
				pz < yMax) {
				m_mines[i].StepOn(p_object);
				Trigger(i, 0);
				return;
			}
		}
	}
}

// 68K 0x10616c5e Add__12CMineManagerFUs7AICOORD
// FUNCTION: LEMBALL 0x00424710
void MineManager::Add(unsigned short p_id, AiCoord p_position)
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

// 68K 0x10616d46 Process__12CMineManagerFv
// FUNCTION: LEMBALL 0x004247b0
void MineManager::Process()
{
	if (0 < m_count) {
		for (int i = 0; i < m_count; i++) {
			m_mines[i].OnGround();
			m_mines[i].m_requestEnabled = 1;
			if (m_mines[i].m_enabled != 0) {
				m_mines[i].Process();
			}
		}
	}
}

// 68K 0x10616de6 GetViewData__12CMineManagerFP9CViewData
// FUNCTION: LEMBALL 0x00424800
int MineManager::GetViewData(ViewData* p_viewData)
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

// 68K 0x10616e68 LoadLevel__12CMineManagerFPUciUc
// FUNCTION: LEMBALL 0x00424850
void MineManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data++;
	Initialise(count);
	if (count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short id;
			if (m_ai->m_levelVersion > 1) {
				id = *data++;
			}
			else {
				id = (unsigned short) GameObject::NextId();
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
