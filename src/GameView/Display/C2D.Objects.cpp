#include "C2D.h"

#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Hazards/CSlinky.h"
#include "Gameplay/Mechanisms/CSwitch.h"
#include "Gameplay/Objects/CViewData.h"
#include "Application/GameTime.h"
#include "Frontend/Loading/CFrontendResourceLoader.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/Facing.h"
#include "GameView/Animation/CLemmingAnimsManager.h"
#include "Engine/Math/FixedPoint.h"

#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Engine/Graphics/Palettes/CBaseRemap.h"

class CBaseQueueHandler;
class CRemap;

#include <stddef.h>

#include "Engine/Resources/Manifest.h"

enum eMoverVisualState {
	MOVER_VISUAL_GROUND = 0,
	MOVER_VISUAL_STAR = 1
};

extern const unsigned int* g_styleObjectClip;

extern const short g_slinkyOffsets[4][2];

extern const short g_sheepOffset[2];

// GLOBAL: LEMBALL 0x00497018
static const short g_lemmingFlyOffsets[8][2] = {
	{10, 16},
	{10, 16},
	{14, 17},
	{14, 17},
	{11, 18},
	{11, 18},
	{7, 18},
	{7, 18},
};

// GLOBAL: LEMBALL 0x0049eef8
static unsigned long g_lemmingFlyResources[] = {
	RES_GAME_JUMP_NE,
	RES_GAME_JUMP_NE,
	RES_GAME_JUMP_SE,
	RES_GAME_JUMP_SE,
	RES_GAME_JUMP_SW,
	RES_GAME_JUMP_SW,
	RES_GAME_JUMP_NW,
	RES_GAME_JUMP_NW,
};

// GLOBAL: LEMBALL 0x0049efa8
static unsigned long g_lemmingExternalResources[] = {
	RES_GAME_LEM_LASER_N,
	RES_GAME_LEM_LASER_E,
	RES_GAME_LEM_LASER_E,
	RES_GAME_LEM_LASER_S,
	RES_GAME_LEM_LASER_S,
	RES_GAME_LEM_LASER_W,
	RES_GAME_LEM_LASER_W,
	RES_GAME_LEM_LASER_N,
};

// GLOBAL: LEMBALL 0x00496fe8
static const short g_lemmingWaitOffsets[][2] = {{13, 25}, {10, 18}, {7, 14}, {7, 14}};

// GLOBAL: LEMBALL 0x00496ff8
static const short g_lemmingHitOffsets[][2] =
	{{28, 25}, {24, 24}, {24, 31}, {19, 25}, {23, 24}, {25, 21}, {32, 36}, {25, 26}};

// GLOBAL: LEMBALL 0x0049ee30
static int g_lemmingFireOffsets[][2] = {{11, 9}, {8, 5}, {10, 5}, {16, 4}, {19, 4}, {26, 6}, {26, 9}, {14, 13}};

// GLOBAL: LEMBALL 0x0049ee78
static unsigned long g_lemmingStandResources[] = {RES_GAME_LEMMINGSTANDNE,
												  RES_GAME_LEMMINGSTANDE,
												  RES_GAME_LEMMINGSTANDSE,
												  RES_GAME_LEMMINGSTANDS,
												  RES_GAME_LEMMINGSTANDSW,
												  RES_GAME_LEMMINGSTANDW,
												  RES_GAME_LEMMINGSTANDNW,
												  RES_GAME_LEMMINGSTANDN};

// GLOBAL: LEMBALL 0x0049ee98
static unsigned long g_lemmingFireResources[] = {RES_GAME_LEMMINGFIRENE,
												 RES_GAME_LEMMINGFIREE,
												 RES_GAME_LEMMINGFIRESE,
												 RES_GAME_LEMMINGFIRES,
												 RES_GAME_LEMMINGFIRESW,
												 RES_GAME_LEMMINGFIREW,
												 RES_GAME_LEMMINGFIRENW,
												 RES_GAME_LEMMINGFIREN};

// GLOBAL: LEMBALL 0x0049eed8
static unsigned long g_lemmingHitResources[] = {RES_GAME_HIT_NORTH_EAST,
												RES_GAME_HIT_EAST,
												RES_GAME_HIT_SOUTH_EAST,
												RES_GAME_HIT_SOUTH,
												RES_GAME_HIT_SOUTH_WEST,
												RES_GAME_HIT_WEST,
												RES_GAME_HIT_NORTH_WEST,
												RES_GAME_HIT_NORTH};

// GLOBAL: LEMBALL 0x0049ef18
static unsigned long g_lemmingWalkResources[] = {RES_GAME_LEMMINGWALKNE,
												 RES_GAME_LEMMINGWALKE,
												 RES_GAME_LEMMINGWALKSE,
												 RES_GAME_LEMMINGWALKS,
												 RES_GAME_LEMMINGWALKSW,
												 RES_GAME_LEMMINGWALKW,
												 RES_GAME_LEMMINGWALKNW,
												 RES_GAME_LEMMINGWALKN};

// GLOBAL: LEMBALL 0x0049ef98
static unsigned long g_lemmingWaitResources[] = {RES_GAME_WAIT_JIG,
												 RES_GAME_WAIT_TOSS,
												 RES_GAME_WAIT_LOOK,
												 RES_GAME_WAIT_LOOK};

// GLOBAL: LEMBALL 0x00496fd8
static const short g_lemmingStandOffset[] = {8, 18};

// GLOBAL: LEMBALL 0x00496fdc
static const short g_lemmingWalkOffset[] = {8, 18};

// GLOBAL: LEMBALL 0x00496fe0
static const short g_lemmingAirOffset[] = {4, 12};

// GLOBAL: LEMBALL 0x00497038
static const short g_lemmingSommersaultOffset[] = {14, 27};

// GLOBAL: LEMBALL 0x00497070
static const short g_bulletOffset[] = {4, 4};

// GLOBAL: LEMBALL 0x00497064
static const short g_ammoOffset[] = {8, 16};

// GLOBAL: LEMBALL 0x00497068
static const short g_pelletOffset[] = {16, 16};

// GLOBAL: LEMBALL 0x00497098
static const short g_trampolineOffset[] = {22, 22};

// GLOBAL: LEMBALL 0x004970b0
static const short g_moverOffset[] = {17, 30};

// GLOBAL: LEMBALL 0x0049709c
static const short g_paintGunOffset[] = {25, 27};

// GLOBAL: LEMBALL 0x0049703c
static const short g_crateOffset[] = {8, 24};

// GLOBAL: LEMBALL 0x00497040
static const short g_crateExplosionOffset[] = {34, 50};

