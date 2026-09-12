#include "AnimsManager.h"

#include "../Foundation/VsPoint.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/Line.h"
#include "../Graphics/VsGdi.h"
#include "../Graphics/Zrle.h"
#include "../Resources/ResAnim.h"
#include "../Resources/ResBase.h"
#include "../Resources/ResBaseList.h"
#include "../Resources/ResZrle.h"
#include "Anim.h"
#include "Frames.h"

#include <new.h>
#include <string.h>

AnimsManager::AnimsManager()
{
}

// 68K 0x10115d70 FreeVram__13CAnimsManagerFv
// FUNCTION: LEMBALL 0x004358c0
void AnimsManager::FreeVram()
{
}

// 68K 0x1020019c __ct__13CAnimsManagerFP4CGDIUliiiUc
// FUNCTION: LEMBALL 0x00467260
AnimsManager::AnimsManager(Gdi* p_gdi,
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
	m_animPrimitives = 0;
	m_zrlePrimitives = 0;
	m_previousGdi = 0;
	m_doubleBuffered = p_doubleBuffered;
	m_loadedResourceCount = 0;
	m_reserved6c = 0xffffffff;
	m_resources = (ResBase**) operator new(m_resourceCapacity * 4);
	m_resourceSlots = (short*) operator new(m_resourceIdCount * 2);
	for (i = 0; i < m_resourceCapacity; i++) {
		m_resources[i] = 0;
	}
	for (i = 0; i < m_resourceIdCount; i++) {
		m_resourceSlots[i] = (short) m_resourceCapacity;
	}
	if (m_doubleBuffered != 0) {
		m_bufferHalf = 0;
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
		m_zrlePrimitives = new Zrle[m_zrleCapacity];
	}
	if (m_animCapacity != 0) {
		m_animPrimitives = new Anim[m_animCapacity];
	}
	m_ownsLinePrimitives = 0;
	m_linePrimitives = 0;
	ResetPrimitives();
}

// 68K 0x10200334 __dt__13CAnimsManagerFv
// FUNCTION: LEMBALL 0x004673d0
AnimsManager::~AnimsManager()
{
	int i;
	int scanned;
	ResBase** resources;

	i = 0;
	if (m_loadedResourceCount != 0 && (scanned = 0, 0 < m_loadedResourceCount)) {
		do {
			resources = m_resources;
			while (resources[i] == 0) {
				i = i + 1;
			}
			resources[i]->UnLoad();
			scanned = scanned + 1;
			i = i + 1;
		} while (scanned < m_loadedResourceCount);
	}
	if (m_resources != 0) {
		operator delete(m_resources);
		m_resources = 0;
	}
	if (m_resourceSlots != 0) {
		operator delete(m_resourceSlots);
		m_resourceSlots = 0;
	}
	if (m_zrlePrimitives != 0) {
		delete[] m_zrlePrimitives;
		m_zrlePrimitives = 0;
	}
	if (m_animPrimitives != 0) {
		delete[] m_animPrimitives;
		m_animPrimitives = 0;
	}
	if (m_ownsLinePrimitives != 0 && m_linePrimitives != 0) {
		delete[] m_linePrimitives;
		m_linePrimitives = 0;
	}
}

// 68K 0x1020043c LoadAnims__13CAnimsManagerFUl
// FUNCTION: LEMBALL 0x00467490
void AnimsManager::LoadAnims(unsigned long p_resourceId)
{
	int slot;

	slot = m_resourceSlots[p_resourceId];
	if (m_resourceCapacity == slot) {
		slot = 0;
		while (m_resources[slot] != 0) {
			slot = slot + 1;
		}
	}
	m_resources[slot] = ResAnim::Load(p_resourceId);
	if (m_resources[slot] == 0) {
		m_resources[slot] = ResZrle::Load(p_resourceId);
	}
	if ((int) m_resourceSlots[p_resourceId] == m_resourceCapacity) {
		m_resourceSlots[p_resourceId] = (short) slot;
		m_loadedResourceCount = m_loadedResourceCount + 1;
	}
}

// 68K 0x102004ea UnLoadAnims__13CAnimsManagerFUl
// FUNCTION: LEMBALL 0x00467500
void AnimsManager::UnLoadAnims(unsigned long p_resourceId)
{
	m_resources[m_resourceSlots[p_resourceId]]->UnLoad();
	m_resources[m_resourceSlots[p_resourceId]] = 0;
	m_resourceSlots[p_resourceId] = (short) m_resourceCapacity;
	m_loadedResourceCount = m_loadedResourceCount - 1;
}

// 68K 0x1020055a GetnAnims__13CAnimsManagerFUl
// FUNCTION: LEMBALL 0x00467540
unsigned long AnimsManager::GetnAnims(unsigned long p_resourceId)
{
	ResBase* resource;

	resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType == 0x5a524c45) {
		return 1;
	}
	return ((ResBaseList*) resource)->m_totalSize;
}

