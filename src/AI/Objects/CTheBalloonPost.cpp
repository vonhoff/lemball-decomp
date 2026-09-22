#include "CTheBalloonPost.h"

#include "AI/Base/CGameObject.h"

// FUNCTION: LEMBALL 0x0042a5c0
CTheBalloonPost::CTheBalloonPost(eObjectType p_arg0, unsigned int p_arg1) : CGameObject(p_arg0, 0, 10)
{
	m_active = p_arg1;
}
