#include "Frames.h"

// 68K 0x10115d46 GetFrameNo__7CFramesFv
// FUNCTION: LEMBALL 0x00435880
unsigned int Frames::GetFrameNo()
{
	return m_frameState;
}
