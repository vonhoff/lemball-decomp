#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_002)
// FUNCTION: LEMBALL 0x0044f240
void __fastcall ForwardQueuedZrleVariantRangePoint(void* pObject,
												   int nUnused,
												   unsigned short param_2,
												   unsigned short param_3)
{
	if (*(void**) ((char*) pObject + 0x4c) != 0) {
		struct Pt {
			unsigned short x;
			unsigned short y;
		} pt;
		pt.x = param_2;
		pt.y = param_3;
		(*(void (**)(void*))(*(void***) *(void**) ((char*) pObject + 0x4c) + 0x38 / 4))(&pt);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_016)
// FUNCTION: LEMBALL 0x0044da30
void __fastcall SetSelectorManagerActionKeyframes(void* pObject, int nUnused, int param_2, int param_3, int param_4)
{
	*(int*) ((char*) pObject + 0x88) = param_2;
	*(int*) ((char*) pObject + 0x8c) = param_3;
	*(int*) ((char*) pObject + 0x90) = param_3;
	*(int*) ((char*) pObject + 0x94) = param_4;
	*(int*) ((char*) pObject + 0x98) = param_2;
	*(int*) ((char*) pObject + 0x9c) = param_3;
	*(int*) ((char*) pObject + 0xa0) = param_4;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_021)
// FUNCTION: LEMBALL 0x00444930
int __fastcall SelectPauseDialogPaletteRemapForIndex(void* pObject, int nUnused, int param_1)
{
	if (*(int*) ((char*) pObject + 0x118) == param_1) {
		return *(int*) ((char*) pObject + 0x1e8);
	}
	if (*(int*) ((char*) pObject + 0x124) <= param_1) {
		return *(int*) ((char*) pObject + 0x1e0);
	}
	if (param_1 <= *(int*) ((char*) pObject + 0x11c) && param_1 > 0) {
		return *(int*) ((char*) pObject + 0x1ec);
	}
	return *(int*) ((char*) pObject + 0x1e4);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_022)
// FUNCTION: LEMBALL 0x0044f3d0
void __fastcall RefreshQueuedZrleVariantRangeSelection(void* pObject)
{
	if (*(void**) ((char*) pObject + 0x44) != 0) {
		*(int*) ((char*) pObject + 0x40) = **(int**) ((char*) pObject + 0x44);
	}
	((void(__cdecl*)(void*, int)) 0x4688e0)(
		*(void**) ((char*) pObject + 0x4c),
		*(int*) (*(int*) ((char*) pObject + 0x48) +
				 (*(int*) ((char*) pObject + 0x40) - *(int*) ((char*) pObject + 0x3c)) * 4));
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_031)
// FUNCTION: LEMBALL 0x00469690
void __fastcall RefreshResourceSpriteTextOwnerLayout(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x14c) != 0) {
		((void(__cdecl*)(void*, int)) 0x469210)(*(void**) ((char*) pObject + 0x154), 0);
		return;
	}
	((void(__cdecl*)(void*, int)) 0x469210)(*(void**) ((char*) pObject + 0x150), 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_044)
