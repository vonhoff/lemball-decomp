#include "LemmingAnimsManager.h"

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
// STUB: LEMBALL 0x004338b0
void LemmingAnimsManager::Unload()
{
}

// 68K 0x10b0b6b6 Draw__20CLemmingAnimsManagerFv
// STUB: LEMBALL 0x00433fb0
void LemmingAnimsManager::Draw()
{
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
