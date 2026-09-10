#include "Slinky.h"

// 68K 0x1061ec2c __ct__7CSlinkyFv
// FUNCTION: LEMBALL 0x0040b480
Slinky::Slinky() : GameObject((eObjectType) 0x35, 0, 0), m_unk0x138(0, 0, 0)
{
}

// 68K 0x1061ecc4 Set__7CSlinkyFiiii
// STUB: LEMBALL 0x0040b4d0
void Slinky::Set(int p_minX, int p_maxX, int p_minY, int p_maxY)
{
}

// 68K 0x1061eda0 GoodEndPt__7CSlinkyFRC7AICOORD
// FUNCTION: LEMBALL 0x0040b630
bool Slinky::GoodEndPt(const AiCoord& p_coordinate)
{
	int x = p_coordinate.m_xFixed >> 12;
	if (m_minX <= x) {
		int y = p_coordinate.m_yFixed >> 12;
		if (m_minY <= y && x <= m_maxX && y <= m_maxY) {
			return true;
		}
	}
	return false;
}

// 68K 0x1061ee14 Move__7CSlinkyFv
// STUB: LEMBALL 0x0040b670
bool Slinky::Move()
{
	return 0;
}

// 68K 0x1061eef4 Process__7CSlinkyFv
// STUB: LEMBALL 0x0040b760
bool Slinky::Process()
{
	return 0;
}

// 68K 0x1011b484 __dt__7CSlinkyFv
Slinky::~Slinky()
{
}
