#include "CMap.h"

#include "Gameplay/Objects/ObjectTypes.h"
#include "tagLoadGroundSurfaceData.h"
#include "CGround.h"
#include "CGroundArray.h"

// FUNCTION: LEMBALL 0x00430db0
void CMap::LoadLevel(tagLoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
	unsigned short* data;
	int x;
	int y;
	int width = p_data->m_width;
	int height = p_data->m_height;
	data = (unsigned short*) (p_data + 1);

	m_ground.Clear();
	ReSize(width, height);

	for (y = 0; height > y; y++) {
		for (x = 0; width > x; x++) {
			eObjectType objectType = (eObjectType) *data++;
			unsigned short objectData = *data++;
			unsigned short groundHeight = *data++;

			CGround* ground = m_ground.m_ground + m_ground.m_width * y + x;
			ground->m_objectType = objectType;
			ground->m_objectData = objectData;
			ground->SetCollision();

			m_ground.m_ground[m_ground.m_width * y + x].m_height = groundHeight;
		}
	}

	CreateWalkBits();
	CalculateCliff();
}
