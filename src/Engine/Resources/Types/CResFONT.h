#ifndef LEMBALL_VISOS_RESOURCES_CRESFONT_H
#define LEMBALL_VISOS_RESOURCES_CRESFONT_H

#include "CResBaseLIST.h"
#include "Engine/Math/CVSSize.h"

class CFontTable;
class CResINT;
class CResZRLE;

// SIZE 0x84
// VTABLE: LEMBALL 0x00498af0
class CResFONT : public CResBaseLIST {
public:
	CResFONT();
	CResFONT(unsigned long p_resourceId);
	CResZRLE* ASCIItoZRLE(unsigned long p_ascii) const;
	CVSSize GetSize(const char* p_text, unsigned long p_flags) const;

	static CResFONT* Load(unsigned long p_resourceId);
	virtual void OnLoad();                                                         // vtable+0x2c
	virtual bool ForceLoadVram(unsigned int p_index);                              // vtable+0x3c
	virtual void UnLoadVramData(unsigned long p_index, bool p_force);              // vtable+0x40
	virtual void AllocateResources(unsigned long p_count);                         // vtable+0x44
	virtual unsigned int GetnVramEntries();                                        // vtable+0x48
	virtual bool DirectResources(unsigned long p_index, unsigned char*& p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned long p_index,
								 unsigned char*& p_headerCursor,
								 unsigned char*& p_dataCursor);        // vtable+0x4c
	virtual void UnLoadResources(unsigned long p_index, bool p_force); // vtable+0x54
	virtual ~CResFONT();                                               // vtable+0x00

	friend class CFontTable;
	friend class CText;

private:
	CFontTable* m_fontTable;      // 0x78
	CResZRLE* m_animationEntries; // 0x7c
	CResINT* m_fontEntries;       // 0x80
};

// SYNTHETIC: LEMBALL 0x0045e8d0
// CResFONT::`scalar deleting destructor'

#endif
