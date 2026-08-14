#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_012)
// FUNCTION: LEMBALL 0x00443360
int __fastcall IsPointInsideLevelScreenActionPanel(void* pObject, int nUnused, short* param_1)
{
	if (*(short*) ((char*) pObject + 0x54) <= param_1[0] &&
		param_1[0] < (*(short*) ((char*) pObject + 0x50) + *(short*) ((char*) pObject + 0x54)) &&
		*(short*) ((char*) pObject + 0x56) <= param_1[1] &&
		param_1[1] < (*(short*) ((char*) pObject + 0x56) + *(short*) ((char*) pObject + 0x52))) {
		return 1;
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_019)
// FUNCTION: LEMBALL 0x00443250
void __fastcall RefreshLevelScreenActionPanel(void* pObject)
{
	int i;
	for (i = 0; i < 4; i++) {
		((void(__fastcall*)(void*)) 0x4032bf)(*(void**) ((char*) pObject + 0x1c + i * 4));
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_023)
// FUNCTION: LEMBALL 0x00442a40
void __fastcall RefreshAlignedZrleChildAndQueueIfDirty(void* pObject, int nUnused, void* param_1)
{
	if (((int(__fastcall*)(void*))(*(void***) pObject)[0xb4 / 4])(pObject) != *(int*) ((char*) pObject + 0x16c)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x16c) = *(int*) ((char*) pObject + 0x16c) == 0;
	}
	if (*(int*) (*(int*) ((char*) pObject + 0x110) + 0x18) != *(int*) ((char*) pObject + 0x170)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x170) = *(int*) (*(int*) ((char*) pObject + 0x110) + 0x18);
	}
	((void(__fastcall*)(void*, void*)) 0x468360)(pObject, param_1);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_024)
// FUNCTION: LEMBALL 0x004431c0
void __fastcall SyncLevelScreenActionPanelChildValues(void* pObject)
{
	int i;
	for (i = 0; i < 4; i++) {
		void* pChild = *(void**) ((char*) pObject + 0x1c + i * 4);
		*(int*) ((char*) pChild + 8) = *(int*) (*(int*) ((char*) pObject + 0x18) + 0x1cc + i * 4 + 4);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_027)
// FUNCTION: LEMBALL 0x00439340
void* __fastcall InitializeLevelScreenHotspotEntry(void* pObject)
{
	*(unsigned short*) ((char*) pObject + 0x2c) = 0xffff;
	*(int*) ((char*) pObject + 0x38) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x3c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x40) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x34) = 0;
	*(int*) ((char*) pObject + 0x30) = 0;
	*(int*) ((char*) pObject + 0x18) = 0;
	*(int*) ((char*) pObject + 0x20) = 0;
	return pObject;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_110)
