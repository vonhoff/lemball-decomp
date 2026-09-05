#include "LemmingAnimsManager.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Frontend/Resources/CdLoadAnim.h"
#include "../../Frontend/Resources/FrontendResourceLoader.h"
#include "../../Visos/Resources/Manifest.h"
#include "../../Visos/Resources/MogRes.h"
#include "../../Visos/Resources/ResFont.h"

// Style-sensitive resource IDs written by SetupStyleSensitive (0x00432c80).
// GLOBAL: LEMBALL 0x004a7850
unsigned int g_unk004a7850;
// GLOBAL: LEMBALL 0x004a7854
unsigned int g_unk004a7854;
// GLOBAL: LEMBALL 0x004a7858
unsigned int g_unk004a7858;
// GLOBAL: LEMBALL 0x004a785c
unsigned int g_unk004a785c;
// GLOBAL: LEMBALL 0x004a7860
unsigned int g_unk004a7860;
// GLOBAL: LEMBALL 0x004a7864
unsigned int g_unk004a7864;
// GLOBAL: LEMBALL 0x004a7868
unsigned int g_unk004a7868;

// 68K 0x10b09d40 __ct__20CLemmingAnimsManagerFP4CGDIP14CMain2DDisplayP3CAI
// STUB: LEMBALL 0x00432b50
LemmingAnimsManager::LemmingAnimsManager(Gdi* p_arg0, Main2DDisplay* p_arg1, Ai* p_arg2)
{
}

// 68K 0x10b09e58 __dt__20CLemmingAnimsManagerFv
// STUB: LEMBALL 0x00432c20
LemmingAnimsManager::~LemmingAnimsManager()
{
}

// 68K 0x10b09f1a SetupStyleSensitive__20CLemmingAnimsManagerFv
// STUB: LEMBALL 0x00432c80
void LemmingAnimsManager::SetupStyleSensitive()
{
}

// 68K 0x10b0a2ae LoadVrammed__20CLemmingAnimsManagerFv
// STUB: LEMBALL 0x00432fe0
void LemmingAnimsManager::LoadVrammed()
{
}

// 68K 0x10b0a920 LoadMainRammed__20CLemmingAnimsManagerFv
// STUB: LEMBALL 0x004334f0
void LemmingAnimsManager::LoadMainRammed()
{
}

// 68K 0x10b0ad4c Load__20CLemmingAnimsManagerF7GROUNDS
// STUB: LEMBALL 0x004337f0
void LemmingAnimsManager::Load(int p_groundStyle)
{
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
		if (g_unk004a7850 != 0) {
			UnLoadAnimation(g_unk004a7850);
		}
		if (g_unk004a7854 != 0) {
			UnLoadAnimation(g_unk004a7854);
		}
		if (g_unk004a7858 != 0) {
			UnLoadAnimation(g_unk004a7858);
		}
		if (g_unk004a785c != 0) {
			UnLoadAnimation(g_unk004a785c);
		}
		if (g_unk004a7860 != 0) {
			UnLoadAnimation(g_unk004a7860);
		}
		if (g_unk004a7864 != 0) {
			UnLoadAnimation(g_unk004a7864);
		}
		if (g_unk004a7868 != 0) {
			UnLoadAnimation(g_unk004a7868);
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
// STUB: LEMBALL 0x00433fc0
void LemmingAnimsManager::DrawAnim(short p_x,
								   short p_y,
								   unsigned long p_resourceId,
								   unsigned long p_animIndex,
								   unsigned long p_time,
								   Remap* p_remap)
{
}

// 68K 0x10b0c052 LoadAnimation__20CLemmingAnimsManagerFUl9ANIM_TYPE
// STUB: LEMBALL 0x00434bc0
void LemmingAnimsManager::LoadAnimation(unsigned long p_resourceId, int p_animType)
{
}

// 68K 0x10b0c1da UpdateNonCacheLoad__20CLemmingAnimsManagerFv
// STUB: LEMBALL 0x00434d10
void LemmingAnimsManager::UpdateNonCacheLoad()
{
}

// 68K 0x10b0c244 LoadAnimation__20CLemmingAnimsManagerFUlUl9ANIM_TYPE
// STUB: LEMBALL 0x00434d40
void LemmingAnimsManager::LoadAnimation(unsigned long p_firstResourceId, unsigned long p_lastResourceId, int p_animType)
{
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
