#include "CLiftManager.h"

#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "CLift.h"

// FUNCTION: LEMBALL 0x00425d30
void CLiftManager::Process()
{
	int i = 0;
	CLiftManager* self = this;
	for (; i < self->m_count; i++) {
		self->m_lifts[i].m_requestEnabled = 1;
		self->m_lifts[i].Process();
		self->m_lifts[i].CheckObjects();
	}
}

// FUNCTION: LEMBALL 0x00425d80
void CLiftManager::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	CLiftManager* self = this;
	int i = 0;
	if (self->m_count > 0) {
		do {
			self->m_lifts[i].StepOn(p_position, p_object);
			i++;
		} while (i < self->m_count);
	}
}
