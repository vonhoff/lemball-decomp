#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_004)
// FUNCTION: LEMBALL 0x00450a10
void __fastcall FinishPaintballSequenceToLevelSelection(void* pObject)
{
	if (*(int*) 0x4a62f8 != 0) {
		(*(void (**)(int)) * (void***) *(void**) 0x4a1bd0)(*(int*) 0x4a97b8);
	}
	if (*(int*) 0x4a62fc != 0) {
		(*(void (**)(int)) * (void***) *(void**) 0x4aa100)(*(int*) 0x4a97b8);
	}
	*(int*) ((char*) pObject + 0x378) = 1;
	*(int*) ((char*) pObject + 0x374) = 4;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_005)
// FUNCTION: LEMBALL 0x00450820
void __fastcall DeactivateEmbeddedMediaIfActive(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x5fc) != 0 && *(int*) ((char*) pObject + 0x524) == 1) {
		(*(void (**)(void))(*(void***) *(void**) ((char*) pObject + 0x520) + 0x74 / 4))();
		*(int*) ((char*) pObject + 0x5fc) = 0;
		*(int*) ((char*) pObject + 0x5f8) = ((unsigned (*)(void)) 0x462e80)() + 0x28;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_025)
// FUNCTION: LEMBALL 0x00454b10
void __fastcall ReleaseNetworkLobbyPaletteRemaps(void* pObject)
{
	int i;
	for (i = 0; i < 6; i++) {
		((PaletteRemapPointerTableMemberView*) *(void**) 0x4a2000)
			->ReleasePaletteRemapVariant((void*) *(int*) ((char*) pObject + 0x414 + i * 4));
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_035)
// FUNCTION: LEMBALL 0x0044a3e0
void __fastcall RefreshLevelSelectionNavigationLocks(void* param_1)
{
	int bVar1 = ((int(__fastcall*)(void*)) 0x402793)(*(void**) 0x49cb68);
	int bVar2 = ((int(__fastcall*)(void*)) 0x401f82)(*(void**) 0x49cb68);
	*(int*) ((char*) param_1 + 0x480) = 0;
	if (bVar1 != 1) {
		*(int*) ((char*) param_1 + 0x480) = 1;
	}
	*(int*) ((char*) param_1 + 0x484) = 0;
	if (bVar2 != 1) {
		*(int*) ((char*) param_1 + 0x484) = 1;
	}
	((void(__fastcall*)(void*, int)) 0x40329c)(*(void**) ((char*) param_1 + 0x338), 0xacef000e);
	((void(__fastcall*)(void*, int)) 0x40329c)(*(void**) ((char*) param_1 + 0x338), 0xacef000f);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_043)
// FUNCTION: LEMBALL 0x00454620
void __fastcall CancelNetworkLobbyPrompt(void* param_1)
{
	void* pController = *(void**) 0x49f140;
	if (*(int*) ((char*) pController + 0x2c) != 0 && *(int*) ((char*) pController + 0x28) == 0) {
		((void(__fastcall*)(void*)) 0x4013a7)(pController);
	}
	((void(__fastcall*)(void*, int)) 0x4012da)(param_1, 0);
	*(int*) ((char*) param_1 + 0x39c) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_046)
// FUNCTION: LEMBALL 0x00450d30
void __fastcall ActivateEmbeddedMediaAndRestoreMusic(void* param_1)
{
	if (*(int*) 0x4a6284 == 0) {
		((void(__fastcall*)(void*)) 0x46e300)(param_1);
	}
	if (*(int*) ((char*) param_1 + 0xd4) == 0) {
		((void(__fastcall*)(void*, int)) 0x401762)(*(void**) 0x49eb80, 1);
		*(int*) ((char*) param_1 + 0xd4) = 1;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_052)
// FUNCTION: LEMBALL 0x00454830
void __fastcall FlushNetworkLobbyDeferredStatus(void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x430) == 7 &&
		(((void(__fastcall*)(void*)) 0x402c11)(*(void**) 0x49f140), *(int*) ((char*) param_1 + 0x3b8) == 0)) {
		return;
	}
	((void(__fastcall*)(void*, int, int)) 0x401d61)(param_1, *(int*) ((char*) param_1 + 0x430), 6000);
	*(int*) ((char*) param_1 + 0x430) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_055)
// FUNCTION: LEMBALL 0x0044a2d0
int __cdecl CopyNextLevelTitleWordToLine(int param_1, int param_2, int* param_3, int* param_4)
{
	char cVar1 = *(char*) (*param_3 + param_1);
	while (cVar1 != '\0' && *(char*) (*param_3 + param_1) != ' ') {
		*(char*) (*param_4 + param_2) = *(char*) (*param_3 + param_1);
		*param_3 = *param_3 + 1;
		*param_4 = *param_4 + 1;
		cVar1 = *(char*) (*param_3 + param_1);
	}
	*(char*) (*param_4 + param_2) = 0;
	if (*(char*) (*param_3 + param_1) != '\0') {
		*param_3 = *param_3 + 1;
		return 0;
	}
	return 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_063)
