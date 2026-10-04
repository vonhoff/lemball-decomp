#include "CGameObject.h"

// FUNCTION: LEMBALL 0x00413050
void ReindexAllObjects()
{
	unsigned int count = g_wObjectCount;
	for (unsigned int index = 0; index < count; ++index) {
		CGameObject* object = g_pObjects[index & OBJECT_ID_MASK];
		if (object != 0 && object->m_objectId != index) {
			object->m_objectId = (unsigned short) index;
		}
	}
}
