#include "CMasterInputItem.h"

// FUNCTION: LEMBALL 0x00472050
CMasterInputItem::CMasterInputItem()
{
	m_unk0x08 = 0;
}

// FUNCTION: LEMBALL 0x00472060
CVsOStream& CMasterInputItem::StreamOut(CVsOStream& p_stream)
{
	return p_stream;
}
