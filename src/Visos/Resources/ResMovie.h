#ifndef LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESMOVIE_H
#define LEMBALL_SCAFFOLD_VISOS_RESOURCES_RESMOVIE_H

#include "../../Common.h"
#include "ResBaseList.h" // complete type

// SIZE 0x80
// VTABLE: LEMBALL 0x00498c88
class ResMovie : public ResBaseList {
public:
	ResMovie(unsigned long p_arg0);
	static ResMovie* Load(unsigned int p_resourceId);
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);                // vtable+0x4c
	virtual void AllocateResources(unsigned int p_count);                      // vtable+0x44
	virtual void UnLoadResources(unsigned int p_index, unsigned char p_force); // vtable+0x54
	~ResMovie();

private:
	void* m_movieEntries; // 0x78
	void* m_fontEntries;  // 0x7c
};

#endif
