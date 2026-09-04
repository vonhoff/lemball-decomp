#ifndef LEMBALL_VISOS_TARGET_TARGETWINGDRAWCODECSTATE_H
#define LEMBALL_VISOS_TARGET_TARGETWINGDRAWCODECSTATE_H

#include "../../Common.h"

// SIZE 0x474
struct TargetWinGDrawCodecState {
	void* m_reserved0;             // 0x00
	void* m_targetDC;              // 0x04
	int m_destinationX;            // 0x08
	int m_destinationY;            // 0x0c
	int m_destinationWidth;        // 0x10
	int m_destinationHeight;       // 0x14
	int m_sourceX;                 // 0x18
	int m_sourceY;                 // 0x1c
	int m_sourceWidth;             // 0x20
	int m_sourceHeight;            // 0x24
	undefined4 m_reserved28;       // 0x28
	Surface* m_surface;            // 0x2c
	void* m_memoryDC;              // 0x30
	void* m_dibBitmap;             // 0x34
	void* m_previousDibBitmap;     // 0x38
	void* m_auxBitmap;             // 0x3c
	void* m_previousAuxBitmap;     // 0x40
	undefined m_reserved44[0x42c]; // 0x44
	GWnd* m_window;                // 0x470
};

extern AnimWnd* g_pAnimWnd;
TargetWinGDrawCodecState* __stdcall TargetWinGDrawCodec_Open(void* p_openInfo);
int __stdcall TargetWinGDrawCodec_Close(TargetWinGDrawCodecState* p_state);
int __stdcall TargetWinGDrawCodec_Draw(TargetWinGDrawCodecState* p_state, void* p_request, long p_param2);
int __stdcall TargetWinGDrawCodec_Begin(TargetWinGDrawCodecState* p_state, void* p_request, long p_param2);
int __stdcall TargetWinGDrawCodec_End(TargetWinGDrawCodecState* p_state);
int __stdcall TargetWinGDrawCodec_QueryFormat(TargetWinGDrawCodecState* p_state, void* p_format);
int __stdcall TargetWinGDrawCodec_SuggestFormat(TargetWinGDrawCodecState* p_state, void* p_request, long p_param2);
int __stdcall TargetWinGDrawCodec_ChangePalette(TargetWinGDrawCodecState* p_state, void* p_request);
unsigned int __stdcall TargetWinGDrawCodec_GetInfo(void* p_info, unsigned int p_size);

#endif
