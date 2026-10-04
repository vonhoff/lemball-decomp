#include "C2D.h"

#include "Gameplay/Objects/CViewData.h"
#include "Views/Animation/CLemmingAnimsManager.h"

// FUNCTION: LEMBALL 0x00440460
void C2D::DrawZBuff_Sprite(int p_index, unsigned short p_z)
{
	m_lemmingAnims->m_primitiveSequence = p_z;
	DrawObject(m_viewData[p_index]);
}