// FUNCTION: LEMBALL 0x0044c5b0
void __fastcall SetDualChildOverlayPositions(void* pThis, int nUnused, short param_2, short param_3)
{
	unsigned short pos[2];
	pos[0] = (unsigned short) param_2;
	pos[1] = (unsigned short) param_3;
	if (*(int**) ((char*) pThis + 0x50) != 0) {
		(*(void (**)(void*))(*(void***) *(void**) ((char*) pThis + 0x50) + 0x38 / 4))(pos);
	}
	if (*(int**) ((char*) pThis + 0x54) != 0) {
		(*(void (**)(void*))(*(void***) *(void**) ((char*) pThis + 0x54) + 0x38 / 4))(pos);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_048)
// FUNCTION: LEMBALL 0x00443d80
void __fastcall RebuildLevelScreenPauseDialogLayout(void* param_1)
{
	short layout[4];
	((void(__fastcall*)(void*)) 0x403198)(param_1);
	((void(__fastcall*)(void*)) 0x402603)(param_1);
	((void(__fastcall*)(void*, short*)) 0x4030fd)(param_1, layout);
	((void(__fastcall*)(void*, short*)) 0x401c85)(param_1, layout);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_090)
extern void* RegisterPaletteRemapVariant(int nPaletteResourceId, unsigned char* pRemapTable, int nBuildMode);

// FUNCTION: LEMBALL 0x004448c0
void __fastcall RegisterPauseDialogPaletteRemaps(void* pObject)
{
	unsigned char** ppRemapTables = (unsigned char**) 0x49f058;
	int i;
	for (i = 0; i < 4; i++) {
		*(void**) ((char*) pObject + 0x1e0 + i * 4) =
			RegisterPaletteRemapVariant(*(int*) (*(char**) ((char*) pObject + 0x114) + 0x54), ppRemapTables[i], 2);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_101)
// FUNCTION: LEMBALL 0x00468b20
void* __fastcall ConstructBufferedResourceGeometryChild(void* pThis, int nUnused, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, int, void*)) 0x467cd0)(pThis, 0, param_2);
	*(void**) pThis = (void*) 0x497630;
	*(int*) ((char*) pThis + 0x108) = 0;
	*(int*) ((char*) pThis + 0x104) = 0;
	*(void**) pThis = (void*) 0x499838;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x497608;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499818;
	*(void**) ((char*) pThis + 0x114) = param_3;
	((void(__fastcall*)(void*)) 0x468b80)(pThis);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_102)
// FUNCTION: LEMBALL 0x00468e50
void* __fastcall ConstructSingleChildOverlayOwnerFromRect(void* pThis,
														  int nUnused,
														  void* param_2,
														  void* param_3,
														  void* param_4,
														  void* param_5,
														  void* param_6)
{
	((void(__fastcall*)(void*, int, void*, void*, void*)) 0x468a40)(pThis, 0, param_2, param_3, param_5);
	*(void**) pThis = (void*) 0x499928;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499908;
	void* pBlock = ((void*(__cdecl*) (int) ) 0x45a780)(0x130);
	if (pBlock != 0) {
		*(void**) ((char*) pThis + 0x118) =
			((void*(__fastcall*) (void*, int, void*, void*, void*, void*) ) 0x468410)(pBlock,
																					  0,
																					  param_2,
																					  param_3,
																					  param_4,
																					  param_6);
	}
	else {
		*(void**) ((char*) pThis + 0x118) = 0;
	}
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_103)
// FUNCTION: LEMBALL 0x00468f90
void* __fastcall ConstructResourceSpriteGeometryChildFromRect(void* pThis,
															  int nUnused,
															  void* param_2,
															  void* param_3,
															  void* param_4,
															  void* param_5)
{
	ConstructBufferedResourceGeometryChild(pThis, 0, (void*) 0xf7, param_3);
	*(unsigned short*) ((char*) pThis + 0x12a) = 0;
	*(unsigned short*) ((char*) pThis + 0x128) = 0;
	*(void**) pThis = (void*) 0x499a18;
	*(unsigned short*) ((char*) pThis + 0x12e) = 0;
	*(unsigned short*) ((char*) pThis + 0x12c) = 0;
	*(unsigned short*) ((char*) pThis + 0x142) = 0;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x4999f8;
	*(unsigned short*) ((char*) pThis + 0x140) = 0;
	unsigned short* pRect = (unsigned short*) param_2;
	unsigned short* pRectWH = (unsigned short*) (param_2 != 0 ? (char*) param_2 + 4 : 0);
	*(unsigned short*) ((char*) pThis + 0xdc) = (unsigned short) (param_2 != 0 ? pRectWH[0] : 0);
	*(unsigned short*) ((char*) pThis + 0xde) = (unsigned short) (param_2 != 0 ? pRectWH[1] : 0);
	*(unsigned short*) ((char*) pThis + 0xc0) = (unsigned short) (param_2 != 0 ? pRect[0] : 0);
	*(unsigned short*) ((char*) pThis + 0xc2) = (unsigned short) (param_2 != 0 ? pRect[1] : 0);
	*(unsigned short*) ((char*) pThis + 0xc4) = 0;
	*(unsigned short*) ((char*) pThis + 0xc6) = 0;
	((void(__fastcall*)(void*, int, int)) 0x46a530)((char*) pThis + 0x90, 0, 1);
	*(void**) ((char*) pThis + 0x130) = param_4;
	*(void**) ((char*) pThis + 0x138) = param_5;
	((void(__fastcall*)(void*)) 0x4693b0)(pThis);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_104)
