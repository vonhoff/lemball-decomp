#include "TargetMasterInputItem.h"

// FUNCTION: LEMBALL 0x00472050
TargetMasterInputItem::TargetMasterInputItem()
{
	m_unk0x08 = 0;
}

// FUNCTION: LEMBALL 0x00472060
CVsOStream& TargetMasterInputItem::StreamOut(CVsOStream& p_stream)
{
	return p_stream;
}
