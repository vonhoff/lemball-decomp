#ifndef LEMBALL_VISOS_RESOURCES_CRESCOL_H
#define LEMBALL_VISOS_RESOURCES_CRESCOL_H

#include "CResBase.h" // complete type

#define kChunkCol 0x434f4c20

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498bc8
class CResCol : public CResBase {
public:
	inline CResCol(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static CResCol* Load(unsigned int p_resourceId);
	virtual void OnLoad();  // vtable+0x2c
	virtual void SetType(); // vtable+0x34

private:
	unsigned int m_colour; // 0x48
};

// SYNTHETIC: LEMBALL 0x0045ea10
// CResCol::`scalar deleting destructor'

#endif
