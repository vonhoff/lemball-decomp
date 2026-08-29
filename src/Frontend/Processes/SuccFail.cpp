#include "SuccFail.h"

#include "../Base/BaseFrontendProcess.h"

// 68K 0x10809e36 __ct__9CSuccFailFP5CGameUc
// FUNCTION: LEMBALL 0x00450c10
SuccFail::SuccFail(Game* p_arg0, unsigned int p_arg1)
	: BaseFrontendProcess(p_arg0)
{
	m_variant = p_arg1;
}

// 68K 0x1011ba68 __dt__9CSuccFailFv
SuccFail::~SuccFail()
{
}