// FUNCTION: LEMBALL 0x004349e0
void __fastcall EmitLevelScreenVariantEntryWithContext(void* pThis,
													   int nUnused,
													   int param_2,
													   unsigned short param_3,
													   unsigned short param_4,
													   int param_5,
													   int param_6,
													   int param_7)
{
	unsigned short uStack_c = param_3;
	unsigned short uStack_a = param_4;
	char auStack_8[8];
	void* puVar1;
	switch (param_5) {
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
	case 0x27:
	case 0xb5:
	case 0xb6:
	case 0xb7:
	case 0xb8:
	case 0xb9:
	case 0xba:
	case 0xbb:
	case 0xbc:
	case 0xbd:
	case 0xbe:
	case 0xbf:
	case 0xc0:
	case 0xc1:
	case 0xc2:
	case 0xc3:
	case 0xc4:
	case 0xc5:
	case 0xc6:
	case 0xc7:
	case 0xc8:
	case 0xc9:
	case 0xca:
	case 0xcb:
	case 0xcc:
	case 0xcd:
	case 0xce:
	case 0xcf:
	case 0xd0:
	case 0xd1:
	case 0xd2:
	case 0xd3:
	case 0xd4:
	case 0xd5:
	case 0xd6:
	case 0xd7:
	case 0xd8:
	case 0xd9:
	case 0xda:
	case 0xdb:
	case 0xdc:
	case 0xdd:
	case 0xde:
	case 0xdf:
	case 0xe0:
	case 0xe1:
	case 0xe4:
	case 0xe5:
	case 0x103:
	case 0x105:
		puVar1 = *(void**) (*(int*) ((char*) pThis + 0x74) +
							(short) *(short*) (*(int*) ((char*) pThis + 0x28) + param_5 * 2) * 4);
		*(int*) ((char*) puVar1 + 0xc) = param_6;
		uStack_c = param_3;
		uStack_a = param_4;
		{
			int uSaved = *(int*) ((char*) pThis + 0x1c);
			*(int*) ((char*) pThis + 0x1c) = param_2;
			((int(__fastcall*)(void*, void*, void*, int, int, void*, int)) 0x467730)(pThis,
																					 auStack_8,
																					 &uStack_c,
																					 param_5,
																					 0,
																					 puVar1,
																					 param_7);
			*(int*) ((char*) pThis + 0x1c) = uSaved;
		}
		return;
	case 0xe7:
	case 0x104:
	case 0x106:
		puVar1 = *(void**) (*(int*) ((char*) pThis + 0x74) +
							(short) *(short*) (*(int*) ((char*) pThis + 0x28) + param_5 * 2) * 4);
		uStack_c = param_3;
		uStack_a = param_4;
		{
			int uSaved = *(int*) ((char*) pThis + 0x1c);
			*(int*) ((char*) pThis + 0x1c) = param_2;
			((int(__fastcall*)(void*, void*, void*, int, int, void*, int)) 0x467730)(pThis,
																					 auStack_8,
																					 &uStack_c,
																					 param_5,
																					 0,
																					 puVar1,
																					 param_7);
			*(int*) ((char*) pThis + 0x1c) = uSaved;
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_116)
// FUNCTION: LEMBALL 0x004429b0
void __fastcall sync_action_button_state(void* pObject, int nUnused, void* param_1)
{
	void* iVar1 = *(void**) ((char*) pObject + 0x110);
	if ((unsigned int) *(unsigned short*) (*(int*) ((char*) iVar1 + 8) + 0x228) != *(int*) ((char*) pObject + 0x164)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x164) = (unsigned int) *(unsigned short*) (*(int*) ((char*) iVar1 + 8) + 0x228);
	}
	if (*(int*) ((char*) iVar1 + 0x1c) != *(int*) ((char*) pObject + 0x174)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x174) = *(int*) ((char*) iVar1 + 0x1c);
	}
	if ((unsigned int) (*(int*) (*(int*) ((char*) iVar1 + 8) + 0xb8) == 8) != *(int*) ((char*) pObject + 0x168)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x168) = (unsigned int) (*(int*) ((char*) pObject + 0x168) == 0);
	}
	{
		int iVar2 = (*(int (**)(void*, int))(*(void***) *(void**) ((char*) iVar1 + 8) +
											 0xb4 / 4))(*(void**) ((char*) iVar1 + 8), 0xc);
		if (iVar2 != *(int*) ((char*) pObject + 0x16c)) {
			*(int*) ((char*) pObject + 0xd8) = 1;
			*(int*) ((char*) pObject + 0x16c) = (unsigned int) (*(int*) ((char*) pObject + 0x16c) == 0);
		}
	}
	if (*(int*) ((char*) *(void**) ((char*) pObject + 0x110) + 0x18) != *(int*) ((char*) pObject + 0x170)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x170) = *(int*) ((char*) *(void**) ((char*) pObject + 0x110) + 0x18);
	}
	((void(__fastcall*)(void*, void*)) 0x468360)(pObject, param_1);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_136)
