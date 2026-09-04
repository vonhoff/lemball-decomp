#include "TheBalloonPost.h"

// 68K 0x106035ea __ct__15CTheBalloonPostF11eObjectTypeUc
// FUNCTION: LEMBALL 0x0042a5c0
TheBalloonPost::TheBalloonPost(eObjectType p_arg0, unsigned char p_arg1) : GameObject(p_arg0, 0, 10)
{
	m_active = *(unsigned int*) &p_arg1;
}

// 68K 0x101186cc __dt__15CTheBalloonPostFv
TheBalloonPost::~TheBalloonPost()
{
}
