#ifndef LEMBALL_VISOS_RESOURCES_RESANIM_H
#define LEMBALL_VISOS_RESOURCES_RESANIM_H

#include "../../Common.h"
#include "ResBaseList.h" // complete type
#include "ResZrle.h"     // complete type
#include "ResourceTypeList.h"

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498a18
class ResAnim : public ResBaseList {
public:
	inline ResAnim(unsigned int p_resourceId)
		: ResBaseList((ResListHeader*) g_pCompressedResourceTypes)
	{
		m_animationEntries = 0;
		DoLoad(p_resourceId);
		m_initialized = 1;
	}
	inline ResAnim() {}
	static ResAnim* Load(unsigned int p_resourceId);
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);                // vtable+0x4c
	virtual bool ForceLoadVram(unsigned int p_index);                          // vtable+0x3c
	virtual unsigned int GetnVramEntries();                                    // vtable+0x48
	virtual void AllocateResources(unsigned int p_count);                      // vtable+0x44
	virtual void UnLoadResources(unsigned int p_index, unsigned int p_force);  // vtable+0x54
	virtual void UnLoadVramData(unsigned int p_index, unsigned int p_force);  // vtable+0x40
	inline virtual ~ResAnim()
	{
		if (m_animationEntries != 0) {
			delete[] m_animationEntries;
		}
	}

	friend class AnimsManager;
	friend class Anim;
	friend class GraphicButton;

private:
	ResZrle* m_animationEntries; // 0x78
};

// SYNTHETIC: LEMBALL 0x0045e7a0
// ResAnim::`scalar deleting destructor'


#endif

