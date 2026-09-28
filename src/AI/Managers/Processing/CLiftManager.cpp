#include "AI/Managers/CLiftManager.h"

#include "AI/Objects/CLift.h"

// FUNCTION: LEMBALL 0x00425d30
void CLiftManager::Process()
{
	CLiftManager* self = this;
	for (int i = 0, offset = 0; i < self->m_count; i++, offset += sizeof(CLift)) {
		((CLift*) ((char*) self->m_lifts + offset))->m_requestEnabled = 1;
		((CLift*) ((char*) self->m_lifts + offset))->Process();
		((CLift*) ((char*) self->m_lifts + offset))->CheckObjects();
	}
}