// GLOBAL: LEMBALL 0x00497088
static const short g_timeBonusOffset[] = {16, 18};

// GLOBAL: LEMBALL 0x00497074
static const short g_ballOffset[] = {10, 15};

// GLOBAL: LEMBALL 0x00497078
static const short g_explosionOffset[] = {15, 17};

// GLOBAL: LEMBALL 0x00497054
static const short g_keyOffset[] = {8, 32};

// GLOBAL: LEMBALL 0x0049704c
static const short g_mineOffset[] = {30, 35};

// GLOBAL: LEMBALL 0x00497050
static const short g_mineStillOffset[] = {2, 2};

// GLOBAL: LEMBALL 0x00497094
static const short g_doorOffset[] = {26, 24};

// GLOBAL: LEMBALL 0x0049707c
static const short g_switchOffset[] = {5, 25};

// GLOBAL: LEMBALL 0x00497080
static const short g_flagOffset[] = {15, 28};

// GLOBAL: LEMBALL 0x00497084
static const short g_bonusOffset[] = {16, 16};

// GLOBAL: LEMBALL 0x00497090
static const short g_trapDoorOffset[] = {48, 40};

// GLOBAL: LEMBALL 0x00497044
static const short g_baseOffset[] = {40, 60};

// GLOBAL: LEMBALL 0x00497048
static const short g_animOffset[] = {40, 60};

// GLOBAL: LEMBALL 0x004970a0
extern const short g_slinkyOffsets[][2] = {{13, 25}, {30, 32}, {29, 26}, {14, 34}};

// GLOBAL: LEMBALL 0x0049706c
extern const short g_sheepOffset[] = {9, 8};

