#include "LemmingAnimsManager.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Frontend/Resources/CdLoadAnim.h"
#include "../../Frontend/Resources/FrontendResourceLoader.h"
#include "../../Visos/Animation/PlayThruAnim.h"
#include "../../Visos/Animation/RepeatAnim.h"
#include "../../Visos/Animation/StaticAnim.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Resources/ResFont.h"
#include "../Display/Main2DDisplay.h"
#include "../Sound/SoundView.h"

// GLOBAL: LEMBALL 0x00496f78
const undefined4 g_style0ObjectClip[4] = {31, 90, 64, 96};
// GLOBAL: LEMBALL 0x00496f88
const undefined4 g_style1ObjectClip[4] = {34, 96, 68, 96};
// GLOBAL: LEMBALL 0x00496f98
const undefined4 g_style2ObjectClip[4] = {34, 96, 68, 96};
// GLOBAL: LEMBALL 0x0049e8b4
const undefined4* g_styleObjectClip = 0;

// Style-sensitive resource IDs written by SetupStyleSensitive (0x00432c80).
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

// 68K 0x10b09d40 __ct__20CLemmingAnimsManagerFP4CGDIP14CMain2DDisplayP3CAI
// FUNCTION: LEMBALL 0x00432b50
LemmingAnimsManager::LemmingAnimsManager(Gdi* p_arg0, Main2DDisplay* p_arg1, Ai* p_arg2)
	: AnimsManager(p_arg0, 0x2b6, 0xc8, 0x28, 0x14, 1)
{
	m_display = p_arg1;
	m_gdi = p_arg0;
	m_ai = p_arg2;
	m_animFrames = (Frames**) operator new(0xad8);
	m_reservedac = 0;
	for (int i = 0; i < 0x2b6; i++) {
		m_animFrames[i] = 0;
	}
	m_loaded = 0;
	m_loadAnim = new CdLoadAnim(m_gdi, m_display);
	m_drawOffsetX = 0;
	m_drawOffsetY = 0;
}

// 68K 0x10b09e58 __dt__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x00432c20
LemmingAnimsManager::~LemmingAnimsManager()
{
	if (m_loadAnim != 0) {
		delete m_loadAnim;
		m_loadAnim = 0;
	}

	Unload();
	if (m_animFrames != 0) {
		operator delete(m_animFrames);
	}
}

// 68K 0x10b09f1a SetupStyleSensitive__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x00432c80
void LemmingAnimsManager::SetupStyleSensitive()
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
	case 0:
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
	case 1:
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
	case 2:
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
	case 3:
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

