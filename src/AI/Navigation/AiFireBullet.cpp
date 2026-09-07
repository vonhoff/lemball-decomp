#include "../Managers/BulletManager.h"
#include "Ai.h"

// 68K 0x10601cf6 FireBullet__3CAIFUs11eBulletType6eOwneri7AICOORD7AICOORD
// FUNCTION: LEMBALL 0x00412600
void Ai::FireBullet(unsigned short p_id,
					eBulletType p_bulletType,
					eOwner p_owner,
					int p_parameter,
					AiCoord p_start,
					AiCoord p_target)
{
	m_bulletManager->RequestBullet(p_id, p_bulletType, p_owner, p_parameter, p_start, p_target);
}
