#include "ViewData.h"

// FUNCTION: LEMBALL 0x00429e80
void ViewData::SetViewActionTuple(eAction p_action, unsigned int p_argument, unsigned int p_stateTimer)
{
	m_action = p_action;
	m_actionArgument = p_argument;
	m_stateTimer = p_stateTimer;
}
