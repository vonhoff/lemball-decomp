#include "CLemmingAnimsManager.h"

#include "Gameplay/Simulation/CAI.h"
#include "GameView/CCDLoadAnim.h"
#include "../../Engine/Animation/CPlayThruAnim.h"
#include "../../Engine/Animation/CRepeatAnim.h"
#include "../../Engine/Animation/CStaticAnim.h"
#include "Engine/Resources/Archive/CMogRes.h"
#include "Engine/Resources/Types/CResFONT.h"
#include "../../Engine/Resources/Manifest.h"
#include "../Display/CMain2DDisplay.h"
#include "../Sound/CSoundView.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Engine/Animation/CAnimsManager.h"
#include "Engine/Animation/CFrames.h"
#include "Engine/Animation/CTimedAnim.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Resources/ResourceLimits.h"

#include <stddef.h>

class CLoadUpdate;

enum {
	LEMMING_WALK_STAND_CYCLE_DURATION_MS = 1000,
	LEMMING_LOOK_CYCLE_DURATION_MS = 1933,
	LEMMING_TOSS_CYCLE_DURATION_MS = 1600,
	LEMMING_JIG_CYCLE_DURATION_MS = 2700,
	LEMMING_FIRE_CYCLE_DURATION_MS = 2000,
	LEMMING_PROJECTILE_CYCLE_DURATION_MS = 500,
	LEMMING_HIT_CYCLE_DURATION_MS = 3000,
	CATAPULT_CYCLE_DURATION_MS = 3133,
	CATAPULT_MOUNT_CYCLE_DURATION_MS = 2333,
	PICKUP_CYCLE_DURATION_MS = 1000,
	FLAG_BONUS_CYCLE_DURATION_MS = 1500,
	EXTRA_PELLET_CYCLE_DURATION_MS = 400,
	MINE_SWITCH_CYCLE_DURATION_MS = 900,
	CRATE_EXPLOSION_CYCLE_DURATION_MS = 1500,
	SHEEP_WALK_CYCLE_DURATION_MS = 1400,
	SHEEP_MUNCH_CYCLE_DURATION_MS = 2400,
	SPIN_ARROW_CYCLE_DURATION_MS = 560
};

// GLOBAL: LEMBALL 0x00496f78
const unsigned int g_style0ObjectClip[4] = {31, 90, 64, 96};
// GLOBAL: LEMBALL 0x00496f88
const unsigned int g_style1ObjectClip[4] = {34, 96, 68, 96};
// GLOBAL: LEMBALL 0x00496f98
const unsigned int g_style2ObjectClip[4] = {34, 96, 68, 96};
// GLOBAL: LEMBALL 0x0049e8b4
const unsigned int* g_styleObjectClip = NULL;

// GLOBAL: LEMBALL 0x004a7850
unsigned int g_groundBlox1ResourceId;
// GLOBAL: LEMBALL 0x004a7854
unsigned int g_groundBlox2ResourceId;
// GLOBAL: LEMBALL 0x004a7858
unsigned int g_groundBlox3ResourceId;
// GLOBAL: LEMBALL 0x004a785c
unsigned int g_groundBlox4ResourceId;
// GLOBAL: LEMBALL 0x004a7860
unsigned int g_groundBlox5ResourceId;
// GLOBAL: LEMBALL 0x004a7864
unsigned int g_groundBlox6ResourceId;
// GLOBAL: LEMBALL 0x004a7868
unsigned int g_groundBlox7ResourceId;

// GLOBAL: LEMBALL 0x004a784c
unsigned int g_dwGroundStyleResourceId;

// GLOBAL: LEMBALL 0x004a786c
unsigned int g_anGroundStyleResourceIds[10];

// FUNCTION: LEMBALL 0x00432b50
CLemmingAnimsManager::CLemmingAnimsManager(CGDI* p_gdi, CMain2DDisplay* p_display, CAI* p_ai)
	: CAnimsManager(p_gdi, RESOURCE_ID_COUNT, 0xc8, 0x28, 0x14, 1)
{
	m_display = p_display;
	m_gdi = p_gdi;
	m_ai = p_ai;
	m_animFrames = (CAnimFrameBASE**) operator new(RESOURCE_ID_COUNT * sizeof(*m_animFrames));
	m_drawFlags = 0;
	for (int i = 0; i < RESOURCE_ID_COUNT; i++) {
		m_animFrames[i] = NULL;
	}
	m_loaded = 0;
	m_loadAnim = new CCDLoadAnim(m_gdi, m_display);
	m_drawOffsetX = 0;
	m_drawOffsetY = 0;
}

// FUNCTION: LEMBALL 0x00432c20
CLemmingAnimsManager::~CLemmingAnimsManager()
{
	if (m_loadAnim != NULL) {
		delete m_loadAnim;
		m_loadAnim = NULL;
	}

	Unload();
	if (m_animFrames != NULL) {
		operator delete(m_animFrames);
	}
}

