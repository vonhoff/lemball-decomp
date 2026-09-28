#include "CViewData.h"

#include "AI/Base/ObjectActions.h"

// FUNCTION: LEMBALL 0x00429e80
void CViewData::SetViewActionTuple(eAction p_action, unsigned int actionArgument, unsigned int p_stateTimer)
{
	m_action = p_action;
	m_actionArgument = actionArgument;
	m_stateTimer = p_stateTimer;
}
