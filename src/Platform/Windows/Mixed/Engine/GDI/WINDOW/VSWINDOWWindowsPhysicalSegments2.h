#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_003)
// FUNCTION: LEMBALL 0x0046aa80
void* __fastcall ConstructPaletteRemapVariant(void* pThis,
											  int nUnused,
											  int param_1,
											  unsigned char* param_2,
											  int param_3)
{
	*(void**) ((char*) pThis + 4) = (void*) ((int(__cdecl*)(int)) 0x45dd90)(param_1);
	if (param_3 == 1) {
		((void(__fastcall*)(void*)) 0x46ab70)(pThis);
		return pThis;
	}
	if (param_3 != 2) {
		*(unsigned char**) pThis = param_2;
		return pThis;
	}
	((void(__fastcall*)(void*, unsigned char*)) 0x46aaf0)(pThis, param_2);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_032)
// FUNCTION: LEMBALL 0x00432650
void __fastcall RestoreInertialEventSourceSinkVtables(void* pObject)
{
	*(void**) ((char*) pObject + 0x60) = (void**) 0x496ca8;
	int i;
	for (i = 0; i < 2; i++) {
		((void(__fastcall*)(void*)) 0x401307)((char*) pObject + 0x34 - i * 0x10);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_033)
// FUNCTION: LEMBALL 0x004327e0
void __fastcall EmitVariantChildSlotRenderMarker(void* pObject, int nUnused, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, short, short, int, int, int, int)) 0x4016a9)(*(void**) ((char*) pObject + 0x10),
																			*(short*) param_3,
																			*(short*) ((char*) param_3 + 2),
																			0xfd,
																			0,
																			0,
																			0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_109)
// FUNCTION: LEMBALL 0x00432590
void* __fastcall ConstructQueuedVariantChildSlotRenderPointSink(void* pThis,
																int nUnused,
																int param_2,
																int param_3,
																int param_4)
{
	int i;
	for (i = 0; i < 2; i++) {
		((void(__fastcall*)(void*)) 0x401c7b)((char*) pThis + 0x14 + i * 0x10);
	}
	*(int*) ((char*) pThis + 0x34) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x38) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x3c) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x40) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x44) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x48) = 0xaa55aa55;
	*(void**) ((char*) pThis + 0x60) = (void*) 0x496ca8;
	*(void**) ((char*) pThis + 0x60) = (void*) 0x496cb8;
	*(short*) ((char*) pThis + 0x66) = 0;
	*(int*) ((char*) pThis + 8) = param_3;
	*(short*) ((char*) pThis + 100) = 0;
	*(short*) ((char*) pThis + 0x6a) = 0;
	*(short*) ((char*) pThis + 0x68) = 0;
	*(int*) ((char*) pThis + 4) = param_4;
	*(int*) ((char*) pThis + 0x10) = param_2;
	*(int*) ((char*) pThis + 0xc) = *(int*) ((char*) param_3 + 0x164);
	*(int*) ((char*) pThis + 0x3c) = 0;
	*(int*) ((char*) pThis + 0x40) = 0;
	*(int*) ((char*) pThis + 0x44) = 0;
	*(int*) ((char*) pThis + 0x48) = 0;
	*(int*) ((char*) pThis + 0x54) = 0;
	*(int*) ((char*) pThis + 0x58) = 0;
	*(int*) ((char*) pThis + 0x34) = 0xa0000;
	*(int*) ((char*) pThis + 0x38) = 0x64000;
	((void(__fastcall*)(void*)) 0x401a78)(pThis);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_111)
// FUNCTION: LEMBALL 0x00432880
void __fastcall StartDirectionalOffsetAnimationIfIdle(void* pObject, int nUnusedEdx, int direction)
{
	if (*(int*) ((char*) pObject + 0x54) == 0) {
		*(int*) ((char*) pObject + 0x44) = (direction == 0) ? -0xcc : 0xcc;
		*(unsigned long*) ((char*) pObject + 0x4c) = timeGetTime();
		*(int*) ((char*) pObject + 0x54) = 1;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_112)
// FUNCTION: LEMBALL 0x004328d0
void __fastcall StartSignedOffsetAnimationIfIdle(void* pObject, int nUnusedEdx, int direction)
{
	if (*(int*) ((char*) pObject + 0x58) == 0) {
		*(int*) ((char*) pObject + 0x48) = (direction == 0) ? -0xcc : 0xcc;
		*(unsigned long*) ((char*) pObject + 0x50) = timeGetTime();
		*(int*) ((char*) pObject + 0x58) = 1;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_113)
// FUNCTION: LEMBALL 0x004326e0
void __fastcall IntegrateInertialCoordinateEventSource(void* pObject, int nUnusedEdx)
{
	unsigned long uNow = timeGetTime();
	if (*(int*) ((char*) pObject + 0x54) != 0) {
		int nStamp = *(int*) ((char*) pObject + 0x4c);
		*(unsigned long*) ((char*) pObject + 0x4c) = uNow;
		int v = *(int*) ((char*) pObject + 0x44) * (uNow - nStamp) / 0x14 + *(int*) ((char*) pObject + 0x3c);
		*(int*) ((char*) pObject + 0x3c) = v;
		if (v > 0x5000) {
			*(int*) ((char*) pObject + 0x3c) = 0x5000;
		}
		if (v < -0x5000) {
			*(int*) ((char*) pObject + 0x3c) = -0x5000;
		}
	}
	int nSignedBusy = *(int*) ((char*) pObject + 0x58);
	if (nSignedBusy != 0) {
		int nStamp = *(int*) ((char*) pObject + 0x50);
		*(unsigned long*) ((char*) pObject + 0x50) = uNow;
		int v = *(int*) ((char*) pObject + 0x48) * (uNow - nStamp) / 0x14 + *(int*) ((char*) pObject + 0x40);
		*(int*) ((char*) pObject + 0x40) = v;
		if (v > 0x5000) {
			*(int*) ((char*) pObject + 0x40) = 0x5000;
		}
		if (v < -0x5000) {
			*(int*) ((char*) pObject + 0x40) = -0x5000;
		}
	}
	if (*(int*) ((char*) pObject + 0x54) == 0 && nSignedBusy == 0 && *(int*) ((char*) pObject + 0x5c) == 0) {
		return;
	}
	if (*(int*) ((char*) pObject + 0x54) != 0 || nSignedBusy != 0) {
		*(int*) ((char*) pObject + 0x34) += *(int*) ((char*) pObject + 0x3c);
		*(int*) ((char*) pObject + 0x38) += *(int*) ((char*) pObject + 0x40);
	}
	((void(__fastcall*)(void*)) 0x432680)(pObject);
	*(int*) ((char*) pObject + 0x5c) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_146)
// FUNCTION: LEMBALL 0x004327b0
void __fastcall emit_inertial_source_marker_render_entry(void* this_, void* nUnused, int arg)
{
	((void(__fastcall*)(void*, int, int, int, int, int, int)) 0x004016a9)(
		*(void**) ((char*) this_ + 0x10),
		(*(int*) ((char*) this_ + 0x34) >> 12) - *(int*) ((char*) this_ + 0x70),
		(*(int*) ((char*) this_ + 0x38) >> 12) - *(int*) ((char*) this_ + 0x74),
		0xfd,
		0,
		0,
		0);
	(void) arg;
}
#endif
