#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_172)
// FUNCTION: LEMBALL 0x00439c40
void __fastcall update_variant_resource_effect_volumes(void* pThis,
													   int nEdxSlop,
													   void* pEffectArray,
													   int nCount,
													   int* pPlayerPos)
{
	unsigned long dwNow;
	int nEffectId;
	int nEffectSlot;
	unsigned int uMasterVolume;
	int nDistance;
	unsigned int uChildVolume;
	int* pEntry;
	int nPlayerX;
	int nPlayerY;
	int nQueuedEffect;
	if (g_fEffectsOptionAvailable != 0 && g_fVariantResourceEffectsEnabled != 0) {
		nQueuedEffect = *(int*) ((char*) pThis + 0x10);
		if (nQueuedEffect != 0) {
			*(int*) ((char*) pThis + 0x10) = 0;
			((void(__fastcall*)(void*, int)) 0x45b460)(g_pAudioManager,
													   *(int*) ((char*) pThis + nQueuedEffect * 0x0c + 0x6c));
		}
		dwNow = ((unsigned long (*)(void)) 0x00462e80)();
		nPlayerX = *pPlayerPos;
		nPlayerY = pPlayerPos[1];
		uMasterVolume = (unsigned int) ((unsigned char (*)(void*)) 0x45b560)(g_pAudioManager);
		uMasterVolume = uMasterVolume & 0xff;
		if (nCount > 0) {
			pEntry = (int*) ((char*) pEffectArray + 0x44);
			do {
				nEffectId = *pEntry;
				if (nEffectId != 0) {
					nDistance = ((int(__cdecl*)(int, int, int, int)) 0x40254a)(
						nPlayerX >> 12,
						nPlayerY >> 12,
						(unsigned int) *(unsigned short*) ((char*) pEntry - 0x34),
						(unsigned int) *(unsigned short*) ((char*) pEntry - 0x32));
					uChildVolume = uMasterVolume;
					if (nDistance - 200 > 0) {
						uChildVolume =
							uMasterVolume + (unsigned int) ((nDistance - 200) * (int) uMasterVolume * -40) / 0x1e780;
						if ((int) uChildVolume > (int) uMasterVolume) {
							uChildVolume = uMasterVolume;
						}
					}
					nEffectSlot = nEffectId * 0x0c;
					if ((int) (dwNow - *(unsigned long*) ((char*) pThis + nEffectSlot + 0x70)) > 100) {
						((void(__fastcall*)(void*, int, int)) 0x45b490)(g_pAudioManager,
																		*(int*) ((char*) pThis + nEffectSlot + 0x6c),
																		(int) uChildVolume);
						*(unsigned long*) ((char*) pThis + nEffectSlot + 0x70) = dwNow;
					}
				}
				pEntry += 0x13;
				--nCount;
			} while (nCount != 0);
		}
	}
	(void) nEdxSlop;
}
#endif
