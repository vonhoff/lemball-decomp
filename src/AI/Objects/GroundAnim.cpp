#include "GroundAnim.h"

#include "../../Control/Game/Game.h"

// 68K 0x1060e30e __ct__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf00
GroundAnim::GroundAnim()
{
}

// 68K 0x1060e332 Restart__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf10
void GroundAnim::Restart()
{
	unsigned int currentTick = g_dwGameTick;
	m_count = 0;
	m_needsValidation = 1;
	m_nextProcessTick = currentTick + 2;
}

// 68K 0x1060e36e __dt__11CGroundAnimFv
// FUNCTION: LEMBALL 0x0040cf30
GroundAnim::~GroundAnim()
{
}

// 68K 0x1060e3ac CheckAllAnims__11CGroundAnimFv
// STUB: LEMBALL 0x0040cf40
bool GroundAnim::CheckAllAnims()
{
	return 0;
}

// 68K 0x1060e4a4 Process__11CGroundAnimFv
// STUB: LEMBALL 0x0040cff0
void GroundAnim::Process()
{
}

// 68K 0x1060e5ae Check__11CGroundAnimFRC8tCoord3d
// STUB: LEMBALL 0x0040d080
bool GroundAnim::Check(const Coord3d& p_coordinate)
{
	return 0;
}

// 68K 0x1060e62a Add__11CGroundAnimFRC8tCoord3dUsUs
// STUB: LEMBALL 0x0040d130
void GroundAnim::Add(const Coord3d& p_coordinate, unsigned short p_startFrame, unsigned short p_endFrame)
{
}

// 68K 0x1060e768 LoadLevel__11CGroundAnimFPUciUc
// FUNCTION: LEMBALL 0x0040d2e0
void GroundAnim::LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip)
{
	unsigned short count = *(unsigned short*) p_data;
	p_data += 2;

	if (count != 0) {
		unsigned int remaining = count;
		do {
			Coord3d coordinate;
			coordinate.m_x = *(unsigned short*) p_data;
			p_data += 2;
			coordinate.m_y = *(unsigned short*) p_data;
			p_data += 2;
			coordinate.m_z = *(unsigned short*) p_data;
			p_data += 2;
			unsigned int startFrame = *(unsigned short*) p_data;
			p_data += 2;
			unsigned int endFrame = *(unsigned short*) p_data;
			p_data += 2;
			Add(coordinate, startFrame, endFrame);
			remaining--;
		} while (remaining != 0);
	}
}