// FUNCTION: LEMBALL 0x004496d0
void __fastcall DestroyLevelSelectionScreen(void* param_1)
{
	*(void**) param_1 = (void*) 0x497bf0;
	*(void**) ((char*) param_1 + 0x4) = (void*) 0x497be0;
	*(void**) ((char*) param_1 + 0x14) = (void*) 0x497bdc;
	FreeVSMemBlock(*(void**) ((char*) param_1 + 0x3c4));
	FreeVSMemBlock(*(void**) ((char*) param_1 + 0x3c8));
	FreeVSMemBlock(*(void**) ((char*) param_1 + 0x3c0));
	((void(__fastcall*)(void*)) 0x401587)(param_1);
	if (*(int*) ((char*) param_1 + 0x9c) != 0) {
		((void(__fastcall*)(void*)) 0x402be4)(param_1);
	}
	char* pChild = (char*) param_1 + 0x3bc;
	int i;
	for (i = 0; i < 2; i++) {
		pChild -= 0x24;
		((void(__fastcall*)(void*)) 0x403558)(pChild);
	}
	((void(__fastcall*)(void*)) 0x402081)(param_1);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_064)
// FUNCTION: LEMBALL 0x004515c0
void __fastcall DestroyPasswordEntryScreen(void* param_1)
{
	*(void**) param_1 = (void*) 0x498408;
	*(void**) ((char*) param_1 + 0x4) = (void*) 0x4983f8;
	*(void**) ((char*) param_1 + 0x14) = (void*) 0x4983f0;
	if (*(int*) ((char*) param_1 + 0x9c) != 0) {
		((void(__fastcall*)(void*)) 0x40244b)(param_1);
	}
	*(void**) ((char*) param_1 + 0x454) = (void*) 0x496ca8;
	((void(__fastcall*)(void*)) 0x402081)(param_1);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_066)
// FUNCTION: LEMBALL 0x004535c0
void __fastcall DestroyNetworkLobbyScreen(void* param_1)
{
	*(void**) param_1 = (void*) 0x498640;
	*(void**) ((char*) param_1 + 0x4) = (void*) 0x498630;
	*(void**) ((char*) param_1 + 0x14) = (void*) 0x49862c;
	if (*(int*) ((char*) param_1 + 0x374) == 0) {
		if (*(void**) 0x49f140 != 0) {
			((void(__fastcall*)(void*)) 0x402cca)(param_1);
		}
	}
	else if (*(void**) 0x49f140 != 0) {
		((void(__fastcall*)(void*)) 0x402c11)(*(void**) 0x49f140);
	}
	int iVar3;
	for (iVar3 = 0; iVar3 < 0x2a8; iVar3 += 0x44) {
		((void(__fastcall*)(void*, void*)) 0x46aa00)(*(void**) (*(int*) ((char*) param_1 + 0x84) + 0x1c),
													 (void*) (*(int*) ((char*) param_1 + 0x3cc) + iVar3));
	}
	void* pPos = *(void**) ((char*) param_1 + 0x3cc);
	if (pPos != 0) {
		(*(void (**)(int))(*(void***) pPos))(3);
	}
	void* uVar1 = *(void**) ((char*) param_1 + 0x398);
	if (uVar1 != 0) {
		FreeVSMemBlock(*(void**) ((char*) uVar1 + 0xc));
		FreeVSMemBlock(uVar1);
	}
	((void(__fastcall*)(void*)) 0x401bd6)(param_1);
	if (*(int*) ((char*) param_1 + 0x9c) != 0) {
		((void(__fastcall*)(void*)) 0x401221)(param_1);
	}
	*(void**) ((char*) param_1 + 0x3ec) = (void*) 0x496ca8;
	((void(__fastcall*)(void*)) 0x402081)(param_1);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_091)
struct LobbyActionButtonFrameRect {
	short m_nX;
	short m_nY;
	short m_nCX;
	short m_nCY;
};

// FUNCTION: LEMBALL 0x004536b0
void __fastcall DrawNetworkLobbyActionButtonFrame(void* pObject, int nUnused, int param_1)
{
	unsigned int* pEntry = (unsigned int*) (*(unsigned int*) ((char*) pObject + 0x42c) + param_1 * 8);
	LobbyActionButtonFrameRect Rect;
	((LobbyActionButtonFrameRect *
	  (__fastcall*) (LobbyActionButtonFrameRect*, int, short, short, short, short) ) 0x403594)(&Rect,
																							   0,
																							   (short) pEntry[0],
																							   (short) pEntry[1],
																							   (short) pEntry[2],
																							   (short) pEntry[3]);
	((void(__fastcall*)(void*, int, LobbyActionButtonFrameRect)) 0x4020e0)(pObject, 0, Rect);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_125)
