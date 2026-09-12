#include "Trampoline.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Foundation/Fixed.h"
#include "../Navigation/Ai.h"

// 68K 0x10620548 __ct__11CTrampolineFv
// FUNCTION: LEMBALL 0x0042a990
Trampoline::Trampoline() : GlobalGameObject(0x22, 0, 0)
{
}

// 68K 0x1062058e Restart__11CTrampolineFv
// FUNCTION: LEMBALL 0x0042a9b0
void Trampoline::Restart()
{
	GlobalGameObject::Restart();
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// 68K 0x1062062e Set__11CTrampolineFUsRC7AICOORD
// FUNCTION: LEMBALL 0x0042a9e0
void Trampoline::Set(unsigned short p_id, const AiCoord& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_action = (eAction) 0x18;
	m_active = 1;
	m_enabled = 1;

	int blockX = p_position.m_xFixed >> 12;
	blockX += (p_position.m_xFixed >> 31) & 0xf;
	blockX >>= 4;
	int blockY = p_position.m_yFixed >> 12;
	blockY += (p_position.m_yFixed >> 31) & 0xf;
	blockY >>= 4;
	if (blockX >= 0 && blockY >= 0) {
		GroundArray* ground = &g_pMap->m_ground;
		if (blockX < ground->m_width && blockY < ground->m_height) {
			ground->m_ground[ground->m_width * blockY + blockX].m_collision |= 0x8000;
		}
	}
}

// 68K 0x106206e2 Process__11CTrampolineFv
// FUNCTION: LEMBALL 0x0042aa80
bool Trampoline::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == (eAction) 0x1b) {
				SetSndEffect((eSoundEffect) 0x17);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_enabled == 0) {
		return 1;
	}
	if (m_action == (eAction) 0x1b && m_actionDeadline < g_dwGameTick) {
		Action((eAction) 0x18);
	}
	return 1;
}

// 68K 0x10620788 Hit__11CTrampolineFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x0042ab90
int Trampoline::Hit(const AiCoord& p_position, GameObject* p_object)
{
	int positionX = p_position.m_xFixed >> 12;
	int positionY = p_position.m_yFixed >> 12;
	int positionZ = p_position.m_zFixed >> 12;
	int trampolineX = m_position.m_xFixed >> 12;
	int trampolineY = m_position.m_yFixed >> 12;
	int trampolineZ = m_position.m_zFixed >> 12;

	if (positionX < trampolineX - 0xc || positionX > trampolineX + 0xc || positionY < trampolineY - 0xc ||
		positionY > trampolineY + 0xc || positionZ < trampolineZ - 4 || positionZ > trampolineZ + 8) {
		return 0;
	}

	Fixed verticalVelocity(p_object->m_flightVelocity.m_zFixed);
	if (verticalVelocity.m_value < 0) {
		verticalVelocity.m_value = -verticalVelocity.m_value;
	}

	C3DVector velocity;
	if (p_object->m_flightVelocity.m_yFixed == 0) {
		if (p_object->m_flightVelocity.m_xFixed < 1) {
			velocity.m_xFixed = p_object->m_flightVelocity.m_xFixed - 0x2000;
		}
		else {
			velocity.m_xFixed = p_object->m_flightVelocity.m_xFixed + 0x2000;
		}
		velocity.m_yFixed = (Fixed(0) + Fixed(p_object->m_flightVelocity.m_yFixed)).m_value;
	}
	else {
		velocity.m_xFixed = p_object->m_flightVelocity.m_xFixed;
		if (p_object->m_flightVelocity.m_yFixed < 1) {
			velocity.m_yFixed = (Fixed(-0x2000) + Fixed(p_object->m_flightVelocity.m_yFixed)).m_value;
		}
		else {
			velocity.m_yFixed = (Fixed(0x2000) + Fixed(p_object->m_flightVelocity.m_yFixed)).m_value;
		}
	}
	velocity.m_zFixed = (Fixed(0x4000) + verticalVelocity).m_value;

	if (velocity.m_xFixed > 0x14000) {
		velocity.m_xFixed = 0x14000;
	}
	if (velocity.m_xFixed < -0x14000) {
		velocity.m_xFixed = -0x14000;
	}
	if (velocity.m_yFixed > 0x14000) {
		velocity.m_yFixed = 0x14000;
	}
	if (velocity.m_yFixed < -0x14000) {
		velocity.m_yFixed = -0x14000;
	}
	if (velocity.m_zFixed > 0x14000) {
		velocity.m_zFixed = 0x14000;
	}

	AiCoord position(m_position.m_xFixed, m_position.m_yFixed, m_position.m_zFixed + 0x8000);
	p_object->m_position = position;
	p_object->StartFly(velocity, 0);
	p_object->m_balloonPostId = 1;
	p_object->ResetInstructions();
	m_actionDeadline = g_dwGameTick + 0x10;
	m_stateTimer = g_dwSimulationTimestamp;
	Action((eAction) 0x1b);
	SetSndEffect((eSoundEffect) 0x17);
	g_pAI->Score(0x32);
	return 1;
}

// 68K 0x1011b5d2 DoActivate__11CTrampolineFv
// FUNCTION: LEMBALL 0x0042b9b0
void Trampoline::DoActivate()
{
}

// 68K 0x106205d0 __dt__11CTrampolineFv
Trampoline::~Trampoline()
{
}
