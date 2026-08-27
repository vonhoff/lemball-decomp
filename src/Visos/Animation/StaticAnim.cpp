#include "StaticAnim.h"

// 68K 0x10115d18 GetFrameNo__11CStaticAnimFv
// FUNCTION: LEMBALL 0x00435870
unsigned int StaticAnim::GetFrameNo()
{
	return m_frameState;
}
