#include "CAnimsManager.h"
#include "Visos/Resources/ResourceChunkTypes.h"

#include "Visos/Math/CVSPoint.h"
#include "Visos/Graphics/Primitives/CGDI.h"
#include "Visos/Graphics/Surfaces/CSurface.h"
#include "Visos/Graphics/Primitives/CZRLE.h"
#include "Visos/Resources/Types/CResANIM.h"
#include "Visos/Resources/Types/CResBase.h"
#include "Visos/Resources/Types/CResZRLE.h"
#include "CAnim.h"
#include "CAnimFrameBASE.h"
#include "Visos/Math/CVSRect.h"

// FUNCTION: LEMBALL 0x00467730
CVSRect CAnimsManager::DrawAnim(const CVSPoint& p_position,
								unsigned long p_resourceId,
								unsigned long p_drawFlags,
								CAnimFrameBASE* p_frame,
								CRemap* p_remap)
{
	unsigned int frameIndex;
	CResBase* resource;
	CResZRLE* sizeSource;
	CZRLE* zrle;
	CAnim* anim;
	CGDI* previous;
	CGDI* current;

	if (m_doubleBuffered != 0) {
		current = m_gdi;
		previous = m_previousGdi;
		if (current != previous && previous != NULL) {
			m_gdi = previous;
			ResetPrimitives();
			m_gdi = current;
		}
	}
	m_previousGdi = m_gdi;
	resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType == RESOURCE_CHUNK_ZRLE) {
		sizeSource = (CResZRLE*) resource;
		if (m_doubleBuffered != 0) {
			if (m_zrleCapacity == m_bufferedZrleCount) {
				ResetPrimitives();
			}
			zrle = m_zrlePrimitives + m_bufferHalf + m_bufferedZrleCount;
			m_bufferedZrleCount += 2;
		}
		else {
			zrle = m_zrlePrimitives + m_zrleCount++;
		}
		zrle->m_state = m_primitiveSequence;
		zrle->m_x = p_position.m_x;
		zrle->m_y = p_position.m_y;
		zrle->m_resource = resource;
		zrle->m_flags = p_drawFlags;
		zrle->m_remap = p_remap;
		zrle->Draw(m_gdi);
	}
	else {
		if (p_frame != NULL) {
			frameIndex = p_frame->GetFrameNo();
			p_frame->m_reserved08 = frameIndex;
		}
		else {
			frameIndex = 0;
		}
		sizeSource = ((CResANIM*) resource)->m_animationEntries + frameIndex;
		if (m_doubleBuffered != 0) {
			if (m_animCapacity == m_bufferedAnimCount) {
				ResetPrimitives();
			}
			anim = m_animPrimitives + m_bufferHalf + m_bufferedAnimCount;
			m_bufferedAnimCount += 2;
		}
		else {
			anim = m_animPrimitives + m_animCount++;
		}
		anim->m_state = m_primitiveSequence;
		anim->m_x = p_position.m_x;
		anim->m_y = p_position.m_y;
		anim->m_animResource = (CResANIM*) resource;
		anim->m_animIndex = frameIndex;
		anim->m_flags = p_drawFlags;
		anim->m_remap = p_remap;
		anim->Draw(m_gdi);
	}
	return CVSRect(sizeSource->m_x, sizeSource->m_y, sizeSource->m_width, sizeSource->m_height);
}
