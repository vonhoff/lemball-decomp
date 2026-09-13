#ifndef LEMBALL_VISOS_TARGET_TARGETDIBCONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETDIBCONTEXT_H

#include "../../Common.h"

// SIZE 0x0c
// VTABLE: LEMBALL 0x00498780
class TargetDibContext {
public:
	TargetDibContext()
	{
		m_bits = 0;
		m_width = 0;
	}
	virtual ~TargetDibContext() {}    // vtable+0x00
	virtual bool Lock();              // vtable+0x04
	virtual bool Unlock();            // vtable+0x08
	virtual unsigned char* GetBits(); // vtable+0x0c
	virtual int GetStride();          // vtable+0x10

	friend class TargetGraphicsDriver;
	friend class TargetGDIDriver;
	friend class Surface;

protected:
	unsigned char* m_bits; // 0x04
	int m_width;           // 0x08
};

// SYNTHETIC: LEMBALL 0x004582f0
// TargetDibContext::`scalar deleting destructor'

#endif
