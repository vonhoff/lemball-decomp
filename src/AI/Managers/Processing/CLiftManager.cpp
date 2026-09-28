#include "AI/Managers/CLiftManager.h"

#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Objects/CLift.h"

// FUNCTION: LEMBALL 0x00425d30
void CLiftManager::Process()
{
	int offset = 0;
	int i = 0;
	CLiftManager* self = this;
	for (; i < self->m_count; offset += sizeof(CLift), i++) {
		((CLift*) ((char*) self->m_lifts + offset))->m_requestEnabled = 1;
		((CLift*) ((char*) self->m_lifts + offset))->Process();
		((CLift*) ((char*) self->m_lifts + offset))->CheckObjects();
	}
}

// FUNCTION: LEMBALL 0x00425d80
void CLiftManager::StepOn(const AiCoord& p_position, CGameObject* p_object)
{
	CLiftManager* self = this;
	int offset;
	int i = 0;
	if (self->m_count > 0) {
		offset = 0;
		do {
			((CLift*) ((char*) self->m_lifts + offset))->StepOn(p_position, p_object);
			offset += sizeof(CLift);
			i++;
		} while (i < self->m_count);
	}
}