// FUNCTION: LEMBALL 0x004421d0
void* __fastcall construct_text_aligned_zrle_child_overlay(void* pThis,
														   unsigned long ulTextId,
														   unsigned short* pPoint,
														   unsigned long ulA,
														   int nB,
														   int nC)
{
	((void*(__fastcall*) (void*, unsigned short*, unsigned long, int, int) ) 0x00468530)(pThis, pPoint, ulA, nB, nC);
	*(unsigned long*) pThis = 0x004973e0;
	*(unsigned long*) ((char*) pThis + 0x130) = 0;
	*(unsigned long*) pThis = 0x004972b8;
	*(unsigned long*) ((char*) pThis + 0x134) = ulTextId;
	*(unsigned long*) ((char*) pThis + 0x138) = 0;
	*(unsigned long*) ((char*) pThis + 0x90) = 0x004973b8;
	*(unsigned long*) ((char*) pThis + 0x90) = 0x00497290;
	*(int*) ((char*) pThis + 0xb8) = 1;
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_149)
// FUNCTION: LEMBALL 0x00441fe0
void __fastcall mark_level_screen_update_grid_rect(void* pThis, int nEdxSlop, short* pRect)
{
	int nX0 = pRect[2] / 16;
	int nY0 = pRect[3] / 16;
	int nWidth = (pRect[0] + pRect[2] - 1) / 16 - nX0 + 1;
	int nHeight = (pRect[1] + pRect[3] - 1) / 16 - nY0 + 1;
	int nGridW = *(short*) ((char*) pThis + 0x8);
	int nGridH = *(short*) ((char*) pThis + 0xa);
	if (nX0 < nGridW && nY0 < nGridH) {
		if (nX0 < 0) {
			nWidth += nX0;
			nX0 = 0;
		}
		if (nY0 < 0) {
			nHeight += nY0;
			nY0 = 0;
		}
		if (nGridW <= nX0 + nWidth) {
			nWidth = nGridW - nX0;
		}
		if (nGridH <= nY0 + nHeight) {
			nHeight = nGridH - nY0;
		}
		if (nWidth > 0 && nHeight > 0) {
			int nBase = nX0 + nY0 * nGridW;
			char* pBuf1 = *(char**) ((char*) pThis + 0x10) + nBase;
			char* pBuf2 = *(char**) ((char*) pThis + 0x14) + nBase;
			do {
				memset(pBuf1, 1, nWidth);
				memset(pBuf2, 1, nWidth);
				pBuf1 += nGridW;
				pBuf2 += nGridW;
			} while (--nHeight != 0);
		}
	}
	(void) nEdxSlop;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_151)
