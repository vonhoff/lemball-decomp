#ifndef LEMBALL_VISOS_TARGET_TARGETWINGDRAWCODECSTATE_H
#define LEMBALL_VISOS_TARGET_TARGETWINGDRAWCODECSTATE_H

#include "../../Common.h"

// SIZE 0x474
struct TargetWinGDrawCodecState {
public:
	// No functions mapped yet.

	void* m_reserved0;         // 0x00
	void* m_targetDC;          // 0x04
	int m_destinationX;        // 0x08
	int m_destinationY;        // 0x0c
	int m_destinationWidth;    // 0x10
	int m_destinationHeight;   // 0x14
	int m_sourceX;             // 0x18
	int m_sourceY;             // 0x1c
	int m_sourceWidth;         // 0x20
	int m_sourceHeight;        // 0x24
	Surface* m_surface;        // 0x2c
	void* m_memoryDC;          // 0x30
	void* m_dibBitmap;         // 0x34
	void* m_previousDibBitmap; // 0x38
	void* m_auxBitmap;         // 0x3c
	void* m_previousAuxBitmap; // 0x40
	GWnd* m_window;            // 0x470
};

#endif
