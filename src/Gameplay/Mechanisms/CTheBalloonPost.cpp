#include "CTheBalloonPost.h"

#include "Gameplay/Objects/CGameObject.h"

enum {
	BALLOON_POST_DESTINATION_CAPACITY = 10
};

// FUNCTION: LEMBALL 0x0042a5c0
CTheBalloonPost::CTheBalloonPost(eObjectType p_objectType, unsigned int p_active)
	: CGameObject(p_objectType, 0, BALLOON_POST_DESTINATION_CAPACITY)
{
	m_active = p_active;
}
