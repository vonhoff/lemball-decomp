#ifndef LEMBALL_VISOS_RESOURCES_CRESFONT_H
#define LEMBALL_VISOS_RESOURCES_CRESFONT_H

#include "../Foundation/CVsSize.h" // complete type
#include "CResBaseList.h"          // complete type
#include "CResInt.h"               // complete type
#include "CResZrle.h"              // complete type

// SIZE 0x84
// VTABLE: LEMBALL 0x00498af0
class CResFont : public CResBaseList {
public:
	CResFont(unsigned long p_arg0);
	CResZrle* AsciItoZrle(unsigned int p_ascii);
	CVsSize* GetSize(CVsSize* p_result, const char* p_text, unsigned int p_flags);
	CVsSize GetSize(const char* p_text, unsigned int p_flags)
	{
		short storage[2];
		return *GetSize((CVsSize*) storage, p_text, p_flags);
	}

	static CResFont* Load(unsigned int p_resourceId);
	virtual void OnLoad();                                                        // vtable+0x2c
	virtual bool ForceLoadVram(unsigned int p_index);                             // vtable+0x3c
	virtual void UnLoadVramData(unsigned int p_index, unsigned int p_force);      // vtable+0x40
	virtual void AllocateResources(unsigned int p_count);                         // vtable+0x44
	virtual unsigned int GetnVramEntries();                                       // vtable+0x48
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);               // vtable+0x4c
	virtual void UnLoadResources(unsigned int p_index, unsigned int p_force); // vtable+0x54
	virtual ~CResFont();                                                      // vtable+0x00

	friend class CFontTable;
	friend class CText;

private:
	CFontTable* m_fontTable;      // 0x78
	CResZrle* m_animationEntries; // 0x7c
	CResInt* m_fontEntries;       // 0x80
};

// SYNTHETIC: LEMBALL 0x0045e8d0
// CResFont::`scalar deleting destructor'

#endif