// 68K 0x10b0a2ae LoadVrammed__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x00432fe0
void LemmingAnimsManager::LoadVrammed()
{
	if (g_groundBlox1ResourceId != 0) {
		LoadAnimation(g_groundBlox1ResourceId, 2);
	}
	if (g_groundBlox2ResourceId != 0) {
		LoadAnimation(g_groundBlox2ResourceId, 2);
	}
	if (g_groundBlox3ResourceId != 0) {
		LoadAnimation(g_groundBlox3ResourceId, 2);
	}
	if (g_groundBlox4ResourceId != 0) {
		LoadAnimation(g_groundBlox4ResourceId, 2);
	}
	if (g_groundBlox5ResourceId != 0) {
		LoadAnimation(g_groundBlox5ResourceId, 2);
	}
	if (g_groundBlox6ResourceId != 0) {
		LoadAnimation(g_groundBlox6ResourceId, 2);
	}
	if (g_groundBlox7ResourceId != 0) {
		LoadAnimation(g_groundBlox7ResourceId, 2);
	}
	if (g_anGroundStyleResourceIds[0] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[0], 2);
	}
	if (g_anGroundStyleResourceIds[4] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[4], 2);
	}
	if (g_anGroundStyleResourceIds[5] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[5], 2);
	}
	if (g_anGroundStyleResourceIds[3] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[3], 2);
	}
	LoadAnimation(RES_GAME_CONVEYOR, 2);
	if (m_ai->GetObjectRequired(4)) {
		LoadAnimation(g_anGroundStyleResourceIds[8], 0);
	}
	LoadAnimation(RES_CURSORS_HAND, 0);
	LoadAnimation(RES_GAME_LEMMINGSELECTED, 0);
	LoadAnimation(RES_GAME_LEMMINGLEADER, 0);
	LoadAnimation(RES_GAME_LEMMINGWALKN, RES_GAME_LEMMINGWALKNW, 1);
	LoadAnimation(RES_GAME_LEMMINGSTANDN, RES_GAME_LEMMINGSTANDNW, 1);
	LoadAnimation(RES_GAME_LEMMINGFIREN, RES_GAME_LEMMINGFIRENW, 1);
	LoadAnimation(RES_GAME_LEMMINGPELLETN, RES_GAME_LEMMINGPELLETNW, 1);
	LoadAnimation(RES_GAME_STARS, 0);
	LoadAnimation(RES_GAME_FILLED_STARS, 0);
	LoadAnimation(RES_GAME_CIRCLES, 0);
	if (m_ai->GetObjectRequired(0x27) || m_ai->GetObjectRequired(0x29) || m_ai->GetObjectRequired(0x2b) ||
		m_ai->GetObjectRequired(0x2d)) {
		LoadAnimation(RES_GAME_BALLOON, 0);
		LoadAnimation(RES_GAME_BALLOON_POST, 0);
	}
	LoadAnimation(RES_GAME_BALLOON_SHADOW, 0);
	LoadAnimation(RES_GAME_JUMP_NE, 2);
	LoadAnimation(RES_GAME_JUMP_NW, 2);
	LoadAnimation(RES_GAME_JUMP_SE, 2);
	LoadAnimation(RES_GAME_JUMP_SW, 2);
	LoadAnimation(RES_GAME_LEMMING_SPIN, 2);
	LoadAnimation(RES_GAME_YELLOW_AMMO, 1);
	LoadAnimation(RES_GAME_EX_PELLET, 3);
	LoadAnimation(RES_GAME_SPINARROW, 1);
	LoadAnimation(RES_GAME_ONBALLOON, 2);
	LoadAnimation(RES_GAME_ONFIRE, 2);
	LoadAnimation(RES_GAME_FLAG_GREEN, 1);
	LoadAnimation(RES_GAME_BONUS, 1);
	if (m_ai->GetObjectRequired(0x14)) {
		LoadAnimation(RES_GAME_SWITCH, 0);
	}
	if (m_ai->GetObjectRequired(7)) {
		LoadAnimation(RES_GAME_SHEEP_WALK_N, RES_GAME_SHEEP_WALK_NW, 1);
		LoadAnimation(RES_GAME_SHEEP_MUNCH_NE, RES_GAME_SHEEP_MUNCH_NW, 1);
	}
	if (m_ai->GetObjectRequired(0x11)) {
		LoadAnimation(RES_GAME_CRATE, 0);
	}
	LoadAnimation(RES_GAME_FLAME, 2);
	LoadAnimation(RES_GAME_ELECTRIC, 2);
	LoadAnimation(RES_GAME_EMBERS, 2);
	if (m_groundStyle != 3 && m_ai->GetObjectRequired(0x0d)) {
		LoadAnimation(g_anGroundStyleResourceIds[6], 0);
	}
	if (m_ai->GetObjectRequired(0x15) || m_ai->GetObjectRequired(0x16) || m_ai->GetObjectRequired(0x17)) {
		LoadAnimation(RES_GAME_KEYS, 0);
	}
	LoadAnimation(RES_GAME_ANIM, 2);
	LoadAnimation(RES_GAME_MINE_STILL, 0);
	LoadAnimation(RES_GAME_BUTAMMO, 2);
	LoadAnimation(RES_GAME_BUTLEMMING, 2);
	LoadAnimation(RES_GAME_BUTBALLOON, 2);
	LoadAnimation(RES_GAME_BUTPAWS, 2);
	if (m_countingLoads == 0) {
		ResFont** fontResources = m_unk0x9c;
		fontResources[0] = 0;
		fontResources[1] = 0;
		fontResources[2] = 0;
		LoadAnims(RES_BORDERS_LORES_BORDERCORNERS);
		LoadAnims(RES_BORDERS_LORES_BORDEREDGES);
		fontResources[0] = ResFont::Load(RES_BORDERS_LORES_CUTFONT);
		LoadAnims(RES_BORDERS_HIRES_BORDERCORNERS);
		LoadAnims(RES_BORDERS_HIRES_BORDEREDGES);
		m_unk0x9c[1] = ResFont::Load(RES_BORDERS_HIRES_CUTFONT);
		m_unk0x9c[2] = ResFont::Load(RES_NEWFRONT_FONTS_GAME_SCORETIME);
	}
	LoadAnimation(RES_GAME_HIT_NORTH, 1);
	LoadAnimation(RES_GAME_HIT_NORTH_EAST, 1);
	LoadAnimation(RES_GAME_HIT_EAST, 1);
	LoadAnimation(RES_GAME_HIT_SOUTH_EAST, 1);
	if (m_ai->GetObjectRequired(0x2f) || m_ai->GetObjectRequired(0x1e)) {
		LoadAnimation(RES_GAME_LEM_LASER_N, 2);
		LoadAnimation(RES_GAME_LEM_LASER_E, 2);
		LoadAnimation(RES_GAME_LEM_LASER_S, 2);
		LoadAnimation(RES_GAME_LEM_LASER_W, 2);
	}
}

