#ifndef LEMBALL_VISOS_RESOURCES_CRESBOOL_H
#define LEMBALL_VISOS_RESOURCES_CRESBOOL_H

#include "CResBase.h" // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498b88
class CResBool : public CResBase {
public:
	CResBool(unsigned int p_resourceId);
	static CResBool* Load(unsigned int p_resourceId);
	virtual void OnLoad();  // vtable+0x2c
	virtual void SetType(); // vtable+0x34

private:
	unsigned int m_value; // 0x48
};

inline CResBool::CResBool(unsigned int p_resourceId)
{
	DoLoad(p_resourceId);
}

// SYNTHETIC: LEMBALL 0x0045e9d0
// CResBool::`scalar deleting destructor'

#endif
