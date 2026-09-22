#include "CAnimsManager.h"

#include "../Foundation/CVsPoint.h"
#include "../Graphics/CGdi.h"
#include "../Graphics/CLine.h"
#include "../Graphics/CZrle.h"
#include "../Graphics/VsGdi.h"
#include "../Resources/CResAnim.h"
#include "../Resources/CResBase.h"
#include "../Resources/CResBaseList.h"
#include "../Resources/CResZrle.h"
#include "CAnim.h"
#include "CFrames.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"

#include <string.h>

// FUNCTION: LEMBALL 0x004358c0
void CAnimsManager::FreeVram()
{
}

// FUNCTION: LEMBALL 0x0044e700
CVsRect* CAnimsManager::DrawAnimOnGdi(CVsRect* p_bounds,
									  CGdi* p_gdi,
									  const CVsPoint& p_position,
									  unsigned long p_resourceId,
									  unsigned long p_animIndex,
									  CFrames* p_frame,
									  CRemap* p_remap)
{
	CGdi* previous = m_gdi;
	m_gdi = p_gdi;
	CVsRect bounds = DrawAnim(p_position, p_resourceId, p_animIndex, p_frame, p_remap);
	m_gdi = previous;
	p_bounds->m_width = bounds.m_width;
	p_bounds->m_height = bounds.m_height;
	p_bounds->m_x = bounds.m_x;
	p_bounds->m_y = bounds.m_y;
	return p_bounds;
}

// FUNCTION: LEMBALL 0x00467260
CAnimsManager::CAnimsManager(CGdi* p_gdi,
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
	m_resources = (CResBase**) operator new(m_resourceCapacity * 4);
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
		m_zrlePrimitives = new CZrle[m_zrleCapacity];
	}
	if (m_animCapacity != 0) {
		m_animPrimitives = new CAnim[m_animCapacity];
	}
	m_ownsLinePrimitives = 0;
	m_linePrimitives = 0;
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

// FUNCTION: LEMBALL 0x00467490
void CAnimsManager::LoadAnims(unsigned long p_resourceId)
{
	int slot = 0;
	if (m_resourceCapacity != m_resourceSlots[p_resourceId]) {
		slot = m_resourceSlots[p_resourceId];
	}
	else {
		while (m_resources[slot] != 0) {
			slot = slot + 1;
		}
	}
	m_resources[slot] = CResAnim::Load(p_resourceId);
	CResBase*& resource = m_resources[slot];
	if (resource == 0) {
		resource = CResZrle::Load(p_resourceId);
	}
	if ((int) m_resourceSlots[p_resourceId] == m_resourceCapacity) {
		m_resourceSlots[p_resourceId] = (short) slot;
		m_loadedResourceCount = m_loadedResourceCount + 1;
	}
}

// FUNCTION: LEMBALL 0x00467500
void CAnimsManager::UnLoadAnims(unsigned long p_resourceId)
{
	m_resources[m_resourceSlots[p_resourceId]]->UnLoad();
	m_resources[m_resourceSlots[p_resourceId]] = 0;
	m_resourceSlots[p_resourceId] = (short) m_resourceCapacity;
	m_loadedResourceCount = m_loadedResourceCount - 1;
}

// FUNCTION: LEMBALL 0x00467540
unsigned long CAnimsManager::GetnAnims(unsigned long p_resourceId)
{
	CResBase* resource;

	resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType == 0x5a524c45) {
		return 1;
	}
	return ((CResBaseList*) resource)->m_totalSize;
}

// FUNCTION: LEMBALL 0x00467570
CVsSize CAnimsManager::GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex)
{
	CVsSize size;
	CResBase* resource = m_resources[m_resourceSlots[p_resourceId]];
	if (resource->m_chunkType != 0x5a524c45) {
		CResZrle* entry = ((CResAnim*) resource)->m_animationEntries + p_animIndex;
		size.m_width = entry->m_width;
		size.m_height = entry->m_height;
	}
	else {
		CResZrle* entry = (CResZrle*) resource;
		size.m_width = entry->m_width;
		size.m_height = entry->m_height;
	}
	return size;
}

// FUNCTION: LEMBALL 0x004676a0
CResZrle* CAnimsManager::ResolveAnimFrameData(unsigned long p_resourceId, CFrames* p_frame)
{
	CResBase* resource = m_resources[m_resourceSlots[p_resourceId]];
	unsigned int frame;
	if (p_frame != 0) {
		frame = p_frame->GetFrameNo();
	}
	else {
		frame = 0;
	}
	if (resource->m_chunkType == 0x5a524c45) {
		return (CResZrle*) resource;
	}
	return ((CResAnim*) resource)->m_animationEntries + frame;
}

// FUNCTION: LEMBALL 0x00467700
void CAnimsManager::DetachGdi(CGdi* p_gdi)
{
	if (m_previousGdi == p_gdi) {
		m_previousGdi = 0;
	}
	if (m_gdi == p_gdi) {
		m_previousGdi = 0;
		m_gdi = 0;
	}
}

// FUNCTION: LEMBALL 0x00467730
CVsRect CAnimsManager::DrawAnim(const CVsPoint& p_position,
								unsigned long p_resourceId,
								unsigned long p_animIndex,
								CFrames* p_frame,
								CRemap* p_remap)
{
	CResBase* resource;
	CResZrle* sizeSource;
	CZrle* zrle;
	CAnim* anim;
	unsigned int frameIndex;
	CGdi* current;

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
		sizeSource = (CResZrle*) resource;
		if (m_doubleBuffered != 0) {
			if (m_zrleCapacity == m_bufferedZrleCount) {
				ResetPrimitives();
			}
			zrle = m_zrlePrimitives + m_bufferHalf + m_bufferedZrleCount;
			m_bufferedZrleCount += 2;
		}
		else {
			m_zrleCount++;
			zrle = m_zrlePrimitives + (m_zrleCount - 1);
		}
		zrle->m_state = m_primitiveSequence;
		zrle->m_x = p_position.m_x;
		zrle->m_y = p_position.m_y;
		zrle->m_resource = resource;
		zrle->m_flags = p_animIndex;
		zrle->m_remap = p_remap;
		zrle->Draw(m_gdi);
	}
	else {
		frameIndex = 0;
		if (p_frame != 0) {
			frameIndex = ((CFrames*) p_frame)->GetFrameNo();
			((CFrames*) p_frame)->m_reserved08 = frameIndex;
		}
		sizeSource = ((CResAnim*) resource)->m_animationEntries + frameIndex;
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
		anim->m_animResource = (CResAnim*) resource;
		anim->m_animIndex = frameIndex;
		anim->m_flags = p_animIndex;
		anim->m_remap = p_remap;
		anim->Draw(m_gdi);
	}
	return CVsRect(sizeSource->m_x, sizeSource->m_y, sizeSource->m_width, sizeSource->m_height);
}

// FUNCTION: LEMBALL 0x004678c0
void CAnimsManager::ResetPrimitives()
{
	m_resetState = 0;
	if (m_doubleBuffered == 0) {
		m_zrleCount = 0;
		struct PrimitiveState {
			int count;
			char drawMark[4];
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
