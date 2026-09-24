#include "../CMap.h"

#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x00430db0
void CMap::LoadLevel(LoadGroundSurfaceData* p_data, unsigned long p_dataSize, unsigned char p_skip)
{
	int width = ((unsigned short*) p_data)[0];
	int height = ((unsigned short*) p_data)[1];
	p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[2];

	m_ground.Clear();
	ReSize(width, height);

	for (int y = 0; height > y; y++) {
		for (int x = 0; width > x; x++) {
			eObjectType objectType = (eObjectType) * (unsigned short*) p_data;
			p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[1];
			unsigned short objectData = *(unsigned short*) p_data;
			p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[1];
			unsigned short groundHeight = *(unsigned short*) p_data;
			p_data = (LoadGroundSurfaceData*) &((unsigned short*) p_data)[1];

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
