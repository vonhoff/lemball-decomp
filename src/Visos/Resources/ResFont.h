#ifndef LEMBALL_VISOS_RESOURCES_RESFONT_H
#define LEMBALL_VISOS_RESOURCES_RESFONT_H

#include "../../Common.h"
#include "../Foundation/VsPoint.h" // complete type
#include "ResBaseList.h"           // complete type

// SIZE 0x84
// VTABLE: LEMBALL 0x00498af0
class ResFont : public ResBaseList {
public:
	ResFont(unsigned long p_arg0);
	ResZrle* AsciItoZrle(unsigned int p_ascii);
	VsPoint GetSize(const char* p_text, unsigned int p_flags);
	static ResFont* Load(unsigned int p_resourceId);
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x48
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);                // vtable+0x44
	virtual bool ForceLoadVram(unsigned int p_index);                          // vtable+0x34
	virtual unsigned int GetnVramEntries();                                    // vtable+0x40
	virtual void AllocateResources(unsigned int p_count);                      // vtable+0x3c
	virtual void OnLoad();                                                     // vtable+0x24
	virtual void UnLoadResources(unsigned int p_index, unsigned char p_force); // vtable+0x4c
	virtual void UnLoadVramData(unsigned int p_index, unsigned char p_force);  // vtable+0x38
	~ResFont();

private:
	FontTable* m_fontTable;   // 0x78
	void* m_animationEntries; // 0x7c
	void* m_fontEntries;      // 0x80
};

#endif
