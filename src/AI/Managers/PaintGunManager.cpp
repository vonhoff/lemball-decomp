#include "PaintGunManager.h"

#include "../Navigation/Ai.h"
#include "../Objects/PaintGun.h"

// 68K 0x1061b8a8 __ct__16CPaintGunManagerFP3CAIi
// FUNCTION: LEMBALL 0x0042bfe0
PaintGunManager::PaintGunManager(Ai* p_arg0, int p_arg1) : BaseObjectManager(0x1f, 0x14)
{
	m_ai = p_arg0;
	m_capacity = p_arg1;
	m_paintGuns = 0;
}

// 68K 0x1061b936 Restart__16CPaintGunManagerFv
// FUNCTION: LEMBALL 0x0042c040
void PaintGunManager::Restart()
{
	if (m_paintGuns != 0) {
		for (int i = 0; i < m_capacity; i++) {
			m_paintGuns[i].Restart();
		}
	}
}

// 68K 0x1061b99c Initialise__16CPaintGunManagerFi
// FUNCTION: LEMBALL 0x0042c070
void PaintGunManager::Initialise(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	if (p_capacity == 0) {
		m_paintGuns = 0;
		return;
	}
	if (m_paintGuns == 0) {
		m_paintGuns = new PaintGun[p_capacity];
		for (int i = 0; i < m_capacity; i++) {
			m_paintGuns[i].Restart();
			m_paintGuns[i].m_manager = this;
		}
	}
}

// 68K 0x1061bae6 Process__16CPaintGunManagerFv
// STUB: LEMBALL 0x0042c4d0
void PaintGunManager::Process()
{
}

// 68K 0x1061bb72 GetViewData__16CPaintGunManagerFP9CViewData
// STUB: LEMBALL 0x0042c520
int PaintGunManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1061bc0a Add__16CPaintGunManagerFUsiiii
// STUB: LEMBALL 0x0042c590
void PaintGunManager::Add(unsigned short p_id, int p_x, int p_y, int p_z, int p_direction)
{
}

// 68K 0x1061bcce LoadLevel__16CPaintGunManagerFPUciUc
// FUNCTION: LEMBALL 0x0042c610
void PaintGunManager::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;
	unsigned int remaining = count;
	Initialise(remaining);
	if (count != 0) {
		unsigned short id;
		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short direction;
		do {
			if (m_ai->m_levelVersion > 1) {
				id = *(unsigned short*) p_data;
				p_data += 2;
			}
			else {
				id = (unsigned short) GameObject::NextId();
			}
			x = *(unsigned short*) p_data;
			p_data += 2;
			y = *(unsigned short*) p_data;
			p_data += 2;
			z = *(unsigned short*) p_data;
			p_data += 2;
			direction = *(unsigned short*) p_data;
			p_data += 2;
			Add(id, x, y, z, direction);
			remaining--;
		} while (remaining != 0);
	}
}

// 68K 0x1061ba6c __dt__16CPaintGunManagerFv
PaintGunManager::~PaintGunManager()
{
}
