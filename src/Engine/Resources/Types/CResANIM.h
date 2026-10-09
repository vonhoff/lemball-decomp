#ifndef LEMBALL_VISOS_RESOURCES_CRESANIM_H
#define LEMBALL_VISOS_RESOURCES_CRESANIM_H

#include "CResBaseLIST.h"
#include "CResZRLE.h"
#include "Engine/Resources/ResourceTypeList.h"

#include <stddef.h>

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498a18
class CResANIM : public CResBaseLIST {
public:
	CResANIM(unsigned long p_resourceId) : CResBaseLIST(g_pCompressedResourceTypes)
	{
		m_animationEntries = NULL;
		DoLoad(p_resourceId);
		m_initialised = true;
	}
	CResANIM() {}
	static CResANIM* Load(unsigned long p_resourceId);
	virtual bool DirectResources(unsigned long p_index, unsigned char*& p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned long p_index,
								 unsigned char*& p_headerCursor,
								 unsigned char*& p_dataCursor);        // vtable+0x4c
	virtual bool ForceLoadVram(unsigned int p_index);                  // vtable+0x3c
	virtual unsigned int GetnVramEntries();                            // vtable+0x48
	virtual void AllocateResources(unsigned long p_count);             // vtable+0x44
	virtual void UnLoadResources(unsigned long p_index, bool p_force); // vtable+0x54
	virtual void UnLoadVramData(unsigned long p_index, bool p_force);  // vtable+0x40
	virtual ~CResANIM()
	{
		if (m_animationEntries != NULL) {
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
	CResZRLE* m_animationEntries; // 0x78
};

// SYNTHETIC: LEMBALL 0x0045e7a0
// CResANIM::`scalar deleting destructor'

#endif