// FUNCTION: LEMBALL 0x00451630
void __fastcall rotate_password_keypad_selection(void* pThis, int nEdxSlop, int nUnusedArg)
{
	if (*(int*) ((char*) pThis + 0x488) == 1) {
		return;
	}
	int nSel = *(int*) ((char*) pThis + 0x480);
	void* pWidget = *(void**) ((char*) pThis + 0x424 + *(int*) (0x4a0028 + nSel * 4) * 4);
	void* pSub = (char*) pWidget + 0x90;
	void** pVt = *(void***) pSub;
	unsigned short auPoint[2] = {0, 0};
	((void(__fastcall*)(void*, unsigned short*, int)) pVt[2])(pSub, auPoint, 0);
	int nNew = nSel + auPoint[0];
	if (nNew < 0) {
		nNew += 12;
	}
	if (nNew > 11) {
		nNew -= 12;
	}
	*(int*) ((char*) pThis + 0x480) = nNew;
	*(int*) ((char*) pThis + 0x478) = *(int*) ((char*) pThis + 0x3c0 + nNew * 8);
	*(int*) ((char*) pThis + 0x47c) = *(int*) ((char*) pThis + 0x3c4 + nNew * 8);
	((void(__fastcall*)(void*, int)) 0x401479)(g_pVariantResourceEntryManager, 0x1b);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_128)
// FUNCTION: LEMBALL 0x0044a330
void __fastcall initialize_level_selection_palette_remap(void* pThis, int nEdxSlop)
{
	unsigned char* pMap;
	int hPal;
	int i;
	hPal = ((int(__cdecl*)(int)) 0x45dd90)(0x10a);
	pMap = (unsigned char*) ((void*(__cdecl*) (int) ) 0x45a780)(0x100);
	*(unsigned int*) ((char*) pThis + 0x3cc) = (unsigned int) pMap;
	for (i = 0; i < 0x100; i++) {
		pMap[i] = (char) i;
	}
	for (i = 0; i < 0x28; i += 4) {
		if (*(int*) (0x49f880 + i) != 0) {
			pMap[*(int*) (0x49f858 + i)] = (char) *(int*) (0x49f880 + i);
		}
	}
	*(unsigned int*) ((char*) pThis + 0x3d0) =
		((unsigned int(__fastcall*)(void*, int, int, void*, int)) 0x46ad70)((void*) 0x4a2000, 0, 0x10a, pMap, 0);
	((void(__fastcall*)(void*)) 0x45d180)((void*) hPal);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_142)
// FUNCTION: LEMBALL 0x00451f10
void __fastcall initialize_password_entry_render_context(void* pThis)
{
	void* pLayout;
	int* pCtxSlot;
	void* pCtx;
	short vSum;
	short region[4];
	pLayout = *(void**) ((char*) pThis + 0x3a8);
	vSum = (short) (*(int*) ((char*) pLayout + 0x60)) + (short) (*(int*) ((char*) pLayout + 0x10));
	pCtxSlot = (int*) ((char*) pThis + 0x3a4);
	pCtx = ((void*(__cdecl*) (int) ) 0x45a780)(0x90);
	if (pCtx != 0) {
		pCtx = ((void*(__fastcall*) (void*) ) 0x403571)(pCtx);
	}
	*pCtxSlot = (int) pCtx;
	region[0] = vSum * 3;
	region[1] = vSum * 4;
	region[2] = (short) (*(int*) ((char*) pLayout + 8)) - 1;
	region[3] = (short) (*(int*) ((char*) pLayout + 0xc)) - 1;
	((void(__fastcall*)(void*, int, short*, int, int))(
		*(int*) (*(int*) pCtx + 4)))(pCtx, 0, region, *(int*) ((char*) pThis + 0x84), 0);
	*(int*) ((char*) pThis + 0x474) = *(int*) ((char*) pCtx + 0x4c);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_147)
