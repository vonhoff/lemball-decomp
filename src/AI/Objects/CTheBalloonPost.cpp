#include "CTheBalloonPost.h"

#include "AI/Base/CGameObject.h"

// FUNCTION: LEMBALL 0x0042a5c0
CTheBalloonPost::CTheBalloonPost(eObjectType p_objectType, unsigned int p_active) : CGameObject(p_objectType, 0, 10)
{
	m_active = p_active;
}