// FUNCTION: LEMBALL 0x0044f970
void __fastcall record_queued_variant_child_slot_state(void* pObject,
													   int nUnused,
													   int param_1,
													   int param_2,
													   int param_3)
{
	int iSlot = *(int*) ((char*) pObject + 0x80);
	*(int*) ((char*) pObject + 0xbc + iSlot * 0x10) = 1;
	*(int*) ((char*) pObject + 0xc0 + iSlot * 0x10) = param_1;
	*(int*) ((char*) pObject + 0xc4 + iSlot * 0x10) = param_2;
	*(int*) ((char*) pObject + 0xc8 + iSlot * 0x10) = param_3;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_105)
// FUNCTION: LEMBALL 0x0044d080
void __fastcall append_non_zrle_selector_child(void* pObject,
											   int nUnused,
											   int param_2,
											   int param_3,
											   int param_4,
											   int param_5,
											   int param_6,
											   int param_7,
											   int param_8,
											   int param_9,
											   unsigned short* param_10,
											   int param_11,
											   int param_12)
{
	int iCounter = *(int*) ((char*) pObject + 0x218) + 1;
	int nChild = *(int*) ((char*) pObject + 0x80);
	*(int*) ((char*) pObject + 0x218) = iCounter;
	void* pEntry = ((void*(__cdecl*) (unsigned int) ) 0x45a780)(0x60);
	if (pEntry == 0) {
		*(int*) ((char*) pObject + nChild * 4 + 0x1e0) = 0;
	}
	else {
		*(int*) ((char*) pObject + nChild * 4 + 0x1e0) = (int) ((
			int*(__fastcall*) (void*, unsigned short*, int, int, int, int, int, int, int, int, int, int) ) 0x402617)(
			pEntry,
			param_10,
			*(int*) ((char*) pObject + 0x214),
			*(int*) ((char*) pObject + 0x210),
			param_2,
			param_3,
			param_4,
			param_5,
			param_8,
			iCounter,
			param_9,
			param_11);
	}
	((void(__fastcall*)(void*, int, int, int, int)) 0x40165e)(
		pObject,
		param_2,
		param_3,
		1,
		*(int*) (*(int*) ((char*) pObject + nChild * 4 + 0x1e0) + 0x30));
	*(int*) (*(int*) (*(int*) (*(int*) ((char*) pObject + nChild * 4 + 0x1e0) + 0x54) + 0x134) + 0x130) = param_12;
	*(int*) ((char*) pObject + 0x80) = nChild + 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_106)
