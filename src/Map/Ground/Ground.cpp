#include "Ground.h"

#include "../../AI/Base/ObjectTypes.h"

// 68K 0x1090000c GetZ__7CGroundFii
// FUNCTION: LEMBALL 0x0042ffe0
unsigned short Ground::GetZ(int p_x, int p_y)
{
	switch (m_objectType) {
	case TERRAIN_TREE:
		return m_height;
	case TERRAIN_BLOX_1:
		return m_height + 0x10;
	case TERRAIN_BLOX_2:
		return m_height + 8;
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		return (m_height - (short) p_y) + 0x10;
	case TERRAIN_BLOX_4:
		return m_height;
	case TERRAIN_BLOX_5:
		return m_height;
	case TERRAIN_BLOX_6:
		return m_height + (short) p_x;
	case TERRAIN_BLOX_7:
		return m_height + (short) p_y;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		return (m_height - (short) p_x) + 0x10;
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		return (m_height - (short) (p_y / 2)) + 7;
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		return (m_height - (short) (p_x / 2)) + 7;
	case TERRAIN_ANIM:
		return m_height;
	case 0x214:
		return m_height;
	case TERRAIN_FLAME:
	case TERRAIN_ELECTRIC:
	case TERRAIN_EMBERS:
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
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
	// 0x49e260, 0x49e2e0, 0x49e360, and 0x49e3e0; xBitMasks is the one-bit selector at 0x49e460.
	static const unsigned int xBitMasks[32] = {
		0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
		0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
		0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
		0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001};
	static const unsigned int standardGroundMasks[32] = {
		0,          0,          0,          0,          0,          0,          0,          0,
		0,          0,          0,          0,          0,          0,          0,          0,
		0x0003c000, 0x000ff000, 0x003ffc00, 0x00ffff00, 0x03ffffc0, 0x0ffffff0, 0x3ffffffc, 0xffffffff,
		0xffffffff, 0x3ffffffc, 0x0ffffff0, 0x03ffffc0, 0x00ffff00, 0x003ffc00, 0x000ff000, 0x0003c000};
	static const unsigned int blox1CollisionMasks[32] = {
		0x0003c000, 0x000ff000, 0x003ffc00, 0x00ffff00, 0x03fffff0, 0x0ffffff0, 0x3fffffff, 0xffffffff,
		0xffffffff, 0x3ffffffc, 0x0ffffff0, 0x03ffffc0, 0x00ffff00, 0x003ffc00, 0x000ff000, 0x0003c000,
		0,          0,          0,          0,          0,          0,          0,          0,
		0,          0,          0,          0,          0,          0,          0,          0};
	static const unsigned int blox2CollisionMasks[32] = {
		0,          0,          0,          0,          0,          0,          0,          0,
		0x0003c000, 0x000ff000, 0x003ffc00, 0x00ffff00, 0x03ffffc0, 0x0ffffff0, 0x3ffffffc, 0xffffffff,
		0xffffffff, 0x3ffffffc, 0x0ffffff0, 0x03ffffc0, 0x00ffff00, 0x003ffc00, 0x000ff000, 0x0003c000,
		0,          0,          0,          0,          0,          0,          0,          0};
	static const unsigned int steepSlopeCollisionMasks[32] = {
		0x0000e000, 0x0001f800, 0x0001fe00, 0x0003ff80, 0x0007ffe0, 0x0007fff8, 0x000ffffe, 0x001ffffe,
		0x001fffff, 0x003fffff, 0x007ffffc, 0x007ffffc, 0x00fffff8, 0x01fffff0, 0x01fffff0, 0x03ffffe0,
		0x07ffffc0, 0x07ffffc0, 0x0fffff80, 0x1fffff00, 0x1fffff00, 0x3ffffe00, 0x7ffffc00, 0x7ffffc00,
		0xfffff800, 0x3ffff000, 0x0ffff000, 0x03fff000, 0x00ffe000, 0x003fc000, 0x000fc000, 0x0003c000};
	static const unsigned int shallowSlopeCollisionMasks[32] = {
		0,          0,          0,          0,          0,          0,          0,          0,
		0,          0,          0x00018000, 0x0003f000, 0x0007fc00, 0x001fff00, 0x003fffc0, 0x007fffff,
		0x01ffffff, 0x03ffffff, 0x07fffffe, 0x0ffffffc, 0x1ffffff8, 0x3ffffff0, 0x7fffffe0, 0xffffffc0,
		0xffffff80, 0x3fffff00, 0x0ffffe00, 0x03fff880, 0x00fff000, 0x003fe000, 0x000fc000, 0x0003c000};
	switch (m_objectType) {
	case TERRAIN_TREE:
		return xBitMasks[p_x] & standardGroundMasks[p_y];
	case TERRAIN_BLOX_1:
		return xBitMasks[p_x] & blox1CollisionMasks[p_y];
	case TERRAIN_BLOX_2:
		return xBitMasks[p_x] & blox2CollisionMasks[p_y];
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
		return xBitMasks[p_x] & steepSlopeCollisionMasks[p_y];
	case TERRAIN_BLOX_4:
		return xBitMasks[p_x] & standardGroundMasks[p_y];
	case TERRAIN_BLOX_5:
		break;
	case TERRAIN_BLOX_6:
	case TERRAIN_BLOX_7:
		return false;
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
		return xBitMasks[31 - p_x] & steepSlopeCollisionMasks[p_y];
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
		return xBitMasks[p_x] & shallowSlopeCollisionMasks[p_y];
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
		return xBitMasks[31 - p_x] & shallowSlopeCollisionMasks[p_y];
	case TERRAIN_ANIM:
		return p_includeSpecial != 0;
	case 0x214:
	case TERRAIN_FLAME:
	case TERRAIN_ELECTRIC:
	case TERRAIN_EMBERS:
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
		return xBitMasks[p_x] & standardGroundMasks[p_y];
	default:
		return false;
	}
	if (p_includeSpecial != 0) {
		return xBitMasks[p_x] & standardGroundMasks[p_y];
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
	case TERRAIN_TREE:
	case 0x204:
	case TERRAIN_ANIM:
		m_collision = 3;
		return;
	case 0x205:
		m_collision = 2;
		return;
	case TERRAIN_BLOX_1:
	case TERRAIN_BLOX_2:
	case TERRAIN_BLOX_3_SLOPE_SW_STEEP:
	case TERRAIN_BLOX_4:
	case TERRAIN_BLOX_6:
	case TERRAIN_BLOX_7:
	case TERRAIN_BLOX_8_SLOPE_SE_STEEP:
	case TERRAIN_BLOX_14_SLOPE_SW_SHALLOW:
	case TERRAIN_BLOX_15_SLOPE_SE_SHALLOW:
	case 0x214:
		m_collision = 0;
		return;
	case TERRAIN_BLOX_5:
		m_collision = 1;
		return;
	case TERRAIN_FLAME:
	case TERRAIN_ELECTRIC:
	case TERRAIN_EMBERS:
		m_collision = 4;
		return;
	case TERRAIN_CONVEYOR_VARIANT_A:
	case TERRAIN_CONVEYOR_VARIANT_B:
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