// FUNCTION: LEMBALL 0x00454520
void __fastcall begin_network_lobby_text_prompt(void* this_, void* nUnused, int param_1)
{
	if (*(int*) ((char*) this_ + 0x39c) != 0 && ((*(int*) ((char*) this_ + 0x3bc) != 0 && param_1 != 0) ||
												 (*(int*) ((char*) this_ + 0x3bc) == 0 && param_1 == 0))) {
		((void(__fastcall*)(void*)) 0x0040118b)(this_);
		return;
	}
	*(int*) ((char*) this_ + 0x3bc) = param_1;
	*(int*) ((char*) this_ + 0x3b8) = 0;
	*(int*) ((char*) this_ + 0x39c) = 0;
	*(int*) ((char*) this_ + 0x430) = 0;
	((void(__fastcall*)(void*)) 0x00402c11)(g_pActiveNetworkLobbyTransportController);
	if (g_szNetworkLobbyLocalPlayerName[0] == '\0') {
		((void(__fastcall*)(void*, int, int)) 0x00403085)(this_, 1, 1);
		return;
	}
	((void(__fastcall*)(void*, char*)) 0x00402243)(*(void**) ((char*) this_ + 0x398), g_szNetworkLobbyLocalPlayerName);
	((void(__fastcall*)(void*, int, int)) 0x00403085)(this_, 1, 0);
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_155)
// FUNCTION: LEMBALL 0x00451d20
void __fastcall HandlePasswordEntryAction(void* pObject, int nUnused, int nAction)
{
	int nResult;
	unsigned long nTime;
	int nLength;

	if (*(int*) ((char*) pObject + 0x488) == 1) {
		return;
	}
	switch (nAction) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		nLength = *(int*) ((char*) pObject + 0x420);
		if (nLength > 9) {
			((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x19);
			return;
		}
		*(char*) ((char*) pObject + nLength + 0x3b0) = (char) nAction + '0';
		*(int*) ((char*) pObject + 0x420) = nLength + 1;
		return;
	case 10:
		nLength = *(int*) ((char*) pObject + 0x420);
		if (nLength < 1) {
			((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x19);
		}
		else {
			*(int*) ((char*) pObject + 0x420) = nLength - 1;
		}
		nLength = *(int*) ((char*) pObject + 0x420);
		if (nLength >= 0 && nLength < 10) {
			*(char*) ((char*) pObject + nLength + 0x3b0) = 0x2d;
			return;
		}
		break;
	case 11:
		nResult = ((GameLevelProgressState*) g_pLevelProgressState)
					  ->ValidateAndApplyPassword((char*) ((char*) pObject + 0x3b0));
		*(int*) ((char*) pObject + 0x484) = nResult;
		((void(__fastcall*)(void*))(*(void***) *(void**) pObject + 0x4c / 4))(pObject);
		((GameLevelProgressState*) g_pLevelProgressState)->Snapshot();
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)
			->PlayVariantResourceEffect((nResult == 0) ? 0x22 : 0x13);
		nTime = ((unsigned (*)(void)) 0x00462e80)();
		*(int*) ((char*) pObject + 0x488) = 1;
		*(unsigned long*) ((char*) pObject + 0x48c) = nTime;
		*(unsigned long*) ((char*) pObject + 0x490) = nTime + 1000;
		break;
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_169)
// FUNCTION: LEMBALL 0x004511f0
void __fastcall delete_paintball_screen_adjusted(void* pObject)
{
	((void(__fastcall*)(void*)) 0x403035)((char*) pObject - 0xc);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_171)
// FUNCTION: LEMBALL 0x00454740
void __fastcall commit_network_lobby_text_prompt(void* pThis, int nEdxSlop)
{
	char* pszText;
	char* pBuffer;
	*(int*) ((char*) pThis + 0x39c) = 0;
	if (*(int*) ((char*) pThis + 0x3a0) == 1) {
		pBuffer = *(char**) (*(int*) ((char*) pThis + 0x398) + 0x0c);
		if (pBuffer[0] == '\0') {
			((void(__fastcall*)(void*)) 0x401d61)(pThis);
			return;
		}
		strcpy(g_szNetworkLobbyLocalPlayerName, pBuffer);
		if (*(int*) ((char*) pThis + 0x3bc) == 0) {
			g_szNetworkLobbyJoinAddress[0] = '\0';
			*(int*) ((char*) pThis + 0x3e0) = 1;
		}
		else {
			*(int*) ((char*) pThis + 0x3a4) = 2;
		}
		if (*(int*) ((char*) g_pActiveNetworkLobbyTransportController + 0x2c) == 0 ||
			*(int*) ((char*) g_pActiveNetworkLobbyTransportController + 0x28) != 0) {
			((void(__fastcall*)(void*)) 0x4012da)(pThis);
			return;
		}
	}
	else if (*(int*) ((char*) pThis + 0x3a0) == 2) {
		pszText = *(char**) (*(int*) ((char*) pThis + 0x398) + 0x0c);
		strcpy(g_szNetworkLobbyJoinAddress, pszText);
		*(int*) ((char*) pThis + 0x3e0) = 1;
	}
	(void) nEdxSlop;
}
#endif
