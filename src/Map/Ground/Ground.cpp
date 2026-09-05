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
// FUNCTION: LEMBALL 0x00430110
bool Ground::IsHit(int p_x, int p_y, unsigned char p_includeSpecial)
{
	// Each table is 32 rows of 32 horizontal collision bits. The original tables are at 0x49e1e0,
	// 0x49e260, 0x49e2e0, 0x49e360, and 0x49e3e0; xMasks is the one-bit selector at 0x49e460.
	static const unsigned int xMasks[32] = {
		0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
		0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
		0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
		0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001};
	static const unsigned int commonMasks[32] = {
		0,          0,          0,          0,          0,          0,          0,          0,
		0,          0,          0,          0,          0,          0,          0,          0,
		0x0003c000, 0x000ff000, 0x003ffc00, 0x00ffff00, 0x03ffffc0, 0x0ffffff0, 0x3ffffffc, 0xffffffff,
		0xffffffff, 0x3ffffffc, 0x0ffffff0, 0x03ffffc0, 0x00ffff00, 0x003ffc00, 0x000ff000, 0x0003c000};
	static const unsigned int masks0206[32] = {
		0x0003c000, 0x000ff000, 0x003ffc00, 0x00ffff00, 0x03fffff0, 0x0ffffff0, 0x3fffffff, 0xffffffff,
		0xffffffff, 0x3ffffffc, 0x0ffffff0, 0x03ffffc0, 0x00ffff00, 0x003ffc00, 0x000ff000, 0x0003c000,
		0,          0,          0,          0,          0,          0,          0,          0,
		0,          0,          0,          0,          0,          0,          0,          0};
	static const unsigned int masks0207[32] = {
		0,          0,          0,          0,          0,          0,          0,          0,
		0x0003c000, 0x000ff000, 0x003ffc00, 0x00ffff00, 0x03ffffc0, 0x0ffffff0, 0x3ffffffc, 0xffffffff,
		0xffffffff, 0x3ffffffc, 0x0ffffff0, 0x03ffffc0, 0x00ffff00, 0x003ffc00, 0x000ff000, 0x0003c000,
		0,          0,          0,          0,          0,          0,          0,          0};
	static const unsigned int masks0208[32] = {
		0x0000e000, 0x0001f800, 0x0001fe00, 0x0003ff80, 0x0007ffe0, 0x0007fff8, 0x000ffffe, 0x001ffffe,
		0x001fffff, 0x003fffff, 0x007ffffc, 0x007ffffc, 0x00fffff8, 0x01fffff0, 0x01fffff0, 0x03ffffe0,
		0x07ffffc0, 0x07ffffc0, 0x0fffff80, 0x1fffff00, 0x1fffff00, 0x3ffffe00, 0x7ffffc00, 0x7ffffc00,
		0xfffff800, 0x3ffff000, 0x0ffff000, 0x03fff000, 0x00ffe000, 0x003fc000, 0x000fc000, 0x0003c000};
	static const unsigned int masks020e[32] = {
		0,          0,          0,          0,          0,          0,          0,          0,
		0,          0,          0x00018000, 0x0003f000, 0x0007fc00, 0x001fff00, 0x003fffc0, 0x007fffff,
		0x01ffffff, 0x03ffffff, 0x07fffffe, 0x0ffffffc, 0x1ffffff8, 0x3ffffff0, 0x7fffffe0, 0xffffffc0,
		0xffffff80, 0x3fffff00, 0x0ffffe00, 0x03fff880, 0x00fff000, 0x003fe000, 0x000fc000, 0x0003c000};
	switch (m_objectType) {
	case 0x202:
		return xMasks[p_x] & commonMasks[p_y];
	case 0x206:
		return xMasks[p_x] & masks0206[p_y];
	case 0x207:
		return xMasks[p_x] & masks0207[p_y];
	case 0x208:
		return xMasks[p_x] & masks0208[p_y];
	case 0x209:
		return xMasks[p_x] & commonMasks[p_y];
	case 0x20a:
		break;
	case 0x20b:
	case 0x20c:
		return false;
	case 0x20d:
		return xMasks[31 - p_x] & masks0208[p_y];
	case 0x20e:
		return xMasks[p_x] & masks020e[p_y];
	case 0x20f:
		return xMasks[31 - p_x] & masks020e[p_y];
	case 0x210:
		return p_includeSpecial != 0;
	case 0x214:
	case 0x215:
	case 0x216:
	case 0x217:
	case 0x219:
	case 0x21a:
		return xMasks[p_x] & commonMasks[p_y];
	default:
		return false;
	}
	if (p_includeSpecial != 0) {
		return xMasks[p_x] & commonMasks[p_y];
	}
	return false;
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
