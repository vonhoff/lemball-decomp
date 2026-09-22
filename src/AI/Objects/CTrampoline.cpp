#include "CTrampoline.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVsMath.h"
#include "../Navigation/CAi.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0042a990
CTrampoline::CTrampoline() : CGlobalGameObject(OBJECT_TRAMPOLINE, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0042a9b0
void CTrampoline::Restart()
{
	CGlobalGameObject::Restart();
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// FUNCTION: LEMBALL 0x0042a9d0
CTrampoline::~CTrampoline()
{
}

// FUNCTION: LEMBALL 0x0042a9e0
void CTrampoline::Set(unsigned short p_id, const AiCoord& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_action = ACTION_0x18;
	m_active = 1;
	m_enabled = 1;

	int blockX = p_position.m_xFixed >> 12;
	blockX += (p_position.m_xFixed >> 31) & 0xf;
	blockX >>= 4;
	int blockY = p_position.m_yFixed >> 12;
	blockY += (p_position.m_yFixed >> 31) & 0xf;
	blockY >>= 4;
	if (blockX >= 0 && blockY >= 0) {
		CGroundArray* ground = &g_pMap->m_ground;
		if (blockX < ground->m_width && blockY < ground->m_height) {
			ground->m_ground[ground->m_width * blockY + blockX].m_collision |= 0x8000;
		}
	}
}

// FUNCTION: LEMBALL 0x0042aa80
bool CTrampoline::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_0x1b) {
				SetSndEffect(SFX_TRMPLINE);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_enabled == 0) {
		return 1;
	}
	if (m_action == ACTION_0x1b && m_actionDeadline < g_dwGameTick) {
		Action(ACTION_0x18);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0042aaf0
int CTrampoline::TryEnableNearPosition(const AiCoord& p_position, CGameObject* p_object)
{
	if ((int) Distance(m_position.m_xFixed >> 12,
					   m_position.m_yFixed >> 12,
					   p_position.m_xFixed >> 12,
					   p_position.m_yFixed >> 12) < 0x20) {
		m_enabled = 1;
		m_lastMovementTick = g_dwGameTick;
		m_stateTimer = g_dwSimulationTimestamp;
		m_position.m_xFixed = p_position.m_xFixed + 0x4000;
		m_position.m_yFixed = p_position.m_yFixed + 0x4000;
		int z = p_position.m_zFixed;
		m_position.m_zFixed = z;
		m_relocationZ = z >> 12;
		p_object->m_unk0x2c = 1;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042ab90
int CTrampoline::Hit(const AiCoord& p_position, CGameObject* p_object)
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

	CFixed verticalVelocity(p_object->m_flightVelocity.m_zFixed);
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
		velocity.m_yFixed = (CFixed(0) + CFixed(p_object->m_flightVelocity.m_yFixed)).m_value;
	}
	else {
		velocity.m_xFixed = p_object->m_flightVelocity.m_xFixed;
		if (p_object->m_flightVelocity.m_yFixed < 1) {
			velocity.m_yFixed = (CFixed(-0x2000) + CFixed(p_object->m_flightVelocity.m_yFixed)).m_value;
		}
		else {
			velocity.m_yFixed = (CFixed(0x2000) + CFixed(p_object->m_flightVelocity.m_yFixed)).m_value;
		}
	}
	velocity.m_zFixed = (CFixed(0x4000) + verticalVelocity).m_value;

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
	Action(ACTION_0x1b);
	SetSndEffect(SFX_TRMPLINE);
	g_pAI->Score(0x32);
	return 1;
}

// FUNCTION: LEMBALL 0x0042b9b0
void CTrampoline::DoActivate()
{
}