// 68K 0x10b0a920 LoadMainRammed__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x004334f0
void LemmingAnimsManager::LoadMainRammed()
{
	LoadAnimation(RES_GAME_STAR, 2);
	if (g_anGroundStyleResourceIds[7] != 0) {
		LoadAnimation(g_anGroundStyleResourceIds[7], 2);
	}
	if (g_dwGroundStyleResourceId != 0) {
		LoadAnimation(g_dwGroundStyleResourceId, 2);
	}
	if (g_anGroundStyleResourceIds[2] != 0 && m_ai->GetObjectRequired(0x1f)) {
		LoadAnimation(g_anGroundStyleResourceIds[2], 2);
	}
	LoadAnimation(RES_GAME_MINE, 3);
	if (m_ai->GetObjectRequired(0x11)) {
		LoadAnimation(RES_GAME_CRATE_EXPLODE, 3);
	}
	if (m_ai->GetObjectRequired(0x14)) {
		LoadAnimation(RES_GAME_SWITCH_ANIM, 3);
	}
	if (m_ai->GetObjectRequired(4)) {
		LoadAnimation(g_anGroundStyleResourceIds[9], 3);
		LoadAnimation(RES_GAME_CATMOUNT_SE, 3);
	}
	LoadAnimation(RES_GAME_HIT_SOUTH, 1);
	LoadAnimation(RES_GAME_HIT_SOUTH_WEST, 1);
	LoadAnimation(RES_GAME_HIT_WEST, 1);
	LoadAnimation(RES_GAME_HIT_NORTH_WEST, 1);
	if (m_ai->GetObjectRequired(0x20)) {
		LoadAnimation(RES_GAME_ROCKET, 2);
	}
	if (m_ai->GetObjectRequired(0x1c)) {
		LoadAnimation(RES_GAME_DUPLICATOR, 2);
	}
	if (m_ai->GetObjectRequired(0x1e)) {
		LoadAnimation(RES_GAME_LASER_EAST, 2);
		LoadAnimation(RES_GAME_LASER_FIRE_EAST, 2);
	}
	if (m_ai->GetObjectRequired(0x2f)) {
		LoadAnimation(RES_GAME_LASER_NORTH, 2);
		LoadAnimation(RES_GAME_LASER_FIRE_NORTH, 2);
	}
	if (m_ai->GetObjectRequired(0x22)) {
		LoadAnimation(RES_GAME_TRAMPOLINE, 2);
	}
	if (m_ai->GetObjectRequired(0x21)) {
		LoadAnimation(g_anGroundStyleResourceIds[1], 2);
		LoadAnimation(RES_GAME_PAINTGUNSHOT, 2);
	}
	if (m_ai->GetObjectRequired(0x35)) {
		LoadAnimation(RES_GAME_SLINKY_SOUTH, 2);
		LoadAnimation(RES_GAME_SLINKY_NORTH, 2);
		LoadAnimation(RES_GAME_SLINKY_EAST, 2);
		LoadAnimation(RES_GAME_SLINKY_WEST, 2);
	}
	LoadAnimation(RES_GAME_WAIT_LOOK, 1);
	LoadAnimation(RES_GAME_WAIT_TOSS, 1);
	LoadAnimation(RES_GAME_WAIT_JIG, 1);
	LoadAnimation(RES_GAME_SOMMERSAULT, 1);
	LoadAnimation(RES_GAME_SOMMERSAULT_REV, 1);
	if (m_ai->GetObjectRequired(0x1a)) {
		LoadAnimation(RES_GAME_DOOR, 2);
	}
	if (m_ai->GetObjectRequired(0x19)) {
		LoadAnimation(RES_GAME_DOOR_2, 2);
	}
	if (m_ai->GetObjectRequired(9)) {
		LoadAnimation(RES_GAME_BALL, 1);
		LoadAnimation(RES_GAME_BALL_EXPLODE, 2);
	}
	if (m_ai->GetObjectRequired(0x1b)) {
		LoadAnimation(RES_GAME_TIME_BONUS, 1);
	}
	LoadAnimation(RES_GAME_SHADOW, 2);
}

