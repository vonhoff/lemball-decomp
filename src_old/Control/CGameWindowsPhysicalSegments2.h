#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_018)
// FUNCTION: LEMBALL 0x00438380
void __fastcall SelectSingleEntityAndDispatchSelection(void* pObject, int nUnused, void* param_1)
{
	if (*(int*) ((char*) param_1 + 0xb8) != 8) {
		*(unsigned short*) ((char*) pObject + 0xa4c) = 0;
		((void(__fastcall*)(void*, unsigned short, int)) 0x401474)(pObject,
																   *(unsigned short*) ((char*) param_1 + 0x6a),
																   0);
		((void(__fastcall*)(void*)) 0x401776)(pObject);
		(*(void (**)(void))(*(void***) param_1 + 0xd4 / 4))();
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_030)
// FUNCTION: LEMBALL 0x00473610
void* __fastcall ConstructPaletteRemapVariantWrapper(void* pThis,
													 int nUnused,
													 int param_1,
													 unsigned char* param_2,
													 int param_3)
{
	ConstructPaletteRemapVariant(pThis, nUnused, param_1, param_2, param_3);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_037)
// FUNCTION: LEMBALL 0x00438330
void __fastcall DispatchLevelScreenProjectileRequestByIndex(void* pObject, int nUnused, int param_1)
{
	void* pHolder = (void*) (*(int*) ((char*) pObject + 0x96c) + 0x1d0 + param_1 * 4);
	int iVar2 = ((int(__fastcall*)(void*)) 0x4015d2)(*(void**) pHolder);
	if (iVar2 != 0xffff && *(int*) (*(int**) pHolder + 0xb8) != 8) {
		(*(void (**)(int))(*(void***) *(int**) pHolder + 0x34 / 4))(0x1f);
		SelectSingleEntityAndDispatchSelection(pObject, nUnused, *(void**) pHolder);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_049)
// FUNCTION: LEMBALL 0x004079e0
void __fastcall EmitMainGameStatusStreamSeparators(void* param_1)
{
	void** pStatus = *(void***) 0x4a97bc;
	void* pvVar1 = (void*) (*(void* (**) (void*, int) ) * *(void***) *(void**) ((char*) param_1 + 0x50))(pStatus, 10);
	((void(__fastcall*)(void*, char)) 0x4585d0)(pvVar1, (char) pStatus);
	void** pStatus2 = *(void***) 0x4a97bc;
	void* pvVar2 = (void*) (*(void* (**) (void*, int) ) * *(void***) *(void**) ((char*) param_1 + 0x54))(pStatus2, 10);
	((void(__fastcall*)(void*, char)) 0x4585d0)(pvVar2, (char) pStatus2);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_057)
// FUNCTION: LEMBALL 0x00455e10
void* __fastcall DeleteLobbyPlayerEntries(void* pThis, int nUnused, unsigned int param_1)
{
	if ((param_1 & 2) == 0) {
		*(void**) pThis = (void**) 0x496ea0;
		if ((param_1 & 1) != 0) {
			FreeVSMemBlock(pThis);
		}
		return pThis;
	}
	int iCount = *(int*) ((char*) pThis - 4);
	int i;
	for (i = iCount - 1; i >= 0; i--) {
		((void(__fastcall*)(void*)) 0x40338c)((char*) pThis + i * 0x44);
	}
	FreeVSMemBlock((char*) pThis - 4);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_095)
// FUNCTION: LEMBALL 0x0044c100
void __fastcall expand_rect_to_cover_rect(void* pThis, int nUnused, short* pRect)
{
	int nW = pRect[0];
	int nH = pRect[1];
	if (nW * nH != 0) {
		if (pRect[2] < *(short*) ((char*) pThis + 4)) {
			*(short*) pThis = *(short*) pThis + (*(short*) ((char*) pThis + 4) - pRect[2]);
			*(short*) ((char*) pThis + 4) = pRect[2];
		}
		if ((short) (*(short*) pThis + *(short*) ((char*) pThis + 4)) < (short) (pRect[0] + pRect[2])) {
			*(short*) pThis = (pRect[2] - *(short*) ((char*) pThis + 4)) + pRect[0];
		}
		if (pRect[3] < *(short*) ((char*) pThis + 6)) {
			*(short*) ((char*) pThis + 2) = *(short*) ((char*) pThis + 2) + (*(short*) ((char*) pThis + 6) - pRect[3]);
			*(short*) ((char*) pThis + 6) = pRect[3];
		}
		if ((short) (*(short*) ((char*) pThis + 2) + *(short*) ((char*) pThis + 6)) < (short) (pRect[3] + pRect[1])) {
			*(short*) ((char*) pThis + 2) = (pRect[1] - *(short*) ((char*) pThis + 6)) + pRect[3];
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_107)
// FUNCTION: LEMBALL 0x004316c0
void __fastcall DestroyPrimaryContext(void* pObject)
{
	void* pRenderQueueBase = (char*) pObject - 0x90;
	*(void**) pObject = (void*) 0x496ba8;
	*(void**) pRenderQueueBase = (void*) 0x496bb8;
	((void(__fastcall*)(void*)) 0x45d180)(*(void**) ((char*) pObject + 0x20));
	((void(__fastcall*)(void*)) 0x45d180)(*(void**) ((char*) pObject + 0x1c));
	((void(__fastcall*)(void*)) 0x45d180)(*(void**) ((char*) pObject + 0x18));
	((void(__fastcall*)(void*, int, void*, int)) 0x4633b0)(g_pSharedRenderDispatchQueue,
														   0,
														   pRenderQueueBase != 0 ? pObject : 0,
														   -0x19);
	*(void**) ((char*) pObject + 0x40) = (void*) 0x496ca8;
	*(void**) (pRenderQueueBase != 0 ? pObject : 0) = (void*) 0x493110;
	((void(__fastcall*)(void*)) 0x463bd0)(pRenderQueueBase);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_176)
// FUNCTION: LEMBALL 0x0044b9e0
void __fastcall update_registration_info_screen_upload(void* pObject, int nUnused, int param_3)
{
	if (*(int*) ((char*) pObject + 0x20) != 0) {
		((void(__fastcall*)(void*)) 0x401d57)(pObject);
	}
	(void) nUnused;
	(void) param_3;
}
#endif
