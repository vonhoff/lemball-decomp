#ifndef LEMBALL_VISOS_RESOURCES_CRESMOVIE_H
#define LEMBALL_VISOS_RESOURCES_CRESMOVIE_H

#include "CResBaseList.h" // complete type
#include "CResInt.h"      // complete type
#include "CResString.h"   // complete type

// SIZE 0x80
// VTABLE: LEMBALL 0x00498c88
class CResMovie : public CResBaseList {
public:
	CResMovie();
	CResMovie(unsigned long p_arg0);
	static CResMovie* Load(unsigned int p_resourceId);
	virtual void AllocateResources(unsigned int p_count);                         // vtable+0x44
	virtual bool DirectResources(unsigned int p_index, unsigned char** p_cursor); // vtable+0x50
	virtual bool DirectResources(unsigned int p_index,
								 unsigned char** p_headerCursor,
								 unsigned char** p_dataCursor);               // vtable+0x4c
	virtual void UnLoadResources(unsigned int p_index, unsigned int p_force); // vtable+0x54
	virtual ~CResMovie();                                                     // vtable+0x00

	friend class CAnimWnd;

private:
	CResString* m_movieEntries; // 0x78
	CResInt* m_fontEntries;     // 0x7c
};

// SYNTHETIC: LEMBALL 0x0045ea80
// CResMovie::`scalar deleting destructor'

#endif