// FUNCTION: LEMBALL 0x0043bce0
unsigned long C2D::LemmingFly(CViewData& p_viewData, int& p_frame)
{
	unsigned int direction =
		((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	int frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;

	p_frame = 0;
	if (frameDelta < 0) {
		return g_lemmingFlyResources[direction];
	}

	CMap* map;
	int viewX;
	int viewY;
	int blockX;
	int blockY;
	viewY = (unsigned short) p_viewData.m_gameY;
	map = m_map;
	viewX = (unsigned short) p_viewData.m_gameX;
	blockY = viewY >> GROUND_BLOCK_PIXEL_SHIFT;
	blockX = viewX >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short groundZ;
	if (viewX >= 0 && viewY >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
		groundZ = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(viewX & GROUND_BLOCK_PIXEL_MASK,
																					   viewY & GROUND_BLOCK_PIXEL_MASK);
	}
	else {
		groundZ = 0;
	}

	if (p_viewData.m_positionZ <= groundZ) {
		p_frame = frameDelta * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND + 7;
		if (p_frame > 12) {
			p_frame = 12;
		}
	}
	else {
		p_frame = frameDelta * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (p_frame > 6) {
			p_frame = 6;
		}
	}
	return g_lemmingFlyResources[direction];
}

// FUNCTION: LEMBALL 0x0043bde0
void C2D::DrawLemmingFlyShadow(CViewData& p_viewData)
{
	int viewX;
	int viewY;
	int screenX;
	CMap* map;
	unsigned short groundZ;

	viewX = (unsigned short) p_viewData.m_gameX;
	map = m_map;
	viewY = (unsigned short) p_viewData.m_gameY;
	int blockX = viewX >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = viewY >> GROUND_BLOCK_PIXEL_SHIFT;
	if (viewX >= 0 && viewY >= 0 && blockX < map->m_ground.m_width && blockY < map->m_ground.m_height) {
		groundZ = map->m_ground.m_ground[map->m_ground.m_width * blockY + blockX].GetZ(viewX & GROUND_BLOCK_PIXEL_MASK,
																					   viewY & GROUND_BLOCK_PIXEL_MASK);
	}
	else {
		groundZ = 0;
	}

	screenX = (viewX << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS;
	viewY = (viewY << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS;
	m_map->GameToScreen(screenX, viewY);
	viewX = viewY - ((int) ((unsigned int) groundZ << FIXED_POINT_FRACTION_BITS) >> FIXED_POINT_FRACTION_BITS);
	int drawX = (screenX - m_viewOriginX) << FIXED_POINT_FRACTION_BITS;
	int drawY = (viewX - m_viewOriginY) << FIXED_POINT_FRACTION_BITS;

	m_lemmingAnims->DrawAnim((short) (drawX >> FIXED_POINT_FRACTION_BITS),
							 (short) (drawY >> FIXED_POINT_FRACTION_BITS),
							 RES_GAME_BALLOON_SHADOW,
							 0,
							 0,
							 NULL);
}

// FUNCTION: LEMBALL 0x0043bee0
void C2D::DrawLemmingJump(CViewData& p_viewData, unsigned int p_remapped)
{
	unsigned int direction;
	int frame;
	int frameDelta;
	int y;
	unsigned long resource;
	int x;
	unsigned int actionArgument;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	x = p_viewData.m_positionX - g_lemmingFlyOffsets[direction][0];
	y = p_viewData.m_positionY - g_lemmingFlyOffsets[direction][1];
	resource = g_lemmingFlyResources[direction];
	frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	actionArgument = (unsigned short) p_viewData.m_actionArgument;
	switch (actionArgument) {
	case JUMP_ANIMATION_EARLY_FRAMES:
		frame = frameDelta * 15 / 1024;
		if (frame > 6) {
			frame = 6;
		}
		break;
	case JUMP_ANIMATION_LATE_FRAMES:
		frame = frameDelta * 15 / 1024 + 7;
		if (frame > 12) {
			frame = 12;
		}
		break;
	}

	if (p_remapped != 0) {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, (CRemap*) m_paletteRemap);
	}
	else {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, NULL);
	}
}

// FUNCTION: LEMBALL 0x0043bfc0
void C2D::DrawLemmingLanding(CViewData& p_viewData, unsigned int p_remapped)
{
	int x;
	int y;
	unsigned long resource;
	unsigned int direction;
	int frame;
	int frameDelta;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	resource = g_lemmingFlyResources[direction];
	x = p_viewData.m_positionX - g_lemmingFlyOffsets[direction][0];
	y = p_viewData.m_positionY - g_lemmingFlyOffsets[direction][1];
	frameDelta = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	frame = frameDelta * 15 / 1024 + 7;
	if (frame > 12) {
		frame = 12;
	}

	if (p_remapped != 0) {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, (CRemap*) m_paletteRemap);
	}
	else {
		m_lemmingAnims->DrawAnim(x, y, resource, frame, 0, NULL);
	}
}

// FUNCTION: LEMBALL 0x0043c070
void C2D::DrawLemmingFall(CViewData& p_viewData, unsigned int p_remapped)
{
	DrawLemmingJump(p_viewData, p_remapped);
}

// FUNCTION: LEMBALL 0x0043c090
void C2D::DrawLemmingExternal(CViewData& p_viewData, unsigned int p_remapped)
{
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	int frameDelta = (int) p_viewData.m_animationTime - (int) p_viewData.m_stateTimer;
	unsigned int frame = frameDelta * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
	unsigned int direction =
		((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	CRemap* remap;

	if ((int) frame < 0) {
		frame = 0;
	}
	if (p_remapped != 0) {
		remap = (CRemap*) m_paletteRemap;
	}
	else {
		remap = NULL;
	}

	switch ((unsigned short) p_viewData.m_actionArgument) {
	case EXTERNAL_CONTROL_ELECTROCUTED: {
		unsigned int frameIndex = (int) frame % 4;
		m_lemmingAnims->DrawAnim(x - 15, y - 22, g_lemmingExternalResources[direction], frameIndex, 0, remap);
		break;
	}
	case EXTERNAL_CONTROL_ON_FIRE:
		if ((int) frame <= 14) {
			m_lemmingAnims->DrawAnim(x - 7, y - 28, RES_GAME_ONFIRE, frame, 0, remap);
		}
		break;
	case EXTERNAL_CONTROL_ON_ICE: {
		unsigned int frameIndex = (int) frame % 8;
		m_lemmingAnims->DrawAnim(x - 15, y - 22, RES_GAME_LEMMING_SPIN, frameIndex, 0, remap);
		break;
	}
	}
}

// FUNCTION: LEMBALL 0x0043c1a0
void C2D::DrawLemmingOnConveyor(CViewData& p_viewData, int p_remapped)
{
	int x;
	int y;
	int frame;
	CBaseRemap* remap;

	frame = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	frame = (frame * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND) % LEMMING_CONVEYOR_ANIMATION_FRAME_COUNT;
	if (p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = NULL;
	}
	m_lemmingAnims->DrawAnim(x - 15, y - 22, RES_GAME_LEMMING_SPIN, frame, 0, (CRemap*) remap);
}

// FUNCTION: LEMBALL 0x0043c200
void C2D::DrawLemming(CViewData& p_viewData, int p_objectNo, unsigned int p_remapped)
{
	int x;
	int y;
	int animationFrame;
	unsigned int direction;
	unsigned short playerIndex;
	int drawEquipment;
	int drawBody;
	unsigned long animationResourceId;
	int offsetX;
	int offsetY;

	animationFrame = p_viewData.m_stateTimer;
	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	y = p_viewData.m_positionY;
	drawEquipment = 1;
	drawBody = 1;
	x = p_viewData.m_positionX;
	playerIndex = p_viewData.m_playerIndex;

	switch (p_viewData.m_action) {
	case ACTION_NONE:
	case ACTION_TURNING:
	case ACTION_FINDING_ROUTE:
	case ACTION_PREPARING_SOMMERSAULT:
	case ACTION_LEAVING:
		animationResourceId = g_lemmingStandResources[direction];
		offsetX = g_lemmingStandOffset[0];
		offsetY = g_lemmingStandOffset[1];
		break;
	case ACTION_WALKING:
		animationResourceId = g_lemmingWalkResources[direction];
		offsetX = g_lemmingWalkOffset[0];
		offsetY = g_lemmingWalkOffset[1];
		break;
	case ACTION_FIRING:
		animationResourceId = g_lemmingFireResources[direction];
		offsetY = g_lemmingFireOffsets[direction][1] + g_lemmingStandOffset[1];
		offsetX = g_lemmingFireOffsets[direction][0] + g_lemmingStandOffset[0];
		drawEquipment = 0;
		break;
	case ACTION_FLYING:
		DrawLemmingFlyShadow(p_viewData);
		drawEquipment = 0;
		animationResourceId = LemmingFly(p_viewData, animationFrame);
		offsetX = g_lemmingAirOffset[0];
		offsetY = g_lemmingAirOffset[1];
		break;
	case ACTION_HIDDEN:
		drawBody = 0;
		drawEquipment = 0;
		break;
	case ACTION_IDLE_ANIMATION: {
		unsigned int waitAnimationIndex;
		if (p_remapped == 0) {
			waitAnimationIndex = (unsigned short) p_viewData.m_actionArgument;
		}
		else {
			waitAnimationIndex = 0;
		}
		offsetX = g_lemmingWaitOffsets[waitAnimationIndex][0];
		offsetY = g_lemmingWaitOffsets[waitAnimationIndex][1];
		animationResourceId = g_lemmingWaitResources[waitAnimationIndex];
		break;
	}
	case ACTION_HIT:
		drawEquipment = 0;
		animationResourceId = g_lemmingHitResources[direction];
		offsetX = g_lemmingHitOffsets[direction][0];
		offsetY = g_lemmingHitOffsets[direction][1];
		break;
	case ACTION_DEAD:
	case ACTION_WAITING_TO_SPAWN:
	case ACTION_WAITING_TO_DIE:
		return;
	case ACTION_JUMPING:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingJump(p_viewData, p_remapped);
		break;
	case ACTION_FALLING:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingFall(p_viewData, p_remapped);
		break;
	case ACTION_SOMMERSAULT:
		offsetX = g_lemmingSommersaultOffset[0];
		offsetY = g_lemmingSommersaultOffset[1];
		drawEquipment = 0;
		animationResourceId = p_viewData.m_actionArgument == SOMMERSAULT_DIRECTION_NORMAL ? RES_GAME_SOMMERSAULT
																						  : RES_GAME_SOMMERSAULT_REV;
		break;
	case ACTION_EXTERNAL_CONTROL:
		DrawLemmingExternal(p_viewData, p_remapped);
		return;
	case ACTION_ON_BALLOON:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingOnBalloon(p_viewData, (unsigned short) p_viewData.m_actionArgument, p_remapped);
		break;
	case ACTION_LANDING:
		drawBody = 0;
		drawEquipment = 0;
		DrawLemmingLanding(p_viewData, p_remapped);
		break;
	case ACTION_ON_CONVEYOR:
		DrawLemmingOnConveyor(p_viewData, p_remapped);
		return;
	}
	if (drawEquipment && p_remapped == 0) {
		if (((unsigned short) p_viewData.m_statusFlags & LEMMING_VIEW_STATUS_GROUP_LEADER) == 0) {
			m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] - 1,
									 (short) y - g_lemmingStandOffset[1] + 14,
									 RES_GAME_CIRCLES,
									 0,
									 0,
									 (CRemap*) m_remaps[playerIndex]);
		}
		else if ((unsigned short) p_viewData.m_statusFlags & LEMMING_VIEW_STATUS_IN_GROUP) {
			m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] - 5,
									 (short) y - g_lemmingStandOffset[1] + 11,
									 RES_GAME_FILLED_STARS,
									 0,
									 0,
									 (CRemap*) m_remaps[playerIndex]);
		}
		else {
			m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] - 5,
									 (short) y - g_lemmingStandOffset[1] + 11,
									 RES_GAME_STARS,
									 0,
									 0,
									 (CRemap*) m_remaps[playerIndex]);
		}
	}
	if (drawBody) {
		if (p_remapped == 0) {
			m_lemmingAnims->DrawAnim((short) x - (short) offsetX,
									 (short) y - (short) offsetY,
									 animationResourceId,
									 animationFrame,
									 p_viewData.m_animationTime,
									 NULL);
		}
		else {
			m_lemmingAnims->DrawAnim((short) x - (short) offsetX,
									 (short) y - (short) offsetY,
									 animationResourceId,
									 animationFrame,
									 p_viewData.m_animationTime,
									 (CRemap*) m_paletteRemap);
		}
	}
	if (drawEquipment && p_remapped == 0 && InGroupByObjectNo(p_objectNo)) {
		m_lemmingAnims->DrawAnim((short) x - g_lemmingStandOffset[0] + 2,
								 (short) y - g_lemmingStandOffset[1] - 16,
								 RES_GAME_SPINARROW,
								 0,
								 p_viewData.m_animationTime,
								 NULL);
	}
}

