#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_067)
// Compiler-state preservation body; SCREENVT.CPP owns the annotated implementation.
int __fastcall PreserveDispatchIndexedEntryByModeCodegen(void* pThis, int, unsigned short* pMode)
{
	unsigned short nMode = *pMode;
	if (nMode > 2 && nMode < 5 && *(int*) ((char*) pThis + 0x14) > 0) {
		int* pSlotId = (int*) (*(int*) ((char*) pThis + 0x10) + 4);
		int iSlot = 0;
		while (*pSlotId != *(int*) (pMode + 4)) {
			pSlotId += 2;
			iSlot++;
			if (*(int*) ((char*) pThis + 0x14) <= iSlot) {
				return 0;
			}
		}
		int* pEntry = (int*) (*(int*) (*(int*) ((char*) pThis + 0x10) + iSlot * 8));
		short position[2] = {0, 0};
		if (nMode == 4) {
			(*(void (**)(void*, int))(*(void***) ((char*) pEntry + 0x90) + 4 / 4))(position, 0);
		}
		else {
			(*(void (**)(void*, int))(*(void***) ((char*) pEntry + 0x90) + 8 / 4))(position, 0);
		}
		return 1;
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_097)
// FUNCTION: LEMBALL 0x0043a130
void* __fastcall ConstructResourceWindowOwnerBuffer(void* pThis,
													void* pUnusedEdx,
													unsigned int param_2,
													unsigned int param_3,
													unsigned int param_4,
													unsigned int param_5)
{
	((void(__cdecl*)(unsigned int, unsigned int, int, int)) 0x468f90)(param_2, param_3, 0x30, 0xc);
	*(void**) pThis = (void*) 0x497108;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x4970e0;
	*(int*) ((char*) pThis + 0x14c) = 0;
	((void(__fastcall*)(void*, int, unsigned int)) 0x403017)(pThis, 0, param_2);
	*(void**) ((char*) pThis + 0xf4) = g_pSharedRenderDispatchQueue;
	*(void**) ((char*) pThis + 0xcc) = (void*) param_2;
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_098)
// FUNCTION: LEMBALL 0x0043a250
void* __fastcall ConstructRegisteredRenderSlotArray(void* pThis, int nUnused, int param_1)
{
	void* pAlloc;
	int i;
	((void*(__fastcall*) (void*, int) ) 0x462ea0)(pThis, 0);
	*(void**) pThis = (void*) 0x497208;
	pAlloc = AllocateVSMemBlock((unsigned int) param_1 * 8);
	*(void**) ((char*) pThis + 0x10) = pAlloc;
	*(int*) ((char*) pThis + 0x18) = param_1;
	*(int*) ((char*) pThis + 0x14) = 0;
	for (i = 0; i < param_1; i++) {
		*(int*) ((char*) pAlloc + i * 8) = 0;
		*(int*) ((char*) pAlloc + i * 8 + 4) = 0;
	}
	((void(__fastcall*)(void*, int, void*, int)) 0x4632a0)(g_pSharedRenderDispatchQueue, 0, pThis, -0x19);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_108)
// FUNCTION: LEMBALL 0x0043a1d0
void __fastcall SetTextOwnerStringAndLayout(void* pThis, int nUnused, char* pszText)
{
	if (*(void**) ((char*) pThis + 0x14c) != 0) {
		FreeVSMemBlock(*(void**) ((char*) pThis + 0x14c));
	}
	*(void**) ((char*) pThis + 0x14c) = AllocateVSMemBlock((unsigned int) strlen(pszText) + 1);
	memcpy(*(void**) ((char*) pThis + 0x14c), pszText, strlen(pszText) + 1);
	((void(__fastcall*)(void*, int, void*, int)) 0x469210)(pThis, 0, *(void**) ((char*) pThis + 0x14c), 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_177)
#pragma auto_inline(off)
#pragma comment(linker, "/include:?destroy_resource_window_owner_buffer_0043a190@@YIXPAX@Z")
// FUNCTION: LEMBALL 0x0043a190
void __fastcall destroy_resource_window_owner_buffer_0043a190(void* pThis)
{
	*(void**) ((char*) pThis + 0x000) = (void*) 0x004970e0;
	*(void**) ((char*) pThis - 0x090) = (void*) 0x00497108;
	if (*(void**) ((char*) pThis + 0x0bc) != 0) {
		FreeVSMemBlock(*(void**) ((char*) pThis + 0x0bc));
	}
	DestroyResourceSpriteGeometryChild(pThis);
}
#pragma auto_inline(on)
#endif
