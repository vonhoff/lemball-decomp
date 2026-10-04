#include "CLaser.h"

#include "Game/CGame.h"
#include "Game/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/Facing.h"
#include "Visos/Network/CConnect.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Geometry/CPt3.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/CViewData.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

enum {
	LASER_REACTIVATION_DELAY_TICKS = 60,
	LASER_ACTIVATION_PHASE_ONE_DURATION_TICKS = 6,
	LASER_ACTIVATION_DURATION_TICKS = 24,
	LASER_BEAM_HORIZONTAL_START_OFFSET_PIXELS = 38,
	LASER_BEAM_VERTICAL_START_OFFSET_PIXELS = 20,
	LASER_BEAM_HEIGHT_ABOVE_EMITTER_PIXELS = 3,
	LASER_PLAYER_ACTIVATION_RADIUS_PIXELS = 48
};

// FUNCTION: LEMBALL 0x00428890
CLaser::CLaser() : CGlobalGameObject(OBJECT_LASER_VERTICAL, 0, 0)
{
}

// FUNCTION: LEMBALL 0x004288b0
void CLaser::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x004288d0
void CLaser::Initialise()
{
	m_action = ACTION_READY;
	m_stateTimer = 0;
	m_active = 0;
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x004288f0
CLaser::~CLaser()
{
}

// FUNCTION: LEMBALL 0x00428900
void CLaser::Set(unsigned short p_id, const AICOORD& p_position, eObjectType p_orientation)
{
	SetId(p_id);
	int x = p_position.m_xFixed;
	m_position.m_xFixed = x;
	int y = p_position.m_yFixed;
	m_position.m_yFixed = y;
	m_position.m_zFixed = p_position.m_zFixed;
	int blockX = (x >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	m_objectType = p_orientation;
	m_enabled = 1;
	int blockY = (y >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;

	switch (p_orientation) {
	case OBJECT_LASER_HORIZONTAL:
		m_action = ACTION_READY;
		m_autoActivate = 1;
		m_active = 1;
		break;
	case OBJECT_LASER_VERTICAL:
		m_action = ACTION_READY;
		m_autoActivate = 1;
		m_active = 1;
		break;
	case OBJECT_LASER_EMITTER_H: {
		m_autoActivate = 0;
		m_action = ACTION_READY;
		m_active = 1;
		for (int i = 1; i < 8; i++) {
			int collisionX = blockX + i;
			if (collisionX >= 0 && blockY >= 0 && collisionX < g_pMap->m_ground.m_width &&
				blockY < g_pMap->m_ground.m_height) {
				g_pMap->m_ground.m_ground[g_pMap->m_ground.m_width * blockY + collisionX].m_collision |=
					GROUND_COLLISION_OBJECT_INTERACTION;
			}
		}
		break;
	}
	case OBJECT_LASER_EMITTER_V: {
		m_autoActivate = 0;
		m_action = ACTION_READY;
		m_active = 1;
		for (int i = 1; i < 8; i++) {
			int collisionY = blockY + i;
			if (blockX >= 0 && collisionY >= 0 && blockX < g_pMap->m_ground.m_width &&
				collisionY < g_pMap->m_ground.m_height) {
				g_pMap->m_ground.m_ground[g_pMap->m_ground.m_width * collisionY + blockX].m_collision |=
					GROUND_COLLISION_OBJECT_INTERACTION;
			}
		}
		break;
	}
	}
	m_actionDeadline = g_dwGameTick + LASER_REACTIVATION_DELAY_TICKS;
}

#include "Gameplay/Simulation/CAI.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00428ab0
bool CLaser::CheckHits()
{
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int z = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	CGameObject* hit = NULL;
	int stepX;
	int stepY;
	switch (m_objectType) {
	case OBJECT_LASER_HORIZONTAL:
	case OBJECT_LASER_EMITTER_H:
		stepX = 16;
		stepY = 0;
		x += GROUND_BLOCK_PIXEL_SIZE / 2;
		break;
	case OBJECT_LASER_VERTICAL:
	case OBJECT_LASER_EMITTER_V:
		y += GROUND_BLOCK_PIXEL_SIZE / 2;
		stepX = 0;
		stepY = 16;
		break;
	default:
		return false;
	}
	for (int step = 0; step < 8; step++) {
		x += stepX;
		y += stepY;
		int blockX;
		CMap* map = g_pMap;
		blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
		unsigned short groundZ;
		if (x >= 0 && y >= 0) {
			int cellYValue;
			int widthValue = map->m_ground.m_width;
			const int& width = widthValue;
			if (blockX < width && map->m_ground.m_height > blockY) {
				int cellX = x & GROUND_BLOCK_PIXEL_MASK;
				cellYValue = y;
				const int& cellY = cellYValue;
				cellYValue &= 15;
				groundZ = map->m_ground.m_ground[blockY * width + blockX].GetZ(cellX, cellY);
			}
			else {
				groundZ = 0;
			}
		}
		else {
			groundZ = 0;
		}
		if (groundZ > z) {
			break;
		}
		CPt3 point;
		point.m_x = x;
		point.m_y = y;
		CAI* ai = g_pAI;
		point.m_z = z;
		ai->m_collisionExclude = NULL;
		ai->m_collisionPoint = point;
		ai->m_collisionIndex = 0;
		if (ai->m_objectCount > 0) {
			do {
				CGameObject* object = ai->m_objects[ai->m_collisionIndex];
				if (ai->m_collisionExclude != object && object->Collision(ai->m_collisionPoint)) {
					hit = ai->m_objects[ai->m_collisionIndex];
					ai->m_collisionIndex++;
					goto found;
				}
				ai->m_collisionIndex++;
			} while (ai->m_collisionIndex < ai->m_objectCount);
		}
		hit = NULL;
	found:
		if (hit != NULL && hit->m_objectType == OBJECT_PLAYER_2) {
			break;
		}
	}
	if (hit != NULL) {
		m_target = hit;
		hit->m_action = ACTION_EXTERNAL_CONTROL;
		hit->m_actionArgument = EXTERNAL_CONTROL_ELECTROCUTED;
		m_target->m_actionDeadline = g_dwGameTick + HAZARD_DEATH_DELAY_TICKS;
		SetSndEffect(SFX_ELECCY);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00428cf0
bool CLaser::Process()
{
	if (m_isRemoteObject != 0) {
		m_active = m_action != ACTION_READY;
		if (m_action == ACTION_ACTIVATED && m_target == NULL) {
			CheckHits();
		}
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case ACTION_RECOVERY:
				if (m_target != NULL) {
					m_target->m_deathRequested = 1;
					m_target = NULL;
				}
				Action(ACTION_READY);
				break;
			case ACTION_ACTIVATING:
				m_target = NULL;
				break;
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	if (g_pActiveConnection != NULL && g_pActiveConnection->m_isHost != 0) {
		return true;
	}
	if (m_active != 0) {
		switch (m_action) {
		case ACTION_RECOVERY:
			if (m_target != NULL) {
				m_target->SetSndEffect(SFX_ELECCY);
				m_target->m_deathRequested = 1;
				m_target = NULL;
			}
			Action(ACTION_READY);
			return true;
		case ACTION_READY:
			if (m_actionDeadline < g_dwGameTick) {
				Activate();
				return true;
			}
			break;
		case ACTION_ACTIVATING:
			if (m_actionPhase1Deadline < g_dwGameTick) {
				m_target = NULL;
				Action(ACTION_ACTIVATED);
				return true;
			}
			break;
		case ACTION_ACTIVATED:
			if (m_target == NULL) {
				CheckHits();
			}
			if (m_actionDeadline < g_dwGameTick) {
				m_enabled = 1;
				m_active = m_autoActivate;
				m_actionDeadline = g_dwGameTick + LASER_REACTIVATION_DELAY_TICKS;
				if (m_target != NULL) {
					m_target->m_deathRequested = 1;
					m_target = NULL;
				}
				Action(ACTION_RECOVERY);
			}
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x00428ec0
bool CLaser::Activate()
{
	m_active = 1;
	if (g_pActiveConnection != NULL && g_pActiveConnection->m_isHost != 0) {
		return false;
	}
	m_lastMovementTick = g_dwGameTick;
	m_actionPhase1Deadline = g_dwGameTick + LASER_ACTIVATION_PHASE_ONE_DURATION_TICKS;
	m_actionDeadline = g_dwGameTick + LASER_ACTIVATION_DURATION_TICKS;
	m_stateTimer = g_dwSimulationTimestamp;
	m_target = NULL;
	Action(ACTION_ACTIVATING);
	return true;
}

// FUNCTION: LEMBALL 0x00428f30
bool CLaser::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	if (p_object->m_objectType == OBJECT_PLAYER_2 &&
		(int) Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) < LASER_PLAYER_ACTIVATION_RADIUS_PIXELS) {
		Activate();
		m_target = p_object;
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00428f90
int CLaser::GetViewData(CViewData* p_viewData)
{
	p_viewData->m_objectId = m_objectId;
	p_viewData->m_objectType = m_objectType;
	p_viewData->m_playerIndex = 0;
	((CPt3&) p_viewData->m_positionX).InitializeFromAiCoord(m_position);
	p_viewData->m_facingDirection = m_facingDirection;
	unsigned int argument = (unsigned short) m_actionArgument;
	const unsigned int timer = m_stateTimer;
	p_viewData->m_action = m_action;
	p_viewData->m_actionArgument = argument;
	p_viewData->m_stateTimer = timer;
	p_viewData->m_statusFlags = 0;
	p_viewData->m_hidden = m_hidden;
	((C3DVector&) p_viewData->m_auxiliaryPosition) = (const C3DVector&) m_auxiliaryPosition;
	p_viewData->m_soundEffect = m_soundEffect;
	p_viewData->m_animationTime = m_isRemoteObject ? g_dwNetworkSimulationTimestamp : g_dwSimulationTimestamp;
	SetSndEffect(SFX_NONE);
	p_viewData->m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
	p_viewData++;
	int count = 1;
	if (m_action == ACTION_ACTIVATED) {
		switch (m_objectType) {
		case OBJECT_LASER_HORIZONTAL:
		case OBJECT_LASER_EMITTER_H: {
			int x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) + LASER_BEAM_HORIZONTAL_START_OFFSET_PIXELS;
			int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			int z = (m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS) + LASER_BEAM_HEIGHT_ABOVE_EMITTER_PIXELS;
			for (unsigned int i = 1; i < 8; i++) {
				unsigned short height;
				int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
				int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
				CMap* map = g_pMap;
				if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
					height = map->m_ground.GetGroundCell(blockX, blockY)
								 ->GetZ(x & GROUND_BLOCK_PIXEL_MASK, y & GROUND_BLOCK_PIXEL_MASK);
				}
				else {
					height = 0;
				}
				if (height > z) {
					break;
				}
				p_viewData->m_objectId = m_objectId;
				p_viewData->m_objectType = m_objectType;
				p_viewData->m_playerIndex = 0;
				((CPt3&) p_viewData->m_positionX).InitializeFromAiCoord(m_position);
				p_viewData->m_facingDirection = m_facingDirection;
				p_viewData->SetViewActionTuple(m_action, (unsigned short) m_actionArgument, m_stateTimer);
				p_viewData->m_statusFlags = 0;
				p_viewData->m_hidden = m_hidden;
				((C3DVector&) p_viewData->m_auxiliaryPosition) = (const C3DVector&) m_auxiliaryPosition;
				p_viewData->m_soundEffect = m_soundEffect;
				p_viewData->m_animationTime =
					m_isRemoteObject ? g_dwNetworkSimulationTimestamp : g_dwSimulationTimestamp;
				SetSndEffect(SFX_NONE);
				p_viewData->m_transientFlags = m_transientFlags;
				m_transientFlags = 0;
				p_viewData->m_positionX = x;
				p_viewData->m_positionY = y;
				p_viewData->m_positionZ = z;
				p_viewData->m_objectType = OBJECT_LASER_VERTICAL_BEAM;
				p_viewData->m_facingDirection = 0;
				p_viewData->m_action = ACTION_ACTIVATED;
				p_viewData++;
				count++;
				x += GROUND_BLOCK_PIXEL_SIZE;
			}
			break;
		}
		case OBJECT_LASER_VERTICAL:
		case OBJECT_LASER_EMITTER_V: {
			int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			int y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) + LASER_BEAM_VERTICAL_START_OFFSET_PIXELS;
			int z = (m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS) + LASER_BEAM_HEIGHT_ABOVE_EMITTER_PIXELS;
			for (unsigned int i = 1; i < 8; i++) {
				unsigned short height;
				int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
				int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
				CMap* map = g_pMap;
				if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
					height = map->m_ground.GetGroundCell(blockX, blockY)
								 ->GetZ(x & GROUND_BLOCK_PIXEL_MASK, y & GROUND_BLOCK_PIXEL_MASK);
				}
				else {
					height = 0;
				}
				if (height > z) {
					break;
				}
				CGameObject::GetViewData(*p_viewData);
				p_viewData->m_positionX = x;
				p_viewData->m_positionY = y;
				p_viewData->m_positionZ = z;
				p_viewData->m_objectType = OBJECT_LASER_HORIZONTAL_BEAM;
				p_viewData->m_facingDirection = 0;
				p_viewData->m_action = ACTION_ACTIVATED;
				p_viewData++;
				count++;
				y += GROUND_BLOCK_PIXEL_SIZE;
			}
			break;
		}
		}
	}
	return count;
}

// FUNCTION: LEMBALL 0x00429e40
void CLaser::DoActivate()
{
}
