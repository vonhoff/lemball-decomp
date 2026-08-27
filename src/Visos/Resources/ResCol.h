#ifndef LEMBALL_VISOS_RESOURCES_RESCOL_H
#define LEMBALL_VISOS_RESOURCES_RESCOL_H

#include "../../Common.h"
#include "ResBase.h" // complete type

#define kChunkCol 0x434f4c20

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498bc8
class ResCol : public ResBase {
public:
	inline ResCol(unsigned int p_resourceId) { DoLoad(p_resourceId); }
	static ResCol* Load(unsigned int p_resourceId);
	virtual void OnLoad();    // vtable+0x2c
	virtual void SetType();   // vtable+0x34
	virtual ~ResCol();        // vtable+0x00

private:
	unsigned int m_colour; // 0x48
};

// SYNTHETIC: LEMBALL 0x0045ea10
// ResCol::`scalar deleting destructor'


#endif

