#ifndef LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETDIBCONTEXT_H
#define LEMBALL_SCAFFOLD_VISOS_TARGET_TARGETDIBCONTEXT_H

#include "../../Common.h"

// SIZE 0x14
// VTABLE: LEMBALL 0x00498798
class TargetDibContext {
public:
	// No functions mapped yet.

private:
	unsigned char* m_bits;  // 0x04
	int m_width;            // 0x08
	void* m_previousBitmap; // 0x0c
	void* m_hBitmap;        // 0x10
};

#endif