// 68K 0x10b0ad4c Load__20CLemmingAnimsManagerF7GROUNDS
// FUNCTION: LEMBALL 0x004337f0
void LemmingAnimsManager::Load(int p_groundStyle)
{
	m_groundStyle = p_groundStyle;
	if (m_loadAnim != 0) {
		m_loadAnim->InitialiseScreen();
	}
	m_nonCacheState = 0;
	m_loadProgress = g_pSoundView->GetnEffects(4);
	m_countingLoads = 1;
	LoadVrammed();
	LoadMainRammed();
	m_countingLoads = 0;
	g_pSoundView->ChangeState(4, (LoadUpdate*) this);
	SetupStyleSensitive();
	LoadVrammed();
	LoadMainRammed();
	if (m_loadAnim != 0) {
		delete m_loadAnim;
		m_loadAnim = 0;
	}
	m_display->AttachPalette(RES_GAME_GAMEPALETTE);
	m_loaded = 1;
}

// 68K 0x10b0ae48 Unload__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x004338b0
void LemmingAnimsManager::Unload()
{
	if (m_loaded != 0) {
		UnLoadAnims(RES_BORDERS_LORES_BORDEREDGES);
		UnLoadAnims(RES_BORDERS_LORES_BORDERCORNERS);
		UnLoadAnims(RES_BORDERS_HIRES_BORDEREDGES);
		UnLoadAnims(RES_BORDERS_HIRES_BORDERCORNERS);
		ResFont** font = m_unk0x9c;
		for (int count = 3; count != 0; font++, count--) {
			if (*font != 0) {
				(*font)->UnLoad();
			}
		}
		UnLoadAnimation(RES_CURSORS_HAND);
		UnLoadAnimation(RES_GAME_LEMMINGWALKN, RES_GAME_LEMMINGWALKNW);
		UnLoadAnimation(RES_GAME_LEMMINGSTANDN, RES_GAME_LEMMINGSTANDNW);
		UnLoadAnimation(RES_GAME_LEMMINGFIREN, RES_GAME_LEMMINGFIRENW);
		UnLoadAnimation(RES_GAME_LEMMINGPELLETN, RES_GAME_LEMMINGPELLETNW);
		if (m_ai->GetObjectRequired(0x2f) || m_ai->GetObjectRequired(0x1e)) {
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
		if (m_ai->GetObjectRequired(7)) {
			UnLoadAnimation(RES_GAME_SHEEP_WALK_N, RES_GAME_SHEEP_WALK_NW);
			UnLoadAnimation(RES_GAME_SHEEP_MUNCH_NE, RES_GAME_SHEEP_MUNCH_NW);
		}
		UnLoadAnimation(RES_GAME_WAIT_LOOK);
		UnLoadAnimation(RES_GAME_WAIT_TOSS);
		UnLoadAnimation(RES_GAME_WAIT_JIG);
		UnLoadAnimation(RES_GAME_LEMMINGLEADER);
		UnLoadAnimation(RES_GAME_LEMMINGSELECTED);
		if (m_ai->GetObjectRequired(4)) {
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
		if (m_ai->GetObjectRequired(0x1a)) {
			UnLoadAnimation(RES_GAME_DOOR);
		}
		if (m_ai->GetObjectRequired(0x19)) {
			UnLoadAnimation(RES_GAME_DOOR_2);
		}
		if (m_ai->GetObjectRequired(9)) {
			UnLoadAnimation(RES_GAME_BALL);
			UnLoadAnimation(RES_GAME_BALL_EXPLODE);
		}
		if (m_ai->GetObjectRequired(0x1b)) {
			UnLoadAnimation(RES_GAME_TIME_BONUS);
		}
		if (m_ai->GetObjectRequired(0x11)) {
			UnLoadAnimation(RES_GAME_CRATE_EXPLODE);
			UnLoadAnimation(RES_GAME_CRATE);
		}
		if (m_groundStyle != 3 && m_ai->GetObjectRequired(0x0d)) {
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
		if (g_anGroundStyleResourceIds[2] != 0 && m_ai->GetObjectRequired(0x1f)) {
			UnLoadAnimation(g_anGroundStyleResourceIds[2]);
		}
		UnLoadAnimation(RES_GAME_SHADOW);
		if (m_ai->GetObjectRequired(0x15) || m_ai->GetObjectRequired(0x16) || m_ai->GetObjectRequired(0x17)) {
			UnLoadAnimation(RES_GAME_KEYS);
		}
		UnLoadAnimation(RES_GAME_ANIM);
		UnLoadAnimation(RES_GAME_MINE);
		UnLoadAnimation(RES_GAME_MINE_STILL);
		UnLoadAnimation(RES_GAME_STAR);
		if (m_ai->GetObjectRequired(0x20)) {
			UnLoadAnimation(RES_GAME_ROCKET);
		}
		if (m_ai->GetObjectRequired(0x1c)) {
			UnLoadAnimation(RES_GAME_DUPLICATOR);
		}
		if (m_ai->GetObjectRequired(0x1e)) {
			UnLoadAnimation(RES_GAME_LASER_EAST);
			UnLoadAnimation(RES_GAME_LASER_FIRE_EAST);
		}
		if (m_ai->GetObjectRequired(0x2f)) {
			UnLoadAnimation(RES_GAME_LASER_NORTH);
			UnLoadAnimation(RES_GAME_LASER_FIRE_NORTH);
		}
		if (m_ai->GetObjectRequired(0x22)) {
			UnLoadAnimation(RES_GAME_TRAMPOLINE);
		}
		if (m_ai->GetObjectRequired(0x21)) {
			UnLoadAnimation(g_anGroundStyleResourceIds[1]);
			UnLoadAnimation(RES_GAME_PAINTGUNSHOT);
		}
		if (m_ai->GetObjectRequired(0x35)) {
			UnLoadAnimation(RES_GAME_SLINKY_SOUTH);
			UnLoadAnimation(RES_GAME_SLINKY_NORTH);
			UnLoadAnimation(RES_GAME_SLINKY_EAST);
			UnLoadAnimation(RES_GAME_SLINKY_WEST);
		}
		if (m_ai->GetObjectRequired(0x27) || m_ai->GetObjectRequired(0x29) || m_ai->GetObjectRequired(0x2b) ||
			m_ai->GetObjectRequired(0x2d)) {
			UnLoadAnimation(RES_GAME_BALLOON);
			UnLoadAnimation(RES_GAME_BALLOON_POST);
		}
		UnLoadAnimation(RES_GAME_BALLOON_SHADOW);
		UnLoadAnimation(RES_GAME_BUTAMMO);
		UnLoadAnimation(RES_GAME_BUTLEMMING);
		UnLoadAnimation(RES_GAME_BUTBALLOON);
		UnLoadAnimation(RES_GAME_BUTPAWS);
		if (m_ai->GetObjectRequired(0x14)) {
			UnLoadAnimation(RES_GAME_SWITCH);
			UnLoadAnimation(RES_GAME_SWITCH_ANIM);
		}
		g_pMogRes->CleanUpResources();
	}
}

// 68K 0x10b0b6b6 Draw__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x00433fb0
void LemmingAnimsManager::Draw()
{
	m_loadAnim->m_progress.m_draw.Draw();
}

// 68K 0x10b0b6f2 DrawAnim__20CLemmingAnimsManagerFssUlUlUlP6CRemap
// FUNCTION: LEMBALL 0x00433fc0
void LemmingAnimsManager::DrawAnim(short p_x,
								   short p_y,
								   unsigned long p_resourceId,
								   unsigned long p_animIndex,
								   unsigned long p_time,
								   Remap* p_remap)
{
	if (p_resourceId == 0) {
		return;
	}
	p_x += m_drawOffsetX;
	p_y += m_drawOffsetY;
	switch (p_resourceId) {
	case RES_GAME_NUMERALS:
	case RES_GAME_COLON: {
		Frames* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, 0, frame, p_remap);
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
		Frames* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		Frames* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1000);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT_LOOK: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1933);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT_TOSS: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1600);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_WAIT_JIG: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(2700);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(2000);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(500);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(3000);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_CATAPULT_ANIMSE:
	case RES_GAME_SNOW_CATAPULT_ANIMSE:
	case RES_GAME_LEGO_CATAPULT_ANIMSE: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(3133);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_CATMOUNT_SE: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(2333);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_YELLOW_AMMO:
	case RES_GAME_TIME_BONUS: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1000);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_FLAG_GREEN:
	case RES_GAME_BONUS: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1500);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_EX_PELLET: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(400);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_MINE:
	case RES_GAME_SWITCH_ANIM: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(900);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_CRATE_EXPLODE: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1500);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(1400);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_SHEEP_MUNCH_NE:
	case RES_GAME_SHEEP_MUNCH_SE:
	case RES_GAME_SHEEP_MUNCH_SW:
	case RES_GAME_SHEEP_MUNCH_NW: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(p_animIndex);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(2400);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_SPINARROW: {
		TimedAnim* frame = (TimedAnim*) m_animFrames[m_resourceSlots[p_resourceId]];
		frame->SetStartTime(0);
		frame->m_fixedTime = p_time;
		frame->SetAnimTime(560);
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
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
		Frames* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	case RES_GAME_CONVEYOR:
	case RES_GAME_FLAME:
	case RES_GAME_ELECTRIC:
	case RES_GAME_EMBERS: {
		Frames* frame = m_animFrames[m_resourceSlots[p_resourceId]];
		frame->m_frameState = p_animIndex;
		AnimsManager::DrawAnim(VsPoint(p_x, p_y), p_resourceId, m_reservedac, frame, p_remap);
		break;
	}
	}
}

