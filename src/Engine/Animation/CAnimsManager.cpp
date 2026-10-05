#include "CAnimsManager.h"

#include "Engine/Math/CVSPoint.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Platform/Windows/Graphics/CSurface.h"
#include "Engine/Graphics/Primitives/CZRLE.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "Engine/Resources/Types/CResBase.h"
#include "Engine/Resources/Types/CResBaseLIST.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "../Resources/ResourceChunkTypes.h"
#include "CAnim.h"
#include "Engine/Animation/CAnimFrameBASE.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"

enum eAnimationBufferHalf {
	ANIMATION_BUFFER_HALF_FIRST = 0,
	ANIMATION_BUFFER_HALF_ALTERNATE = 1
};

#include "Engine/Resources/ResourceChunkTypes.h"
#include "CAnimFrameBASE.h"

// FUNCTION: LEMBALL 0x004358c0
void CAnimsManager::FreeVram()
{
}

// FUNCTION: LEMBALL 0x0044e700
CVSRect* CAnimsManager::DrawAnimOnGdi(CVSRect* p_bounds,
									  CGDI* p_gdi,
									  const CVSPoint& p_position,
									  unsigned long p_resourceId,
									  unsigned long p_drawFlags,
									  CAnimFrameBASE* p_frame,
									  CRemap* p_remap)
{
	CGDI* previous = m_gdi;
	m_gdi = p_gdi;
	CVSRect bounds = DrawAnim(p_position, p_resourceId, p_drawFlags, p_frame, p_remap);
	m_gdi = previous;
	p_bounds->m_width = bounds.m_width;
	p_bounds->m_height = bounds.m_height;
	p_bounds->m_x = bounds.m_x;
	p_bounds->m_y = bounds.m_y;
	return p_bounds;
}

// FUNCTION: LEMBALL 0x00467260
CAnimsManager::CAnimsManager(CGDI* p_gdi,
							 unsigned long p_resourceIdCount,
							 int p_resourceCapacity,
							 int p_animCapacity,
							 int p_zrleCapacity,
							 unsigned int p_doubleBuffered)
{
	int i;

	m_animCount = 0;
	m_zrleCount = 0;
	m_gdi = p_gdi;
	m_resourceCapacity = p_resourceCapacity;
	m_resourceIdCount = (int) p_resourceIdCount;
	m_animPrimitives = NULL;
	m_zrlePrimitives = NULL;
	m_previousGdi = NULL;
	m_doubleBuffered = p_doubleBuffered;
	m_loadedResourceCount = 0;
	m_reserved6c = 0xffffffff;
	m_resources = new CResBase*[m_resourceCapacity];
	m_resourceSlots = new short[m_resourceIdCount];
	for (i = 0; i < m_resourceCapacity; i++) {
		m_resources[i] = NULL;
	}
	for (i = 0; i < m_resourceIdCount; i++) {
		m_resourceSlots[i] = (short) m_resourceCapacity;
	}
	if (m_doubleBuffered != 0) {
		m_bufferHalf = ANIMATION_BUFFER_HALF_FIRST;
		m_zrleCapacity = p_zrleCapacity * 2;
		m_bufferedZrleCount = 0;
		m_bufferedAnimCount = 0;
		m_animCapacity = p_animCapacity * 2;
	}
	else {
		m_zrleCapacity = p_zrleCapacity;
		m_animCapacity = p_animCapacity;
	}
	if (m_zrleCapacity != 0) {
		m_zrlePrimitives = new CZRLE[m_zrleCapacity];
	}
	if (m_animCapacity != 0) {
		m_animPrimitives = new CAnim[m_animCapacity];
	}
	m_ownsLinePrimitives = 0;
	m_linePrimitives = NULL;
	ResetPrimitives();
}

// FUNCTION: LEMBALL 0x004673d0
CAnimsManager::~CAnimsManager()
{
	int i;
	int scanned;
	CResBase** resources;

	i = 0;
	if (m_loadedResourceCount != 0 && (scanned = 0, 0 < m_loadedResourceCount)) {
		do {
			resources = m_resources;
			while (resources[i] == NULL) {
				i = i + 1;
			}
			resources[i]->UnLoad();
			scanned = scanned + 1;
			i = i + 1;
		} while (scanned < m_loadedResourceCount);
	}
	if (m_resources != NULL) {
		delete[] m_resources;
		m_resources = NULL;
	}
	if (m_resourceSlots != NULL) {
		delete[] m_resourceSlots;
		m_resourceSlots = NULL;
	}
	if (m_zrlePrimitives != NULL) {
		delete[] m_zrlePrimitives;
		m_zrlePrimitives = NULL;
	}
	if (m_animPrimitives != NULL) {
		delete[] m_animPrimitives;
		m_animPrimitives = NULL;
	}
	if (m_ownsLinePrimitives != 0 && m_linePrimitives != NULL) {
		delete[] m_linePrimitives;
		m_linePrimitives = NULL;
	}
}