// FUNCTION: LEMBALL 0x0043c610
void C2D::DrawBullet(CViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049ef38
	static unsigned long g_bulletResources[] = {
		RES_GAME_LEMMINGPELLETNE,
		RES_GAME_LEMMINGPELLETE,
		RES_GAME_LEMMINGPELLETSE,
		RES_GAME_LEMMINGPELLETS,
		RES_GAME_LEMMINGPELLETSW,
		RES_GAME_LEMMINGPELLETW,
		RES_GAME_LEMMINGPELLETNW,
		RES_GAME_LEMMINGPELLETN,
	};

	unsigned int direction;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_bulletOffset[0],
							 p_viewData.m_positionY - g_bulletOffset[1],
							 g_bulletResources[direction],
							 0,
							 p_viewData.m_animationTime,
							 NULL);
}

// FUNCTION: LEMBALL 0x0043c660
void C2D::DrawAmmo(CViewData& p_viewData, int p_objectNo)
{
	switch (p_viewData.m_action) {
	case ACTION_READY:
	case ACTION_ACTIVATING:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_ammoOffset[0],
								 p_viewData.m_positionY - g_ammoOffset[1],
								 RES_GAME_YELLOW_AMMO,
								 0,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	case ACTION_ACTIVATED:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_pelletOffset[0],
								 p_viewData.m_positionY - g_pelletOffset[1],
								 RES_GAME_EX_PELLET,
								 p_viewData.m_stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043c6e0
void C2D::DrawRocket(CViewData& p_viewData)
{
	enum {
		ROCKET_EXTRA_FRAME_NONE = -1
	};
	int elapsed;
	int frame;
	int extraFrame;

	elapsed =
		(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
	extraFrame = ROCKET_EXTRA_FRAME_NONE;
	if (elapsed <= 7) {
		frame = elapsed < 4 ? elapsed : 4;
	}
	else if (elapsed >= 8 && elapsed <= 13) {
		frame = elapsed < 11 ? elapsed - 3 : 8;
	}
	else if (elapsed >= 14 && elapsed <= 19) {
		frame = elapsed - 4;
		extraFrame = 9;
	}
	else if (elapsed >= 20 && elapsed <= 31) {
		frame = (elapsed - 20) % 2 + 17;
		extraFrame = 16;
	}
	else if (elapsed >= 32 && elapsed <= 42) {
		frame = elapsed - 11;
		if (frame > 25) {
			extraFrame = 26;
			frame++;
		}
	}
	else {
		frame = (elapsed & ROCKET_ANIMATION_ALTERNATE_FRAME_MASK) + 33;
		extraFrame = 32;
	}

	if (extraFrame != ROCKET_EXTRA_FRAME_NONE) {
		m_lemmingAnims
			->DrawAnim(p_viewData.m_positionX - 13, p_viewData.m_positionY - 73, RES_GAME_ROCKET, extraFrame, 0, NULL);
	}
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - 13, p_viewData.m_positionY - 73, RES_GAME_ROCKET, frame, 0, NULL);
}

// FUNCTION: LEMBALL 0x0043c7f0
void C2D::DrawHand(CViewData& p_viewData)
{
	int drawX;
	int drawY;
	int frame;
	CBaseRemap* remap;
	eAction action = p_viewData.m_action;

	drawX = p_viewData.m_positionX - 0x31;
	drawY = p_viewData.m_positionY - 0x14;
	remap = NULL;
	if (p_viewData.m_actionArgument != REMOTE_PALETTE_REMAP_DISABLED) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case ACTION_RECOVERY:
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(drawX, drawY, g_anGroundStyleResourceIds[2], 0, 0, NULL);
		break;
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		frame =
			(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(drawX, drawY, g_anGroundStyleResourceIds[2], frame, 0, (CRemap*) remap);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043c8a0
void C2D::DrawLemmingOnBalloon(CViewData& p_viewData, int p_balloonType, int p_remapped)
{
	unsigned int phase;
	int x;
	int y;
	int xOffset;
	int yOffset;
	CBaseRemap* remap;
	CBaseRemap* balloonRemap;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	phase = ((p_viewData.m_animationTime - p_viewData.m_stateTimer) & ANIMATION_PHASE_TIME_MASK) >>
			ANIMATION_PHASE_TIME_SHIFT;

	if (p_remapped != 0) {
		remap = m_paletteRemap;
	}
	else {
		remap = NULL;
	}

	if (phase <= 7) {
		xOffset = phase - 4;
	}
	else {
		xOffset = 12 - phase;
	}

	yOffset = phase - 4;
	if (phase > 7) {
		yOffset = 12 - phase;
	}

	DrawLemmingFlyShadow(p_viewData);

	if (p_balloonType < 4) {
		balloonRemap = m_remaps[p_balloonType];
	}
	else {
		balloonRemap = NULL;
	}

	m_lemmingAnims->DrawAnim(x - 16, y - 64, RES_GAME_BALLOON, 0, 0, (CRemap*) balloonRemap);
	m_lemmingAnims->DrawAnim(x - g_lemmingStandOffset[0] - 14,
							 y - g_lemmingStandOffset[1] - 12,
							 RES_GAME_ONBALLOON,
							 0,
							 0,
							 (CRemap*) remap);
}

// FUNCTION: LEMBALL 0x0043c940
void C2D::DrawBalloon(CViewData& p_viewData, int p_playerIndex)
{
	C2D* view = this;
	CBaseRemap* remap;
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	int xOffset;
	int yOffset;
	unsigned int phase = ((p_viewData.m_animationTime - p_viewData.m_stateTimer) & ANIMATION_PHASE_TIME_MASK) >>
						 ANIMATION_PHASE_TIME_SHIFT;

	if (phase <= 7) {
		xOffset = phase - 4;
	}
	else {
		xOffset = 12 - phase;
	}

	yOffset = phase - 4;
	if (phase > 7) {
		yOffset = 12 - phase;
	}

	if (p_playerIndex < 4) {
		remap = view->m_remaps[p_playerIndex];
	}
	else {
		remap = NULL;
	}

	view->m_lemmingAnims->DrawAnim(x + xOffset - 16, y + yOffset / 4 - 64, RES_GAME_BALLOON, 0, 0, (CRemap*) remap);
	view->m_lemmingAnims->DrawAnim(x + xOffset - 9, y + yOffset / 4 - 9, RES_GAME_BALLOON_SHADOW, 0, 0, NULL);
}

// FUNCTION: LEMBALL 0x0043c9f0
void C2D::DrawBalloonPost(CViewData& p_viewData, int p_playerIndex)
{
	int x;
	int y;
	CBaseRemap* remap;

	x = p_viewData.m_positionX - 0x10;
	y = p_viewData.m_positionY - 0x40;
	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = NULL;
	}
	m_lemmingAnims->DrawAnim(x, y, RES_GAME_BALLOON_POST, 0, 0, (CRemap*) remap);
}

// FUNCTION: LEMBALL 0x0043ca30
void C2D::DrawTrampoline(CViewData& p_viewData)
{
	int x;
	int y;
	int frame;

	x = p_viewData.m_positionX - g_trampolineOffset[0];
	y = p_viewData.m_positionY - g_trampolineOffset[1];

	switch (p_viewData.m_action) {
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_TRAMPOLINE, 0, 0, NULL);
		break;

	case ACTION_RUNNING:
		frame =
			(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 11) {
			frame = 11;
		}
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_TRAMPOLINE, frame, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043cac0
void C2D::DrawMover(CViewData& p_viewData)
{
	unsigned short frame;
	int x = p_viewData.m_positionX;
	int y = p_viewData.m_positionY;
	unsigned short state = p_viewData.m_actionArgument;
	switch (m_ai->m_mapType) {
	case GROUND_STYLE_GRASS:
		frame = 0x50;
		break;
	case GROUND_STYLE_LEGO:
		frame = 0x1a;
		break;
	case GROUND_STYLE_SNOW:
		frame = 0x52;
		break;
	case GROUND_STYLE_SPACE:
		frame = 0x38;
		break;
	}
	switch ((unsigned int) state) {
	case MOVER_VISUAL_GROUND:
		m_lemmingAnims
			->DrawAnim(x - g_groundOffset[0], y - g_groundOffset[1] - 12, g_groundBlox4ResourceId, frame, 0, NULL);
		break;
	case MOVER_VISUAL_STAR: {
		unsigned int animFrame =
			(g_dwSimulationTimestamp / SPECIAL_ANIMATION_FRAME_INTERVAL_MS) & SPECIAL_ANIMATION_FRAME_MASK;
		m_lemmingAnims->DrawAnim(x - g_moverOffset[0], y - g_moverOffset[1] - 8, RES_GAME_STAR, animFrame, 0, NULL);
		break;
	}
	}
}

// FUNCTION: LEMBALL 0x0043cbb0
void C2D::DrawSlinky(CViewData& p_viewData)
{
	unsigned int direction = (unsigned short) p_viewData.m_actionArgument;
	int x = p_viewData.m_positionX - g_slinkyOffsets[direction][0];
	int y = p_viewData.m_positionY - g_slinkyOffsets[direction][1];
	int frame;
	switch (p_viewData.m_action) {
	case ACTION_READY:
		switch (direction) {
		case SLINKY_DIRECTION_EAST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_EAST, 0, 0, NULL);
			break;
		case SLINKY_DIRECTION_WEST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_WEST, 0, 0, NULL);
			break;
		case SLINKY_DIRECTION_SOUTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_SOUTH, 0, 0, NULL);
			break;
		case SLINKY_DIRECTION_NORTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_NORTH, 0, 0, NULL);
			break;
		}
		break;
	case ACTION_RUNNING:
		frame = (int) ((p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS) /
				MILLISECONDS_PER_SECOND;
		if (frame > 12) {
			frame = 12;
		}
		switch (direction) {
		case SLINKY_DIRECTION_EAST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_EAST, frame, 0, NULL);
			break;
		case SLINKY_DIRECTION_WEST:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_WEST, frame, 0, NULL);
			break;
		case SLINKY_DIRECTION_SOUTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_SOUTH, frame, 0, NULL);
			break;
		case SLINKY_DIRECTION_NORTH:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SLINKY_NORTH, frame, 0, NULL);
			break;
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0043cd50
void C2D::DrawPaintGun(CViewData& p_viewData)
{
	int x = p_viewData.m_positionX - g_paintGunOffset[0];
	int y = p_viewData.m_positionY - g_paintGunOffset[1];
	int frame;
	if (p_viewData.m_action == ACTION_FIRING || p_viewData.m_action == ACTION_RUNNING) {
		frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) * PAINT_GUN_ANIMATION_FRAME_RATE_FPS /
				MILLISECONDS_PER_SECOND;
		if (frame > 57) {
			frame = 0;
		}
		if (frame < 12) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[1], frame, 0, NULL);
		}
		if (frame > 11 && frame < 47) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[1], 11, 0, NULL);
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_PAINTGUNSHOT, frame - 12, 0, NULL);
		}
		if (frame > 46) {
			m_lemmingAnims->DrawAnim(x, frame + y - 47, g_anGroundStyleResourceIds[1], 11, 0, NULL);
		}
	}
}