// 68K 0x102005b8 GetAnimSize__13CAnimsManagerFUlUl
// FUNCTION: LEMBALL 0x00467570
VsSize AnimsManager::GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex)
{
	VsSize size;
	ResBase* resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType != 0x5a524c45) {
		ResZrle* entry = ((ResAnim*) resource)->m_animationEntries + p_animIndex;
		size.m_width = entry->m_width;
		size.m_height = entry->m_height;
	}
	else {
		ResZrle* entry = (ResZrle*) resource;
		size.m_width = entry->m_width;
		size.m_height = entry->m_height;
	}
	return size;
}

// 68K 0x1020064e DrawAnim__13CAnimsManagerFRC8CVSPointUlUlP14CAnimFrameBASEP6CRemap
// FUNCTION: LEMBALL 0x00467730
VsRect AnimsManager::DrawAnim(const VsPoint& p_position,
							  unsigned long p_resourceId,
							  unsigned long p_animIndex,
							  Frames* p_frame,
							  Remap* p_remap)
{
	VsRect result;
	ResBase* resource;
	ResZrle* sizeSource;
	Zrle* zrle;
	Anim* anim;
	unsigned int frameIndex;
	int index;
	Gdi* current;

	if (m_doubleBuffered != 0) {
		current = m_gdi;
		if (current != m_previousGdi && m_previousGdi != 0) {
			m_gdi = m_previousGdi;
			ResetPrimitives();
			m_gdi = current;
		}
	}
	m_previousGdi = m_gdi;
	resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType == 0x5a524c45) {
		if (m_doubleBuffered == 0) {
			index = m_zrleCount;
			m_zrleCount = index + 1;
			zrle = m_zrlePrimitives + index;
		}
		else {
			if (m_zrleCapacity == m_bufferedZrleCount) {
				ResetPrimitives();
			}
			index = m_bufferedZrleCount;
			m_bufferedZrleCount = index + 2;
			zrle = m_zrlePrimitives + m_bufferHalf + index;
		}
		zrle->m_state = m_primitiveSequence;
		zrle->m_x = p_position.m_x;
		zrle->m_y = p_position.m_y;
		zrle->m_resource = resource;
		zrle->m_flags = p_animIndex;
		zrle->m_remap = p_remap;
		zrle->Draw(m_gdi);
		sizeSource = (ResZrle*) resource;
	}
	else {
		frameIndex = 0;
		if (p_frame != 0) {
			frameIndex = ((Frames*) p_frame)->GetFrameNo();
			((Frames*) p_frame)->m_reserved08 = frameIndex;
		}
		sizeSource = (ResZrle*) ((char*) ((ResAnim*) resource)->m_animationEntries + frameIndex * 0x54);
		if (m_doubleBuffered == 0) {
			index = m_animCount;
			m_animCount = index + 1;
			anim = m_animPrimitives + index;
		}
		else {
			if (m_animCapacity == m_bufferedAnimCount) {
				ResetPrimitives();
			}
			index = m_bufferedAnimCount;
			m_bufferedAnimCount = index + 2;
			anim = m_animPrimitives + m_bufferHalf + index;
		}
		anim->m_state = m_primitiveSequence;
		anim->m_x = p_position.m_x;
		anim->m_y = p_position.m_y;
		anim->m_animResource = (ResAnim*) resource;
		anim->m_animIndex = frameIndex;
		anim->m_flags = p_animIndex;
		anim->m_remap = p_remap;
		anim->Draw(m_gdi);
	}
	result.m_width = sizeSource->m_width;
	result.m_height = sizeSource->m_height;
	result.m_x = sizeSource->m_x;
	result.m_y = sizeSource->m_y;
	return result;
}

// 68K 0x1020087c ResetPrimitives__13CAnimsManagerFv
// FUNCTION: LEMBALL 0x004678c0
void AnimsManager::ResetPrimitives()
{
	m_resetState = 0;
	if (m_doubleBuffered == 0) {
		m_zrleCount = 0;
		struct PrimitiveState {
			int count;
			undefined drawMark[4];
		};
		PrimitiveState* animState = (PrimitiveState*) &m_animCount;
		PrimitiveState* zrleState = (PrimitiveState*) &m_zrleCount;
		*animState = *zrleState;
		unsigned char* drawMark = (unsigned char*) m_gdi->m_renderTarget->GetCurrDb();
		m_animDrawMark[0] = *drawMark;
		m_zrleDrawMark[0] = *drawMark;
		return;
	}
	m_gdi->Render();
	m_gdi->m_primitiveCount = 0;
	m_bufferedZrleCount = 0;
	m_bufferedAnimCount = 0;
	m_bufferHalf ^= 1;
}
