#include "TransportObjectMess.h"

// 68K 0x1011855a GetData__20CTransportObjectMessFv
// FUNCTION: LEMBALL 0x00417d30
void TransportObjectMess::GetData()
{
	m_transportObjectId = GetWord();
}

// 68K 0x1011859a __dt__20CTransportObjectMessFv
// SYNTHETIC: LEMBALL 0x00417d50
// TransportObjectMess::`scalar deleting destructor'
TransportObjectMess::~TransportObjectMess()
{
}