// 68K 0x10b0c052 LoadAnimation__20CLemmingAnimsManagerFUl9ANIM_TYPE
// FUNCTION: LEMBALL 0x00434bc0
void LemmingAnimsManager::LoadAnimation(unsigned long p_resourceId, int p_animType)
{
	if (m_countingLoads != 0) {
		m_loadProgress++;
		return;
	}
	LoadAnims(p_resourceId);
	Frames* frame = 0;
	switch (p_animType) {
	case 0:
		frame = new StaticAnim();
		break;
	case 1:
		frame = new RepeatAnim(GetnAnims(p_resourceId), 1);
		break;
	case 2:
		frame = new Frames(1);
		break;
	case 3:
		frame = new PlayThruAnim(GetnAnims(p_resourceId), 1);
		break;
	}
	m_animFrames[m_resourceSlots[p_resourceId]] = frame;
	UpdateNonCacheLoad();
}

// 68K 0x10b0c1da UpdateNonCacheLoad__20CLemmingAnimsManagerFv
// FUNCTION: LEMBALL 0x00434d10
void LemmingAnimsManager::UpdateNonCacheLoad()
{
	int loaded = m_nonCacheState + 1;
	m_nonCacheState = loaded;
	if (m_loadAnim != 0) {
		m_loadAnim->m_progress.Draw((short) ((loaded * 100) / m_loadProgress));
	}
}