// FUNCTION: LEMBALL 0x0044e700
void __fastcall emit_variant_render_entry_with_temporary_context(void* pObject,
																 int nUnused,
																 unsigned short* param_1,
																 int param_2,
																 unsigned short* param_3,
																 int param_4,
																 int param_5,
																 int* param_6,
																 int param_7)
{
	int nPrevContext = *(int*) ((char*) pObject + 0x1c);
	*(int*) ((char*) pObject + 0x1c) = param_2;
	unsigned short localRect[4];
	((void(__fastcall*)(void*, unsigned short*, unsigned short*, int, int, int*, int)) 0x467730)(pObject,
																								 &localRect[0],
																								 param_3,
																								 param_4,
																								 param_5,
																								 param_6,
																								 param_7);
	*(int*) ((char*) pObject + 0x1c) = nPrevContext;
	param_1[0] = localRect[0];
	param_1[1] = localRect[1];
	param_1[2] = localRect[2];
	param_1[3] = localRect[3];
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_114)
// FUNCTION: LEMBALL 0x00468b80
void __fastcall initialize_buffered_geometry_child_storage(void* self)
{
	int* block;
	int* entry;
	block = (int*) ((void*(__cdecl*) (int) ) 0x45a780)(0x14);
	if (block == 0) {
		*(int*) ((char*) self + 0x10c) = 0;
	}
	else {
		*block = 1;
		entry = block + 1;
		((void(__fastcall*)(void*)) 0x40272f)(entry);
		*(int**) ((char*) self + 0x10c) = entry;
	}
	*(int*) ((char*) self + 0x50) += 1;
	block = (int*) ((void*(__cdecl*) (int) ) 0x45a780)(0x44);
	if (block != 0) {
		*block = 4;
		entry = block + 1;
		((void(__fastcall*)(void*)) 0x401c7b)(entry);
		((void(__fastcall*)(void*)) 0x401c7b)(entry + 0x10);
		((void(__fastcall*)(void*)) 0x401c7b)(entry + 0x20);
		((void(__fastcall*)(void*)) 0x401c7b)(entry + 0x30);
		*(int**) ((char*) self + 0x110) = entry;
	}
	else {
		*(int*) ((char*) self + 0x110) = 0;
	}
	*(int*) ((char*) self + 0x50) += 4;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_115)