// FUNCTION: LEMBALL 0x00432c80
void CLemmingAnimsManager::SetupStyleSensitive()
{
	g_groundBlox1ResourceId = 0;
	g_groundBlox2ResourceId = 0;
	g_groundBlox3ResourceId = 0;
	g_groundBlox4ResourceId = 0;
	g_groundBlox5ResourceId = 0;
	g_groundBlox6ResourceId = 0;
	g_groundBlox7ResourceId = 0;
	g_anGroundStyleResourceIds[0] = 0;
	g_anGroundStyleResourceIds[4] = 0;
	g_anGroundStyleResourceIds[5] = 0;
	g_anGroundStyleResourceIds[3] = 0;
	g_anGroundStyleResourceIds[7] = 0;
	g_dwGroundStyleResourceId = 0;
	g_anGroundStyleResourceIds[2] = 0;
	g_anGroundStyleResourceIds[8] = 0;
	g_anGroundStyleResourceIds[9] = 0;
	g_anGroundStyleResourceIds[1] = 0;
	g_anGroundStyleResourceIds[6] = 0;
	switch (m_groundStyle) {
	case GROUND_STYLE_GRASS:
		g_groundBlox1ResourceId = RES_GAME_BLOX_1;
		g_groundBlox2ResourceId = RES_GAME_BLOX_2;
		g_groundBlox3ResourceId = RES_GAME_BLOX_3;
		g_groundBlox4ResourceId = RES_GAME_BLOX_4;
		g_groundBlox5ResourceId = RES_GAME_BLOX_5;
		g_groundBlox6ResourceId = RES_GAME_BLOX_6;
		g_groundBlox7ResourceId = RES_GAME_BLOX_7;
		g_anGroundStyleResourceIds[0] = RES_GAME_BLOX_8;
		g_anGroundStyleResourceIds[4] = RES_GAME_BLOX_14;
		g_anGroundStyleResourceIds[5] = RES_GAME_BLOX_15;
		g_anGroundStyleResourceIds[3] = RES_GRASS_TREE;
		g_anGroundStyleResourceIds[7] = RES_GAME_SPARKLE;
		g_dwGroundStyleResourceId = RES_GAME_TRAP_DOOR;
		g_anGroundStyleResourceIds[2] = RES_GAME_SPACE_HAND;
		g_anGroundStyleResourceIds[8] = RES_GAME_CATAPULT_SE;
		g_anGroundStyleResourceIds[9] = RES_GAME_CATAPULT_ANIMSE;
		g_anGroundStyleResourceIds[1] = RES_GAME_GRASSPAINTGUN;
		g_anGroundStyleResourceIds[6] = RES_GRASS_TOWER;
		g_styleObjectClip = g_style0ObjectClip;
		break;
	case GROUND_STYLE_LEGO:
		g_groundBlox1ResourceId = RES_GAME_LEGO_1;
		g_groundBlox2ResourceId = RES_GAME_LEGO_2;
		g_groundBlox3ResourceId = RES_GAME_LEGO_3;
		g_groundBlox4ResourceId = RES_GAME_LEGO_4;
		g_groundBlox5ResourceId = RES_GAME_LEGO_5;
		g_groundBlox6ResourceId = RES_GAME_LEGO_6;
		g_groundBlox7ResourceId = RES_GAME_LEGO_7;
		g_anGroundStyleResourceIds[0] = RES_GAME_LEGO_8;
		g_anGroundStyleResourceIds[4] = RES_GAME_LEGO_14;
		g_anGroundStyleResourceIds[5] = RES_GAME_LEGO_15;
		g_anGroundStyleResourceIds[3] = RES_LEGO_LEGOTREE;
		g_anGroundStyleResourceIds[7] = RES_GAME_LEGO_SPARKLE;
		g_dwGroundStyleResourceId = RES_GAME_LEGO_TRAP_DOOR;
		g_anGroundStyleResourceIds[2] = RES_GAME_SPACE_HAND;
		g_anGroundStyleResourceIds[8] = RES_GAME_LEGO_CATAPULT_SE;
		g_anGroundStyleResourceIds[9] = RES_GAME_LEGO_CATAPULT_ANIMSE;
		g_anGroundStyleResourceIds[1] = RES_GAME_SPACEPAINTGUN;
		g_anGroundStyleResourceIds[6] = RES_LEGO_HUT;
		g_styleObjectClip = g_style1ObjectClip;
		break;
	case GROUND_STYLE_SNOW:
		g_groundBlox1ResourceId = RES_GAME_SNOW_1;
		g_groundBlox2ResourceId = RES_GAME_SNOW_2;
		g_groundBlox3ResourceId = RES_GAME_SNOW_3;
		g_groundBlox4ResourceId = RES_GAME_SNOW_4;
		g_groundBlox5ResourceId = RES_GAME_SNOW_5;
		g_groundBlox6ResourceId = RES_GAME_SNOW_6;
		g_groundBlox7ResourceId = RES_GAME_SNOW_7;
		g_anGroundStyleResourceIds[0] = RES_GAME_SNOW_8;
		g_anGroundStyleResourceIds[4] = RES_GAME_SNOW_14;
		g_anGroundStyleResourceIds[5] = RES_GAME_SNOW_15;
		g_anGroundStyleResourceIds[3] = RES_SNOW_SNOWTREE;
		g_anGroundStyleResourceIds[7] = RES_GAME_SNOW_SPARKLE;
		g_dwGroundStyleResourceId = RES_GAME_SNOW_TRAP_DOOR;
		g_anGroundStyleResourceIds[2] = RES_GAME_SNOW_HAND;
		g_anGroundStyleResourceIds[8] = RES_GAME_SNOW_CATAPULT_SE;
		g_anGroundStyleResourceIds[9] = RES_GAME_SNOW_CATAPULT_ANIMSE;
		g_anGroundStyleResourceIds[1] = RES_GAME_SNOWPAINTGUN;
		g_anGroundStyleResourceIds[6] = RES_SNOW_HUT;
		g_styleObjectClip = g_style2ObjectClip;
		break;
	case GROUND_STYLE_SPACE:
		g_groundBlox1ResourceId = RES_GAME_SPACE_1;
		g_groundBlox2ResourceId = RES_GAME_SPACE_2;
		g_groundBlox3ResourceId = RES_GAME_SPACE_3;
		g_groundBlox4ResourceId = RES_GAME_SPACE_4;
		g_groundBlox5ResourceId = RES_GAME_SPACE_5;
		g_groundBlox6ResourceId = RES_GAME_SPACE_6;
		g_groundBlox7ResourceId = RES_GAME_SPACE_7;
		g_anGroundStyleResourceIds[0] = RES_GAME_SPACE_8;
		g_anGroundStyleResourceIds[4] = RES_GAME_SPACE_14;
		g_anGroundStyleResourceIds[5] = RES_GAME_SPACE_15;
		g_anGroundStyleResourceIds[7] = RES_GAME_SPACE_SPARKLE;
		g_dwGroundStyleResourceId = RES_GAME_SPACE_TRAP_DOOR;
		g_anGroundStyleResourceIds[2] = RES_GAME_SPACE_HAND;
		g_anGroundStyleResourceIds[8] = RES_GAME_CATAPULT_SE;
		g_anGroundStyleResourceIds[9] = RES_GAME_CATAPULT_ANIMSE;
		g_anGroundStyleResourceIds[1] = RES_GAME_SPACEPAINTGUN;
		break;
	}
}

