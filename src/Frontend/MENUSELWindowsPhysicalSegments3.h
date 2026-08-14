#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_003)
extern void __fastcall expand_text_owner_bounds_for_extent(void* pThis, int nEdxSlop, short* pExtent);

// NOTE: 0x004766F0 unmatchable: natural clipped ZRLE decoder structure measured 15.20%.
// NOTE: 0x00476EE0 unmatchable: mirrored clipped decoder register/loop shape measured 7.06%.

// FUNCTION: LEMBALL 0x00468C50
void __fastcall update_buffered_geometry_child_fill_and_edges(void* pObject)
{
	char* pThis;
	char* pEntry;
	int nTarget;
	int nOffset;
	int nFill;
	int nEdge;
	int nActive;
	short nWidth;
	short nHeight;

	pThis = (char*) pObject;
	nTarget = *(int*) (*(int*) (pThis + 0x4c) + 0x0c);
	((void(__fastcall*)(void*))(*(void***) (*(int*) (*(int*) (nTarget + 0x40) + 4) + nTarget + 0x40))[0x38 / 4])(
		(void*) (*(int*) (*(int*) (nTarget + 0x40) + 4) + nTarget + 0x40));

	pEntry = *(char**) (pThis + 0x10c);
	*(short*) (pEntry + 4) = *(short*) (pThis + 0xc0);
	*(short*) (pEntry + 6) = *(short*) (pThis + 0xc2);
	*(short*) (pEntry + 8) = 0;
	*(short*) (pEntry + 0x0a) = 0;
	*(int*) (pEntry + 0x0c) = *(int*) (pThis + 0x114);
	((void(__fastcall*)(void*, int, void*))(*(void***) pEntry)[1])(pEntry, 0, *(void**) (pThis + 0x4c));

	nActive = (*(int*) (pThis + 0xd0) != 0 && *(int*) (pThis + 0x94) != 0);
	if (nActive) {
		nEdge = 0xf8;
		nFill = 0xff;
	}
	else {
		nEdge = 0xff;
		nFill = 0xf8;
	}

	pEntry = *(char**) (pThis + 0x110);
	nWidth = *(short*) (pThis + 0xc0);
	nHeight = *(short*) (pThis + 0xc2);
	*(short*) (pEntry + 4) = 0;
	*(short*) (pEntry + 6) = 0;
	*(short*) (pEntry + 8) = nWidth - 1;
	*(short*) (pEntry + 0x0a) = 0;
	*(int*) (pEntry + 0x0c) = nEdge;

	*(short*) (pEntry + 0x14) = 0;
	*(short*) (pEntry + 0x16) = 0;
	*(short*) (pEntry + 0x18) = 0;
	*(short*) (pEntry + 0x1a) = nHeight - 1;
	*(int*) (pEntry + 0x1c) = nEdge;

	*(short*) (pEntry + 0x24) = nWidth - 1;
	*(short*) (pEntry + 0x26) = 0;
	*(short*) (pEntry + 0x28) = nWidth - 1;
	*(short*) (pEntry + 0x2a) = nHeight - 1;
	*(int*) (pEntry + 0x2c) = nFill;

	*(short*) (pEntry + 0x34) = 0;
	*(short*) (pEntry + 0x36) = nHeight - 1;
	*(short*) (pEntry + 0x38) = nWidth - 1;
	*(short*) (pEntry + 0x3a) = nHeight - 1;
	*(int*) (pEntry + 0x3c) = nFill;

	nOffset = 0;
	do {
		pEntry = *(char**) (pThis + 0x110) + nOffset;
		nOffset += 0x10;
		((void(__fastcall*)(void*, int, void*))(*(void***) pEntry)[1])(pEntry, 0, *(void**) (pThis + 0x4c));
	} while (nOffset < 0x40);
}