// FUNCTION: LEMBALL 0x0043ce30
void C2D::DrawLaserFire(CViewData& p_viewData)
{
	int x;
	int y;

	switch (p_viewData.m_objectType) {
	case OBJECT_LASER_HORIZONTAL_BEAM:
		x = p_viewData.m_positionX - 0xd;
		y = p_viewData.m_positionY - 9;
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_LASER_FIRE_NORTH, 0, 0, NULL);
		break;
	case OBJECT_LASER_VERTICAL_BEAM:
		x = p_viewData.m_positionX - 0x16;
		y = p_viewData.m_positionY - 0xf;
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_LASER_FIRE_EAST, 0, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043cea0
void C2D::DrawLaser(CViewData& p_viewData)
{
	eAction action;
	int x;
	int y;
	unsigned long resourceId;
	int frame;

	action = p_viewData.m_action;

	switch (p_viewData.m_objectType) {
	case OBJECT_LASER_HORIZONTAL:
	case OBJECT_LASER_EMITTER_H:
		resourceId = RES_GAME_LASER_EAST;
		x = p_viewData.m_positionX - 0x14;
		y = p_viewData.m_positionY - 0xa;
		break;
	case OBJECT_LASER_VERTICAL:
	case OBJECT_LASER_EMITTER_V:
		resourceId = RES_GAME_LASER_NORTH;
		x = p_viewData.m_positionX - 0x2e;
		y = p_viewData.m_positionY - 0xa;
		break;
	}

	switch (action) {
	case ACTION_RECOVERY:
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		break;
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		frame =
			(p_viewData.m_animationTime - p_viewData.m_stateTimer) * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 17) {
			frame = 17;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, frame, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043cfa0
void C2D::DrawDuplicator(CViewData& p_viewData)
{
	int x;
	int y;
	CBaseRemap* remap;
	eAction action;
	unsigned int elapsed;
	int frame;

	elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
	action = p_viewData.m_action;
	x = p_viewData.m_positionX - 0x1c;
	y = p_viewData.m_positionY - 0x3f;
	remap = NULL;

	if (p_viewData.m_actionArgument != REMOTE_PALETTE_REMAP_DISABLED) {
		remap = m_paletteRemap;
	}
	m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, 0, 0, (CRemap*) remap);

	switch (action) {
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, 0x3f, 0, (CRemap*) remap);
		break;
	case ACTION_ACTIVATED:
		frame = elapsed * ANIMATION_FRAME_RATE_FPS / MILLISECONDS_PER_SECOND;
		if (frame > 0x3e) {
			frame = 0x3e;
		}
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_DUPLICATOR, frame + 1, 0, (CRemap*) remap);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d070
void C2D::DrawCrate(CViewData& p_viewData, int p_objectNo)
{
	eAction action;
	unsigned int stateTimer;

	action = p_viewData.m_action;
	stateTimer = p_viewData.m_stateTimer;

	switch (action) {
	case ACTION_READY:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_crateOffset[0],
								 p_viewData.m_positionY - g_crateOffset[1],
								 RES_GAME_CRATE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_crateExplosionOffset[0],
								 p_viewData.m_positionY - g_crateExplosionOffset[1],
								 RES_GAME_CRATE_EXPLODE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d0f0
void C2D::DrawTimeBonus(CViewData& p_viewData)
{
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_timeBonusOffset[0],
							 p_viewData.m_positionY - g_timeBonusOffset[1],
							 RES_GAME_TIME_BONUS,
							 p_viewData.m_stateTimer,
							 p_viewData.m_animationTime,
							 NULL);
}

// FUNCTION: LEMBALL 0x0043d130
void C2D::DrawCatapult(CViewData& p_viewData, int p_objectNo)
{
	enum {
		CATAPULT_ACTIVE_ANIMATION_TIME_OFFSET = 0x640
	};
	int x;
	int y;
	CBaseRemap* remap;
	eAction action;
	unsigned int stateTimer;
	C2D& owner = *this;

	action = p_viewData.m_action;
	stateTimer = p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	remap = NULL;

	if (p_viewData.m_actionArgument != REMOTE_PALETTE_REMAP_DISABLED) {
		remap = m_paletteRemap;
	}

	switch (action) {
	case ACTION_READY:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, NULL);
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 0, 0, NULL);
		break;

	case ACTION_ACTIVATING:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, NULL);
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 0, 0, NULL);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0] - 8,
									   y - g_animOffset[1],
									   RES_GAME_CATMOUNT_SE,
									   stateTimer,
									   p_viewData.m_animationTime,
									   (CRemap*) remap);
		break;

	case ACTION_ACTIVATED:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, NULL);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0],
									   y - g_animOffset[1],
									   g_anGroundStyleResourceIds[9],
									   stateTimer + CATAPULT_ACTIVE_ANIMATION_TIME_OFFSET,
									   p_viewData.m_animationTime,
									   NULL);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0] - 8,
									   y - g_animOffset[1],
									   RES_GAME_CATMOUNT_SE,
									   stateTimer,
									   p_viewData.m_animationTime,
									   (CRemap*) remap);
		break;

	case ACTION_RUNNING:
		owner.m_lemmingAnims
			->DrawAnim(x - g_baseOffset[0], y - g_baseOffset[1], g_anGroundStyleResourceIds[8], 1, 0, NULL);
		owner.m_lemmingAnims->DrawAnim(x - g_animOffset[0],
									   y - g_animOffset[1],
									   g_anGroundStyleResourceIds[9],
									   stateTimer + CATAPULT_ACTIVE_ANIMATION_TIME_OFFSET,
									   p_viewData.m_animationTime,
									   NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d370
void C2D::DrawSheep(CViewData& p_viewData, int p_objectNo)
{
	// GLOBAL: LEMBALL 0x0049ef58
	static unsigned long g_sheepWalkResources[] = {
		RES_GAME_SHEEP_WALK_NE,
		RES_GAME_SHEEP_WALK_E,
		RES_GAME_SHEEP_WALK_SE,
		RES_GAME_SHEEP_WALK_S,
		RES_GAME_SHEEP_WALK_SW,
		RES_GAME_SHEEP_WALK_W,
		RES_GAME_SHEEP_WALK_NW,
		RES_GAME_SHEEP_WALK_N,
	};
	// GLOBAL: LEMBALL 0x0049ef78
	static unsigned long g_sheepMunchResources[] = {
		RES_GAME_SHEEP_MUNCH_NE,
		RES_GAME_SHEEP_WALK_E,
		RES_GAME_SHEEP_MUNCH_SE,
		RES_GAME_SHEEP_WALK_S,
		RES_GAME_SHEEP_MUNCH_SW,
		RES_GAME_SHEEP_WALK_W,
		RES_GAME_SHEEP_MUNCH_NW,
		RES_GAME_SHEEP_WALK_N,
	};

	unsigned int direction;
	unsigned int stateTimer;
	int y;
	int x;

	direction = ((unsigned short) p_viewData.m_facingDirection + m_viewOrientation * 2) & FACING_DIRECTION_MASK;
	stateTimer = p_viewData.m_stateTimer;
	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case ACTION_NONE:
	case ACTION_TURNING:
	case ACTION_FLYING:
		m_lemmingAnims->DrawAnim(x - g_sheepOffset[0],
								 y - g_sheepOffset[1],
								 g_sheepMunchResources[direction],
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;

	case ACTION_WALKING:
		m_lemmingAnims->DrawAnim(x - g_sheepOffset[0],
								 y - g_sheepOffset[1],
								 g_sheepWalkResources[direction],
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d420
void C2D::DrawBall(CViewData& p_viewData)
{
	int x;
	int y;
	int elapsed;
	int frame;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;

	switch (p_viewData.m_action) {
	case ACTION_BALL_MOVING:
		m_lemmingAnims
			->DrawAnim(x - g_ballOffset[0], y - g_ballOffset[1], RES_GAME_BALL, 0, p_viewData.m_animationTime, NULL);
		break;
	case ACTION_BALL_EXPLODING:
		elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;
		frame = elapsed / 64;
		if (frame > 8) {
			frame = 8;
		}
		m_lemmingAnims
			->DrawAnim(x - g_explosionOffset[0], y - g_explosionOffset[1], RES_GAME_BALL_EXPLODE, frame, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d4b0
void C2D::DrawKey(CViewData& p_viewData, int p_playerIndex)
{
	CBaseRemap* remap;

	if (p_playerIndex < 4) {
		remap = m_remaps[p_playerIndex];
	}
	else {
		remap = NULL;
	}

	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_keyOffset[0],
							 p_viewData.m_positionY - g_keyOffset[1],
							 RES_GAME_KEYS,
							 0,
							 0,
							 (CRemap*) remap);
}

// FUNCTION: LEMBALL 0x0043d500
void C2D::DrawMine(CViewData& p_viewData)
{
	int x;
	int y;
	unsigned int stateTimer;
	eAction action;

	x = p_viewData.m_positionX;
	y = p_viewData.m_positionY;
	stateTimer = p_viewData.m_stateTimer;
	action = p_viewData.m_action;

	switch (action) {
	case ACTION_DEAD:
		break;
	case ACTION_READY:
	case ACTION_ACTIVATING:
	case ACTION_ACTIVATED:
		m_lemmingAnims->DrawAnim(x - g_mineStillOffset[0], y - g_mineStillOffset[1], RES_GAME_MINE_STILL, 0, 0, NULL);
		break;
	case ACTION_RUNNING:
		m_lemmingAnims->DrawAnim(x - g_mineOffset[0],
								 y - g_mineOffset[1],
								 RES_GAME_MINE,
								 stateTimer,
								 p_viewData.m_animationTime,
								 NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d590
void C2D::DrawDoor(CViewData& p_viewData)
{
	int x;
	int y;
	int elapsed;
	eAction action;
	eObjectType objectType;
	unsigned long resourceId;
	int playerIndex;
	CBaseRemap* remap;
	int frame;

	x = p_viewData.m_positionX - g_doorOffset[0];
	y = p_viewData.m_positionY - g_doorOffset[1];
	action = p_viewData.m_action;
	objectType = p_viewData.m_objectType;
	elapsed = p_viewData.m_animationTime - p_viewData.m_stateTimer;

	switch (objectType) {
	case OBJECT_DOOR_1:
		resourceId = RES_GAME_DOOR_2;
		break;
	case OBJECT_DOOR_2:
		resourceId = RES_GAME_DOOR;
		break;
	}

	switch (action) {
	case ACTION_DOOR_LOCKED_FEEDBACK:
		switch ((unsigned short) p_viewData.m_actionArgument) {
		case OBJECT_SWITCH:
			playerIndex = DOOR_LOCK_NO_PLAYER_REMAP;
			break;
		case OBJECT_KEY_1:
			playerIndex = 3;
			break;
		case OBJECT_KEY_2:
			playerIndex = 1;
			break;
		case OBJECT_KEY_3:
			playerIndex = 4;
			break;
		}

		if (playerIndex >= 0) {
			if (playerIndex < 4) {
				remap = m_remaps[playerIndex];
			}
			else {
				remap = NULL;
			}
			m_lemmingAnims->DrawAnim(x + 16, y - 20, RES_GAME_KEYS, 0, 0, (CRemap*) remap);
		}

		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 1, 0, NULL);
		break;

	case ACTION_DOOR_LOCKED:
	case ACTION_DOOR_CLOSED:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 1, 0, NULL);
		break;

	case (eAction) ACTION_DOOR_OPENING:
		frame = elapsed * 15 / 1024;
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, frame + 1, 0, NULL);
		break;

	case (eAction) ACTION_DOOR_OPEN:
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 8, 0, NULL);
		break;

	case (eAction) ACTION_DOOR_CLOSING:
		frame = elapsed * 15 / 1024;
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, resourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, resourceId, 8 - frame, 0, NULL);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d7e0
void C2D::DrawSwitch(CViewData& p_viewData)
{
	int x;
	int y;
	unsigned int stateTimer;
	unsigned short actionArgument;
	eAction action;

	x = p_viewData.m_positionX - g_switchOffset[0];
	y = p_viewData.m_positionY - g_switchOffset[1];
	stateTimer = p_viewData.m_stateTimer;
	action = p_viewData.m_action;
	actionArgument = (unsigned short) p_viewData.m_actionArgument;

	switch (action) {
	case ACTION_HIT:
	case ACTION_READY:
		switch (actionArgument) {
		case SWITCH_STATE_INACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH, 0, 0, NULL);
			break;
		case SWITCH_STATE_ACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH, 0, 0, NULL);
			break;
		}
		break;

	case ACTION_ACTIVATED:
		switch (actionArgument) {
		case SWITCH_STATE_INACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH_ANIM, stateTimer, p_viewData.m_animationTime, NULL);
			break;
		case SWITCH_STATE_ACTIVE:
			m_lemmingAnims->DrawAnim(x, y, RES_GAME_SWITCH_ANIM, stateTimer, p_viewData.m_animationTime, NULL);
			break;
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d8d0
void C2D::DrawFlag(CViewData& p_viewData, eObjectType p_objectType)
{
	int x;
	int y;

	x = p_viewData.m_positionX - g_flagOffset[0];
	y = p_viewData.m_positionY - g_flagOffset[1];

	switch (p_objectType) {
	case OBJECT_FLAG_1:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_FLAG_GREEN, 0, p_viewData.m_animationTime, (CRemap*) m_remaps[3]);
		break;

	case OBJECT_FLAG_2:
		m_lemmingAnims->DrawAnim(x, y, RES_GAME_FLAG_GREEN, 0, p_viewData.m_animationTime, (CRemap*) m_remaps[1]);
		break;
	}
}

// FUNCTION: LEMBALL 0x0043d950
void C2D::DrawBonus(CViewData& p_viewData)
{
	m_lemmingAnims->DrawAnim(p_viewData.m_positionX - g_bonusOffset[0],
							 p_viewData.m_positionY - g_bonusOffset[1],
							 RES_GAME_BONUS,
							 0,
							 p_viewData.m_animationTime,
							 NULL);
}

// FUNCTION: LEMBALL 0x0043d990
void C2D::DrawTrapDoor(CViewData& p_viewData)
{
	int frame;
	int x = p_viewData.m_positionX - g_trapDoorOffset[0];
	int y = p_viewData.m_positionY - g_trapDoorOffset[1];
	int shadowX = x + 16;
	int shadowY = y + 78;
	frame = (p_viewData.m_animationTime - p_viewData.m_stateTimer) / 66;
	switch (p_viewData.m_action) {
	case ACTION_ARRIVING:
		if (frame > 40) {
			frame = 40;
		}
		if (frame < 33) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], frame, 0, NULL);
		}
		else {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 33, 0, NULL);
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], frame + 1, 0, NULL);
		}
		if (frame > 23) {
			if (frame > 33) {
				frame = 33;
			}
			m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, frame - 23, 0, NULL);
		}
		break;
	case ACTION_OPENING:
		if (frame > 14) {
			frame = 14;
		}
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, frame + 1, 0, NULL);
		m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		break;
	case ACTION_OPEN:
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 15, 0, NULL);
		m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		break;
	case ACTION_CLOSING:
		if (frame > 7) {
			frame = 7;
		}
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 0, 0, NULL);
		m_lemmingAnims->DrawAnim(x, y, g_dwGroundStyleResourceId, 8 - frame, 0, NULL);
		m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		break;
	case ACTION_LEAVING:
		if (frame > 40) {
			frame = 40;
		}
		if (40 - frame < 33) {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 40 - frame, 0, NULL);
		}
		else {
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 33, 0, NULL);
			m_lemmingAnims->DrawAnim(x, y, g_anGroundStyleResourceIds[7], 41 - frame, 0, NULL);
		}
		if (frame < 13) {
			m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 10, 0, NULL);
		}
		else if (frame < 23) {
			m_lemmingAnims->DrawAnim(shadowX, shadowY, RES_GAME_SHADOW, 23 - frame, 0, NULL);
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0043dc70
void C2D::DrawObject(CViewData& p_viewData)
{
	int objectNo = p_viewData.m_objectId;
	switch (p_viewData.m_objectType) {
	case OBJECT_PLAYER_1:
		DrawLemming(p_viewData, objectNo, 1);
		return;
	case OBJECT_PLAYER_2:
		DrawLemming(p_viewData, objectNo, 0);
		return;
	case OBJECT_BULLET:
		DrawBullet(p_viewData, objectNo);
		return;
	case OBJECT_CATAPULT:
		DrawCatapult(p_viewData, objectNo);
		return;
	case OBJECT_AMMO:
		DrawAmmo(p_viewData, objectNo);
		return;
	case OBJECT_SHEEP:
		DrawSheep(p_viewData, objectNo);
		return;
	case OBJECT_BALL:
		DrawBall(p_viewData);
		return;
	case OBJECT_FLAG_1:
		DrawFlag(p_viewData, OBJECT_FLAG_1);
		return;
	case OBJECT_FLAG_2:
		DrawFlag(p_viewData, OBJECT_FLAG_2);
		return;
	case OBJECT_TOWER:
		if (m_ai->m_mapType != GROUND_STYLE_SPACE) {
			m_lemmingAnims->DrawAnim((short) p_viewData.m_positionX - (short) g_styleObjectClip[0],
									 (short) p_viewData.m_positionY - (short) g_styleObjectClip[1],
									 g_anGroundStyleResourceIds[6],
									 0,
									 0,
									 NULL);
		}
		return;
	case OBJECT_CRATE:
		DrawCrate(p_viewData, objectNo);
		return;
	case OBJECT_BONUS:
		DrawBonus(p_viewData);
		return;
	case OBJECT_MINE:
		DrawMine(p_viewData);
		return;
	case OBJECT_SWITCH:
		DrawSwitch(p_viewData);
		return;
	case OBJECT_KEY_1:
		DrawKey(p_viewData, 3);
		return;
	case OBJECT_KEY_2:
		DrawKey(p_viewData, 1);
		return;
	case OBJECT_KEY_3:
		DrawKey(p_viewData, 4);
		return;
	case OBJECT_TRAP_DOOR:
		DrawTrapDoor(p_viewData);
		return;
	case OBJECT_DOOR_1:
	case OBJECT_DOOR_2:
		DrawDoor(p_viewData);
		return;
	case OBJECT_TIME_BONUS:
		DrawTimeBonus(p_viewData);
		return;
	case OBJECT_DUPLICATOR:
		DrawDuplicator(p_viewData);
		return;
	case OBJECT_LASER_HORIZONTAL:
	case OBJECT_LASER_VERTICAL:
	case OBJECT_LASER_EMITTER_H:
	case OBJECT_LASER_EMITTER_V:
		DrawLaser(p_viewData);
		return;
	case OBJECT_HAND:
		DrawHand(p_viewData);
		return;
	case OBJECT_ROCKET:
		DrawRocket(p_viewData);
		return;
	case OBJECT_PAINT_GUN:
		DrawPaintGun(p_viewData);
		return;
	case OBJECT_TRAMPOLINE:
		DrawTrampoline(p_viewData);
		return;
	case OBJECT_LASER_HORIZONTAL_BEAM:
	case OBJECT_LASER_VERTICAL_BEAM:
		DrawLaserFire(p_viewData);
		return;
	case OBJECT_BALLOON_0:
		DrawBalloon(p_viewData, 3);
		return;
	case OBJECT_BALLOON_1:
		DrawBalloonPost(p_viewData, 3);
		return;
	case OBJECT_BALLOON_2:
		DrawBalloon(p_viewData, 1);
		return;
	case OBJECT_BALLOON_3:
		DrawBalloonPost(p_viewData, 1);
		return;
	case OBJECT_BALLOON_4:
		DrawBalloon(p_viewData, 4);
		return;
	case OBJECT_BALLOON_5:
		DrawBalloonPost(p_viewData, 4);
		return;
	case OBJECT_BALLOON_6:
		DrawBalloon(p_viewData, 0);
		return;
	case OBJECT_BALLOON_7:
		DrawBalloonPost(p_viewData, 0);
		return;
	case OBJECT_MOVER:
		DrawMover(p_viewData);
		return;
	case OBJECT_SLINKY:
		DrawSlinky(p_viewData);
		return;
	}
}
