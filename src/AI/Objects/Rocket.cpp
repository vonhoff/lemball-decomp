#include "Rocket.h"

// 68K 0x1061d8ce __ct__7CRocketFv
// STUB: LEMBALL 0x004267d0
Rocket::Rocket()
{
}

// 68K 0x1061d910 Initialise__7CRocketFv
// FUNCTION: LEMBALL 0x004267f0
void Rocket::Initialise()
{
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// 68K 0x1061d942 Restart__7CRocketFv
// FUNCTION: LEMBALL 0x00426810
void Rocket::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x1061d9cc Set__7CRocketFUsRC7AICOORD
// STUB: LEMBALL 0x00426840
void Rocket::Set(unsigned short p_id, const AiCoord& p_position)
{
}

// 68K 0x1061da76 Process__7CRocketFv
// STUB: LEMBALL 0x004268e0
bool Rocket::Process()
{
	return 0;
}

// 68K 0x1061db8c StepOn__7CRocketFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x004269d0
int Rocket::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x1061dc6a DoActivate__7CRocketFv
// STUB: LEMBALL 0x00426a60
void Rocket::DoActivate()
{
}

// 68K 0x1011b220 GetViewData__7CRocketFR9CViewData
// STUB: LEMBALL 0x004273f0
void Rocket::GetViewData(ViewData& p_viewData)
{
}

// 68K 0x1061d972 __dt__7CRocketFv
Rocket::~Rocket()
{
}