// FUNCTION: LEMBALL 0x00469480
void __fastcall refresh_resource_sprite_window_owner_frame_draw_entry(void* pObject)
{
	short uStack_4;
	short uStack_2;
	int iVar1;
	int iVar3;
	bool bVar2 = (*(int*) ((char*) pObject + 0xd0) != 0) && (*(int*) ((char*) pObject + 0x94) != 0);
	if (bVar2) {
		uStack_4 = *(unsigned short*) ((char*) pObject + 0x128);
		iVar3 = *(int*) ((char*) pObject + 0x11c);
		uStack_2 = *(unsigned short*) ((char*) pObject + 0x12a);
	}
	else {
		uStack_4 = *(unsigned short*) ((char*) pObject + 0x12c);
		iVar3 = *(int*) ((char*) pObject + 0x118);
		uStack_2 = *(unsigned short*) ((char*) pObject + 0x12e);
	}
	if (iVar3 != 0) {
		iVar1 = *(int*) (*(int*) ((char*) pObject + 0x4c) + 0xc);
		(*(void (**)(void))(*(void***) (*(int*) (*(int*) (iVar1 + 0x40) + 4) + iVar1 + 0x40) + 0x38 / 4))();
		(*(void (**)(short*, int, int, int, int))(
			*(void***) *(void**) ((char*) pObject + 0x124) +
			0x14 / 4))(&uStack_4, *(int*) ((char*) pObject + 0x134), iVar3, 0x20, *(int*) ((char*) pObject + 0x144));
		(*(void (**)(void*))(*(void***) *(void**) ((char*) pObject + 0x124) + 4 / 4))(
			*(void**) ((char*) pObject + 0x4c));
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_118)
// FUNCTION: LEMBALL 0x00468f00
void* __fastcall construct_single_child_overlay_owner_from_point(void* self,
																 int nEdxSlop,
																 void* pPoint,
																 void* argColor,
																 int argOriginX,
																 void* argRes,
																 int argOriginY)
{
	void* block;
	int* child;
	((void(__fastcall*)(void*, void*, void*)) 0x468b20)(self, argColor, argRes);
	*(void**) self = (void*) 0x499928;
	*(void**) ((char*) self + 0x90) = (void*) 0x499908;
	block = (void*) ((void*(__cdecl*) (int) ) 0x45a780)(0x130);
	if (block == 0) {
		*(int*) ((char*) self + 0x118) = 0;
	}
	else {
		child = ((int*(
			__fastcall*) (void*, void*, void*, int, int) ) 0x468530)(block, pPoint, argColor, argOriginX, argOriginY);
		*(int**) ((char*) self + 0x118) = child;
	}
	*(unsigned short*) ((char*) self + 0xdc) = *(unsigned short*) pPoint;
	*(unsigned short*) ((char*) self + 0xde) = *(unsigned short*) ((char*) pPoint + 2);
	return self;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_121)
// FUNCTION: LEMBALL 0x004695d0
void* __fastcall construct_resource_sprite_window_owner_variant_a(void* pThis,
																  void* pUnusedEdx,
																  void* param_2,
																  void* param_3,
																  void* param_4,
																  void* param_5,
																  void* param_6)
{
	((void*(
		__fastcall*) (void*, int, void*, void*, void*, void*) ) 0x468f90)(pThis, 0, param_3, param_4, param_5, param_6);
	*(void**) pThis = (void*) 0x499b28;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499b08;
	((void(__fastcall*)(void*)) 0x469670)(pThis);
	*(void**) ((char*) pThis + 0xcc) = param_2;
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_122)
// FUNCTION: LEMBALL 0x00469620
void* __fastcall construct_resource_sprite_window_owner_variant_b(void* pThis,
																  void* pUnusedEdx,
																  void* param_2,
																  void* param_3,
																  void* param_4,
																  void* param_5,
																  void* param_6)
{
	((void*(
		__fastcall*) (void*, int, void*, void*, void*, void*) ) 0x469070)(pThis, 0, param_3, param_4, param_5, param_6);
	*(void**) pThis = (void*) 0x499b28;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499b08;
	((void(__fastcall*)(void*)) 0x469670)(pThis);
	*(void**) ((char*) pThis + 0xcc) = param_2;
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_126)
// FUNCTION: LEMBALL 0x00469120
void __fastcall expand_text_owner_bounds_for_extent(void* pThis, int nEdxSlop, short* pExtent)
{
	short sW, sH;
	if ((int) *(short*) ((char*) pThis + 0x140) * (int) *(short*) ((char*) pThis + 0x142) != 0) {
		sW = *(short*) ((char*) pThis + 0x140) * 2 + pExtent[0];
		sH = pExtent[1] + *(short*) ((char*) pThis + 0x142) * 2;
		if (*(short*) ((char*) pThis + 0xc0) < sW) {
			*(short*) ((char*) pThis + 0xc0) = sW;
		}
		if (*(short*) ((char*) pThis + 0xc2) < sH) {
			*(short*) ((char*) pThis + 0xc2) = sH;
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_127)
// FUNCTION: LEMBALL 0x00469180
void __fastcall align_text_owner_extent_within_bounds(void* pThis, int nEdxSlop, short* pPos, short* pExtent)
{
	if ((*(unsigned int*) ((char*) pThis + 0x138) & 0x10) == 0) {
		if ((*(unsigned int*) ((char*) pThis + 0x138) & 4) != 0) {
			pPos[0] = (short) (((int) *(short*) ((char*) pThis + 0xc0) - (int) pExtent[0]) / 2);
		}
	}
	else {
		pPos[0] = *(short*) ((char*) pThis + 0xc0) - pExtent[0];
	}
	if ((*(unsigned int*) ((char*) pThis + 0x138) & 0x20) == 0) {
		if ((*(unsigned int*) ((char*) pThis + 0x138) & 8) != 0) {
			pPos[1] = (short) (((int) *(short*) ((char*) pThis + 0xc2) - (int) pExtent[1]) / 2);
		}
	}
	else {
		pPos[1] = *(short*) ((char*) pThis + 0xc2) - pExtent[1];
	}
	*(unsigned int*) ((char*) pThis + 0xd8) = 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_129)
// FUNCTION: LEMBALL 0x0044f290
void __fastcall load_queued_zrle_variant_range_resources(void* pThis, int nUnused, int* param_1)
{
	void* pArray;
	void* pOverlay;
	int* pInner;
	short pt[2];
	int i;
	*(int*) ((char*) pThis + 0x48) = (int) param_1;
	pArray = ((void*(__cdecl*) (unsigned int) ) 0x45a780)((unsigned int) *(int*) ((char*) pThis + 0x34) << 2);
	*(int*) ((char*) pThis + 0x50) = (int) pArray;
	for (i = 0; i < *(int*) ((char*) pThis + 0x34); i++) {
		*(int*) ((char*) pArray + i * 4) = (int) ((void*(__cdecl*) (int) ) 0x45d610)(param_1[i]);
	}
	pOverlay = ((void*(__cdecl*) (unsigned int) ) 0x45a780)(0x130);
	if (pOverlay == 0) {
		*(int*) ((char*) pThis + 0x4c) = 0;
	}
	else {
		pt[0] = (short) *(int*) ((char*) pThis + 0x18);
		pt[1] = (short) *(int*) ((char*) pThis + 0x1c);
		*(int*) ((char*) pThis + 0x4c) = (int) ((void*(__fastcall*) (void*, int, short*, int, int, int) ) 0x468530)(
			pOverlay,
			0,
			pt,
			*(int*) ((char*) pThis + 0x24),
			param_1[*(int*) ((char*) pThis + 0x40) - *(int*) ((char*) pThis + 0x3c)],
			3);
	}
	pOverlay = (void*) *(int*) ((char*) pThis + 0x4c);
	pInner = (int*) *(int*) (*(int*) ((char*) pOverlay + 0x4c) + 0xc);
	((void(__fastcall*)(void*, int, int)) 0x467e40)(pOverlay, 0, 0);
	*(int*) (*(int*) (*(int*) ((char*) pInner + 0x40) + 4) + 0x70 + (int) pInner) = 0;
	*(int*) ((char*) pOverlay + 0xf4) = (int) g_pSharedRenderDispatchQueue;
	*(int*) ((char*) pOverlay + 0xcc) = *(int*) ((char*) pThis + 0x2c);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_130)
// FUNCTION: LEMBALL 0x0044fa00
void __fastcall sample_helper_point_into_owner_and_emit_variant_entry(void* param_1)
{
	void* pRenderContext;
	char* pGeometryOwner;
	char* pGeometryDispatch;
	int nPointBias;
	int nSavedOwner;
	short pt[2];
	char auStack_c[12];
	if (*(int*) ((char*) param_1 + 0x144) != 0) {
		pRenderContext = *(void**) ((char*) param_1 + 0x12c);
		nPointBias = *(int*) ((char*) param_1 + 0x130) == 1 ? -1 : -2;
		pGeometryOwner = *(char**) ((char*) pRenderContext + 0x0c);
		pGeometryDispatch = pGeometryOwner + 0x40 + *(int*) ((char*) *(void**) (pGeometryOwner + 0x40) + 4);
		*(short*) ((char*) param_1 + 0x100) = *(short*) (pGeometryDispatch + 0x14);
		*(short*) ((char*) param_1 + 0x102) = *(short*) (pGeometryDispatch + 0x16);
		*(short*) ((char*) param_1 + 0x104) = 0;
		*(short*) ((char*) param_1 + 0x106) = 0;
		*(int*) ((char*) param_1 + 0x108) = 0x10000;
		(*(void(__fastcall**)(void*, int, void*))(
			*(void***) ((char*) param_1 + 0xfc))[0x4 / 4])((char*) param_1 + 0xfc, 0, pRenderContext);
		*(int*) ((char*) param_1 + 0x124) = 0;
		pt[0] = (short) (*(int*) ((char*) param_1 + 0x98) + nPointBias);
		pt[1] = (short) (*(int*) ((char*) param_1 + 0x9c) + nPointBias);
		nSavedOwner = *(int*) ((char*) param_1 + 0x2c);
		*(int*) ((char*) param_1 + 0x2c) = (int) pRenderContext;
		((int(__fastcall*)(void*, void*, void*, int, int, void*, int)) 0x467730)(
			(char*) param_1 + 0x10,
			auStack_c,
			pt,
			g_nQueuedVariantChildSlotManagerModeSelectedResourceId,
			0,
			(char*) param_1 + 0x118,
			0);
		*(int*) ((char*) param_1 + 0x2c) = nSavedOwner;
		((void(__fastcall*)(void*)) 0x4678c0)((char*) param_1 + 0x10);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_137)
// FUNCTION: LEMBALL 0x00469530
void __fastcall queue_resource_sprite_window_owner_if_dirty(int* pThis)
{
	int* pBuffer;
	void* pQueue;
	int nRet;
	int nHelper;
	if (pThis[0x52] != pThis[0x51]) {
		pThis[0x36] = 1;
	}
	pBuffer = (int*) pThis[0x13];
	if ((*(int*) ((char*) pBuffer + 4) == 0) &&
		((pThis[0x3e] != 0) || (pThis[0x36] != 0) || (pThis[0x34] != pThis[0x35]))) {
		nRet = (**(int(__fastcall**)(void*))(*pThis + 0x68))(pThis);
		if (nRet != 0) {
			(**(void(__fastcall**)(void*))(*pThis + 0xb8))(pThis);
			((void(__fastcall*)(int)) 0x468c50)((int) pThis);
			(**(void(__fastcall**)(void*))(*pThis + 0xbc))(pThis);
		}
		pQueue = *(void**) ((char*) pBuffer + 0xc);
		nHelper = (**(int(__fastcall**)(void*))(*(int*) pQueue + 8))(pQueue);
		((void(__fastcall*)(void*, int, int)) 0x4670f0)(pBuffer, 0, pThis[0x38]);
		((void(__fastcall*)(int)) 0x466b60)(nHelper);
		pThis[0x3f] = 1;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_141)
// FUNCTION: LEMBALL 0x00444a90
void __fastcall update_pause_dialog_hovered_option_at_point(void* p, short* param_1)
{
	int i;
	*(int*) ((char*) p + 0x64) = 0;
	i = *(int*) ((char*) p + 0x80);
	if (i < *(int*) ((char*) p + 0x7c)) {
		short sx = (short) (param_1[0] - *(short*) ((char*) p - 0x8c));
		short sy = (short) (param_1[1] - *(short*) ((char*) p - 0x8a));
		short* r = (short*) (*(int*) ((char*) p + 0x154) + 4 + i * 8);
		do {
			if ((r[-2] <= sx && sx < (short) (r[-2] + r[0])) && (r[-1] <= sy && sy < (short) (r[-1] + r[1]))) {
				*(int*) ((char*) p + 0x64) = 4;
				*(int*) ((char*) p + 0x74) = i;
				break;
			}
			r += 4;
			++i;
		} while (i < *(int*) ((char*) p + 0x7c));
	}
	((void(__cdecl*)(int, int)) 0x401b72)(1, *(int*) ((char*) p + 0x64));
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_148)
// FUNCTION: LEMBALL 0x00469070
void* __fastcall construct_resource_sprite_geometry_child_from_point_size(void* pThis,
																		  int nEdxSlop,
																		  const short* pPoint,
																		  const short* pSize,
																		  int nBufferSize,
																		  int nColor)
{
	((void(__fastcall*)(void*, int, int)) 0x468b20)(pThis, nBufferSize, 0xf7);
	*(unsigned short*) ((char*) pThis + 0x128) = 0;
	*(unsigned short*) ((char*) pThis + 0x12a) = 0;
	*(unsigned short*) ((char*) pThis + 0x12c) = 0;
	*(unsigned short*) ((char*) pThis + 0x12e) = 0;
	*(unsigned short*) ((char*) pThis + 0x140) = 0;
	*(unsigned short*) ((char*) pThis + 0x142) = 0;
	*(void**) ((char*) pThis + 0x000) = (void*) 0x00499a18;
	*(void**) ((char*) pThis + 0x090) = (void*) 0x004999f8;
	*(unsigned short*) ((char*) pThis + 0x0dc) = pPoint[0];
	*(unsigned short*) ((char*) pThis + 0x0de) = pPoint[1];
	*(int*) ((char*) pThis + 0x130) = nColor;
	*(int*) ((char*) pThis + 0x138) = 0x0c;
	*(unsigned short*) ((char*) pThis + 0x140) = pSize[0];
	*(unsigned short*) ((char*) pThis + 0x142) = pSize[1];
	((void(__fastcall*)(void*)) 0x4693b0)(pThis);
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_164)
struct BufferedGeometryChildWindowView {
	virtual void ReservedSlot0(void);
	virtual void ConfigureBufferedGeometryChild(short* pRect, void* pOwner, const char* pszName);
};

// FUNCTION: LEMBALL 0x00468a40
void* __fastcall ConstructBufferedGeometryChildOverlay(void* pThis,
													   int nUnusedEdx,
													   void* pRect,
													   void* pRenderOwner,
													   void* pOverlayOwner)
{
	short aRect[4];
	char* pChildSubobject;
	char* pBytes;
	void* pAdjustedChild;

	pChildSubobject = (char*) pThis + 0x90;
	pBytes = (char*) pThis;
	((void(__fastcall*)(void*, int, void*, void*)) 0x467c10)(pThis, 0, pRect, pRenderOwner);
	*(void**) pBytes = (void*) 0x497630;
	*(void**) pChildSubobject = (void*) 0x497608;
	*(int*) (pBytes + 0x108) = 0;
	*(int*) (pBytes + 0x104) = 0;
	*(void**) pBytes = (void*) 0x499838;
	*(void**) pChildSubobject = (void*) 0x499818;
	*(void**) (pBytes + 0x114) = pOverlayOwner;
	initialize_buffered_geometry_child_storage(pThis);
	aRect[0] = *(short*) (pBytes + 0xc0);
	aRect[1] = *(short*) (pBytes + 0xc2);
	aRect[2] = *(short*) (pBytes + 0xdc);
	aRect[3] = *(short*) (pBytes + 0xde);
	((BufferedGeometryChildWindowView*) pThis)
		->ConfigureBufferedGeometryChild(aRect, *(void**) (pBytes + 0xc8), (const char*) 0x49f02c);
	*(short*) (pBytes + 0xc4) += *(short*) (pBytes + 0x18);
	*(short*) (pBytes + 0xc6) += *(short*) (pBytes + 0x1a);
	pAdjustedChild = pThis != 0 ? pChildSubobject : 0;
	AppendGeometryOwnerChild(*(void**) (*(int*) (pBytes + 0xc8) + 0x1c), 0, pAdjustedChild);
	(void) nUnusedEdx;
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_167)
// FUNCTION: LEMBALL 0x0044f400
void* __fastcall construct_queued_variant_child_context_object(void* pObject)
{
	((void(__fastcall*)(void*)) 0x463b50)(pObject);
	*(void**) pObject = (void*) 0x498160;
	*(int*) ((char*) pObject + 0x50) = *(int*) ((char*) pObject + 0x50) + 1;
	return pObject;
}
#endif
