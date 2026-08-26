#include "Key.h"

// 68K 0x1011ad1a Usage__4CKeyFv
// FUNCTION: LEMBALL 0x0041c5f0
int Key::Usage()
{
	return 2;
}

// 68K 0x1011ad36 __dt__4CKeyFv
// SYNTHETIC: LEMBALL 0x0041c600
// Key::`scalar deleting destructor'
Key::~Key()
{
}

// 68K 0x10619c40 Process__4CKeyFv
// STUB: LEMBALL 0x0041d480
bool Key::Process()
{
	return 0;
}

// 68K 0x10619d06 Activate__4CKeyFP11CGameObject
// STUB: LEMBALL 0x0041d560
bool Key::Activate(GameObject* p_object)
{
	return 0;
}

// 68K 0x10619d6e DoActivate__4CKeyFv
// STUB: LEMBALL 0x0041d5a0
void Key::DoActivate()
{
}

// 68K 0x10619dd0 ActivatePosition__4CKeyFv
// STUB: LEMBALL 0x0041d5d0
AiCoord Key::ActivatePosition()
{
	return *(AiCoord*) 0;
}
