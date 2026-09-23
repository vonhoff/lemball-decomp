#include "CTrampoline.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../Navigation/CAI.h"
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
		p_object->m_deathRequested = 1;
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
	int maximumX = trampolineX + 0xc;
	int minimumX = trampolineX - 0xc;
	int minimumY = trampolineY - 0xc;
	int maximumY = trampolineY + 0xc;
	int maximumZ = trampolineZ + 8;
	int minimumZ = trampolineZ - 4;

	if (minimumX > positionX || maximumX < positionX || minimumY > positionY || maximumY < positionY ||
		minimumZ > positionZ || maximumZ < positionZ) {
		return 0;
	}

	C3DVector velocity;
	velocity.m_xFixed = DEBUG_SENTINEL;
	velocity.m_yFixed = DEBUG_SENTINEL;
	velocity.m_zFixed = DEBUG_SENTINEL;
	CFixed incoming[3] = {CFixed(p_object->m_flightVelocity.m_xFixed),
						  CFixed(p_object->m_flightVelocity.m_yFixed),
						  CFixed(p_object->m_flightVelocity.m_zFixed)};
	if (incoming[2].m_value < 0) {
		incoming[2].m_value = -incoming[2].m_value;
	}

	if (incoming[1].m_value != 0) {
		if (incoming[1].m_value > 0) {
			CFixed impulse[3] = {CFixed(0), CFixed(0x2000), CFixed(0x4000)};
			CFixed bouncedX(incoming[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incoming[1];
			CFixed bouncedZ = impulse[2] + incoming[2];
			C3DVector bounced(bouncedX, bouncedY, bouncedZ);
			velocity.m_xFixed = bounced.m_xFixed;
			velocity.m_yFixed = bounced.m_yFixed;
			velocity.m_zFixed = bounced.m_zFixed;
		}
		else {
			CFixed impulse[3] = {CFixed(0), CFixed(-0x2000), CFixed(0x4000)};
			CFixed bouncedX(incoming[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incoming[1];
			CFixed bouncedZ = impulse[2] + incoming[2];
			C3DVector bounced(bouncedX, bouncedY, bouncedZ);
			velocity.m_xFixed = bounced.m_xFixed;
			velocity.m_yFixed = bounced.m_yFixed;
			velocity.m_zFixed = bounced.m_zFixed;
		}
	}
	else {
		if (incoming[0].m_value > 0) {
			CFixed impulse[3] = {CFixed(0x2000), CFixed(0), CFixed(0x4000)};
			CFixed bouncedX(incoming[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incoming[1];
			CFixed bouncedZ = impulse[2] + incoming[2];
			C3DVector bounced(bouncedX, bouncedY, bouncedZ);
			velocity.m_xFixed = bounced.m_xFixed;
			velocity.m_yFixed = bounced.m_yFixed;
			velocity.m_zFixed = bounced.m_zFixed;
		}
		else {
			CFixed impulse[3] = {CFixed(-0x2000), CFixed(0), CFixed(0x4000)};
			CFixed bouncedX(incoming[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incoming[1];
			CFixed bouncedZ = impulse[2] + incoming[2];
			C3DVector bounced(bouncedX, bouncedY, bouncedZ);
			velocity.m_xFixed = bounced.m_xFixed;
			velocity.m_yFixed = bounced.m_yFixed;
			velocity.m_zFixed = bounced.m_zFixed;
		}
	}

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