// FUNCTION: LEMBALL 0x00467490
void CAnimsManager::LoadAnims(unsigned long p_resourceId)
{
	CAnimsManager* owner = this;
	unsigned long resourceId = p_resourceId;
	int slot = 0;
	if (owner->m_resourceCapacity != owner->m_resourceSlots[resourceId]) {
		slot = owner->m_resourceSlots[resourceId];
	}
	else {
		while (owner->m_resources[slot] != NULL) {
			slot = slot + 1;
		}
	}
	owner->m_resources[slot] = CResANIM::Load(resourceId);
	CResBase*& resource = owner->m_resources[slot];
	if (resource == NULL) {
		resource = CResZRLE::Load(resourceId);
	}
	if ((int) owner->m_resourceSlots[resourceId] == owner->m_resourceCapacity) {
		owner->m_resourceSlots[resourceId] = (short) slot;
		owner->m_loadedResourceCount = owner->m_loadedResourceCount + 1;
	}
}

// FUNCTION: LEMBALL 0x00467500
void CAnimsManager::UnLoadAnims(unsigned long p_resourceId)
{
	m_resources[m_resourceSlots[p_resourceId]]->UnLoad();
	m_resources[m_resourceSlots[p_resourceId]] = NULL;
	m_resourceSlots[p_resourceId] = (short) m_resourceCapacity;
	m_loadedResourceCount = m_loadedResourceCount - 1;
}

// FUNCTION: LEMBALL 0x00467540
unsigned long CAnimsManager::GetnAnims(unsigned long p_resourceId)
{
	CResBase* resource;

	resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType == RESOURCE_CHUNK_ZRLE) {
		return 1;
	}
	return ((CResBaseLIST*) resource)->m_totalSize;
}

// FUNCTION: LEMBALL 0x00467570
CVSSize CAnimsManager::GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex)
{
	CVSSize size;
	CResBase* resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType != RESOURCE_CHUNK_ZRLE) {
		CResZRLE* entry = ((CResANIM*) resource)->m_animationEntries + p_animIndex;
		size.m_width = entry->m_width;
		size.m_height = entry->m_height;
	}
	else {
		CResZRLE* entry = (CResZRLE*) resource;
		size.m_width = entry->m_width;
		size.m_height = entry->m_height;
	}
	return size;
}

// FUNCTION: LEMBALL 0x004675d0
CVSSize CAnimsManager::GetMaxAnimSize(unsigned long p_resourceId)
{
	CVSSize maxSize;
	short& maximumWidth = maxSize.m_width;
	short& maximumHeight = maxSize.m_height;
	unsigned long animCount = GetnAnims(p_resourceId);
	unsigned long animIndex = 0;

	if (animCount != 0) {
		do {
			CVSSize animSize = GetAnimSize(p_resourceId, animIndex);
			if (maximumWidth < animSize.m_width) {
				maximumWidth = animSize.m_width;
			}
			if (maximumHeight < animSize.m_height) {
				maximumHeight = animSize.m_height;
			}
			++animIndex;
			animCount = GetnAnims(p_resourceId);
		} while (animIndex < animCount);
	}

	return maxSize;
}

// FUNCTION: LEMBALL 0x00467660
CVSSize CAnimsManager::GetMaxAnimHalfSize(unsigned long p_resourceId)
{
	const CVSSize& size = GetMaxAnimSize(p_resourceId);
	short height = (short) (size.m_height / 2);
	return CVSSize((short) (size.m_width / 2), height);
}

// FUNCTION: LEMBALL 0x004676a0
CResZRLE* CAnimsManager::ResolveAnimFrameData(unsigned long p_resourceId, CAnimFrameBASE* p_frame)
{
	CResBase* resource = m_resources[m_resourceSlots[p_resourceId]];
	unsigned int frame;
	if (p_frame != NULL) {
		frame = p_frame->GetFrameNo();
	}
	else {
		frame = 0;
	}
	if (resource->m_chunkType == RESOURCE_CHUNK_ZRLE) {
		return (CResZRLE*) resource;
	}
	return ((CResANIM*) resource)->m_animationEntries + frame;
}

// FUNCTION: LEMBALL 0x00467700
void CAnimsManager::DetachGdi(CGDI* p_gdi)
{
	if (m_previousGdi == p_gdi) {
		m_previousGdi = NULL;
	}
	if (m_gdi == p_gdi) {
		m_previousGdi = NULL;
		m_gdi = NULL;
	}
}

// FUNCTION: LEMBALL 0x00467730
CVSRect CAnimsManager::DrawAnim(const CVSPoint& p_position,
								unsigned long p_resourceId,
								unsigned long p_drawFlags,
								CAnimFrameBASE* p_frame,
								CRemap* p_remap)
{
	CResBase* resource;
	unsigned int frameIndex;
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

// FUNCTION: LEMBALL 0x004678c0
void CAnimsManager::ResetPrimitives()
{
	m_resetState = 0;
	if (m_doubleBuffered == 0) {
		m_zrleCount = 0;
		struct PrimitiveState {
			int m_count;
			char m_drawMark[4];
		};
		PrimitiveState* animState = (PrimitiveState*) &m_animCount;
		PrimitiveState* zrleState = (PrimitiveState*) &m_zrleCount;
		*animState = *zrleState;
		unsigned char* drawMark = (unsigned char*) m_gdi->m_renderTarget->GetCurrDB();
		m_animDrawMark[0] = *drawMark;
		m_zrleDrawMark[0] = *drawMark;
		return;
	}
	m_gdi->Render();
	m_gdi->m_primitiveCount = 0;
	m_bufferedZrleCount = 0;
	m_bufferedAnimCount = 0;
	m_bufferHalf ^= ANIMATION_BUFFER_HALF_ALTERNATE;
}
