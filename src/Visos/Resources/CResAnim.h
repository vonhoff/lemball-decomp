#ifndef LEMBALL_VISOS_RESOURCES_CRESANIM_H
#define LEMBALL_VISOS_RESOURCES_CRESANIM_H

#include "CResBaseList.h" // complete type
#include "CResZrle.h"     // complete type
#include "ResourceTypeList.h"

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498a18
class CResAnim : public CResBaseList {
public:
	inline CResAnim(unsigned long p_resourceId) : CResBaseList((ResListHeader*) g_pCompressedResourceTypes)
	{
		m_animationEntries = 0;
		DoLoad(p_resourceId);
		m_initialized = 1;
	}
	inline CResAnim() {}
	static CResAnim* Load(unsigned long p_resourceId);
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);               // vtable+0x4c
	virtual bool ForceLoadVram(unsigned int p_index);                         // vtable+0x3c
	virtual unsigned int GetnVramEntries();                                   // vtable+0x48
	virtual void AllocateResources(unsigned int p_count);                     // vtable+0x44
	virtual void UnLoadResources(unsigned int p_index, unsigned int p_force); // vtable+0x54
	virtual void UnLoadVramData(unsigned int p_index, unsigned int p_force);  // vtable+0x40
	inline virtual ~CResAnim()
	{
		if (m_animationEntries != 0) {
			delete[] m_animationEntries;
		}
	}

	friend class CAnimsManager;
	friend class CAnim;
	friend class CGraphicButton;
	friend class CPauseWindow;
	friend class CBaseCursor;
	friend class CPanel;

private:
	CResZrle* m_animationEntries; // 0x78
};

// SYNTHETIC: LEMBALL 0x0045e7a0
// CResAnim::`scalar deleting destructor'

#endif
