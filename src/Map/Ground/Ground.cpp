#include "Ground.h"

// 68K 0x1090000c GetZ__7CGroundFii
// FUNCTION: LEMBALL 0x0042ffe0
unsigned short Ground::GetZ(int p_x, int p_y)
{
	switch (m_objectType) {
	case 0x202:
		return m_height;
	case 0x206:
		return m_height + 0x10;
	case 0x207:
		return m_height + 8;
	case 0x208:
		return (m_height - (short) p_y) + 0x10;
	case 0x209:
		return m_height;
	case 0x20a:
		return m_height;
	case 0x20b:
		return m_height + (short) p_x;
	case 0x20c:
		return m_height + (short) p_y;
	case 0x20d:
		return (m_height - (short) p_x) + 0x10;
	case 0x20e:
		return (m_height - (short) (p_y / 2)) + 7;
	case 0x20f:
		return (m_height - (short) (p_x / 2)) + 7;
	case 0x210:
		return m_height;
	case 0x214:
		return m_height;
	case 0x215:
	case 0x216:
	case 0x217:
	case 0x219:
	case 0x21a:
		return m_height;
	default:
		return 0;
	}
}

// 68K 0x10900148 IsHit__7CGroundFiiUc
// STUB: LEMBALL 0x00430110
bool Ground::IsHit(int p_x, int p_y, unsigned char p_includeSpecial)
{
	return 0;
}

// 68K 0x10900356 SetCollision__7CGroundFv
// FUNCTION: LEMBALL 0x004302d0
void Ground::SetCollision()
{
	switch (m_objectType) {
	case 0x200:
	case 0x201:
	case 0x203:
		m_collision = 0;
		return;
	case 0x202:
	case 0x204:
	case 0x210:
		m_collision = 3;
		return;
	case 0x205:
		m_collision = 2;
		return;
	case 0x206:
	case 0x207:
	case 0x208:
	case 0x209:
	case 0x20b:
	case 0x20c:
	case 0x20d:
	case 0x20e:
	case 0x20f:
	case 0x214:
		m_collision = 0;
		return;
	case 0x20a:
		m_collision = 1;
		return;
	case 0x215:
	case 0x216:
	case 0x217:
		m_collision = 4;
		return;
	case 0x219:
	case 0x21a:
		m_collision = 0;
		return;
	}
}

// 68K 0x1011c68c __ct__7CGroundFv
// FUNCTION: LEMBALL 0x004314f0
Ground::Ground()
{
	m_objectType = (eObjectType) 0x209;
	m_objectData = 0;
	m_collision = 0;
	m_height = 0;
	m_cliff = 0;
}
