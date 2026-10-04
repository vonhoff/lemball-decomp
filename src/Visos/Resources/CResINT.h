#ifndef LEMBALL_VISOS_RESOURCES_CRESINT_H
#define LEMBALL_VISOS_RESOURCES_CRESINT_H

#include "CResBase.h"

// SIZE 0x04
struct IntPayload {
	int m_value;
};

// SIZE 0x4c
// VTABLE: LEMBALL 0x00498b48
class CResINT : public CResBase {
public:
	CResINT();
	CResINT(unsigned int p_resourceId);
	static CResINT* Load(unsigned int p_resourceId);
	virtual void OnLoad();  // vtable+0x2c
	virtual void SetType(); // vtable+0x34

	friend class CFontTable;

private:
	int m_value; // 0x48
};

inline CResINT::CResINT(unsigned int p_resourceId)
{
	DoLoad(p_resourceId);
}

// SYNTHETIC: LEMBALL 0x0045e930
// CResINT::`vector deleting destructor'

#endif
