#include "AnimsManager.h"

#include "../Foundation/VsPoint.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/Line.h"
#include "../Graphics/Zrle.h"
#include "../Resources/ResAnim.h"
#include "../Resources/ResBase.h"
#include "../Resources/ResBaseList.h"
#include "../Resources/ResZrle.h"
#include "Anim.h"
#include "Frames.h"

#include <new.h>

AnimsManager::AnimsManager()
{
}

// 68K 0x10115d70 FreeVram__13CAnimsManagerFv
// FUNCTION: LEMBALL 0x004358c0
void AnimsManager::FreeVram()
{
}

// 68K 0x1020019c __ct__13CAnimsManagerFP4CGDIUliiiUc
// STUB: LEMBALL 0x00467260
AnimsManager::AnimsManager(Gdi* p_arg0, unsigned long p_arg1, int p_arg2, int p_arg3, int p_arg4, unsigned char p_arg5)
{
	int i;

	m_animCount = 0;
	m_zrleCount = 0;
	m_gdi = p_arg0;
	m_resourceCapacity = p_arg2;
	m_resourceIdCount = (int) p_arg1;
	m_animPrimitives = 0;
	m_zrlePrimitives = 0;
	m_previousGdi = 0;
	m_doubleBuffered = p_arg5;
	m_loadedResourceCount = 0;
	m_reserved6c = 0xffffffff;
	m_resources = 0;
	m_resourceSlots = 0;
	if (m_resourceCapacity > 0) {
		m_resources = (ResBase**) operator new(m_resourceCapacity * 4);
		i = 0;
		while (i < m_resourceCapacity) {
			m_resources[i] = 0;
			i = i + 1;
		}
	}
	if (m_resourceIdCount > 0) {
		m_resourceSlots = (short*) operator new(m_resourceIdCount * 2);
		i = 0;
		while (i < m_resourceIdCount) {
			m_resourceSlots[i] = (short) m_resourceCapacity;
			i = i + 1;
		}
	}
	if (m_doubleBuffered == 0) {
		m_zrleCapacity = p_arg4;
		m_animCapacity = p_arg3;
	}
	else {
		m_bufferHalf = 0;
		m_zrleCapacity = p_arg4 * 2;
		m_bufferedZrleCount = 0;
		m_bufferedAnimCount = 0;
		m_animCapacity = p_arg3 * 2;
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

	i = 0;
	if (m_loadedResourceCount != 0 && 0 < m_loadedResourceCount) {
		scanned = 0;
		do {
			while (m_resources[i] == 0) {
				i = i + 1;
			}
			m_resources[i]->UnLoad();
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
// STUB: LEMBALL 0x00467490
void AnimsManager::LoadAnims(unsigned long p_resourceId)
{
	int slot;

	if (m_resourceSlots == 0 || m_resources == 0) {
		return;
	}
	if ((int) p_resourceId >= m_resourceIdCount) {
		return;
	}
	slot = m_resourceSlots[p_resourceId];
	if (slot == m_resourceCapacity) {
		slot = 0;
		while (slot < m_resourceCapacity && m_resources[slot] != 0) {
			slot = slot + 1;
		}
		if (slot >= m_resourceCapacity) {
			return;
		}
	}
	m_resources[slot] = ResAnim::Load(p_resourceId);
	if (m_resources[slot] == 0) {
		m_resources[slot] = ResZrle::Load(p_resourceId);
	}
	if (m_resourceSlots[p_resourceId] == (short) m_resourceCapacity) {
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
// STUB: LEMBALL 0x00467540
unsigned long AnimsManager::GetnAnims(unsigned long p_resourceId)
{
	ResBase* resource;
	int slot;

	if (m_resourceSlots == 0 || m_resources == 0 || (int) p_resourceId >= m_resourceIdCount) {
		return 0;
	}
	slot = m_resourceSlots[p_resourceId];
	if (slot == m_resourceCapacity) {
		return 0;
	}
	resource = m_resources[slot];
	if (resource == 0) {
		return 0;
	}
	if (resource->m_chunkType == 0x5a524c45) {
		return 1;
	}
	return ((ResBaseList*) resource)->m_totalSize;
}

// 68K 0x102005b8 GetAnimSize__13CAnimsManagerFUlUl
// STUB: LEMBALL 0x00467570
VsSize AnimsManager::GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex)
{
	VsSize size;
	ResBase* resource;
	ResZrle* entry;
	int slot;

	size.m_width = 0;
	size.m_height = 0;
	if (m_resourceSlots == 0 || m_resources == 0 || (int) p_resourceId >= m_resourceIdCount) {
		return size;
	}
	slot = m_resourceSlots[p_resourceId];
	if (slot == m_resourceCapacity) {
		return size;
	}
	resource = m_resources[slot];
	if (resource == 0) {
		return size;
	}
	if (resource->m_chunkType == 0x5a524c45) {
		size.m_width = ((ResZrle*) resource)->m_width;
		size.m_height = ((ResZrle*) resource)->m_height;
		return size;
	}
	entry = ((ResAnim*) resource)->m_animationEntries;
	if (entry == 0) {
		return size;
	}
	entry = entry + p_animIndex;
	size.m_width = entry->m_width;
	size.m_height = entry->m_height;
	return size;
}

// 68K 0x1020064e DrawAnim__13CAnimsManagerFRC8CVSPointUlUlP14CAnimFrameBASEP6CRemap
// STUB: LEMBALL 0x00467730
VsRect AnimsManager::DrawAnim(const VsPoint& p_position,
							  unsigned long p_resourceId,
							  unsigned long p_animIndex,
							  AnimFrameBASE* p_frame,
							  Remap* p_remap)
{
	VsRect result;
	ResBase* resource;
	ResZrle* sizeSource;
	Zrle* zrle;
	Anim* anim;
	unsigned int frameIndex;
	int slot;
	int index;

	result.m_width = 0;
	result.m_height = 0;
	result.m_x = 0;
	result.m_y = 0;
	if (m_gdi == 0 || m_resourceSlots == 0 || m_resources == 0 || (int) p_resourceId >= m_resourceIdCount) {
		return result;
	}
	if (m_doubleBuffered != 0 && m_previousGdi != 0 && m_gdi != m_previousGdi) {
		Gdi* current;
		current = m_gdi;
		m_gdi = m_previousGdi;
		ResetPrimitives();
		m_gdi = current;
	}
	m_previousGdi = m_gdi;
	slot = m_resourceSlots[p_resourceId];
	if (slot == m_resourceCapacity) {
		return result;
	}
	resource = m_resources[slot];
	if (resource == 0) {
		return result;
	}
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
		if (zrle == 0) {
			return result;
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
		if (anim == 0) {
			return result;
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
	if (sizeSource != 0) {
		result.m_width = sizeSource->m_width;
		result.m_height = sizeSource->m_height;
		result.m_x = sizeSource->m_x;
		result.m_y = sizeSource->m_y;
	}
	return result;
}

// 68K 0x1020087c ResetPrimitives__13CAnimsManagerFv
// STUB: LEMBALL 0x004678c0
void AnimsManager::ResetPrimitives()
{
	m_animCount = 0;
	m_zrleCount = 0;
	m_bufferedAnimCount = 0;
	m_bufferedZrleCount = 0;
}
