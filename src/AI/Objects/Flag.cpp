#include "Flag.h"

#include "../Navigation/Ai.h"

// 68K 0x10605e54 SetSFX__5CFlagFv
// FUNCTION: LEMBALL 0x00422b30
void Flag::SetSfx()
{
	SetSndEffect(2);
}

// 68K 0x10605e84 Process__5CFlagFv
// STUB: LEMBALL 0x00422b40
bool Flag::Process()
{
	return 0;
}

// 68K 0x10605f2e Collected__5CFlagFv
// FUNCTION: LEMBALL 0x00422c00
int Flag::Collected()
{
	m_activator->AddObject(m_objectType, this);
	g_pAI->Score(2500);
	g_pAI->m_flagCounts[0]--;
	return 1;
}

// 68K 0x10119982 __dt__5CFlagFv
Flag::~Flag()
{
}
