#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_117)
// FUNCTION: LEMBALL 0x00444b20
void __fastcall select_pause_option_at_point(void* pThis, int nUnused, short* param_1, int param_2)
{
	int iVar4 = *(int*) ((char*) pThis + 0x80);
	if (iVar4 < *(int*) ((char*) pThis + 0x7c)) {
		short sVar1 = param_1[0] - *(short*) ((char*) pThis - 0x8c);
		short sVar2 = param_1[1] - *(short*) ((char*) pThis - 0x8a);
		short* psVar3 = (short*) (*(int*) ((char*) pThis + 0x154) + 4 + iVar4 * 8);
		while (sVar1 < *psVar3 || (short) (psVar3[-2] + *psVar3) <= sVar1 || sVar2 < psVar3[1] ||
			   (short) (psVar3[-1] + psVar3[1]) <= sVar2) {
			psVar3 = psVar3 + 4;
			iVar4 = iVar4 + 1;
			if (*(int*) ((char*) pThis + 0x7c) <= iVar4) {
				return;
			}
		}
		(*(void (**)(int))(*(void***) *(void**) ((char*) pThis + 0x6c)))(*(int*) ((char*) pThis + 0x74) + 1);
		*(int*) ((char*) pThis + 0x74) = iVar4;
		*(int*) ((char*) pThis + 0x64) = 1;
		SetLevelScreenStatusIndicatorMode(1, 1);
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(3);
	}
}
#endif
