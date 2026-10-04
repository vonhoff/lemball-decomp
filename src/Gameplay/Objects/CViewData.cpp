#include "CViewData.h"

#include "ObjectActions.h"

// FUNCTION: LEMBALL 0x00429e80
void CViewData::SetViewActionTuple(eAction p_action, unsigned int p_argument, unsigned int p_stateTimer)
{
	m_action = p_action;
	m_actionArgument = p_argument;
	m_stateTimer = p_stateTimer;
}

// FUNCTION: LEMBALL 0x0043ff60
int ViewDataCmp(const void* p_left, const void* p_right)
{
	const CViewData* left;
	const CViewData* right;

	left = (const CViewData*) p_left;
	right = (const CViewData*) p_right;
	return (int) left->m_sortZKey - (int) right->m_sortZKey;
}