// 68K 0x10b0c244 LoadAnimation__20CLemmingAnimsManagerFUlUl9ANIM_TYPE
// FUNCTION: LEMBALL 0x00434d40
void LemmingAnimsManager::LoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId, int p_animType)
{
	if (m_countingLoads != 0) {
		m_loadProgress += p_lastResourceId - p_firstResourceId;
		return;
	}
	for (; (int) p_lastResourceId >= (int) p_firstResourceId; p_firstResourceId++) {
		LoadAnims(p_firstResourceId);
		Frames* frame = 0;
		switch (p_animType) {
		case 0:
			frame = new StaticAnim();
			break;
		case 1:
			frame = new RepeatAnim(GetnAnims(p_firstResourceId), 1);
			break;
		case 2:
			frame = new Frames(1);
			break;
		case 3:
			frame = new PlayThruAnim(GetnAnims(p_firstResourceId), 1);
			break;
		}
		m_animFrames[m_resourceSlots[p_firstResourceId]] = frame;
		UpdateNonCacheLoad();
	}
}

// 68K 0x10b0c3e4 UnLoadAnimation__20CLemmingAnimsManagerFUl
// FUNCTION: LEMBALL 0x00434ec0
void LemmingAnimsManager::UnLoadAnimation(unsigned long p_resourceId)
{
	if (m_animFrames[m_resourceSlots[p_resourceId]] != 0) {
		delete m_animFrames[m_resourceSlots[p_resourceId]];
		m_animFrames[m_resourceSlots[p_resourceId]] = 0;
	}
	UnLoadAnims(p_resourceId);
}

// 68K 0x10b0c46c UnLoadAnimation__20CLemmingAnimsManagerFUlUl
// FUNCTION: LEMBALL 0x00434f00
void LemmingAnimsManager::UnLoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId)
{
	for (; (int) p_lastResourceId >= (int) p_firstResourceId; p_firstResourceId++) {
		if (m_animFrames[m_resourceSlots[p_firstResourceId]] != 0) {
			delete m_animFrames[m_resourceSlots[p_firstResourceId]];
			m_animFrames[m_resourceSlots[p_firstResourceId]] = 0;
		}
		UnLoadAnims(p_firstResourceId);
	}
}