// FUNCTION: LEMBALL 0x00469210
void __fastcall layout_resource_sprite_text_owner(void* pObject, int, char* pPrimaryText, char* pSecondaryText)
{
	char* pThis;
	short aPrimaryExtent[4];
	short aSecondaryExtent[4];
	short aRect[4];
	short aPrimaryPosition[2];
	short aSecondaryPosition[2];
	void* pChild;

	pThis = (char*) pObject;
	*(char**) (pThis + 0x11c) = pSecondaryText;
	*(char**) (pThis + 0x118) = pPrimaryText;
	((void(__fastcall*)(void*, int, short*, char*, int)) 0x0045DB30)(*(void**) (pThis + 0x134),
																	 0,
																	 aPrimaryExtent,
																	 pPrimaryText,
																	 0x20);
	expand_text_owner_bounds_for_extent(pThis, 0, aPrimaryExtent);

	aSecondaryExtent[0] = aPrimaryExtent[0];
	aSecondaryExtent[1] = aPrimaryExtent[1];
	if (pSecondaryText != 0) {
		((void(__fastcall*)(void*, int, short*, char*, int)) 0x0045DB30)(*(void**) (pThis + 0x134),
																		 0,
																		 aSecondaryExtent,
																		 pSecondaryText,
																		 0x20);
		expand_text_owner_bounds_for_extent(pThis, 0, aSecondaryExtent);
	}

	((void(__fastcall*)(void*, int, short*, short*)) 0x00469180)(pThis, 0, (short*) (pThis + 0x12c), aPrimaryPosition);
	if (pSecondaryText == 0) {
		*(char**) (pThis + 0x11c) = pPrimaryText;
		*(short*) (pThis + 0x128) = *(short*) (pThis + 0x12c);
		*(short*) (pThis + 0x12a) = *(short*) (pThis + 0x12e);
	}
	else {
		((void(__fastcall*)(void*, int, short*, short*)) 0x00469180)(pThis,
																	 0,
																	 (short*) (pThis + 0x128),
																	 aSecondaryPosition);
	}
	++*(short*) (pThis + 0x128);
	++*(short*) (pThis + 0x12a);

	if (*(int*) (pThis + 0x13c) == 0) {
		aRect[0] = *(short*) (pThis + 0xc0);
		aRect[1] = *(short*) (pThis + 0xc2);
		aRect[2] = *(short*) (pThis + 0xdc);
		aRect[3] = *(short*) (pThis + 0xde);
		((void(__fastcall*)(void*, int, short*, void*, const char*))(
			*(void***) pThis)[1])(pThis, 0, aRect, *(void**) (pThis + 0xc8), (const char*) 0x0049F02C);
		pChild = pThis != 0 ? pThis + 0x90 : 0;
		*(short*) (pThis + 0xc4) += *(short*) (pThis + 0x18);
		*(short*) (pThis + 0xc6) += *(short*) (pThis + 0x1a);
		((void(__fastcall*)(void*, int, void*)) 0x0046A9A0)(*(void**) (*(int*) (pThis + 0xc8) + 0x1c), 0, pChild);
		((void(__fastcall*)(void*, int, int)) 0x0046A530)(pThis + 0x90, 0, 1);
		*(int*) (pThis + 0x13c) = 1;
	}
	*(int*) (pThis + 0xd8) = 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_004)
extern void* g_pSharedRenderDispatchQueue;

// FUNCTION: LEMBALL 0x00443AF0
void __fastcall configure_level_screen_pause_dialog_options(void* pObject)
{
	char* pThis;
	const char** ppText;
	short* pRects;
	short* pRect;
	int nMode;
	int nIndex;
	int nRemaining;
	int nRectCount;
	int nChild;

	pThis = (char*) pObject;
	nIndex = 0;
	nMode = *(int*) (pThis + 0x104);
	if (nMode >= 0) {
		nRemaining = nMode + 1;
		do {
			*(int*) (pThis + 0x120) = 0;
			ppText = (const char**) 0x0049F068 + nIndex;
			do {
				++ppText;
				++*(int*) (pThis + 0x120);
				++nIndex;
			} while (*ppText != 0);
			++nIndex;
			--nRemaining;
		} while (nRemaining != 0);
	}
	*(int*) (pThis + 0x124) = 0;
	*(int*) (pThis + 0x11c) = 0;
	*(const char***) (pThis + 0x100) = (const char**) 0x0049F064 + (nIndex - *(int*) (pThis + 0x120));
	switch (nMode) {
	case 0:
		*(int*) (pThis + 0x124) = 1;
		if (((int(__fastcall*)(void*))(*(void***) *(void**) (pThis + 0x110))[1])(*(void**) (pThis + 0x110)) == 0) {
			++*(int*) (pThis + 0x124);
			++*(int*) (pThis + 0x11c);
		}
		*(int*) (pThis + 0x118) = *(int*) (pThis + 0x124);
		break;
	case 1:
	case 2:
	case 4:
		*(int*) (pThis + 0x124) = 1;
		*(int*) (pThis + 0x118) = 1;
		break;
	case 3:
		*(int*) (pThis + 0x124) = 1;
		*(int*) (pThis + 0x118) = 2;
		break;
	default:
		*(int*) (pThis + 0x118) = 0;
		break;
	}
	*(int*) (pThis + 0x128) = *(int*) (pThis + 0x118);
	nChild = pThis != 0 ? (int) (pThis + 0x90) : 0;
	((void(__fastcall*)(void*, int, int, int)) 0x004632A0)(g_pSharedRenderDispatchQueue, 0, nChild, 0);

	nRectCount = *(int*) (pThis + 0x120);
	*(int*) (pThis + 500) = 0;
	pRects = (short*) ((void*(__cdecl*) (unsigned int) ) 0x0045A780)(nRectCount * 8);
	if (pRects == 0) {
		*(short**) (pThis + 0x1f8) = 0;
	}
	else {
		pRect = pRects;
		while (--nRectCount >= 0) {
			((void(__fastcall*)(void*)) 0x004017C6)(pRect);
			pRect += 4;
		}
		*(short**) (pThis + 0x1f8) = pRects;
	}
	((void(__fastcall*)(void*)) 0x00402F54)(pThis);
	*(int*) (pThis + 0x208) = 0;
	*(int*) (pThis + 0x1dc) = 0;
}
#endif
