#include "TargetMasterInputItem.h"

// FUNCTION: LEMBALL 0x00472050
TargetMasterInputItem::TargetMasterInputItem()
{
	m_unk0x08 = 0;
}

// FUNCTION: LEMBALL 0x00472060
VsOStream& TargetMasterInputItem::StreamOut(VsOStream& p_stream)
{
	return p_stream;
}
