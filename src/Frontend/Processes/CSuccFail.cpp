#include "CSuccFail.h"

#include "../Base/CBaseFrontendProcess.h"

// FUNCTION: LEMBALL 0x00450c10
CSuccFail::CSuccFail(CGame* p_arg0, unsigned int p_arg1) : CBaseFrontendProcess(p_arg0)
{
	m_variant = p_arg1;
}
