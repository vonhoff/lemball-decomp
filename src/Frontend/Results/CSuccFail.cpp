#include "CSuccFail.h"

#include "Frontend/CBaseFrontendProcess.h"

// FUNCTION: LEMBALL 0x00450c10
CSuccFail::CSuccFail(CGame* p_game, unsigned int p_success) : CBaseFrontendProcess(p_game)
{
	m_variant = p_success;
}
