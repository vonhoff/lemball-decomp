#include "Crate.h"

// 68K 0x1011ab0e Usage__6CCrateFv
// FUNCTION: LEMBALL 0x0041c530
int Crate::Usage()
{
	return 2;
}

// 68K 0x1011aa7e __dt__6CCrateFv
Crate::~Crate()
{
}

// 68K 0x1061918c Restart__6CCrateFv
// FUNCTION: LEMBALL 0x0041cca0
void Crate::Restart()
{
	BaseGlobalObject::Restart();
	m_pendingAction = (eAction) 0x18;
}

// 68K 0x106191bc TriggerContents__6CCrateFv
// STUB: LEMBALL 0x0041ccc0
void Crate::TriggerContents()
{
}

// 68K 0x10619230 Process__6CCrateFv
// STUB: LEMBALL 0x0041cd20
bool Crate::Process()
{
	return 0;
}

// 68K 0x10619384 Activate__6CCrateFP11CGameObject
// STUB: LEMBALL 0x0041ce50
bool Crate::Activate(GameObject* p_object)
{
	return 0;
}

// 68K 0x106193e0 DoActivate__6CCrateFv
// STUB: LEMBALL 0x0041ce90
void Crate::DoActivate()
{
}

// 68K 0x1061947a ActivatePosition__6CCrateFv
// STUB: LEMBALL 0x0041cf10
AiCoord Crate::ActivatePosition()
{
	return *(AiCoord*) 0;
}