// FUNCTION: LEMBALL 0x004432c0
int __fastcall handle_level_screen_pause_action_button_event(void* pThis, int nUnused, short* pActionButtonEvent)
{
	void* pBase = *(void**) ((char*) pThis + 0x10);
	if (*(int*) ((char*) pBase + 0xa7c) == 0 && *(int*) (*(char**) ((char*) pBase + 0x96c) + 0x108) != 1) {
		if (*(short*) pActionButtonEvent == 4) {
			if (((int(__fastcall*)(void*, int, int)) 0x00402a81)(pThis, 0, *(int*) ((char*) pActionButtonEvent + 8)) ==
				8) {
				*(int*) ((char*) g_pVariantResourceEntryManager + 0x10) = 3;
				((void(__fastcall*)(void*, int, int)) 0x00402ed7)(
					pBase,
					0,
					(unsigned int) (*(int*) ((char*) pBase + 0xa7c) == 0));
				void* pCur = *(void**) ((char*) pThis + 0x2c);
				int nPause = *(int*) ((char*) pBase + 0xa7c);
				*(int*) ((char*) pCur + 0x130) = nPause;
				*(int*) ((char*) pCur + 0x104) = nPause;
				return 1;
			}
		}
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_152)
#pragma auto_inline(off)
#pragma comment(linker, "/include:?toggle_level_pause_state@@YIXPAXHH@Z")
// FUNCTION: LEMBALL 0x004422b0
void __fastcall toggle_level_pause_state(void* pThis, int nUnused, int bEnablePause)
{
	if (bEnablePause == 0) {
		unsigned int nPauseState = *(unsigned int*) ((char*) pThis + 0x130) ^ 1;

		*(unsigned int*) ((char*) pThis + 0x130) = nPauseState;
		*(unsigned int*) ((char*) pThis + 0x104) = nPauseState;
		if ((nPauseState == 0 &&
			 ((int(__fastcall*)(void*))(
				 *(void***) ((char*) *(void**) (*(char**) ((char*) pThis + 0x134) + 0x10) + 0x4c))[1])(
				 (char*) *(void**) (*(char**) ((char*) pThis + 0x134) + 0x10) + 0x4c) != 0) ||
			(nPauseState != 0 &&
			 *(int*) (*(char**) ((char*) *(void**) (*(char**) ((char*) pThis + 0x134) + 0x10) + 0x96c) + 0x108) != 1)) {
			(*(C2D**) (*(char**) ((char*) pThis + 0x134) + 0x10))->TriggerPause(nPauseState);
		}
		int nActualPauseState = *(int*) ((char*) *(void**) (*(char**) ((char*) pThis + 0x134) + 0x10) + 0xa7c);
		*(unsigned int*) ((char*) pThis + 0x130) = nActualPauseState;
		*(unsigned int*) ((char*) pThis + 0x104) = nActualPauseState;
		*(unsigned int*) ((char*) pThis + 0x138) = 0;
		SetLevelScreenStatusIndicatorModeThunk(1, 0);
	}
	(void) nUnused;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_168)
// FUNCTION: LEMBALL 0x00439840
void __fastcall delete_large_render_adjusted(void* pObject)
{
	((void(__fastcall*)(void*)) 0x403521)((char*) pObject - 4);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_175)
// FUNCTION: LEMBALL 0x00442dd0
void __fastcall refresh_level_screen_action_button(char* param_1)
{
	void* this_;
	int iVar2;
	int iVar3;
	unsigned int uVar4;
	iVar3 = *(int*) ((char*) param_1 + 8);
	if (*(int*) ((char*) iVar3 + 0xb8) == 8) {
		uVar4 = 0;
	}
	else {
		this_ = *(void**) (*(int*) ((char*) param_1 + 0xc) + 0x10);
		if (*(int*) ((char*) this_ + 0xa48) == 1) {
			uVar4 = ((unsigned int(__fastcall*)(void*, void*)) 0x4021c6)(this_, (void*) iVar3);
		}
		else {
			iVar2 = ((int(__fastcall*)(void*)) 0x4021df)(*(void**) (*(int*) ((char*) this_ + 0x96c) + 0x15c));
			iVar3 = ((int(__fastcall*)(int)) 0x4037ba)(iVar3);
			uVar4 = (unsigned int) (iVar2 == iVar3);
		}
	}
	switch (((unsigned int(__fastcall*)(void*)) 0x4015d2)(*(void**) ((char*) param_1 + 8))) {
	case 0x27:
		*(int*) ((char*) param_1 + 0x18) = 3;
		break;
	case 0x29:
		*(int*) ((char*) param_1 + 0x18) = 1;
		break;
	case 0x2b:
		*(int*) ((char*) param_1 + 0x18) = 4;
		break;
	case 0x2d:
		*(int*) ((char*) param_1 + 0x18) = 0;
		break;
	default:
		*(int*) ((char*) param_1 + 0x18) = -1;
		break;
	}
	iVar3 = 0;
	*(int*) ((char*) param_1 + 0x1c) = 0;
	if (0 < *(int*) (*(int*) ((char*) param_1 + 8) + 0x220)) {
		do {
			iVar2 = ((int(__fastcall*)(void*, int)) 0x40269e)(*(void**) ((char*) param_1 + 8), iVar3);
			if (iVar2 == 0x15) {
				*(int*) ((char*) param_1 + 0x20 + *(int*) ((char*) param_1 + 0x1c) * 4) = 3;
				*(int*) ((char*) param_1 + 0x1c) += 1;
			}
			else if (iVar2 == 0x16) {
				*(int*) ((char*) param_1 + 0x20 + *(int*) ((char*) param_1 + 0x1c) * 4) = 1;
				*(int*) ((char*) param_1 + 0x1c) += 1;
			}
			else if (iVar2 == 0x17) {
				*(int*) ((char*) param_1 + 0x20 + *(int*) ((char*) param_1 + 0x1c) * 4) = 4;
				*(int*) ((char*) param_1 + 0x1c) += 1;
			}
			iVar3 = iVar3 + 1;
		} while (iVar3 < *(int*) (*(int*) ((char*) param_1 + 8) + 0x220));
	}
	*(unsigned int*) (*(int*) ((char*) param_1 + 4) + 0x104) = uVar4;
}
#endif
