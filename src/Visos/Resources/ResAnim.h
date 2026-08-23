#ifndef LEMBALL_VISOS_RESOURCES_RESANIM_H
#define LEMBALL_VISOS_RESOURCES_RESANIM_H

#include "../../Common.h"
#include "ResBaseList.h" // complete type

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498a18
class ResAnim : public ResBaseList {
public:
	static ResAnim* Load(unsigned int p_resourceId);
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);                // vtable+0x4c
	virtual bool ForceLoadVram(unsigned int p_index);                          // vtable+0x3c
	virtual unsigned int GetnVramEntries();                                    // vtable+0x48
	virtual void AllocateResources(unsigned int p_count);                      // vtable+0x44
	virtual void UnLoadResources(unsigned int p_index, unsigned char p_force); // vtable+0x54
	virtual void UnLoadVramData(unsigned int p_index, unsigned char p_force);  // vtable+0x40
	virtual ~ResAnim();                                                        // vtable+0x00

private:
	ResBase* m_animationEntries; // 0x78
};

#endif
