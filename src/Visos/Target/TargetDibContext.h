#ifndef LEMBALL_VISOS_TARGET_TARGETDIBCONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETDIBCONTEXT_H

#include "../../Common.h"

// SIZE 0x14
// VTABLE: LEMBALL 0x00498798
class TargetDibContext {
public:
	virtual ~TargetDibContext(); // vtable+0x00
	unsigned char* GetBits();    // vtable+0x0c
	int GetStride();             // vtable+0x10

	friend class TargetGraphicsDriver;
	friend class Surface;

private:
	unsigned char* m_bits;  // 0x04
	int m_width;            // 0x08
	void* m_previousBitmap; // 0x0c
	void* m_hBitmap;        // 0x10
};

#endif