// FUNCTION: LEMBALL 0x00432fe0
void CLemmingAnimsManager::LoadVrammed()
{
	if (g_groundBlox1ResourceId != 0) {
		LoadAnimation(g_groundBlox1ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_groundBlox2ResourceId != 0) {
		LoadAnimation(g_groundBlox2ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_groundBlox3ResourceId != 0) {
		LoadAnimation(g_groundBlox3ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_groundBlox4ResourceId != 0) {
		LoadAnimation(g_groundBlox4ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_groundBlox5ResourceId != 0) {
		LoadAnimation(g_groundBlox5ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_groundBlox6ResourceId != 0) {
		LoadAnimation(g_groundBlox6ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_groundBlox7ResourceId != 0) {
		LoadAnimation(g_groundBlox7ResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_anGroundStyleResourceIds[0] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[0], ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_anGroundStyleResourceIds[4] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[4], ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_anGroundStyleResourceIds[5] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[5], ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_anGroundStyleResourceIds[3] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[3], ANIM_FRAME_SINGLE_FRAME);
	}
	LoadAnimation(RES_GAME_CONVEYOR, ANIM_FRAME_SINGLE_FRAME);
	if (m_ai->GetObjectRequired(OBJECT_CATAPULT)) {
		LoadAnimation(g_anGroundStyleResourceIds[8], ANIM_FRAME_STATIC);
	}
	LoadAnimation(RES_CURSORS_HAND, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_LEMMINGSELECTED, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_LEMMINGLEADER, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_LEMMINGWALKN, RES_GAME_LEMMINGWALKNW, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_LEMMINGSTANDN, RES_GAME_LEMMINGSTANDNW, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_LEMMINGFIREN, RES_GAME_LEMMINGFIRENW, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_LEMMINGPELLETN, RES_GAME_LEMMINGPELLETNW, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_STARS, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_FILLED_STARS, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_CIRCLES, ANIM_FRAME_STATIC);
	if (m_ai->GetObjectRequired(OBJECT_BALLOON_0) || m_ai->GetObjectRequired(OBJECT_BALLOON_2) ||
		m_ai->GetObjectRequired(OBJECT_BALLOON_4) || m_ai->GetObjectRequired(OBJECT_BALLOON_6)) {
		LoadAnimation(RES_GAME_BALLOON, ANIM_FRAME_STATIC);
		LoadAnimation(RES_GAME_BALLOON_POST, ANIM_FRAME_STATIC);
	}
	LoadAnimation(RES_GAME_BALLOON_SHADOW, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_JUMP_NE, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_JUMP_NW, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_JUMP_SE, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_JUMP_SW, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_LEMMING_SPIN, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_YELLOW_AMMO, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_EX_PELLET, ANIM_FRAME_PLAY_THROUGH);
	LoadAnimation(RES_GAME_SPINARROW, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_ONBALLOON, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_ONFIRE, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_FLAG_GREEN, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_BONUS, ANIM_FRAME_REPEAT);
	if (m_ai->GetObjectRequired(OBJECT_SWITCH)) {
		LoadAnimation(RES_GAME_SWITCH, ANIM_FRAME_STATIC);
	}
	if (m_ai->GetObjectRequired(OBJECT_SHEEP)) {
		LoadAnimation(RES_GAME_SHEEP_WALK_N, RES_GAME_SHEEP_WALK_NW, ANIM_FRAME_REPEAT);
		LoadAnimation(RES_GAME_SHEEP_MUNCH_NE, RES_GAME_SHEEP_MUNCH_NW, ANIM_FRAME_REPEAT);
	}
	if (m_ai->GetObjectRequired(OBJECT_CRATE)) {
		LoadAnimation(RES_GAME_CRATE, ANIM_FRAME_STATIC);
	}
	LoadAnimation(RES_GAME_FLAME, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_ELECTRIC, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_EMBERS, ANIM_FRAME_SINGLE_FRAME);
	if (m_groundStyle != GROUND_STYLE_SPACE && m_ai->GetObjectRequired(OBJECT_TOWER)) {
		LoadAnimation(g_anGroundStyleResourceIds[6], ANIM_FRAME_STATIC);
	}
	if (m_ai->GetObjectRequired(OBJECT_KEY_1) || m_ai->GetObjectRequired(OBJECT_KEY_2) ||
		m_ai->GetObjectRequired(OBJECT_KEY_3)) {
		LoadAnimation(RES_GAME_KEYS, ANIM_FRAME_STATIC);
	}
	LoadAnimation(RES_GAME_ANIM, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_MINE_STILL, ANIM_FRAME_STATIC);
	LoadAnimation(RES_GAME_BUTAMMO, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_BUTLEMMING, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_BUTBALLOON, ANIM_FRAME_SINGLE_FRAME);
	LoadAnimation(RES_GAME_BUTPAWS, ANIM_FRAME_SINGLE_FRAME);
	if (m_countingLoads == 0) {
		CResFONT** fontResources = m_interfaceFonts;
		fontResources[0] = NULL;
		fontResources[1] = NULL;
		fontResources[2] = NULL;
		LoadAnims(RES_BORDERS_LORES_BORDERCORNERS);
		LoadAnims(RES_BORDERS_LORES_BORDEREDGES);
		fontResources[0] = CResFONT::Load(RES_BORDERS_LORES_CUTFONT);
		LoadAnims(RES_BORDERS_HIRES_BORDERCORNERS);
		LoadAnims(RES_BORDERS_HIRES_BORDEREDGES);
		m_interfaceFonts[1] = CResFONT::Load(RES_BORDERS_HIRES_CUTFONT);
		m_interfaceFonts[2] = CResFONT::Load(RES_NEWFRONT_FONTS_GAME_SCORETIME);
	}
	LoadAnimation(RES_GAME_HIT_NORTH, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_HIT_NORTH_EAST, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_HIT_EAST, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_HIT_SOUTH_EAST, ANIM_FRAME_REPEAT);
	if (m_ai->GetObjectRequired(OBJECT_LASER_VERTICAL) || m_ai->GetObjectRequired(OBJECT_LASER_HORIZONTAL)) {
		LoadAnimation(RES_GAME_LEM_LASER_N, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_LEM_LASER_E, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_LEM_LASER_S, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_LEM_LASER_W, ANIM_FRAME_SINGLE_FRAME);
	}
}

// FUNCTION: LEMBALL 0x004334f0
void CLemmingAnimsManager::LoadMainRammed()
{
	LoadAnimation(RES_GAME_STAR, ANIM_FRAME_SINGLE_FRAME);
	if (g_anGroundStyleResourceIds[7] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[7], ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_dwGroundStyleResourceId != 0) {
		LoadAnimation(g_dwGroundStyleResourceId, ANIM_FRAME_SINGLE_FRAME);
	}
	if (g_anGroundStyleResourceIds[2] != 0 && m_ai->GetObjectRequired(OBJECT_HAND)) {
		LoadAnimation(g_anGroundStyleResourceIds[2], ANIM_FRAME_SINGLE_FRAME);
	}
	LoadAnimation(RES_GAME_MINE, ANIM_FRAME_PLAY_THROUGH);
	if (m_ai->GetObjectRequired(OBJECT_CRATE)) {
		LoadAnimation(RES_GAME_CRATE_EXPLODE, ANIM_FRAME_PLAY_THROUGH);
	}
	if (m_ai->GetObjectRequired(OBJECT_SWITCH)) {
		LoadAnimation(RES_GAME_SWITCH_ANIM, ANIM_FRAME_PLAY_THROUGH);
	}
	if (m_ai->GetObjectRequired(OBJECT_CATAPULT)) {
		LoadAnimation(g_anGroundStyleResourceIds[9], ANIM_FRAME_PLAY_THROUGH);
		LoadAnimation(RES_GAME_CATMOUNT_SE, ANIM_FRAME_PLAY_THROUGH);
	}
	LoadAnimation(RES_GAME_HIT_SOUTH, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_HIT_SOUTH_WEST, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_HIT_WEST, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_HIT_NORTH_WEST, ANIM_FRAME_REPEAT);
	if (m_ai->GetObjectRequired(OBJECT_ROCKET)) {
		LoadAnimation(RES_GAME_ROCKET, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_DUPLICATOR)) {
		LoadAnimation(RES_GAME_DUPLICATOR, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_LASER_HORIZONTAL)) {
		LoadAnimation(RES_GAME_LASER_EAST, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_LASER_FIRE_EAST, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_LASER_VERTICAL)) {
		LoadAnimation(RES_GAME_LASER_NORTH, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_LASER_FIRE_NORTH, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_TRAMPOLINE)) {
		LoadAnimation(RES_GAME_TRAMPOLINE, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_PAINT_GUN)) {
		LoadAnimation(g_anGroundStyleResourceIds[1], ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_PAINTGUNSHOT, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_SLINKY)) {
		LoadAnimation(RES_GAME_SLINKY_SOUTH, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_SLINKY_NORTH, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_SLINKY_EAST, ANIM_FRAME_SINGLE_FRAME);
		LoadAnimation(RES_GAME_SLINKY_WEST, ANIM_FRAME_SINGLE_FRAME);
	}
	LoadAnimation(RES_GAME_WAIT_LOOK, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_WAIT_TOSS, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_WAIT_JIG, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_SOMMERSAULT, ANIM_FRAME_REPEAT);
	LoadAnimation(RES_GAME_SOMMERSAULT_REV, ANIM_FRAME_REPEAT);
	if (m_ai->GetObjectRequired(OBJECT_DOOR_2)) {
		LoadAnimation(RES_GAME_DOOR, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_DOOR_1)) {
		LoadAnimation(RES_GAME_DOOR_2, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_BALL)) {
		LoadAnimation(RES_GAME_BALL, ANIM_FRAME_REPEAT);
		LoadAnimation(RES_GAME_BALL_EXPLODE, ANIM_FRAME_SINGLE_FRAME);
	}
	if (m_ai->GetObjectRequired(OBJECT_TIME_BONUS)) {
		LoadAnimation(RES_GAME_TIME_BONUS, ANIM_FRAME_REPEAT);
	}
	LoadAnimation(RES_GAME_SHADOW, ANIM_FRAME_SINGLE_FRAME);
}

// FUNCTION: LEMBALL 0x004337f0
void CLemmingAnimsManager::Load(int p_groundStyle)
{
	m_groundStyle = p_groundStyle;
	if (m_loadAnim != NULL) {
		m_loadAnim->InitialiseScreen();
	}
	m_nonCacheState = 0;
	m_loadProgress = g_pSoundView->GetnEffects(SOUND_STATE_GAMEPLAY);
	m_countingLoads = 1;
	LoadVrammed();
	LoadMainRammed();
	m_countingLoads = 0;
	g_pSoundView->ChangeState(SOUND_STATE_GAMEPLAY, (CLoadUpdate*) this);
	SetupStyleSensitive();
	LoadVrammed();
	LoadMainRammed();
	if (m_loadAnim != NULL) {
		delete m_loadAnim;
		m_loadAnim = NULL;
	}
	m_display->AttachPalette(RES_GAME_GAMEPALETTE);
	m_loaded = 1;
}

// FUNCTION: LEMBALL 0x004338b0
void CLemmingAnimsManager::Unload()
{
	if (m_loaded != 0) {
		UnLoadAnims(RES_BORDERS_LORES_BORDEREDGES);
		UnLoadAnims(RES_BORDERS_LORES_BORDERCORNERS);
		UnLoadAnims(RES_BORDERS_HIRES_BORDEREDGES);
		UnLoadAnims(RES_BORDERS_HIRES_BORDERCORNERS);
		for (int fontIndex = 0; fontIndex < 3; fontIndex++) {
			CResFONT* font = m_interfaceFonts[fontIndex];
			if (font != NULL) {
				font->UnLoad();
			}
		}
		UnLoadAnimation(RES_CURSORS_HAND);
		UnLoadAnimation(RES_GAME_LEMMINGWALKN, RES_GAME_LEMMINGWALKNW);
		UnLoadAnimation(RES_GAME_LEMMINGSTANDN, RES_GAME_LEMMINGSTANDNW);
		UnLoadAnimation(RES_GAME_LEMMINGFIREN, RES_GAME_LEMMINGFIRENW);
		UnLoadAnimation(RES_GAME_LEMMINGPELLETN, RES_GAME_LEMMINGPELLETNW);
		if (m_ai->GetObjectRequired(OBJECT_LASER_VERTICAL) || m_ai->GetObjectRequired(OBJECT_LASER_HORIZONTAL)) {
			UnLoadAnimation(RES_GAME_LEM_LASER_N);
			UnLoadAnimation(RES_GAME_LEM_LASER_E);
			UnLoadAnimation(RES_GAME_LEM_LASER_S);
			UnLoadAnimation(RES_GAME_LEM_LASER_W);
		}
		UnLoadAnimation(RES_GAME_STARS);
		UnLoadAnimation(RES_GAME_FILLED_STARS);
		UnLoadAnimation(RES_GAME_CIRCLES);
		UnLoadAnimation(RES_GAME_SOMMERSAULT);
		UnLoadAnimation(RES_GAME_SOMMERSAULT_REV);
		if (m_ai->GetObjectRequired(OBJECT_SHEEP)) {
			UnLoadAnimation(RES_GAME_SHEEP_WALK_N, RES_GAME_SHEEP_WALK_NW);
			UnLoadAnimation(RES_GAME_SHEEP_MUNCH_NE, RES_GAME_SHEEP_MUNCH_NW);
		}
		UnLoadAnimation(RES_GAME_WAIT_LOOK);
		UnLoadAnimation(RES_GAME_WAIT_TOSS);
		UnLoadAnimation(RES_GAME_WAIT_JIG);
		UnLoadAnimation(RES_GAME_LEMMINGLEADER);
		UnLoadAnimation(RES_GAME_LEMMINGSELECTED);
		if (m_ai->GetObjectRequired(OBJECT_CATAPULT)) {
			UnLoadAnimation(g_anGroundStyleResourceIds[8]);
			UnLoadAnimation(g_anGroundStyleResourceIds[9]);
			UnLoadAnimation(RES_GAME_CATMOUNT_SE);
		}
		UnLoadAnimation(RES_GAME_YELLOW_AMMO);
		UnLoadAnimation(RES_GAME_EX_PELLET);
		UnLoadAnimation(RES_GAME_SPINARROW);
		UnLoadAnimation(RES_GAME_ONBALLOON);
		UnLoadAnimation(RES_GAME_ONFIRE);
		UnLoadAnimation(RES_GAME_JUMP_NW);
		UnLoadAnimation(RES_GAME_JUMP_NE);
		UnLoadAnimation(RES_GAME_JUMP_SE);
		UnLoadAnimation(RES_GAME_JUMP_SW);
		UnLoadAnimation(RES_GAME_LEMMING_SPIN);
		UnLoadAnimation(RES_GAME_HIT_NORTH);
		UnLoadAnimation(RES_GAME_HIT_NORTH_EAST);
		UnLoadAnimation(RES_GAME_HIT_EAST);
		UnLoadAnimation(RES_GAME_HIT_SOUTH_EAST);
		UnLoadAnimation(RES_GAME_HIT_SOUTH);
		UnLoadAnimation(RES_GAME_HIT_SOUTH_WEST);
		UnLoadAnimation(RES_GAME_HIT_WEST);
		UnLoadAnimation(RES_GAME_HIT_NORTH_WEST);
		UnLoadAnimation(RES_GAME_FLAG_GREEN);
		UnLoadAnimation(RES_GAME_BONUS);
		if (m_ai->GetObjectRequired(OBJECT_DOOR_2)) {
			UnLoadAnimation(RES_GAME_DOOR);
		}
		if (m_ai->GetObjectRequired(OBJECT_DOOR_1)) {
			UnLoadAnimation(RES_GAME_DOOR_2);
		}
		if (m_ai->GetObjectRequired(OBJECT_BALL)) {
			UnLoadAnimation(RES_GAME_BALL);
			UnLoadAnimation(RES_GAME_BALL_EXPLODE);
		}
		if (m_ai->GetObjectRequired(OBJECT_TIME_BONUS)) {
			UnLoadAnimation(RES_GAME_TIME_BONUS);
		}
		if (m_ai->GetObjectRequired(OBJECT_CRATE)) {
			UnLoadAnimation(RES_GAME_CRATE_EXPLODE);
			UnLoadAnimation(RES_GAME_CRATE);
		}
		if (m_groundStyle != GROUND_STYLE_SPACE && m_ai->GetObjectRequired(OBJECT_TOWER)) {
			UnLoadAnimation(g_anGroundStyleResourceIds[6]);
		}
		UnLoadAnimation(RES_GAME_FLAME);
		UnLoadAnimation(RES_GAME_ELECTRIC);
		UnLoadAnimation(RES_GAME_EMBERS);
		UnLoadAnimation(RES_GAME_CONVEYOR);
		if (g_groundBlox1ResourceId != 0) {
			UnLoadAnimation(g_groundBlox1ResourceId);
		}
		if (g_groundBlox2ResourceId != 0) {
			UnLoadAnimation(g_groundBlox2ResourceId);
		}
		if (g_groundBlox3ResourceId != 0) {
			UnLoadAnimation(g_groundBlox3ResourceId);
		}
		if (g_groundBlox4ResourceId != 0) {
			UnLoadAnimation(g_groundBlox4ResourceId);
		}
		if (g_groundBlox5ResourceId != 0) {
			UnLoadAnimation(g_groundBlox5ResourceId);
		}
		if (g_groundBlox6ResourceId != 0) {
			UnLoadAnimation(g_groundBlox6ResourceId);
		}
		if (g_groundBlox7ResourceId != 0) {
			UnLoadAnimation(g_groundBlox7ResourceId);
		}
		if (g_anGroundStyleResourceIds[0] != 0) {
			UnLoadAnimation(g_anGroundStyleResourceIds[0]);
		}
		if (g_anGroundStyleResourceIds[4] != 0) {
			UnLoadAnimation(g_anGroundStyleResourceIds[4]);
		}
		if (g_anGroundStyleResourceIds[5] != 0) {
			UnLoadAnimation(g_anGroundStyleResourceIds[5]);
		}
		if (g_anGroundStyleResourceIds[3] != 0) {
			UnLoadAnimation(g_anGroundStyleResourceIds[3]);
		}
		if (g_anGroundStyleResourceIds[7] != 0) {
			UnLoadAnimation(g_anGroundStyleResourceIds[7]);
		}
		if (g_dwGroundStyleResourceId != 0) {
			UnLoadAnimation(g_dwGroundStyleResourceId);
		}
		if (g_anGroundStyleResourceIds[2] != 0 && m_ai->GetObjectRequired(OBJECT_HAND)) {
			UnLoadAnimation(g_anGroundStyleResourceIds[2]);
		}
		UnLoadAnimation(RES_GAME_SHADOW);
		if (m_ai->GetObjectRequired(OBJECT_KEY_1) || m_ai->GetObjectRequired(OBJECT_KEY_2) ||
			m_ai->GetObjectRequired(OBJECT_KEY_3)) {
			UnLoadAnimation(RES_GAME_KEYS);
		}
		UnLoadAnimation(RES_GAME_ANIM);
		UnLoadAnimation(RES_GAME_MINE);
		UnLoadAnimation(RES_GAME_MINE_STILL);
		UnLoadAnimation(RES_GAME_STAR);
		if (m_ai->GetObjectRequired(OBJECT_ROCKET)) {
			UnLoadAnimation(RES_GAME_ROCKET);
		}
		if (m_ai->GetObjectRequired(OBJECT_DUPLICATOR)) {
			UnLoadAnimation(RES_GAME_DUPLICATOR);
		}
		if (m_ai->GetObjectRequired(OBJECT_LASER_HORIZONTAL)) {
			UnLoadAnimation(RES_GAME_LASER_EAST);
			UnLoadAnimation(RES_GAME_LASER_FIRE_EAST);
		}
		if (m_ai->GetObjectRequired(OBJECT_LASER_VERTICAL)) {
			UnLoadAnimation(RES_GAME_LASER_NORTH);
			UnLoadAnimation(RES_GAME_LASER_FIRE_NORTH);
		}
		if (m_ai->GetObjectRequired(OBJECT_TRAMPOLINE)) {
			UnLoadAnimation(RES_GAME_TRAMPOLINE);
		}
		if (m_ai->GetObjectRequired(OBJECT_PAINT_GUN)) {
			UnLoadAnimation(g_anGroundStyleResourceIds[1]);
			UnLoadAnimation(RES_GAME_PAINTGUNSHOT);
		}
		if (m_ai->GetObjectRequired(OBJECT_SLINKY)) {
			UnLoadAnimation(RES_GAME_SLINKY_SOUTH);
			UnLoadAnimation(RES_GAME_SLINKY_NORTH);
			UnLoadAnimation(RES_GAME_SLINKY_EAST);
			UnLoadAnimation(RES_GAME_SLINKY_WEST);
		}
		if (m_ai->GetObjectRequired(OBJECT_BALLOON_0) || m_ai->GetObjectRequired(OBJECT_BALLOON_2) ||
			m_ai->GetObjectRequired(OBJECT_BALLOON_4) || m_ai->GetObjectRequired(OBJECT_BALLOON_6)) {
			UnLoadAnimation(RES_GAME_BALLOON);
			UnLoadAnimation(RES_GAME_BALLOON_POST);
		}
		UnLoadAnimation(RES_GAME_BALLOON_SHADOW);
		UnLoadAnimation(RES_GAME_BUTAMMO);
		UnLoadAnimation(RES_GAME_BUTLEMMING);
		UnLoadAnimation(RES_GAME_BUTBALLOON);
		UnLoadAnimation(RES_GAME_BUTPAWS);
		if (m_ai->GetObjectRequired(OBJECT_SWITCH)) {
			UnLoadAnimation(RES_GAME_SWITCH);
			UnLoadAnimation(RES_GAME_SWITCH_ANIM);
		}
		g_pMogRes->CleanUpResources();
	}
}

// FUNCTION: LEMBALL 0x00433fb0
void CLemmingAnimsManager::Draw()
{
	m_loadAnim->Draw();
}

// FUNCTION: LEMBALL 0x00433fc0
void CLemmingAnimsManager::DrawAnim(short p_x,
									short p_y,
									unsigned long p_resourceId,
									unsigned long p_animIndex,
									unsigned long p_time,
									CRemap* p_remap)
{
	if (p_resourceId == 0) {
		return;
	}
	p_x += m_drawOffsetX;
	p_y += m_drawOffsetY;
	switch (p_resourceId) {
	case RES_GAME_NUMERALS:
	case RES_GAME_COLON: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, 0, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT:
	case RES_GAME_STARS:
	case RES_GAME_FILLED_STARS:
	case RES_GAME_CIRCLES:
	case RES_GAME_MINE_STILL:
	case RES_GAME_CRATE:
	case RES_GAME_BALLOON:
	case RES_GAME_BALLOON_POST:
	case RES_GAME_BALLOON_SHADOW:
	case RES_GAME_LEMMINGSELECTED:
	case RES_GAME_LEMMINGLEADER:
	case RES_GAME_SWITCH:
	case RES_GAME_KEYS:
	case RES_GAME_CLOUD:
	case RES_GRASS_TOWER:
	case RES_CURSORS_HAND:
	case RES_SNOW_HUT:
	case RES_LEGO_HUT: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_SHADOW:
	case RES_GAME_SPARKLE:
	case RES_GAME_TRAP_DOOR:
	case RES_GAME_LEGO_SPARKLE:
	case RES_GAME_LEGO_TRAP_DOOR:
	case RES_GAME_SNOW_SPARKLE:
	case RES_GAME_SNOW_TRAP_DOOR:
	case RES_GAME_SPACE_SPARKLE:
	case RES_GAME_SPACE_TRAP_DOOR:
	case RES_GAME_DOOR:
	case RES_GAME_DOOR_2:
	case RES_GAME_LEMMING_SPIN:
	case RES_GAME_JUMP_NW:
	case RES_GAME_JUMP_NE:
	case RES_GAME_JUMP_SW:
	case RES_GAME_JUMP_SE:
	case RES_GAME_CATAPULT_SE:
	case RES_GAME_ONBALLOON:
	case RES_GAME_ONFIRE:
	case RES_GAME_SNOW_CATAPULT_SE:
	case RES_GAME_LEGO_CATAPULT_SE:
	case RES_GAME_BALL_EXPLODE:
	case RES_GAME_ROCKET:
	case RES_GAME_DUPLICATOR:
	case RES_GAME_SNOW_HAND:
	case RES_GAME_SPACE_HAND:
	case RES_GAME_LASER_EAST:
	case RES_GAME_LASER_FIRE_EAST:
	case RES_GAME_LASER_NORTH:
	case RES_GAME_LASER_FIRE_NORTH:
	case RES_GAME_LEM_LASER_N:
	case RES_GAME_LEM_LASER_E:
	case RES_GAME_LEM_LASER_S:
	case RES_GAME_LEM_LASER_W:
	case RES_GAME_TRAMPOLINE:
	case RES_GAME_PAINTGUNSHOT:
	case RES_GAME_GRASSPAINTGUN:
	case RES_GAME_SNOWPAINTGUN:
	case RES_GAME_SPACEPAINTGUN:
	case RES_GAME_SLINKY_EAST:
	case RES_GAME_SLINKY_SOUTH:
	case RES_GAME_SLINKY_WEST:
	case RES_GAME_SLINKY_NORTH:
	case RES_GAME_STAR:
	case RES_GAME_OUTLINE:
	case RES_GAME_ANIM:
	case RES_LEGO_LEGOTREE: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_LEMMINGWALKN:
	case RES_GAME_LEMMINGWALKNE:
	case RES_GAME_LEMMINGWALKE:
	case RES_GAME_LEMMINGWALKSE:
	case RES_GAME_LEMMINGWALKS:
	case RES_GAME_LEMMINGWALKSW:
	case RES_GAME_LEMMINGWALKW:
	case RES_GAME_LEMMINGWALKNW:
	case RES_GAME_SOMMERSAULT:
	case RES_GAME_SOMMERSAULT_REV:
	case RES_GAME_LEMMINGSTANDN:
	case RES_GAME_LEMMINGSTANDNE:
	case RES_GAME_LEMMINGSTANDE:
	case RES_GAME_LEMMINGSTANDSE:
	case RES_GAME_LEMMINGSTANDS:
	case RES_GAME_LEMMINGSTANDSW:
	case RES_GAME_LEMMINGSTANDW:
	case RES_GAME_LEMMINGSTANDNW: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_WALK_STAND_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT_LOOK: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_LOOK_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT_TOSS: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_TOSS_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT_JIG: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_JIG_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_LEMMINGFIREN:
	case RES_GAME_LEMMINGFIRENE:
	case RES_GAME_LEMMINGFIREE:
	case RES_GAME_LEMMINGFIRESE:
	case RES_GAME_LEMMINGFIRES:
	case RES_GAME_LEMMINGFIRESW:
	case RES_GAME_LEMMINGFIREW:
	case RES_GAME_LEMMINGFIRENW: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_FIRE_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_LEMMINGPELLETN:
	case RES_GAME_LEMMINGPELLETNE:
	case RES_GAME_LEMMINGPELLETE:
	case RES_GAME_LEMMINGPELLETSE:
	case RES_GAME_LEMMINGPELLETS:
	case RES_GAME_LEMMINGPELLETSW:
	case RES_GAME_LEMMINGPELLETW:
	case RES_GAME_LEMMINGPELLETNW:
	case RES_GAME_BALL: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_PROJECTILE_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_HIT_NORTH:
	case RES_GAME_HIT_NORTH_EAST:
	case RES_GAME_HIT_EAST:
	case RES_GAME_HIT_SOUTH_EAST:
	case RES_GAME_HIT_SOUTH:
	case RES_GAME_HIT_SOUTH_WEST:
	case RES_GAME_HIT_WEST:
	case RES_GAME_HIT_NORTH_WEST: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(LEMMING_HIT_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_CATAPULT_ANIMSE:
	case RES_GAME_SNOW_CATAPULT_ANIMSE:
	case RES_GAME_LEGO_CATAPULT_ANIMSE: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(CATAPULT_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_CATMOUNT_SE: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(CATAPULT_MOUNT_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_YELLOW_AMMO:
	case RES_GAME_TIME_BONUS: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(PICKUP_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_FLAG_GREEN:
	case RES_GAME_BONUS: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(FLAG_BONUS_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_EX_PELLET: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(EXTRA_PELLET_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_MINE:
	case RES_GAME_SWITCH_ANIM: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(MINE_SWITCH_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_CRATE_EXPLODE: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(CRATE_EXPLOSION_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_SHEEP_WALK_N:
	case RES_GAME_SHEEP_WALK_NE:
	case RES_GAME_SHEEP_WALK_E:
	case RES_GAME_SHEEP_WALK_SE:
	case RES_GAME_SHEEP_WALK_S:
	case RES_GAME_SHEEP_WALK_SW:
	case RES_GAME_SHEEP_WALK_W:
	case RES_GAME_SHEEP_WALK_NW: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(SHEEP_WALK_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_SHEEP_MUNCH_NE:
	case RES_GAME_SHEEP_MUNCH_SE:
	case RES_GAME_SHEEP_MUNCH_SW:
	case RES_GAME_SHEEP_MUNCH_NW: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(SHEEP_MUNCH_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_SPINARROW: {
		CTimedAnim* frame = (CTimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(0);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(SPIN_ARROW_CYCLE_DURATION_MS);
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_BLOX_1:
	case RES_GAME_BLOX_2:
	case RES_GAME_BLOX_3:
	case RES_GAME_BLOX_4:
	case RES_GAME_BLOX_5:
	case RES_GAME_BLOX_6:
	case RES_GAME_BLOX_7:
	case RES_GAME_BLOX_8:
	case RES_GAME_BLOX_14:
	case RES_GAME_BLOX_15:
	case RES_GAME_LEGO_1:
	case RES_GAME_LEGO_2:
	case RES_GAME_LEGO_3:
	case RES_GAME_LEGO_4:
	case RES_GAME_LEGO_5:
	case RES_GAME_LEGO_6:
	case RES_GAME_LEGO_7:
	case RES_GAME_LEGO_8:
	case RES_GAME_LEGO_14:
	case RES_GAME_LEGO_15:
	case RES_GAME_SNOW_1:
	case RES_GAME_SNOW_2:
	case RES_GAME_SNOW_3:
	case RES_GAME_SNOW_4:
	case RES_GAME_SNOW_5:
	case RES_GAME_SNOW_6:
	case RES_GAME_SNOW_7:
	case RES_GAME_SNOW_8:
	case RES_GAME_SNOW_14:
	case RES_GAME_SNOW_15:
	case RES_GAME_SPACE_1:
	case RES_GAME_SPACE_2:
	case RES_GAME_SPACE_3:
	case RES_GAME_SPACE_4:
	case RES_GAME_SPACE_5:
	case RES_GAME_SPACE_6:
	case RES_GAME_SPACE_7:
	case RES_GAME_SPACE_8:
	case RES_GAME_SPACE_14:
	case RES_GAME_SPACE_15:
	case RES_GRASS_FLAT:
	case RES_GRASS_BLOX:
	case RES_GRASS_TREE:
	case RES_GRASS_PATH:
	case RES_GRASS_ROCK:
	case RES_SNOW_SNOWTREE: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	case RES_GAME_CONVEYOR:
	case RES_GAME_FLAME:
	case RES_GAME_ELECTRIC:
	case RES_GAME_EMBERS: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		CAnimsManager::DrawAnim(CVSPoint(p_x, p_y), p_resourceId, m_drawFlags, frame, p_remap);
		break;
	}
	}
}

// FUNCTION: LEMBALL 0x004349e0
void CLemmingAnimsManager::DrawAnimOnGdi(CGDI* p_gdi,
										 short p_x,
										 short p_y,
										 unsigned long p_resourceId,
										 unsigned long p_animIndex,
										 CRemap* p_remap)
{
	switch (p_resourceId) {
	case RES_GAME_LEGO_SPARKLE:
	case RES_GAME_LEGO_TRAP_DOOR:
	case RES_GAME_SNOW_SPARKLE:
	case RES_GAME_SNOW_TRAP_DOOR:
	case RES_GAME_SPACE_SPARKLE:
	case RES_GAME_SPACE_TRAP_DOOR:
	case RES_GAME_BLOX_1:
	case RES_GAME_BLOX_2:
	case RES_GAME_BLOX_3:
	case RES_GAME_BLOX_4:
	case RES_GAME_BLOX_5:
	case RES_GAME_BLOX_6:
	case RES_GAME_BLOX_7:
	case RES_GAME_BLOX_8:
	case RES_GAME_BLOX_14:
	case RES_GAME_BLOX_15:
	case RES_GAME_LEGO_1:
	case RES_GAME_LEGO_2:
	case RES_GAME_LEGO_3:
	case RES_GAME_LEGO_4:
	case RES_GAME_LEGO_5:
	case RES_GAME_LEGO_6:
	case RES_GAME_LEGO_7:
	case RES_GAME_LEGO_8:
	case RES_GAME_LEGO_14:
	case RES_GAME_LEGO_15:
	case RES_GAME_SNOW_1:
	case RES_GAME_SNOW_2:
	case RES_GAME_SNOW_3:
	case RES_GAME_SNOW_4:
	case RES_GAME_SNOW_5:
	case RES_GAME_SNOW_6:
	case RES_GAME_SNOW_7:
	case RES_GAME_SNOW_8:
	case RES_GAME_SNOW_14:
	case RES_GAME_SNOW_15:
	case RES_GAME_SPACE_1:
	case RES_GAME_SPACE_2:
	case RES_GAME_SPACE_3:
	case RES_GAME_SPACE_4:
	case RES_GAME_SPACE_5:
	case RES_GAME_SPACE_6:
	case RES_GAME_SPACE_7:
	case RES_GAME_SPACE_8:
	case RES_GAME_SPACE_14:
	case RES_GAME_SPACE_15:
	case RES_GAME_CONVEYOR:
	case RES_GAME_ANIM:
	case RES_GAME_FLAME:
	case RES_GAME_ELECTRIC:
	case RES_GAME_EMBERS:
	case RES_GRASS_TREE:
	case RES_GRASS_PATH:
	case RES_SNOW_SNOWTREE:
	case RES_LEGO_LEGOTREE: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		CVSPoint position(p_x, p_y);
		CGDI* previous = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = p_gdi;
		CAnimsManager::DrawAnim(position, p_resourceId, 0, frame, p_remap);
		CAnimsManager::m_gdi = previous;
		break;
	}
	case RES_GRASS_TOWER:
	case RES_SNOW_HUT:
	case RES_LEGO_HUT: {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		CVSPoint position(p_x, p_y);
		CGDI* previous = CAnimsManager::m_gdi;
		CAnimsManager::m_gdi = p_gdi;
		CAnimsManager::DrawAnim(position, p_resourceId, 0, frame, p_remap);
		CAnimsManager::m_gdi = previous;
		break;
	}
	}
}

// FUNCTION: LEMBALL 0x00434bc0
void CLemmingAnimsManager::LoadAnimation(unsigned long p_resourceId, int p_animType)
{
	if (m_countingLoads != 0) {
		m_loadProgress++;
		return;
	}
	LoadAnims(p_resourceId);
	CAnimFrameBASE* frame;
	switch (p_animType) {
	case ANIM_FRAME_STATIC:
		frame = new CStaticAnim();
		break;
	case ANIM_FRAME_REPEAT: {
		unsigned int count = GetnAnims(p_resourceId);
		frame = new CRepeatAnim(count, 1);
	} break;
	case ANIM_FRAME_SINGLE_FRAME:
		frame = new CFrames(1);
		break;
	case ANIM_FRAME_PLAY_THROUGH: {
		unsigned int count = GetnAnims(p_resourceId);
		frame = new CPlayThruAnim(count, 1);
	} break;
	}
	m_animFrames[m_resourceSlots[p_resourceId]] = frame;
	UpdateNonCacheLoad();
}

// FUNCTION: LEMBALL 0x00434d10
void CLemmingAnimsManager::UpdateNonCacheLoad()
{
	int loaded = m_nonCacheState + 1;
	m_nonCacheState = loaded;
	if (m_loadAnim != NULL) {
		m_loadAnim->Draw((short) ((loaded * 100) / m_loadProgress));
	}
}

// FUNCTION: LEMBALL 0x00434d40
void CLemmingAnimsManager::LoadAnimation(unsigned long p_firstResourceId,
										 unsigned long p_lastResourceId,
										 int p_animType)
{
	if (m_countingLoads != 0) {
		m_loadProgress += p_lastResourceId - p_firstResourceId;
		return;
	}
	CAnimFrameBASE* frame;
	for (; (int) p_lastResourceId >= (int) p_firstResourceId; p_firstResourceId++) {
		LoadAnims(p_firstResourceId);
		switch (p_animType) {
		case ANIM_FRAME_STATIC:
			frame = new CStaticAnim();
			break;
		case ANIM_FRAME_REPEAT: {
			unsigned int count = GetnAnims(p_firstResourceId);
			frame = new CRepeatAnim(count, 1);
		} break;
		case ANIM_FRAME_SINGLE_FRAME:
			frame = new CFrames(1);
			break;
		case ANIM_FRAME_PLAY_THROUGH: {
			unsigned int count = GetnAnims(p_firstResourceId);
			frame = new CPlayThruAnim(count, 1);
		} break;
		}
		m_animFrames[m_resourceSlots[p_firstResourceId]] = frame;
		UpdateNonCacheLoad();
	}
}

// FUNCTION: LEMBALL 0x00434ec0
void CLemmingAnimsManager::UnLoadAnimation(unsigned long p_resourceId)
{
	CAnimFrameBASE* frames = m_animFrames[m_resourceSlots[p_resourceId]];
	if (frames != NULL) {
		delete frames;
		m_animFrames[m_resourceSlots[p_resourceId]] = NULL;
	}
	UnLoadAnims(p_resourceId);
}

// FUNCTION: LEMBALL 0x00434f00
void CLemmingAnimsManager::UnLoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId)
{
	unsigned long resourceId = p_firstResourceId;
	for (; (int) resourceId <= (int) p_lastResourceId; resourceId++) {
		CAnimFrameBASE* frame = m_animFrames[m_resourceSlots[resourceId]];
		if (frame != NULL) {
			delete frame;
			m_animFrames[m_resourceSlots[resourceId]] = NULL;
		}
		UnLoadAnims(resourceId);
	}
}
