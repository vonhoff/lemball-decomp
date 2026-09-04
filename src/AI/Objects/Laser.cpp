#include "Laser.h"

// 68K 0x10613518 __ct__6CLaserFv
// STUB: LEMBALL 0x00428890
Laser::Laser()
{
}

// 68K 0x10613558 Restart__6CLaserFv
// FUNCTION: LEMBALL 0x004288b0
void Laser::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x1061358a Initialise__6CLaserFv
// FUNCTION: LEMBALL 0x004288d0
void Laser::Initialise()
{
	m_action = (eAction) 0x18;
	m_stateTimer = 0;
	m_active = 0;
	m_enabled = 0;
}

// 68K 0x10613618 Set__6CLaserFUsRC7AICOORD11eObjectType
// STUB: LEMBALL 0x00428900
void Laser::Set(unsigned short p_id, const AiCoord& p_position, eObjectType p_orientation)
{
}

// 68K 0x1061378e CheckHits__6CLaserFv
// STUB: LEMBALL 0x00428ab0
bool Laser::CheckHits()
{
	return 0;
}

// 68K 0x106138ea Process__6CLaserFv
// STUB: LEMBALL 0x00428cf0
bool Laser::Process()
{
	return 0;
}

// 68K 0x10613ace Activate__6CLaserFv
// STUB: LEMBALL 0x00428ec0
bool Laser::Activate()
{
	return 0;
}

// 68K 0x10613b58 StepOn__6CLaserFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x00428f30
bool Laser::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x10613bec GetViewData__6CLaserFP9CViewData
// STUB: LEMBALL 0x00428f90
int Laser::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1011a726 DoActivate__6CLaserFv
// FUNCTION: LEMBALL 0x00429e40
void Laser::DoActivate()
{
}

// 68K 0x106135c0 __dt__6CLaserFv
Laser::~Laser()
{
}
