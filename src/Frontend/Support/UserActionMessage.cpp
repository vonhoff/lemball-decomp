#include "UserActionMessage.h"

// 68K 0x1080000c __ct__18CUserActionMessageFv
// FUNCTION: LEMBALL 0x004453b0
UserActionMessage::UserActionMessage() : NetworkMessage(8)
{
	m_payloadCapacity += 8;
	m_headerEnabled = 1;
}

// 68K 0x10800066 AddData__18CUserActionMessageFv
// FUNCTION: LEMBALL 0x004453e0
void UserActionMessage::AddData()
{
	Add((unsigned long) m_action);
	Add((unsigned long) m_stage);
}

// 68K 0x108000bc GetData__18CUserActionMessageFv
// FUNCTION: LEMBALL 0x00445400
void UserActionMessage::GetData()
{
	m_action = (eUserActions) GetDword();
	m_stage = (eUserActionStages) GetDword();
}

// 68K 0x1011bf5e __dt__18CUserActionMessageFv
UserActionMessage::~UserActionMessage()
{
}
