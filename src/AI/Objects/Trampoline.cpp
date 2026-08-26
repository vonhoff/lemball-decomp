#include "Trampoline.h"

// 68K 0x10620548 __ct__11CTrampolineFv
// STUB: LEMBALL 0x0042a990
Trampoline::Trampoline()
{
}

// 68K 0x1062058e Restart__11CTrampolineFv
// STUB: LEMBALL 0x0042a9b0
void Trampoline::Restart()
{
}

// 68K 0x1062062e Set__11CTrampolineFUsRC7AICOORD
// STUB: LEMBALL 0x0042a9e0
void Trampoline::Set(unsigned short p_id, const AiCoord& p_position)
{
}

// 68K 0x106206e2 Process__11CTrampolineFv
// STUB: LEMBALL 0x0042aa80
bool Trampoline::Process()
{
	return 0;
}

// 68K 0x10620788 Hit__11CTrampolineFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0042ab90
int Trampoline::Hit(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x1011b5d2 DoActivate__11CTrampolineFv
// FUNCTION: LEMBALL 0x0042b9b0
void Trampoline::DoActivate()
{
}

// 68K 0x106205d0 __dt__11CTrampolineFv
// SYNTHETIC: LEMBALL 0x0042ba40
// Trampoline::`scalar deleting destructor'
Trampoline::~Trampoline()
{
}
