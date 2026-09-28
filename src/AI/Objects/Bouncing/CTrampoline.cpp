#include "../CTrampoline.h"

#include "../../../Control/Game/CGame.h"
#include "../../../Control/Game/GameTime.h"
#include "../../../Map/Base/CMap.h"
#include "../../../Visos/Foundation/CFixed.h"
#include "../../../Visos/Foundation/CVSMath.h"
#include "../../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0042ab90
int CTrampoline::Hit(const AiCoord& p_position, CGameObject* p_object)
{
	int positionX = p_position.m_xFixed >> 12;
	int positionY = p_position.m_yFixed >> 12;
	int positionZ = p_position.m_zFixed >> 12;
	int minimumX = (m_position.m_xFixed >> 12) - 0xc;
	int maximumX = minimumX + 0x18;
	int minimumY = (m_position.m_yFixed >> 12) - 0xc;
	int maximumY = minimumY + 0x18;
	int minimumZ = m_position.m_zFixed >> 12;
	int maximumZ = minimumZ + 8;
	minimumZ -= 4;

	if (minimumX > positionX || maximumX < positionX || minimumY > positionY || maximumY < positionY ||
		minimumZ > positionZ || maximumZ < positionZ) {
		return 0;
	}

	C3DVector flightVelocity;
	flightVelocity.m_xFixed = DEBUG_SENTINEL;
	flightVelocity.m_yFixed = DEBUG_SENTINEL;
	flightVelocity.m_zFixed = DEBUG_SENTINEL;
	CFixed incomingVelocity[3] = {CFixed(p_object->m_flightVelocity.m_xFixed),
								  CFixed(p_object->m_flightVelocity.m_yFixed),
								  CFixed(p_object->m_flightVelocity.m_zFixed)};
	if (incomingVelocity[2].m_value < 0) {
		incomingVelocity[2].m_value = -incomingVelocity[2].m_value;
	}

	if (incomingVelocity[1].m_value != 0) {
		if (incomingVelocity[1].m_value > 0) {
			CFixed impulse[3] = {CFixed(0), CFixed(0x2000), CFixed(0x4000)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
		else {
			CFixed impulse[3] = {CFixed(0), CFixed(-0x2000), CFixed(0x4000)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
	}
	else {
		if (incomingVelocity[0].m_value > 0) {
			CFixed impulse[3] = {CFixed(0x2000), CFixed(0), CFixed(0x4000)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
		else {
			CFixed impulse[3] = {CFixed(-0x2000), CFixed(0), CFixed(0x4000)};
			CFixed bouncedX(incomingVelocity[0].m_value + impulse[0].m_value);
			CFixed bouncedY = impulse[1] + incomingVelocity[1];
			CFixed bouncedZ = impulse[2] + incomingVelocity[2];
			C3DVector bouncedVelocity(bouncedX, bouncedY, bouncedZ);
			flightVelocity.m_xFixed = bouncedVelocity.m_xFixed;
			flightVelocity.m_yFixed = bouncedVelocity.m_yFixed;
			flightVelocity.m_zFixed = bouncedVelocity.m_zFixed;
		}
	}

	if (flightVelocity.m_xFixed > 0x14000) {
		flightVelocity.m_xFixed = 0x14000;
	}
	if (flightVelocity.m_xFixed < -0x14000) {
		flightVelocity.m_xFixed = -0x14000;
	}
	if (flightVelocity.m_yFixed > 0x14000) {
		flightVelocity.m_yFixed = 0x14000;
	}
	if (flightVelocity.m_yFixed < -0x14000) {
		flightVelocity.m_yFixed = -0x14000;
	}
	if (flightVelocity.m_zFixed > 0x14000) {
		flightVelocity.m_zFixed = 0x14000;
	}

	AiCoord position(m_position.m_xFixed, m_position.m_yFixed, m_position.m_zFixed + 0x8000);
	AiCoord& (AiCoord::*assignPosition)(const AiCoord&) = &AiCoord::operator=;
	(p_object->m_position.*assignPosition)(position);
	p_object->StartFly(flightVelocity, 0);
	p_object->m_balloonPostId = 1;
	p_object->ResetInstructions();
	m_actionDeadline = g_dwGameTick + 0x10;
	m_stateTimer = g_dwSimulationTimestamp;
	Action(ACTION_RUNNING);
	SetSndEffect(SFX_TRMPLINE);
	g_pAI->Score(0x32);
	return 1;
}
